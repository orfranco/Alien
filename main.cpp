
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
// Xsens DOT SDK C++ example DOT receive data.
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
#include <iostream>
#include <vector>
#include <string>
#include <rtmidi/RtMidi.h>

using namespace std;

class CallbackHandler : public XsDotCallback
{
public:
	CallbackHandler(size_t maxBufferSize = 5)
		: m_maxNumberOfPacketsInBuffer(maxBufferSize)
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

	bool packetsAvailable() const
	{
		for (auto const& device : m_detectedDots)
			if (!packetAvailable(device.bluetoothAddress()))
				return false;
		return true;
	}

	bool packetAvailable(const XsString& bluetoothAddress) const
	{
		xsens::Lock locky(&m_mutex);
		if (m_numberOfPacketsInBuffer.find(bluetoothAddress) == m_numberOfPacketsInBuffer.end())
			return false;
		return m_numberOfPacketsInBuffer.at(bluetoothAddress) > 0;
	}

	XsDataPacket getNextPacket(const XsString& bluetoothAddress)
	{
		if (!packetAvailable(bluetoothAddress))
			return XsDataPacket();
		xsens::Lock locky(&m_mutex);
		XsDataPacket oldestPacket(m_packetBuffer[bluetoothAddress].front());
		m_packetBuffer[bluetoothAddress].pop_front();
		--m_numberOfPacketsInBuffer[bluetoothAddress];
		return oldestPacket;
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

	void onBatteryUpdated(XsDotDevice* device, int batteryLevel, int chargingStatus) override
	{
		cout << device->deviceTagName() << " BatteryLevel: " << batteryLevel << " Charging status: " << chargingStatus;
		cout << endl;
	}

	void onError(const XsString* error) override
	{
		cout << "Error received: " << *error << endl;
		m_errorReceived = true;
	}

	void onLiveDataAvailable(XsDotDevice* device, const XsDataPacket* packet) override
	{
		xsens::Lock locky(&m_mutex);
		assert(packet != nullptr);
		while (m_numberOfPacketsInBuffer[device->portInfo().bluetoothAddress()] >= m_maxNumberOfPacketsInBuffer)
			(void)getNextPacket(device->portInfo().bluetoothAddress());

		m_packetBuffer[device->portInfo().bluetoothAddress()].push_back(*packet);
		++m_numberOfPacketsInBuffer[device->portInfo().bluetoothAddress()];
		assert(m_numberOfPacketsInBuffer[device->portInfo().bluetoothAddress()] <= m_maxNumberOfPacketsInBuffer);
	}
private:
	mutable xsens::Mutex m_mutex;
	bool m_errorReceived = false;
	XsPortInfoArray m_detectedDots;

	size_t m_maxNumberOfPacketsInBuffer;
	map<XsString, size_t> m_numberOfPacketsInBuffer;
	map<XsString, list<XsDataPacket>> m_packetBuffer;
};


//--------------------------------------------------------------------------------
int main(void)
{
	// Create an RtMidiOut object
	RtMidiOut midiOut;

	// Check if there are any available MIDI output ports
	if (midiOut.getPortCount() == 0) {
		std::cout << "No available MIDI output ports.\n";
		return 0;
	}

	for (size_t i = 0; i < midiOut.getPortCount(); i++)
	{
		cout << midiOut.getPortName(i) << endl;
	}

	// Open the first available MIDI port
	midiOut.openPort(2);

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

	// Optionally, on Linux you can use another available bluetooth adapter using the name
	// Note that this adapter needs to be changed prior to enabling Device Detection, uncomment the lines below
	// cout << "Setting bluetooth adapter to use" << endl;
	// auto adapterList = manager->getAvailableBluetoothAdapters();
	// for (auto& adapter : adapterList)
	// 	cout << "Found Bluetooth adapter: " << adapter << endl;

	// bool setAdapter = manager->setPreferredBluetoothAdapter("hci1");
	// if (!setAdapter)
	// 	cout << "Could not set adapter. It does not exist or is not available." << endl;
	// else
	// 	cout << "Succesfully set Preferred Adapter" << endl;

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
		auto filterProfiles = device->getAvailableFilterProfiles();
		cout << filterProfiles.size() << " available filter profiles:" << endl;
		for (auto& f : filterProfiles)
			cout << f.label() << endl;

		cout << "Current profile: " << device->onboardFilterProfile().label() << endl;
		if (device->setOnboardFilterProfile(XsString("General")))
			cout << "Succesfully set profile to General" << endl;
		else
			cout << "Setting filter profile failed!" << endl;

		cout << "Setting quaternion CSV output" << endl;
		device->setLogOptions(XsLogOptions::Euler);

		XsString logFileName = XsString("logfile_") << portInfo.bluetoothAddress().replacedAll(":", "-") << ".csv";
		cout << "Enable logging to: " << logFileName.c_str() << endl;
		if (!device->enableLogging(logFileName))
			cout << "Failed to enable logging. Reason: " << manager->lastResultText() << endl;

		cout << "Putting device into measurement mode." << endl;
		if (!device->startMeasurement(XsPayloadMode::ExtendedEuler))
		{
			cout << "Could not put device into measurement mode. Reason: " << manager->lastResultText() << endl;
			continue;
		}
	}

