#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <vector>
#include <ctype.h>
#include<iostream>
#include<fstream>
#include <string>

#include "XmlOpeation.h"

using namespace std;

#define OBJECTCONFIG_PATH  "../ObjectConfig"
#define OBJECT_OUTPUT_PATH "../ECSObject"

struct _Object_Info
{
    string m_strName;            //��ǰ������
    string m_strType;            //��ǰ��������
    string m_strMin;             //��ǰ������Сֵ
    string m_strMax;             //��ǰ�������ֵ
    string m_strInit;            //��ǰ������ʼֵ
};

typedef vector<_Object_Info> vec_Object_Info;

struct _ObjectClass
{
    string m_strClassName;                //������
    int    m_nBuffSize;                   //������󻺳��С
    int    m_nClassType;                  //��������ID

    vec_Object_Info m_vec_Object_Info;    //�����б�
};

typedef vector<_ObjectClass> vec_ObjectClass;

typedef vector<string> vec_Xml_File_Name;

//��ȫ���ַ�����ֵ
static void sprintf_safe(char* szText, int nLen, const char* fmt ...)
{
    if (szText == NULL)
    {
        return;
    }

    va_list ap;
    va_start(ap, fmt);

    vsnprintf(szText, nLen, fmt, ap);
    szText[nLen - 1] = '\0';

    va_end(ap);
};

//��ĸСдת����д
static void To_Upper_String(const char* pSrc, char* pRet)
{
    int nLen = (int)strlen(pSrc);

    for (int i = 0; i < nLen; i++)
    {
        pRet[i] = toupper(pSrc[i]);
    }

    pRet[nLen] = '\0';
}

//���һ���ļ���
static string Get_File_From_Path(string strPath)
{
    char szPath[300]     = { '\0' };
    char szFileName[300] = { '\0' };

    sprintf_safe(szPath, 300, "%s", strPath.c_str());
    int nLen = (int)strlen(szPath);
    int nPos = 0;

    for (int i = nLen - 1; i >= 0; i--)
    {
        if ('\\' == szPath[i] || '/' == szPath[i])
        {
            nPos = i + 1;
            break;
        }
    }

    memcpy(szFileName, &szPath[nPos], nLen - nPos);
    szFileName[nLen - nPos] = '\0';

    //ȥ���ļ�ĩβ��׺
    for (int i = (int)strlen(szFileName); i >= 0; i--)
    {
        if ('.' == szFileName[i])
        {
            szFileName[i] = '\0';
            break;
        }
    }

    return (string)szFileName;
}

#endif
