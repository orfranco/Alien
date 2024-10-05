
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

#ifndef GUARD_XSDOTDEVICE_H_TMP
#define GUARD_XSDOTDEVICE_H_TMP
#include <xstypes/xsstringarray.h>
#include <xstypes/xsversion.h>
#include <xstypes/xsfilterprofile.h>
#include <xstypes/xsfilterprofilearray.h>
#include <xstypes/xsexception.h>
#include <xstypes/xsresetmethod.h>
#include <xstypes/xsresultvalue.h>
#include <xscontroller/xserrormode.h>
#include "xspayloadmode.h"
#include "xslogoptions.h"
#include "dotsdkconfig.h"
#ifdef __cplusplus
extern "C" {
#endif
/*! \addtogroup cinterface C Interface
	@{ */
struct XsDotDevice;
#ifndef __cplusplus
typedef struct XsDotDevice XsDotDevice;
#endif
XDDA_DLL_API void XsDotDevice_readDeviceControl(const struct XsDotDevice* thisPtr);/*!< \copydoc XsDotDevice::readDeviceControl() const*/
XDDA_DLL_API XsVersion* XsDotDevice_firmwareVersion(const struct XsDotDevice* thisPtr, XsVersion* returnValue);/*!< \copydoc XsDotDevice::firmwareVersion() const*/
XDDA_DLL_API XsVersion* XsDotDevice_hardwareVersion(const struct XsDotDevice* thisPtr, XsVersion* returnValue);/*!< \copydoc XsDotDevice::hardwareVersion() const*/
XDDA_DLL_API XsString* XsDotDevice_buildInfo(const struct XsDotDevice* thisPtr, XsString* returnValue);/*!< \copydoc XsDotDevice::buildInfo() const*/
XDDA_DLL_API XsString* XsDotDevice_productCode(const struct XsDotDevice* thisPtr, XsString* returnValue);/*!< \copydoc XsDotDevice::productCode() const*/
XDDA_DLL_API uint32_t XsDotDevice_softDeviceVersion(const struct XsDotDevice* thisPtr);/*!< \copydoc XsDotDevice::softDeviceVersion() const*/
XDDA_DLL_API XsString* XsDotDevice_deviceTagName(const struct XsDotDevice* thisPtr, XsString* returnValue);/*!< \copydoc XsDotDevice::deviceTagName() const*/
XDDA_DLL_API int XsDotDevice_setDeviceTagName(const struct XsDotDevice* thisPtr, XsString tagName);/*!< \copydoc XsDotDevice::setDeviceTagName(XsString) const*/
XDDA_DLL_API uint16_t XsDotDevice_outputRate(const struct XsDotDevice* thisPtr);/*!< \copydoc XsDotDevice::outputRate() const*/
XDDA_DLL_API int XsDotDevice_setOutputRate(const struct XsDotDevice* thisPtr, uint16_t outputRate);/*!< \copydoc XsDotDevice::setOutputRate(uint16_t) const*/
XDDA_DLL_API int XsDotDevice_identify(const struct XsDotDevice* thisPtr);/*!< \copydoc XsDotDevice::identify() const*/
XDDA_DLL_API int XsDotDevice_powerOff(const struct XsDotDevice* thisPtr);/*!< \copydoc XsDotDevice::powerOff() const*/
XDDA_DLL_API int XsDotDevice_setPowerOnByUsb(struct XsDotDevice* thisPtr, int enable);/*!< \copydoc XsDotDevice::setPowerOnByUsb(bool)*/
XDDA_DLL_API int XsDotDevice_isCharging(const struct XsDotDevice* thisPtr);/*!< \copydoc XsDotDevice::isCharging() const*/
XDDA_DLL_API XsFilterProfile* XsDotDevice_onboardFilterProfile(struct XsDotDevice* thisPtr, XsFilterProfile* returnValue);/*!< \copydoc XsDotDevice::onboardFilterProfile()*/
XDDA_DLL_API int XsDotDevice_setOnboardFilterProfile(struct XsDotDevice* thisPtr, const XsString* profileType);/*!< \copydoc XsDotDevice::setOnboardFilterProfile(const XsString&)*/
XDDA_DLL_API XsFilterProfileArray* XsDotDevice_getAvailableFilterProfiles(struct XsDotDevice* thisPtr, XsFilterProfileArray* returnValue);/*!< \copydoc XsDotDevice::getAvailableFilterProfiles()*/
XDDA_DLL_API XsStringArray* XsDotDevice_getCharacteristicStrings(const struct XsDotDevice* thisPtr, XsStringArray* returnValue);/*!< \copydoc XsDotDevice::getCharacteristicStrings() const*/
XDDA_DLL_API int XsDotDevice_restoreFactoryDefaults(struct XsDotDevice* thisPtr);/*!< \copydoc XsDotDevice::restoreFactoryDefaults()*/
XDDA_DLL_API XsPayloadMode XsDotDevice_payloadMode(struct XsDotDevice* thisPtr);/*!< \copydoc XsDotDevice::payloadMode()*/
XDDA_DLL_API int XsDotDevice_startMeasurement(struct XsDotDevice* thisPtr, XsPayloadMode mode);/*!< \copydoc XsDotDevice::startMeasurement(XsPayloadMode)*/
XDDA_DLL_API int XsDotDevice_stopMeasurement(struct XsDotDevice* thisPtr);/*!< \copydoc XsDotDevice::stopMeasurement()*/
XDDA_DLL_API int XsDotDevice_resetOrientation(struct XsDotDevice* thisPtr, XsResetMethod resetMethod);/*!< \copydoc XsDotDevice::resetOrientation(XsResetMethod)*/
XDDA_DLL_API XsVersion* XsDotDevice_checkForFirmwareUpdate(struct XsDotDevice* thisPtr, XsVersion* returnValue);/*!< \copydoc XsDotDevice::checkForFirmwareUpdate()*/
XDDA_DLL_API XsVersion* XsDotDevice_startFirmwareUpdateFromServer(struct XsDotDevice* thisPtr, XsVersion* returnValue);/*!< \copydoc XsDotDevice::startFirmwareUpdateFromServer()*/
XDDA_DLL_API void XsDotDevice_startFirmwareUpdateFromFile(struct XsDotDevice* thisPtr, const XsString* firmwareFile);/*!< \copydoc XsDotDevice::startFirmwareUpdateFromFile(const XsString&)*/
XDDA_DLL_API int XsDotDevice_gotoConfig(struct XsDotDevice* thisPtr);/*!< \copydoc XsDotDevice::gotoConfig()*/
XDDA_DLL_API int XsDotDevice_enableLogging(struct XsDotDevice* thisPtr, const XsString* filename);/*!< \copydoc XsDotDevice::enableLogging(const XsString&)*/
XDDA_DLL_API int XsDotDevice_disableLogging(struct XsDotDevice* thisPtr);/*!< \copydoc XsDotDevice::disableLogging()*/
XDDA_DLL_API void XsDotDevice_setLogOptions(struct XsDotDevice* thisPtr, XsLogOptions options);/*!< \copydoc XsDotDevice::setLogOptions(XsLogOptions)*/
XDDA_DLL_API void XsDotDevice_writeDeviceSettingsToFile(struct XsDotDevice* thisPtr, XsPayloadMode mode);/*!< \copydoc XsDotDevice::writeDeviceSettingsToFile(XsPayloadMode)*/
XDDA_DLL_API XsString* XsDotDevice_portName(const struct XsDotDevice* thisPtr, XsString* returnValue);/*!< \copydoc XsDotDevice::portName() const*/
XDDA_DLL_API XsPortInfo* XsDotDevice_portInfo(const struct XsDotDevice* thisPtr, XsPortInfo* returnValue);/*!< \copydoc XsDotDevice::portInfo() const*/
XDDA_DLL_API XsDeviceId* XsDotDevice_deviceId(const struct XsDotDevice* thisPtr, XsDeviceId* returnValue);/*!< \copydoc XsDotDevice::deviceId() const*/
XDDA_DLL_API XsResultValue XsDotDevice_lastResult(const struct XsDotDevice* thisPtr);/*!< \copydoc XsDotDevice::lastResult() const*/
XDDA_DLL_API XsString* XsDotDevice_lastResultText(const struct XsDotDevice* thisPtr, XsString* returnValue);/*!< \copydoc XsDotDevice::lastResultText() const*/
XDDA_DLL_API void XsDotDevice_addRef(struct XsDotDevice* thisPtr);/*!< \copydoc XsDotDevice::addRef()*/
XDDA_DLL_API void XsDotDevice_removeRef(struct XsDotDevice* thisPtr);/*!< \copydoc XsDotDevice::removeRef()*/
XDDA_DLL_API XsSize XsDotDevice_refCounter(const struct XsDotDevice* thisPtr);/*!< \copydoc XsDotDevice::refCounter() const*/
XDDA_DLL_API void XsDotDevice_clearXsDotCallbackHandlers(struct XsDotDevice* thisPtr, int chain);/*!< \copydoc XsDotDevice::clearXsDotCallbackHandlers(bool)*/
XDDA_DLL_API void XsDotDevice_addXsDotCallbackHandler(struct XsDotDevice* thisPtr, XsDotCallbackPlainC* cb, int chain);/*!< \copydoc XsDotDevice::addXsDotCallbackHandler(XsDotCallbackPlainC*,bool)*/
XDDA_DLL_API void XsDotDevice_removeXsDotCallbackHandler(struct XsDotDevice* thisPtr, XsDotCallbackPlainC* cb, int chain);/*!< \copydoc XsDotDevice::removeXsDotCallbackHandler(XsDotCallbackPlainC*,bool)*/
/*! @} */
#ifdef __cplusplus
} // extern "C"
struct XsDotDevice {
	inline void readDeviceControl(void) const
	{
		XsDotDevice_readDeviceControl(this);
	}