	cout << "\nMain loop. Logging data for 10 seconds." << endl;
	cout << string(83, '-') << endl;

	// First printing some headers so we see which data belongs to which device
	for (auto const& device : deviceList)
		cout << setw(42) << left << device->portInfo().bluetoothAddress();
	cout << endl;

	bool orientationResetDone = false;
	startTime = XsTime::timeStampNow();
	while (XsTime::timeStampNow() - startTime <= 10000000)
	{
		if (callback.packetsAvailable())
		{
			cout << "\r";
			for (auto const& device : deviceList)
			{
				// Retrieve a packet
				XsDataPacket packet = callback.getNextPacket(device->portInfo().bluetoothAddress());

				if (packet.containsOrientation())
				{
					XsEuler euler = packet.orientationEuler();

					cout << "Roll:" << right << setw(7) << fixed << setprecision(2) << euler.roll()
						<< ", Pitch:" << right << setw(7) << fixed << setprecision(2) << euler.pitch()
						<< ", Yaw:" << right << setw(7) << fixed << setprecision(2) << euler.yaw()
						<< "| ";


					// Create a vector to store MIDI message
					std::vector<unsigned char> message;

					// Create a Note On message (Channel 1, Middle C, Velocity 100)
					message.push_back(0xB0);  // Control Change message on channel 1
					message.push_back(9);    // CC Number
					message.push_back(((euler.roll() + 180) / 360.0) * 127.0);   // Velocity

					// Send the message
					midiOut.sendMessage(&message);
				}
			}
			cout << flush;
			if (!orientationResetDone && (XsTime::timeStampNow() - startTime) > 5000)
			{
				for (auto const& device : deviceList)
				{
					cout << endl << "Resetting heading for device " << device->portInfo().bluetoothAddress() << ": ";
					if (device->resetOrientation(XRM_Heading))
						cout << "OK";
					else
						cout << "NOK: " << device->lastResultText();
				}
				cout << endl;
				orientationResetDone = true;
			}
		}
		XsTime::msleep(0);
	}
	cout << "\n" << string(83, '-') << "\n";
	cout << endl;

	for (auto const& device : deviceList)
	{
		cout << endl << "Resetting heading to default for device " << device->portInfo().bluetoothAddress() << ": ";
		if (device->resetOrientation(XRM_DefaultAlignment))
			cout << "OK";
		else
			cout << "NOK: " << device->lastResultText();
	}
	cout << endl << endl;

	cout << "Stopping measurement..." << endl;
	for (auto device : deviceList)
	{
		if (!device->stopMeasurement())
			cout << "Failed to stop measurement.";

		if (!device->disableLogging())
			cout << "Failed to disable logging.";
	}

	cout << "Closing ports..." << endl;
	manager->close();

	cout << "Freeing XsDotConnectionManager object..." << endl;
	manager->destruct();

	cout << "Successful exit." << endl;

	return 0;
}
