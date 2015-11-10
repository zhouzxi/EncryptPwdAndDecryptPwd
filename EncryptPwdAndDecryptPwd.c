/**********************************************************************
*版权所有 (C)2015, Zhou Zhaoxiong。
*
*文件名称：EncryptPwdAndDecryptPwd.c
*文件标识：无
*内容摘要：从配置文件中读取密码并进行加解密
*其它说明：无
*当前版本：V1.0
*作    者：Zhou Zhaoxiong
*完成日期：20150306
*
**********************************************************************/
#include <stdio.h>
#include <windows.h>
 
// 重新定义数据类型
typedef unsigned char UINT8;
typedef          int  INT32;
typedef unsigned int  UINT32;
 
// 对函数进行声明
INT32 EncryptPwdStr(UINT8 *pszPwdStr, UINT32 iInPwdLen, UINT32 iOperFlag);
 
 
/**********************************************************************
*功能描述：主函数
*输入参数：无
*输出参数：无
*返 回 值：无
*其它说明：无
* 修改日期        版本号       修改人        修改内容
* -----------------------------------------------------------------
* 20150306        V1.0     Zhou Zhaoxiong     创建
***********************************************************************/
INT32 main()
{
    UINT8 szConfigFile[128] = {0};
    UINT8 szPassword[128]   = {0};
    INT32 iRetVal           = 0;
    UINT32 iDecryptFlag     = 0;
 
    // 获取配置文件全路径(包括文件名)
    GetCurrentDirectory(sizeof(szConfigFile)-1, szConfigFile);
    strcat(szConfigFile, "\\");
    strcat(szConfigFile, "Config.ini");
 
    // 读入密码明文
    GetPrivateProfileString("PWDINFO", "Password", "", szPassword, sizeof(szPassword), szConfigFile);
 
    iRetVal = EncryptPwdStr(szPassword, strlen(szPassword), 1);   // 加密
    if (iRetVal == 0)   // 加密成功
    {
       printf("Encrypt password successfully! Password is: %s\n", szPassword);
    }
    else
    {
       printf("Encrypt password failed!\n");
       return -1;               // main函数执行失败返回-1
    }
 
    // 将加密后的密码写入配置文件中
    printf("Write the encrypted password into config file.\n");
    WritePrivateProfileString("PWDINFO", "Password", szPassword, szConfigFile);
 
    // 判断是否对加密后的密码进行解密
    printf("To decrypt the encrypted password or not? 0-No, 1-Yes\n");
    scanf("%d", &iDecryptFlag);
    if (iDecryptFlag != 0 && iDecryptFlag != 1)   // 判断输入值是否正确
    {
       printf("Input error, you must input 0 or 1, please check!\n");
       return -1;
    }
 
    if (iDecryptFlag == 1)   // 需要解密, 并将解密后的密码写入配置文件中
    {
       iRetVal = EncryptPwdStr(szPassword, strlen(szPassword), 2);   // 解密
       if (iRetVal == 0)   // 解密成功
       {
          printf("Decrypt password successfully! Password is: %s\n", szPassword);
       }
       else
       {
          printf("Decrypt password failed!\n");
          return -1;             // main函数执行失败返回-1
       }
 
       // 将解密后的密码写入配置文件中
       printf("Write the decrypted password into config file.\n");
       WritePrivateProfileString("PWDINFO", "Password", szPassword, szConfigFile);
    }
    else
    {
       printf("Don't decrypt the encrypted password.\n");
    }
 
    return 0;               // main函数执行成功返回0
}
 
 
/**********************************************************************
*功能描述：对密码字符串的每一位进行加解密处理
*输入参数：*pszPwdStr-输入/输出密码字符串
            iInPwdLen-输入密码字符串长度
            iOperFlag-操作标识, 1-加密, 2-解密
*输出参数：*pszPwdStr-输入/输出密码字符串
*返 回 值： 0-成功 -1-失败
*其它说明： 密码字符串的每一位的ASCII码值加上16或减去16
* 修改日期        版本号       修改人           修改内容
* --------------------------------------------------------------------
* 20150306        V1.0     Zhou Zhaoxiong         创建
***********************************************************************/
INT32 EncryptPwdStr(UINT8 *pszPwdStr, UINT32 iInPwdLen, UINT32 iOperFlag)
{
   UINT32 iLoopFlag = 0;
 
   if (pszPwdStr == NULL)       // 异常保护
   {
       printf("EncryptPwdStr: Input string is NULL!\n");
       return -1;             // 返回-1表示该函数执行失败
   }
 
   for (iLoopFlag = 0; iLoopFlag < iInPwdLen; iLoopFlag ++)
   {
       if (iOperFlag == 1)     // 加密
       {
           pszPwdStr[iLoopFlag] = pszPwdStr[iLoopFlag] + 16;
       }
       else                   // 解密
       {
           pszPwdStr[iLoopFlag] = pszPwdStr[iLoopFlag] - 16;
       }
   }
 
   return 0;                 // 返回0表示该函数执行成功
}
