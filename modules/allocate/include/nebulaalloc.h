/*
***************************************************************************************************
// FileName     :nebulaalloc.h
//
// Brief        :Memory Malloc & Pool Allocator.
//
// Author       :WangBin
//
// Description  :
//                  Memory Malloc & Pool Allocator.
//
//
// CopyRight    : WangBin.
//                                              <perturbed@sina.com>
//
***************************************************************************************************
*/

#ifndef __WANGBIN_NEBULA_MODULES_ALLOCATE_INCLUDE_NEBULAALLOC_H__
#define __WANGBIN_NEBULA_MODULES_ALLOCATE_INCLUDE_NEBULAALLOC_H__

/*
* For Microsoft Compiler In Windows.
*/
#if defined(_MSC_VER) && _MSC_VER > 1000
#   pragma once
#endif // if defined(_MSC_VER) && _MSC_VER > 1000


#include "preheader.h"

#include <cstdlib>

__NEBULA_BEGIN_NAMESPACE    //Begin Namespace

template <int inst>
class __nebula_malloc_alloc_template
{
public:
    static void* allocate(size_t __bytes)
    {
        void* result=malloc(__bytes);
        if(0==result)
        {
            result=oom_malloc(__bytes);
        }
        return result;
    }

    static void deallocate(void* __pointer,size_t /*__bytes*/)
    {
        free(__pointer);
    }

    static void* reallocate(void* __pointer,size_t /*__old_size*/,size_t __new_size)
    {
        void* result=realloc(__pointer,__new_size);
        if(0==result)
        {
            result=oom_realloc(__pointer,__new_size);
        }
        return result;
    }


    /*
    * for your reading:
    * typedef void (*function_handler_type)();
    * function_handler_type set_nebula_malloc_handler(function_handler_type)
    * for more information reference <<C++ Primer>>.
    *                                       add by <wangbin>
    */
#ifndef HAVE_COMPILER_STD_FLAG_CPP11_SUPPORT
    static void (*set_nebula_malloc_handler(void (*function_handler)()))()
    {
        void (*old)() =__nebula_malloc_alloc_oom_handler;
        __nebula_malloc_alloc_oom_handler=function_handler;
        return(old);
    }
#else
    static auto set_nebula_malloc_handler(void(*function_handler)()) -> void (*)()
    {
        void (*old)() =__nebula_malloc_alloc_oom_handler;
        __nebula_malloc_alloc_oom_handler=function_handler;
        return(old);
    }
#endif
private:
    static void* oom_malloc(size_t);                    //out of memory function malloc handler
    static void* oom_realloc(void* ,size_t);            //out of memory function realloc handler
    static void (*__nebula_malloc_alloc_oom_handler)(); //out of memory function user handler

};

template<int inst>
void (*__nebula_malloc_alloc_template<inst>::__nebula_malloc_alloc_oom_handler)()=0;

template<int inst>
void* __nebula_malloc_alloc_template<inst>::oom_malloc(size_t __bytes)
{
    void (*user_malloc_handler)();
    void* result;
    for(;;)
    {
        user_malloc_handler=__nebula_malloc_alloc_oom_handler;
        if(0==user_malloc_handler)
        {
            __THROW_NEBULA_BAD_ALLOC;
        }
        (*user_malloc_handler)();
        result=malloc(__bytes);
        if(result)
        {
            return result;
        }
    }
}

template<int inst>
void* __nebula_malloc_alloc_template<inst>::oom_realloc(void* __pointer,size_t __bytes)
{
    void (*user_malloc_handler)();
    void* result;
    for(;;)
    {
        user_malloc_handler=__nebula_malloc_alloc_oom_handler;
        if(0==user_malloc_handler)
        {
            __THROW_NEBULA_BAD_ALLOC;
        }
        (*user_malloc_handler)();
        result=realloc(__pointer,__bytes);
        if(result)
        {
            return result;
        }
    }
}

typedef __nebula_malloc_alloc_template<0>   nebula_malloc_alloc;

