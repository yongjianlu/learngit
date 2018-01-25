

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Tue Mar 14 17:52:26 2017
 */
/* Compiler settings for .\StatusLamp.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_StatusLampLib,0x719C8216,0xBFF6,0x4A40,0xA6,0x46,0x03,0x18,0xC1,0x7E,0xBD,0xDE);


MIDL_DEFINE_GUID(IID, DIID__DStatusLamp,0xCD85728B,0x92CA,0x4397,0xA2,0xBF,0xF7,0x02,0x31,0x09,0xE9,0x6C);


MIDL_DEFINE_GUID(IID, DIID__DStatusLampEvents,0x96B7E964,0xD07D,0x419E,0x94,0x04,0xA7,0xFE,0x5F,0xB9,0x46,0xBB);


MIDL_DEFINE_GUID(CLSID, CLSID_StatusLamp,0x45A71C0E,0xE00C,0x4D19,0x99,0x1E,0xF6,0xC0,0x85,0x95,0x53,0x32);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



