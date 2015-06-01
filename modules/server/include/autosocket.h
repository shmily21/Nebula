/*
***************************************************************************************************
// FileName     :
//
// Brief        :
//
// Author       :WangBin
//
// Description  :
//
//
//
// CopyRight    : WangBin.
//                                              <perturbed@sina.com>
//
***************************************************************************************************
*/

#ifndef __WANGBIN_NEBULA_MODULES_SERVER_INCLUDE_AUTOSOCKET_H__
#define __WANGBIN_NEBULA_MODULES_SERVER_INCLUDE_AUTOSOCKET_H__

/*
* For Microsoft Compiler In Windows.
*/
#if defined(_MSC_VER) && _MSC_VER > 1000
#   pragma once
#endif // if defined(_MSC_VER) && _MSC_VER > 1000


#include "global.h"

#ifdef NEBULA_FOR_LINUX
/*
* 在 linux 下使用epoll模型
*/
#include "linuxsocket.h"
#elif defined(NEBULA_FOR_WINDOWS)
/*
* Windows 下使用 IOCP 模型
*/

#include "winsocket.h"
#endif /* NEBULA_FOR_LINUX */

#endif /* __WANGBIN_NEBULA_MODULES_SERVER_INCLUDE_AUTOSOCKET_H__ */