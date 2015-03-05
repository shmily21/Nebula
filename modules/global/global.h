/*
***************************************************************************************************
// FileName     :global.h
//
// Brief        :Global Header
//
// Author       :WangBin
//
// Description  :
//                  Global Header.
//
//
// CopyRight    : WangBin.
//                                              <perturbed@sina.com>
//
***************************************************************************************************
*/

#ifndef __WANGBIN_NEBULA_MODULES_GLOBAL_GLOBAL_H__
#define __WANGBIN_NEBULA_MODULES_GLOBAL_GLOBAL_H__

/*
* For Microsoft Compiler In Windows.
*/
#if defined(_MSC_VER) && _MSC_VER > 1000
#   pragma once
#endif // if defined(_MSC_VER) && _MSC_VER > 1000

/*
* Use Namespace: Nebula
*/
#define __NEBULA_GLOBAL_USE_NAMESPACE

/*
* Use Exceptions
*/
#define __NEBULA_USE_EXCEPTIONS

#include "include/nebulaconfig.h"
#include "include/nebulatype.h"
#include "include/nebulabase.h"

#endif // __WANGBIN_NEBULA_MODULES_GLOBAL_GLOBAL_H__