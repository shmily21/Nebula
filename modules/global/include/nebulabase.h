/*
***************************************************************************************************
// FileName     :nebulabase.h
//
// Brief        :Nebula Base Function.
//
// Author       :WangBin
//
// Description  :
//                  Nebula Base Function.
//
//
// CopyRight    : WangBin.
//                                              <perturbed@sina.com>
//
***************************************************************************************************
*/

#ifndef __WANGBIN_NEBULA_MODULES_GLOBAL_INCLUDE_NEBULABASE_H__
#define __WANGBIN_NEBULA_MODULES_GLOBAL_INCLUDE_NEBULABASE_H__

/*
* For Microsoft Compiler In Windows.
*/
#if defined(_MSC_VER) && _MSC_VER > 1000
#   pragma once
#endif // if defined(_MSC_VER) && _MSC_VER > 1000

#include "nebulaconfig.h"

__NEBULA_BEGIN_NAMESPACE

template<typename _Tp>
inline _Tp* __nebula_addressof(_Tp& __pa) __NEBULA_NOEXCEPT
{
    return reinterpret_cast<_Tp*>(
        &const_cast<char&>(
            reinterpret_cast<const volatile char&>(__pa)
            )
        );
}

__NEBULA_END_NAMESPACE

#endif // __WANGBIN_NEBULA_MODULES_GLOBAL_INCLUDE_NEBULABASE_H__