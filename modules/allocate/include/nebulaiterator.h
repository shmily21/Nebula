/*
***************************************************************************************************
// FileName     :nebulaiterator.h
//
// Brief        :Cplusplus TYpe Traits.
//
// Author       :WangBin
//
// Description  :
//                  Cplusplus TYpe Traits.
//
//
// CopyRight    : WangBin.
//                                              <perturbed@sina.com>
//
***************************************************************************************************
*/
#ifndef __WANGBIN_NEBULA_MODULES_ALLOCATE_INCLUDE_NEBULAITERATOR_H__
#define __WANGBIN_NEBULA_MODULES_ALLOCATE_INCLUDE_NEBULAITERATOR_H__

/*
* For Microsoft Compiler In Windows.
*/
#if defined(_MSC_VER) && _MSC_VER > 1000
#   pragma once
#endif // if defined(_MSC_VER) && _MSC_VER > 1000

#include "preheader.h"

#include <cstdlib>

__NEBULA_BEGIN_NAMESPACE

struct __nebula_input_iterator_tag
{

};

struct __nebula_output_iterator_tag
{

};

struct __nebula_forward_iterator_tag
        :public __nebula_input_iterator_tag
{

};

struct __nebula_bidirectional_iterator_tag
        :public __nebula_forward_iterator_tag
{

};

struct __nebula_random_access_iterator_tag
        :public __nebula_bidirectional_iterator_tag
{

};

template <class Category,
            class Ty,
            class Distance=ptrdiff_t,
            class Pointer=Ty*,
            class Reference=Ty&>
struct __nebula_iterator
{
    typedef Category    iterator_category;
    typedef Ty          value_type;
    typedef Distance    difference_type;
    typedef Pointer     pointer;
    typedef Reference   reference;
};

template <class Iterator>
struct __nebula_iterator_traits
{
    typedef typename Iterator::iterator_category    iterator_category;
    typedef typename Iterator::value_type           value_type;
    typedef typename Iterator::difference_type      difference_type;
    typedef typename Iterator::pointer              pointer;
    typedef typename Iterator::reference            reference;
};

__NEBULA_END_NAMESPACE

#endif // __WANGBIN_NEBULA_MODULES_ALLOCATE_INCLUDE_NEBULAITERATOR_H__
