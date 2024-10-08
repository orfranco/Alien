
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

#ifndef XSDOTDEVICEPTRARRAY_H
#define XSDOTDEVICEPTRARRAY_H

#include "dotsdkconfig.h"
#include "xsdotdeviceptr.h"
#include <xstypes/xsarray.h>

#ifdef __cplusplus
extern "C" {
#endif

extern XsArrayDescriptor const XDDA_DLL_API g_xsDotDevicePtrArrayDescriptor;

#ifndef __cplusplus
#define XSDOTDEVICEPTRARRAY_INITIALIZER	XSARRAY_INITIALIZER(&g_xsDotDevicePtrArrayDescriptor)

XSARRAY_STRUCT(XsDotDevicePtrArray, XsDotDevicePtr);
typedef struct XsDotDevicePtrArray XsDotDevicePtrArray;

XDDA_DLL_API void XsDotDevicePtrArray_construct(XsDotDevicePtrArray* thisPtr, XsSize count, XsDotDevicePtr const* src);
#else
} // extern "C"
#endif

#ifdef __cplusplus
	struct XsDotDevicePtrArray : public XsArrayImpl<XsDotDevicePtr, g_xsDotDevicePtrArrayDescriptor, XsDotDevicePtrArray>
	{
		//! \brief Constructs an XsDotDevicePtrArray
		inline explicit XsDotDevicePtrArray(XsSize sz = 0, XsDotDevicePtr const* src = 0)
			: ArrayImpl(sz, src)
		{
		}

		//! \brief Constructs an XsDotDevicePtrArray as a copy of \a other
		inline XsDotDevicePtrArray(XsDotDevicePtrArray const& other)
			: ArrayImpl(other)
		{
		}

		//! \brief Constructs an XsDotDevicePtrArray that references the data supplied in \a ref
		inline explicit XsDotDevicePtrArray(XsDotDevicePtr* ref, XsSize sz, XsDataFlags flags /* = XSDF_None */)
			: ArrayImpl(ref, sz, flags)
		{
		}

#ifndef XSENS_NOITERATOR
		//! \brief Constructs an XsDotDevicePtrArray with the array bound by the supplied iterators \a beginIt and \a endIt
		template <typename Iterator>
		inline XsDotDevicePtrArray(Iterator beginIt, Iterator endIt)
			: ArrayImpl(beginIt, endIt)
		{
		}
#endif
	};
#endif
#endif
