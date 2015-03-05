/*
***************************************************************************************************
// FileName     :pool_allocate.h
//
// Brief        :Memory Pool Allocator.
//
// Author       :WangBin
//
// Description  :
//                  Memory Pool Allocator.
//
//
// CopyRight    : WangBin.
//                                              <perturbed@sina.com>
//
***************************************************************************************************
*/

#ifndef __WANGBIN_NEBULA_MODULES_ALLOCATE_INCLUDE_POOL__ALLOCATE_H__
#define __WANGBIN_NEBULA_MODULES_ALLOCATE_INCLUDE_POOL__ALLOCATE_H__

/*
* For Microsoft Compiler In Windows.
*/
#if defined(_MSC_VER) && _MSC_VER > 1000
#   pragma once
#endif // if defined(_MSC_VER) && _MSC_VER > 1000


#include "preheader.h"

#include <cstdlib>
#include <new>

__NEBULA_BEGIN_NAMESPACE

template<bool threads,int inst>
class __nebula_default_alloc_template
{
protected:
    /*
    * for your reading:
    * some compiler may not support outside const static. so use enum make const
    * value(most compiler are support).
    *                       add by <wangbin>
    */
    enum {__ALIGN = 8 };
    enum {__MAX_BYTES = 128 };
    enum {__NFREELISTS = (size_t)__MAX_BYTES / (size_t)__ALIGN };

};

__NEBULA_END_NAMESPACE

#endif // __WANGBIN_NEBULA_MODULES_ALLOCATE_INCLUDE_POOL__ALLOCATE_H__