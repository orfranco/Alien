
//  Copyright (c) 2003-2022 Movella Technologies B.V. or subsidiaries worldwide.
//  All rights reserved.
//  
//  Redistribution and use in source and binary forms, with or without modification,
//  are permitted provided that the following conditions are met:
//  
//  1.	Redistributions of source code must retain the above copyright notice,
//  	this list of conditions and the following disclaimer.
//  
//  2.	Redistributions in binary form must reproduce the above copyright notice,
//  	this list of conditions and the following disclaimer in the documentation
//  	and/or other materials provided with the distribution.
//  
//  3.	Neither the names of the copyright holders nor the names of their contributors
//  	may be used to endorse or promote products derived from this software without
//  	specific prior written permission.
//  
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
//  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
//  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
//  THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//  SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
//  OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY OR
//  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//  

//--------------------------------------------------------------------------------
// Xsens DOT SDK C++ example DOT firmware update.
//--------------------------------------------------------------------------------
#include <xsensdot_pc_sdk.h>
#include <xstypes/xstime.h>
#include <xstypes/xsportinfoarray.h>
#include <xscommon/xsens_mutex.h>

#include <iostream>
#include <iomanip>
#include <map>
#include <list>
#include <string>

#include "conio.h"
#include "user_settings.h"

using namespace std;

class CallbackHandler : public XsDotCallback
{
public:
	CallbackHandler()
	{
	}

	virtual ~CallbackHandler() noexcept
	{
	}
	
	XsPortInfoArray getDetectedDots() const
	{
		xsens::Lock locky(&m_mutex);
		return m_detectedDots;
	}

	bool errorReceived() const
	{
		return m_errorReceived;
	}

	bool updateDone() const
	{
		return m_updateDone;
	}

	void setUpdateDone(bool value)
	{
		m_updateDone = value;
	}

protected:
	void onAdvertisementFound(const XsPortInfo* portInfo) override
	{
		xsens::Lock locky(&m_mutex);
		if (!UserSettings().m_whiteList.size() || UserSettings().m_whiteList.find(portInfo->bluetoothAddress()) != -1)
			m_detectedDots.push_back(*portInfo);
		else
			std::cout << "Ignoring " << portInfo->bluetoothAddress() << std::endl;
	}

	void onError(const XsString* error) override
	{
		cout << "Error received: " << *error << endl;
		m_errorReceived = true;
	}

	void onProgressUpdated(XsDotDevice* device, int current, int total, const XsString* remark) override
	{
		(void)device;
		cout << "\r";
		cout << "Update: " << current << " Total: " << total << " Remark: " << *remark;
		cout << flush;
	}

	void onDeviceUpdateDone(const XsPortInfo* portInfo, XsDotFirmwareUpdateResult result) override
	{
		cout << endl << portInfo->bluetoothAddress() << " Firmware Update done. Result: " << XsDotFirmwareUpdateResult_toString(result) << std::endl;
		m_updateDone = true;
	}
private:
	mutable xsens::Mutex m_mutex;
	bool m_errorReceived = false;
	bool m_updateDone = false;
	XsPortInfoArray m_detectedDots;
};

//--------------------------------------------------------------------------------
int main(void)
{
	XsDotConnectionManager* manager = nullptr;

	// Lambda function for error handling
	auto handleError = [=](string errorString)
	{
		manager->destruct();
		cout << errorString << endl;
		return -1;
	};

	// Print SDK version
	XsVersion version;
	xsdotsdkDllVersion(&version);
	cout << "Using Xsens DOT SDK version: " << version.toString().toStdString() << endl;

	// Create connection manager
	cout << "Creating Xsens DOT Connection Manager object..." << endl;
	manager = XsDotConnectionManager::construct();
	if (manager == nullptr)
		return handleError("Manager could not be constructed, exiting.");

	// Create and attach callback handler to connection manager
	CallbackHandler callback;
	manager->addXsDotCallbackHandler(&callback);

	// Start a scan and wait until we have found one or more Xsens DOT Devices
	cout << "Scanning for devices..." << endl;
	manager->enableDeviceDetection();

	cout << "Press any key or wait 20 seconds to stop scanning..." << endl;
	bool waitForConnections = true;
	size_t connectedDOTCount = 0;
	int64_t startTime = XsTime::timeStampNow();
	do
	{
		XsTime::msleep(100);

		size_t nextCount = callback.getDetectedDots().size();
		if (nextCount != connectedDOTCount)
		{
			cout << "Number of detected DOTs: " << nextCount << ". Press any key to start." << endl;
			connectedDOTCount = nextCount;
		}
		if (_kbhit())
			waitForConnections = false;
	} while (waitForConnections && !callback.errorReceived() && (XsTime::timeStampNow() - startTime <= 20000));

	manager->disableDeviceDetection();
	cout << "Stopped scanning for devices." << endl;

	if (callback.getDetectedDots().empty())
		return handleError("No Xsens DOT device(s) found. Aborting.");

	XsDotDevicePtrArray deviceList;
	for (auto& portInfo : callback.getDetectedDots())
	{
		cout << "Opening DOT with address: " << portInfo.bluetoothAddress() << endl;
		if (!manager->openPort(portInfo))
		{
			cout << "Connection to Device " << portInfo.bluetoothAddress() << " failed, retrying..." << endl;
			cout << "Device " << portInfo.bluetoothAddress() << " retry connected: " << endl;
			if (!manager->openPort(portInfo))
			{
				cout << "Could not open DOT. Reason: " << manager->lastResultText() << endl;
				continue;
			}
		}
		XsDotDevice* device = manager->device(portInfo.deviceId());

		if (device == nullptr)
			continue;

		deviceList.push_back(device);
		cout << "Found a device with tag: " << device->deviceTagName().toStdString() << " @ address: " << device->portInfo().bluetoothAddress() << endl;
		cout << "Updating firmware for device: " << device->deviceTagName().toStdString() << endl;

		// Reset the update done flag to enable updating the next device in the example.
		callback.setUpdateDone(false);

		// Start updating the firmware of this DOT from the update server
		// Version will be 0.0.0 if there is no update, otherwise firmwareVersion will hold a version and start updating the DOT to it.
		XsVersion firmwareVersion = device->startFirmwareUpdateFromServer();

		cout << "Updating Firmware to version " << firmwareVersion.toSimpleString() << endl;
		while (!callback.updateDone())
		{
			if (firmwareVersion.empty())
			{
				cout << "Did not find a new Firmware on the server, device result: " << device->lastResultText() << endl;
				break;
			}
			xsYield();
		}
	}

	cout << "Closing ports..." << endl;
	manager->close();

	cout << "Freeing XsDotConnectionManager object..." << endl;
	manager->destruct();

	cout << "Successful exit." << endl;

	return 0;
}
