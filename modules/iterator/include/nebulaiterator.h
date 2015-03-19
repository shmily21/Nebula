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
#ifndef __WANGBIN_NEBULA_MODULES_ITERATOR_INCLUDE_NEBULAITERATOR_H__
#define __WANGBIN_NEBULA_MODULES_ITERATOR_INCLUDE_NEBULAITERATOR_H__

/*
* For Microsoft Compiler In Windows.
*/
#if defined(_MSC_VER) && _MSC_VER > 1000
#   pragma once
#endif // if defined(_MSC_VER) && _MSC_VER > 1000

#include "global.h"

#include <cstdlib>

__NEBULA_BEGIN_NAMESPACE

/*
*以下类均作为标记使用，分别代表 只读，只写，写入型，双向移动和可随机访问型迭代器
*/

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

/*
* 一个标准的类模板，自行开发的迭代器都可以从此处继承
*/
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

/*
* 用于类型别中的 内嵌型别 的类型获取
*/
template <class Iterator>
struct __nebula_iterator_traits
{
    typedef typename Iterator::iterator_category    iterator_category;
    typedef typename Iterator::value_type           value_type;
    typedef typename Iterator::difference_type      difference_type;
    typedef typename Iterator::pointer              pointer;
    typedef typename Iterator::reference            reference;
};

/*
* 针对原始指针时的偏特化
*/
template <class T>
struct __nebula_iterator_traits
{
    typedef __nebula_random_access_iterator_tag     iterator_category;
    typedef T                                       value_type;
    typedef ptrdiff_t                               difference_type;
    typedef T*                                      pointer;
    typedef T&                                      reference;
};

/*
* 解决模板型别参数带有const属性时，对应型别变量只能是右值的问题
*/
template <class T>
struct __nebula_iterator_traits<const T*>
{
    typedef __nebula_random_access_iterator_tag     iterator_category;
    typedef T                                       value_type;
    typedef ptrdiff_t                               difference_type;
    typedef const T*                                pointer;
    typedef const T&                                reference;
};

/*
* 全局函数，用于从迭代器中萃取类型 iterator_category 值
*/
template <class Iterator>
inline typename __NEBULA::__nebula_iterator_traits<Iterator>::iterator_category
__nebula_iterator_category(const Iterator&)
{
    typedef typename __NEBULA::__nebula_iterator_traits<Iterator>::iterator_category category;
    return category();
}

/*
* 全局函数，用于从迭代器中萃取 difference_type 类型指针
*/
template <class Iterator>
inline typename __NEBULA::__nebula_iterator_traits<Iterator>::difference_type*
__nebula_distance_type(const Iterator&)
{
    return static_cast<typename __NEBULA::__nebula_iterator_traits<Iterator>::difference_type*>(0);
}

/*
* 全局函数，用于从迭代器中萃取 value_type 类型指针
*/
template <class Iterator>
inline typename __NEBULA::__nebula_iterator_traits<Iterator>::value_type*
__nebula_value_type(const Iterator&)
{
    return static_cast<typename __NEBULA::__nebula_iterator_traits<Iterator>::value_type*>(0);
}

/*
* __nebula_distance 用于计算两个迭代器之间的距离
*/

template <class InputIterator>
inline typename __NEBULA::__nebula_iterator_traits<InputIterator>::difference_type
__nebula_distance(InputIterator first,InputIterator last,__NEBULA::__nebula_input_iterator_tag)
{
    __NEBULA::__nebula_iterator_traits<InputIterator>::difference_type __n=0;
    while(first!=last)
    {
        ++first;
        ++__n;
    }
    return __n;
}

template <class RandomAccessIterator>
inline typename __NEBULA::__nebula_iterator_traits<RandomAccessIterator>::difference_type
__nebula_distance(RandomAccessIterator first,RandomAccessIterator last,__NEBULA::__nebula_random_access_iterator_tag)
{
    return last-first;
}

template <class InputIterator>
inline typename __NEBULA::__nebula_iterator_traits<InputIterator>::difference_type
__nebula_distance(InputIterator first,InputIterator last)
{
    typedef typename __NEBULA::__nebula_iterator_traits<InputIterator>::iterator_category category;
    return __nebula_distance(first,last,category());
}



/*
* __nebula_advance 将迭代器前进指定的距离
*/

template <class InputIterator,class Distance>
inline void __nebula_advance(InputIterator& i,Distance n,__NEBULA::__nebula_input_iterator_tag)
{
    while(n--) ++i;
}

template <class BidirectionalIterator,class Distance>
inline void __nebula_advance(BidirectionalIterator& i,Distance n,__NEBULA::__nebula_bidirectional_iterator_tag)
{
    if(n>=0) while(n--) ++i;
    else while(n++) --i;
}

template <class RandomAccessIterator,class Distance>
inline void __nebula_advance(InputIterator& i,Distance n,__NEBULA::__nebula_random_access_iterator_tag)
{
    i+=n;
}

template <class InputIterator,class Distance>
inline void __nebula_advance(InputIterator& i,Distance n)
{
    __NEBULA::__nebula_advance(i,n,__NEBULA::__nebula_iterator_category(i));
}

__NEBULA_END_NAMESPACE

#endif // __WANGBIN_NEBULA_MODULES_ALLOCATE_INCLUDE_NEBULAITERATOR_H__
