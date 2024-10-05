
//  WARNING: COPYRIGHT (C) 2022 MOVELLA TECHNOLOGIES OR SUBSIDIARIES WORLDWIDE. ALL RIGHTS RESERVED.
//  THIS FILE AND THE SOURCE CODE IT CONTAINS (AND/OR THE BINARY CODE FILES FOUND IN THE SAME
//  FOLDER THAT CONTAINS THIS FILE) AND ALL RELATED SOFTWARE (COLLECTIVELY, "CODE") ARE SUBJECT
//  TO A RESTRICTED LICENSE AGREEMENT ("AGREEMENT") BETWEEN XSENS AS LICENSOR AND THE AUTHORIZED
//  LICENSEE UNDER THE AGREEMENT. THE CODE MUST BE USED SOLELY WITH XSENS PRODUCTS INCORPORATED
//  INTO LICENSEE PRODUCTS IN ACCORDANCE WITH THE AGREEMENT. ANY USE, MODIFICATION, COPYING OR
//  DISTRIBUTION OF THE CODE IS STRICTLY PROHIBITED UNLESS EXPRESSLY AUTHORIZED BY THE AGREEMENT.
//  IF YOU ARE NOT AN AUTHORIZED USER OF THE CODE IN ACCORDANCE WITH THE AGREEMENT, YOU MUST STOP
//  USING OR VIEWING THE CODE NOW, REMOVE ANY COPIES OF THE CODE FROM YOUR COMPUTER AND NOTIFY
//  XSENS IMMEDIATELY BY EMAIL TO INFO@XSENS.COM. ANY COPIES OR DERIVATIVES OF THE CODE (IN WHOLE
//  OR IN PART) IN SOURCE CODE FORM THAT ARE PERMITTED BY THE AGREEMENT MUST RETAIN THE ABOVE
//  COPYRIGHT NOTICE AND THIS PARAGRAPH IN ITS ENTIRETY, AS REQUIRED BY THE AGREEMENT.
//  

#ifndef XSDOTCALLBACK_H
#define XSDOTCALLBACK_H

#include "xsdotcallbackplainc.h"

#ifdef __cplusplus

/*! \brief Structure that contains callback functions for the Xsens DOT PC SDK API
	\details When programming in C++, simply overload the callback that you want to use and supply
	your XsDotCallbackPlainC-overloaded class to one of the setCallback functions

	When programming in C, create an XsDotCallbackPlainC structure and initialize each function pointer to
	the right function to call. The supplied first parameter is the address of the XsCallbackPlainC
	object that you supplied to the setCallback function. If you do not wish to receive a specific
	callback in C, set the function pointer to 0.

	In both cases, the calling application remains in control of the XsDotCallbackPlainC object and thus
	remains responsible for cleaning it up when the it is no longer necessary.

	\note Any callback function in %XsDotCallback that is not overloaded will only be called once to
	minimize callback overhead.
*/
class XsDotCallback : public XsDotCallbackPlainC
{
public:
	/*! \brief Constructor */
	XsDotCallback()
	{
		m_onAdvertisementFound = sOnAdvertisementFound;
		m_onBatteryUpdated = sonBatteryUpdated;
		m_onLiveDataAvailable = sonLiveDataAvailable;
		m_onProgressUpdated = sonProgressUpdated;
		m_onDeviceUpdateDone = sonDeviceUpdateDone;
		m_onError = sOnError;
	}

	/*! \brief Destructor
		\note Make sure that the object is removed from callback generating objects before destroying it!
	*/
	virtual ~XsDotCallback() {}

// Swig needs these functions to be protected, not private, otherwise they are ignored.
protected:
/*! \protectedsection
	\addtogroup Callbacks
	@{
*/
	//! \copydoc m_onAdvertisementFound
	virtual void onAdvertisementFound(const XsPortInfo* portInfo)
	{
		(void)portInfo;
		m_onAdvertisementFound = 0;
	}
	//! \copydoc m_onBatteryUpdated
	virtual void onBatteryUpdated(XsDotDevice* device, int batteryLevel, int chargingStatus)
	{
		(void)device;
		(void)batteryLevel;
		(void)chargingStatus;
		m_onBatteryUpdated = 0;
	}
	//! \copybrief m_onLiveDataAvailable
	virtual void onLiveDataAvailable(XsDotDevice* device, const XsDataPacket* packet)
	{
		(void)device;
		(void)packet;
		m_onLiveDataAvailable = 0;
	}
	//! \copybrief m_onProgressUpdated
	virtual void onProgressUpdated(XsDotDevice* device, int current, int total, const XsString* identifier)
	{
		(void)device;
		(void)current;
		(void)total;
		(void)identifier;
		m_onProgressUpdated = 0;
	}
	//! \copybrief m_onDeviceUpdateDone
	virtual void onDeviceUpdateDone(const XsPortInfo* portInfo, XsDotFirmwareUpdateResult result)
	{
		(void)portInfo;
		(void)result;
	}
	//! \copydoc m_onError
	virtual void onError(const XsString* error)
	{
		(void)error;
		m_onError = 0;
	}
//! @}

private:
/*! \privatesection */
	static void sOnAdvertisementFound(XsDotCallbackPlainC* cb, const XsPortInfo* portInfo) { ((XsDotCallback*)cb)->onAdvertisementFound(portInfo); }
	static void sonBatteryUpdated(XsDotCallbackPlainC* cb, XsDotDevice* device, int batteryLevel, int chargingStatus)
	{
		((XsDotCallback*)cb)->onBatteryUpdated(device, batteryLevel, chargingStatus);
	}
	static void sonLiveDataAvailable(XsDotCallbackPlainC* cb, XsDotDevice* device, const XsDataPacket* packet)
	{
		((XsDotCallback*)cb)->onLiveDataAvailable(device, packet);
	}
	static void sonProgressUpdated(XsDotCallbackPlainC* cb, XsDotDevice* device, int current, int total, const XsString* identifier)
	{
		((XsDotCallback*)cb)->onProgressUpdated(device, current, total, identifier);
	}
	static void sonDeviceUpdateDone(XsDotCallbackPlainC* cb, const XsPortInfo* portInfo, XsDotFirmwareUpdateResult result)
	{
		((XsDotCallback*)cb)->onDeviceUpdateDone(portInfo, result);
	}
	static void sOnError(XsDotCallbackPlainC* cb, const XsString* error) { ((XsDotCallback*)cb)->onError(error); }
};

#endif

#endif