	inline XsVersion firmwareVersion(void) const
	{
		XsVersion returnValue;
		return *XsDotDevice_firmwareVersion(this, &returnValue);
	}

	inline XsVersion hardwareVersion(void) const
	{
		XsVersion returnValue;
		return *XsDotDevice_hardwareVersion(this, &returnValue);
	}

	inline XsString buildInfo(void) const
	{
		XsString returnValue;
		return *XsDotDevice_buildInfo(this, &returnValue);
	}

	inline XsString productCode(void) const
	{
		XsString returnValue;
		return *XsDotDevice_productCode(this, &returnValue);
	}

	inline uint32_t softDeviceVersion(void) const
	{
		return XsDotDevice_softDeviceVersion(this);
	}

	inline XsString deviceTagName(void) const
	{
		XsString returnValue;
		return *XsDotDevice_deviceTagName(this, &returnValue);
	}

	inline bool setDeviceTagName(XsString tagName) const
	{
		return 0 != XsDotDevice_setDeviceTagName(this, tagName);
	}

	inline uint16_t outputRate(void) const
	{
		return XsDotDevice_outputRate(this);
	}

	inline bool setOutputRate(uint16_t outputRate) const
	{
		return 0 != XsDotDevice_setOutputRate(this, outputRate);
	}

