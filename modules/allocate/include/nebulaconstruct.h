/*
***************************************************************************************************
// FileName     :NebulaConstruct.h
//
// Brief        :Construct.
//
// Author       :WangBin
//
// Description  :
//                  Construct.
//
//
// CopyRight    : WangBin.
//                                              <perturbed@sina.com>
//
***************************************************************************************************
*/

#ifndef __WANGBIN_NEBULA_MODULES_ALLOCATE_INCLUDE_NEBULACONSTRUCT_H__
#define __WANGBIN_NEBULA_MODULES_ALLOCATE_INCLUDE_NEBULACONSTRUCT_H__

/*
* For Microsoft Compiler In Windows.
*/
#if defined(_MSC_VER) && _MSC_VER > 1000
#   pragma once
#endif // if defined(_MSC_VER) && _MSC_VER > 1000

#include "preheader.h"

// C++ STD Headers
#include <new>

__NEBULA_BEGIN_NAMESPACE    // Begin Namespace if Wanted

/*
*
*/
template < class _Tp_1,class _Tp_2 >
inline void construct(_Tp_1* pAddress,const _Tp_2& value)
{
    new (pAddress) _Tp_1(value);    //eq. pAddress=new _Tp_1(value)
}

/*
*
*/
template < class _Tp_1>
inline void destroy(_Tp_1* pInstance)
{
    pInstance->~_Tp_1();
}
/*
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first,_ForwardIterator __last,__flase_type)
{
    for(;__first<__last;++__first)
    {
        __NEBULA::destroy(&*__first);
    }
}

template <class _ForwardIterator,class __Tp>
inline void __destroy(_ForwardIterator __first,_ForwardIterator __last)
{
    typedef typename __NEBULA::__type_traits<__Tp>::has_trivial_destructor trival_destructor;
    __NEBULA::__destroy_aux(first,last,trival_destructor());
}

template <class _ForwardIterator>
inline void destroy(_ForwardIterator __first,_ForwardIterator __last)
{
    __NEBULA::__destroy(__first,__last);
}
*/
__NEBULA_END_NAMESPACE       //End Namespace if Any

#endif //__WANGBIN_NEBULA_MODULES_ALLOCATE_INCLUDE_NEBULACONSTRUCT_H__