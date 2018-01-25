

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __StatusLampidl_h__
#define __StatusLampidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DStatusLamp_FWD_DEFINED__
#define ___DStatusLamp_FWD_DEFINED__
typedef interface _DStatusLamp _DStatusLamp;
#endif 	/* ___DStatusLamp_FWD_DEFINED__ */


#ifndef ___DStatusLampEvents_FWD_DEFINED__
#define ___DStatusLampEvents_FWD_DEFINED__
typedef interface _DStatusLampEvents _DStatusLampEvents;
#endif 	/* ___DStatusLampEvents_FWD_DEFINED__ */


#ifndef __StatusLamp_FWD_DEFINED__
#define __StatusLamp_FWD_DEFINED__

#ifdef __cplusplus
typedef class StatusLamp StatusLamp;
#else
typedef struct StatusLamp StatusLamp;
#endif /* __cplusplus */

#endif 	/* __StatusLamp_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __StatusLampLib_LIBRARY_DEFINED__
#define __StatusLampLib_LIBRARY_DEFINED__

/* library StatusLampLib */
/* [control][helpstring][helpfile][version][uuid] */ 


EXTERN_C const IID LIBID_StatusLampLib;

#ifndef ___DStatusLamp_DISPINTERFACE_DEFINED__
#define ___DStatusLamp_DISPINTERFACE_DEFINED__

/* dispinterface _DStatusLamp */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DStatusLamp;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("CD85728B-92CA-4397-A2BF-F7023109E96C")
    _DStatusLamp : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DStatusLampVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DStatusLamp * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DStatusLamp * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DStatusLamp * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DStatusLamp * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DStatusLamp * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DStatusLamp * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DStatusLamp * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DStatusLampVtbl;

    interface _DStatusLamp
    {
        CONST_VTBL struct _DStatusLampVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DStatusLamp_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DStatusLamp_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DStatusLamp_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DStatusLamp_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DStatusLamp_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DStatusLamp_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DStatusLamp_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DStatusLamp_DISPINTERFACE_DEFINED__ */


#ifndef ___DStatusLampEvents_DISPINTERFACE_DEFINED__
#define ___DStatusLampEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DStatusLampEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DStatusLampEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("96B7E964-D07D-419E-9404-A7FE5FB946BB")
    _DStatusLampEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DStatusLampEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DStatusLampEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DStatusLampEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DStatusLampEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DStatusLampEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DStatusLampEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DStatusLampEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DStatusLampEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DStatusLampEventsVtbl;

    interface _DStatusLampEvents
    {
        CONST_VTBL struct _DStatusLampEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DStatusLampEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DStatusLampEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DStatusLampEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DStatusLampEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DStatusLampEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DStatusLampEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DStatusLampEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DStatusLampEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_StatusLamp;

#ifdef __cplusplus

class DECLSPEC_UUID("45A71C0E-E00C-4D19-991E-F6C085955332")
StatusLamp;
#endif
#endif /* __StatusLampLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


