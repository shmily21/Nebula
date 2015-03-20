/*
***************************************************************************************************
// FileName     :typetraits.h
//
// Brief        :Type Traits
//
// Author       :WangBin
//
// Description  :
//                  Type Traits.
//
//
// CopyRight    : WangBin.
//                                              <perturbed@sina.com>
//
***************************************************************************************************
*/

#ifndef __WANGBIN_NEBULA_MODULES_ITERATOR_TYPETRAITS_H__
#define __WANGBIN_NEBULA_MODULES_ITERATOR_TYPETRAITS_H__

/*
* For Microsoft Compiler In Windows.
*/
#if defined(_MSC_VER) && _MSC_VER > 1000
#   pragma once
#endif // if defined(_MSC_VER) && _MSC_VER > 1000

__NEBULA_BEGIN_NAMESPACE

struct __nebula_true_type
{

};

struct __nebula_false_type
{

};

template <class _type>
struct __nebula_type_traits
{
    typedef __nebula_true_type this_dummy_member_must_be_free;

};

__NEBULA_END_NAMESPACE

#endif // __WANGBIN_NEBULA_MODULES_ITERATOR_TYPETRAITS_H__