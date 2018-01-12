#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>

#include "XmlOpeation.h"

using namespace std;

#define OBJECTCONFIG_PATH  "../ObjectConfig"

struct _Object_Info
{
    string m_strName;            //��ǰ������
    string m_Type;               //��ǰ��������
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

#endif
