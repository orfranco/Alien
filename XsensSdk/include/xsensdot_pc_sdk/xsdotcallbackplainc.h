
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

#ifndef XSDOTCALLBACKPLAINC_H
#define XSDOTCALLBACKPLAINC_H

#include <xstypes/pstdint.h>
#include <xstypes/xsportinfo.h>
#include <xstypes/xsdeviceidarray.h>
#include <xstypes/xsdatapacketptrarray.h>
#include "xsdotfirmwareupdateresult.h"

#ifndef __cplusplus
#define XSDOTCALLBACK_INITIALIZER		{ 0 }
#endif

struct XsDotDevicePtrArray;

/*! \brief Structure that contains callback functions for the Xsens DOT PC SDK API
	\details When using C++, please use the overloaded class XsDotCallback instead.

	This structure contains pointers to functions that will be called by
	the xsens dot pc sdk library when certain events occur. To use it
	in C, set any callback you do not wish to use to 0 and put a valid
	function pointer in the others. Then pass the object to the XsDotConnectionManager
	object's addXsDotCallbackHandler function.

	\note XsDotConnectionManager does not copy the structure contents and does not take
	ownership of it. So make sure it is allocated on the heap or at least
	removed from wherever it was added by calling removeXsDotCallbackHandler
	before it is destroyed.
*/
typedef struct XsDotCallbackPlainC
{
/*! \defgroup Callbacks Callback functions.
	\addtogroup Callbacks
	@{
*/
	/*! \brief Called when an Xsens DOT device advertisement was received
		\param portInfo The XsPortInfo of the discovered information
	*/
	void (*m_onAdvertisementFound)(struct XsDotCallbackPlainC* thisPtr, const struct XsPortInfo* portInfo);

	/*! \brief Called when a battery status update is available
		\param device The device that initiated the callback. This may be 0 in some cases
		\param batteryLevel The battery level in percentage
		\param chargingStatus The charging status of the battery. 0: Not charging, 1: charging
	*/
	void (*m_onBatteryUpdated)(struct XsDotCallbackPlainC* thisPtr, struct XsDotDevice* device, int batteryLevel, int chargingStatus);

	/*! \brief Called when new data has been received from a device or read from a file.
		\details This callback is for the Live stream, so there may be gaps in the data, but it will always contain the latest data.
		\param device The device that initiated the callback.
		\param packet The data packet that has been received (and processed). This may be 0 when the callback originates from a non-device, such as the XsDataBundler.
	*/
	void (*m_onLiveDataAvailable)(struct XsDotCallbackPlainC* thisPtr, struct XsDotDevice* device, const struct XsDataPacket* packet);

	/*! \brief Called when a long-duration operation has made some progress or has completed. Examples include device firmware upgrades. When \a current == \a total the operation has completed.
		\param device The device that initiated the callback.
		\param current The current progress.
		\param total The total work to be done. When \a current equals \a total, the task is completed.
		\param identifier An identifier for the task. This may for example be a filename for file read operations.
	*/
	void (*m_onProgressUpdated)(struct XsDotCallbackPlainC* thisPtr, struct XsDotDevice* device, int current, int total, const struct XsString* identifier);

	/*! \brief Called when the firmware update process has completed
		\param portInfo The XsPortInfo of the updated device
		\param result The XsDotFirmwareUpdateResult of the firmware update
	*/
	void (*m_onDeviceUpdateDone)(struct XsDotCallbackPlainC* thisPtr, const struct XsPortInfo* portInfo, XsDotFirmwareUpdateResult result);

	/*! \brief Called when an internal error has occurred
		\param error The error string with information on the problem that occurred
	*/
	void (*m_onError)(struct XsDotCallbackPlainC* thisPtr, const struct XsString* error);
//! @}

#ifdef __cplusplus
	// Make sure that this struct is not used in C++ (except as base class for XsCallback)
	friend class XsDotCallback;
	XsDotCallbackPlainC()
		: m_onAdvertisementFound(nullptr)
		, m_onBatteryUpdated(nullptr)
		, m_onLiveDataAvailable(nullptr)
		, m_onProgressUpdated(nullptr)
		, m_onDeviceUpdateDone(nullptr)
		, m_onError(nullptr)
	{}
	~XsDotCallbackPlainC() noexcept {}
private:
	XsDotCallbackPlainC(XsDotCallbackPlainC const &);
	XsDotCallbackPlainC& operator = (XsDotCallbackPlainC const&);

#endif

} XsDotCallbackPlainC;

#endif