	inline bool identify(void) const
	{
		return 0 != XsDotDevice_identify(this);
	}

	inline bool powerOff(void) const
	{
		return 0 != XsDotDevice_powerOff(this);
	}

	inline bool setPowerOnByUsb(bool enable)
	{
		return 0 != XsDotDevice_setPowerOnByUsb(this, enable);
	}

	inline bool isCharging(void) const
	{
		return 0 != XsDotDevice_isCharging(this);
	}

	inline XsFilterProfile onboardFilterProfile(void)
	{
		XsFilterProfile returnValue;
		return *XsDotDevice_onboardFilterProfile(this, &returnValue);
	}

	inline bool setOnboardFilterProfile(const XsString& profileType)
	{
		return 0 != XsDotDevice_setOnboardFilterProfile(this, &profileType);
	}

	inline XsFilterProfileArray getAvailableFilterProfiles(void)
	{
		XsFilterProfileArray returnValue;
		return *XsDotDevice_getAvailableFilterProfiles(this, &returnValue);
	}

	inline XsStringArray getCharacteristicStrings(void) const
	{
		XsStringArray returnValue;
		return *XsDotDevice_getCharacteristicStrings(this, &returnValue);
	}

	inline bool restoreFactoryDefaults(void)
	{
		return 0 != XsDotDevice_restoreFactoryDefaults(this);
	}

