/*
***************************************************************************************************
// FileName     :preheader.h
//
// Brief        :Pre Header
//
// Author       :WangBin
//
// Description  :
//                  Pre Header.
//
//
// CopyRight    : WangBin.
//                                              <perturbed@sina.com>
//
***************************************************************************************************
*/

#ifndef __WANGBIN_NEBULA_MODULES_ALLOCATE_INCLUDE_PREHEADER_H__
#define __WANGBIN_NEBULA_MODULES_ALLOCATE_INCLUDE_PREHEADER_H__

/*
* For Microsoft Compiler In Windows.
*/
#if defined(_MSC_VER) && _MSC_VER > 1000
#   pragma once
#endif // if defined(_MSC_VER) && _MSC_VER > 1000

/*
* global.h ,location:modules/golbal/
* global config and type defined.
*/
#include "global.h"

#if 0
#   include <new>
#   define __THROW_NEBULA_BAD_ALLOC throw std::bad_alloc
#elif !defined(__THROW_NEBULA_BAD_ALLOC)
#   include <cstdio>
#   define __THROW_NEBULA_BAD_ALLOC std::fprintf(stderr, "Nebula Memory Error: %s\n","Out Of Memory"); \
        exit(1)
#endif

#endif // __WANGBIN_NEBULA_MODULES_ALLOCATE_INCLUDE_PREHEADER_H__