/*
* Default Alloc Template
*/
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
    enum { __ALIGN = 8 };
    enum { __MAX_BYTES = 128 };
    enum { __NFREELISTS = (size_t)__MAX_BYTES / (size_t)__ALIGN };

    union obj
    {
        union obj* free_list_link;
        char client_data[1]; //As some book explians:the client sees this
    };

    static obj* volatile    free_list[__NFREELISTS];
    static size_t FREELIST_INDEX(size_t __bytes)
    { return (((__bytes) + __ALIGN-1) /__ALIGN -1);}

    static void* refill(size_t n);

    static char* chunk_alloc(size_t __size,int& __nobjs);

    static char* start_free;
    static char* end_free;
    static size_t heap_size;

public:
    static void* allocate(size_t __bytes);
    static void* deallocate(void* __pointer,size_t __bytes);
    static void* realloc(void* __pointer,size_t __old_size,size_t __new_size);

};

template<bool threads,int inst>
char* __nebula_default_alloc_template<threads,inst>::start_free=0;

template<bool threads,int inst>
char* __nebula_default_alloc_template<threads,inst>::end_free=0;

template<bool threads,int inst>
char* __nebula_default_alloc_template<threads,inst>::heap_size=0;

template<bool threads,int inst>
__nebula_default_alloc_template<threads,inst>::obj* volatile
__nebula_default_alloc_template<threads,inst>::free_list[__NFREELISTS]=
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, }

template<bool threads,int inst>
void* __nebula_default_alloc_template<threads,inst>::allocate(size_t __bytes)
{
    obj* volatile* user_free_list;
    obj* result;
    if(__bytes>(size_t)__MAX_BYTES)
    {
        return (__NEBULA::nebula_malloc_alloc::allocate(__bytes));
    }
    user_free_list=free_list+FREELIST_INDEX(__bytes);
    result=*user_free_list;
    if(0==result)
    {
        void* __ret=refill(ROUND_UP(__bytes));
        return __ret;
    }
    *user_free_list=result->free_list_link;
    return result;
}

template<bool threads,int inst>
void __nebula_default_alloc_template<threads,inst>::deallocate(void* __pointer,size_t __bytes)
{
    if(__bytes > (size_t)__MAX_BYTES)
    {
        __NEBULA::nebula_malloc_alloc::deallocate(__pointer,__bytes);
        return;
    }
    obj* volatile* user_free_list=free_list + FREELIST_INDEX(__bytes);
    obj* __q=(obj*)__pointer;
    __q->free_list_link=*user_free_list;
    *user_free_list=__q;
}

template<bool threads,int inst>
void* __nebula_default_alloc_template<threads,inst>::refill(size_t __bytes)
{
    int __nobjs(20);
    char* __chunk_pointer(chunk_alloc(__bytes,__nobjs));
    if(1==__nobjs) return __chunk_pointer;
    obj* volatile* user_free_list=free_list + FREELIST_INDEX(__bytes);
    obj* result=(obj*)__chunk_pointer;
    obj* current_obj,*next_obj;
    *user_free_list=next_obj=(obj*)(__chunk_pointer+__bytes);
    for(int __i(1);;__i++)
    {
        current_obj=next_obj;
        next_obj=(obj*)((char*)next_obj+__bytes);
        if(__i == __nobjs -1 )
        {
            current_obj -> free_list_link = 0;
            break;
        }
        else
        {
            current_obj->free_list_link=next_obj;
        }
    }
    return result;
}

template<bool threads,int inst>
char* __nebula_default_alloc_template<threads,inst>::chunk_alloc(size_t __bytes,int* __nobjs)
{
    char* result;
    size_t total_bytes(__bytes*__nobjs);
    size_t bytes_left(end_free - start_free);
    if(bytes_left >= total_bytes)
    {

    }
}

__NEBULA_END_NAMESPACE      //End Namespace

#endif // __WANGBIN_NEBULA_MODULES_ALLOCATE_INCLUDE_NEBULAALLOC_H__