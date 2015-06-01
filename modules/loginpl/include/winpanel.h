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

#ifndef __WANGBIN_NEBULA_MODULES_LOGINPL_INCLUDE_WINPANEL_H__
#define __WANGBIN_NEBULA_MODULES_LOGINPL_INCLUDE_WINPANEL_H__

/*
* For Microsoft Compiler In Windows.
*/
#if defined(_MSC_VER) && _MSC_VER > 1000
#   pragma once
#endif // if defined(_MSC_VER) && _MSC_VER > 1000

/*
**************************************************************************************************
函数列表及描述
    login_panel                 :显示登录界面



***************************************************************************************************
*/

struct login_info
{
    const char* loginname;
    const char* loginpassword;

};

void login_panel();



#endif /* __WANGBIN_NEBULA_MODULES_LOGINPL_INCLUDE_WINPANEL_H__ */