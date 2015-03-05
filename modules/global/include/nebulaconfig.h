/*
***************************************************************************************************
// FileName     :NebulaConfig.h
//
// Brief        :Common Config or Macro Defined Here.
//
// Author       :WangBin
//
// Description  :
//                  Common Config or Macro Defined Here.
//
//
// CopyRight    : WangBin.
//                                              <perturbed@sina.com>
//
***************************************************************************************************
*/

#ifndef __WANGBIN_NEBULA_MODULES_GLOBAL_INCLUDE_NEBULACONFIG_H__
#define __WANGBIN_NEBULA_MODULES_GLOBAL_INCLUDE_NEBULACONFIG_H__

/*
* For Microsoft Compiler In Windows.
*/
#if defined(_MSC_VER) && _MSC_VER > 1000
#   pragma once
#endif // if defined(_MSC_VER) && _MSC_VER > 1000

/*
* For enable or disable namespace without having to edit library headers
*/
#if defined(__NEBULA_GLOBAL_USE_NAMESPACE) && !defined(__NEBULA_GLOBAL_NO_NAMESPACE)
#   define __NEBULA Nebula
#   define __NEBULA_BEGIN_NAMESPACE namespace Nebula {
#   define __NEBULA_END_NAMESPACE }
#   define __NEBULA_USE_NAMESPACE_FOR_RELOPS
#   define __NEBULA_BEGIN_RELOPS_NAMESPACE namespace Nebula {
#   define __NEBULA_END_RELOPS_NAMESPACE }
#   define __NEBULA_RELOPS Nebula
#else
#   define __NEBULA
#   define __NEBULA_BEGIN_NAMESPACE
#   define __NEBULA_END_NAMESPACE
#   define __NEBULA_USE_NAMESPACE_FOR_RELOPS
#   define __NEBULA_BEGIN_RELOPS_NAMESPACE
#   define __NEBULA_END_RELOPS_NAMESPACE
#   define __NEBULA_RELOPS
#endif // defined(__NEBULA_GLOBAL_USE_NAMESPACE) && !defined(__NEBULA_GLOBAL_NO_NAMESPACE)

/*
* If we use exception ,define __NEBULA_USE_EXCEPTIONS,else do not define it,
* and as cmake guess in CMakeLists.txt,if the compiler support c++11 keywork:noexcept,use it.
*/
#ifdef __NEBULA_USE_EXCEPTIONS
#   define __NEBULA_TRY try
#   define __NEBULA_CATCH_ALL catch(...) // eg. catch(std::exception& e)
#   define __NEBULA_RETHROW throw
    // If the Compiler Support C++11,use new keyword:noexcept
#   ifdef HAVE_COMPILER_STD_FLAG_CPP11_SUPPORT
#       define __NEBULA_NOTHROW noexcept
#   else    // else use old keyword:throw()
#       define __NEBULA_NOTHROW throw()
#   endif // HAVE_COMPILER_STD_FLAG_CPP11_SUPPORT
#   define __NEBULA_UNWIND(action) catch(...) { action;throw; }
#else
#   define __NEBULA_TRY
#   define __NEBULA_CATCH_ALL if(false)
#   define __NEBULA_RETHROW
#   define __NEBULA_NOTHROW
#   define __NEBULA_UNWIND(action)
#endif // __NEBULA_USE_EXCEPTIONS

#define __NEBULA_NOEXCEPT __NEBULA_NOTHROW

/*
* Define __NEBULA_ASSERTIONS if We use assertion,then the function __nebula_assert will
* be a good replace,else it will be a null macro.
*/
#ifdef __NEBULA_ASSERTIONS
#   include <cstdio>
#   define __nebula_assert(expr) \
        if(!(expr)) \
        { \
            std::fprintf(stderr, "Nebula Assertion Failure : %s \n<file= %s>\t<line= %d >\n", \
            # expr,__FILE__,__LINE__); \
            std::abort(); \
        } \
#else
#   define __nebula_assert(expr)
#endif //__NEBULA_ASSERTIONS

#endif // __WANGBIN_NEBULA_MODULES_GLOBAL_INCLUDE_NEBULACONFIG_H__