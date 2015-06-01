/*
***************************************************************************************************
// FileName     :runtimest.h
//
// Brief        :runtimest
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

#ifndef __WANGBIN_NEBULA_MODULES_GLOBAL_INCLUDE_RUNTIMEST_H__
#define __WANGBIN_NEBULA_MODULES_GLOBAL_INCLUDE_RUNTIMEST_H__

/*
* For Microsoft Compiler In Windows.
*/
#if defined(_MSC_VER) && _MSC_VER > 1000
#   pragma once
#endif // if defined(_MSC_VER) && _MSC_VER > 1000


#define NUM_OF_CONFIG                   500                                         // 配置文件config数组长度
#define FILE_MAPPING_NAME               "08616da4-efee-4e35-9c2b-80727627bb71"      // 映射文件文件名

typedef struct NEBULA_VERSION_ST
{
    int         major;                  // 主版本号
    int         minor;                  // 次版本号
    int         build;                  // 编译版本号
    int         revision;               // 修正版本号
}version_info;

/*
* For Nebula Runtime Data Struct
*/
typedef struct NEBULA_DATA_ST
{


    version_info                version;                        // 版本号
    unsigned long long int      watch;                          // 程序监视数据
    // for nebula struct
    float                       config[NUM_OF_CONFIG];                  // 配置
}nebula_data_st;

#endif  /* __WANGBIN_NEBULA_MODULES_GLOBAL_INCLUDE_RUNTIMEST_H__ */