	inline XsPayloadMode payloadMode(void)
	{
		return XsDotDevice_payloadMode(this);
	}

	inline bool startMeasurement(XsPayloadMode mode)
	{
		return 0 != XsDotDevice_startMeasurement(this, mode);
	}

	inline bool stopMeasurement(void)
	{
		return 0 != XsDotDevice_stopMeasurement(this);
	}

	inline bool resetOrientation(XsResetMethod resetMethod)
	{
		return 0 != XsDotDevice_resetOrientation(this, resetMethod);
	}

	inline XsVersion checkForFirmwareUpdate(void)
	{
		XsVersion returnValue;
		return *XsDotDevice_checkForFirmwareUpdate(this, &returnValue);
	}

	inline XsVersion startFirmwareUpdateFromServer(void)
	{
		XsVersion returnValue;
		return *XsDotDevice_startFirmwareUpdateFromServer(this, &returnValue);
	}

	inline void startFirmwareUpdateFromFile(const XsString& firmwareFile)
	{
		XsDotDevice_startFirmwareUpdateFromFile(this, &firmwareFile);
	}

	inline bool gotoConfig(void)
	{
		return 0 != XsDotDevice_gotoConfig(this);
	}

	inline bool enableLogging(const XsString& filename)
	{
		return 0 != XsDotDevice_enableLogging(this, &filename);
	}

	inline bool disableLogging(void)
	{
		return 0 != XsDotDevice_disableLogging(this);
	}

	inline void setLogOptions(XsLogOptions options)
	{
		XsDotDevice_setLogOptions(this, options);
	}

	inline void writeDeviceSettingsToFile(XsPayloadMode mode)
	{
		XsDotDevice_writeDeviceSettingsToFile(this, mode);
	}

	inline XsString portName(void) const
	{
		XsString returnValue;
		return *XsDotDevice_portName(this, &returnValue);
	}

	inline XsPortInfo portInfo(void) const
	{
		XsPortInfo returnValue;
		return *XsDotDevice_portInfo(this, &returnValue);
	}

	inline XsDeviceId deviceId(void) const
	{
		XsDeviceId returnValue;
		return *XsDotDevice_deviceId(this, &returnValue);
	}

	inline XsResultValue lastResult(void) const
	{
		return XsDotDevice_lastResult(this);
	}

	inline XsString lastResultText(void) const
	{
		XsString returnValue;
		return *XsDotDevice_lastResultText(this, &returnValue);
	}

	inline void addRef(void)
	{
		XsDotDevice_addRef(this);
	}

	inline void removeRef(void)
	{
		XsDotDevice_removeRef(this);
	}

	inline XsSize refCounter(void) const
	{
		return XsDotDevice_refCounter(this);
	}

	inline void clearXsDotCallbackHandlers(bool chain = true)
	{
		XsDotDevice_clearXsDotCallbackHandlers(this, chain);
	}

	inline void addXsDotCallbackHandler(XsDotCallbackPlainC* cb, bool chain = true)
	{
		XsDotDevice_addXsDotCallbackHandler(this, cb, chain);
	}

	inline void removeXsDotCallbackHandler(XsDotCallbackPlainC* cb, bool chain = true)
	{
		XsDotDevice_removeXsDotCallbackHandler(this, cb, chain);
	}

private:
	XsDotDevice(); // Constructor not implemented to prevent faulty memory allocation, object needs to be created by another object
	~XsDotDevice(); //!< \brief Destructor not implemented, use destruct function instead (if available)
#ifndef SWIG
	void* operator new (size_t); //!< \brief new operator not implemented to prevent faulty memory allocation by user, use construct() function instead
	void* operator new[] (size_t); //!< \brief array new operator not implemented to prevent faulty memory allocation by user, use construct() function instead
	void operator delete (void*); //!< \brief delete operator not implemented to prevent faulty memory deallocation by user, use destruct() function instead
	void operator delete[] (void*); //!< \brief array delete operator not implemented to prevent faulty memory deallocation by user, use destruct() function instead
#endif
};
#endif // __cplusplus
#endif // GUARD_XSDOTDEVICE_H_TMP
