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


#ifdef WIN32
#include <io.h>
#include <direct.h>
#else
#include <unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#endif

#include "XmlOpeation.h"

using namespace std;

#define FUNCTION_CONFIG_PATH     "../Conf/Function"
#define OBJECT_MESSAGE_PATH      "../Conf/ObjectMessage"
#define OBJECT_CONFIG_PATH       "../Conf/ObjectConf"
#define MYSQL_CONFIG_PATH        "../Conf/MysqlConf/Mysql.xml"
#define OBJECT_OUTPUT_PATH       "../ProjectCode/OMMFObject"
#define MESSAGE_OUTPUT_PATH      "../ProjectCode/OMMFMessage"
#define FUNCTION_OUTPUT_PATH     "../ProjectCode/OMMFFunction"
#define MYSQL_OUTPUT_PATH        "../ProjectCode/MysqlCode"
#define MYSQL_OUTPUT_SCRIPT_PATH "../ProjectCode/MysqlScript"
#define OBJECT_BASETYPE_PATH     "../Conf/ObjectConf/BaseType.xml"
#define OBJECT_BASETYPE_FILE     "BaseType.h"
#define OBJECT_BASECLASS_FILE    "BaseObject.h"
#define OBJECT_LIST_MANAGER_NAME "ObjectManager"
#define OBJECT_TEST_PATH         "../Test"
#define OBJECT_TEST_FILENAME     "TestManager"

#define MAX_CODE_LINE_SIZE  500
#define MAX_MYSQL_LINE_SIZE 2048

struct _Function_Info
{
    string m_strFunctionName;       //������������
    string m_strMessageIn;          //���Message
    string m_strMessageOut;         //����Message
};

typedef vector<_Function_Info> vec_Function_Info;

struct _Object_Info
{
    string m_strName;            //��ǰ������
    string m_strType;            //��ǰ��������
    string m_strMin;             //��ǰ������Сֵ
    string m_strMax;             //��ǰ�������ֵ
    string m_strAttribute;       //��ǰ���������ͣ�Ϊ�������ݿ������Ŀǰ֧�� STRING NUMBER
    string m_strInit;            //��ǰ������ʼֵ
};

typedef vector<_Object_Info> vec_Object_Info;

struct _Message_Info
{
    string          m_strMessageName;      //��Ϣ����
    vec_Object_Info m_vec_Object_Info;     //��Ϣ�����
};

typedef vector<_Message_Info> vec_Message_Info;

struct _ObjectClass
{
    string m_strClassName;                //������
    int    m_nBuffSize;                   //������󻺳��С
    int    m_nClassType;                  //��������ID
    int    m_nClassID;                    //object������ΨһID
    int    m_nObjectListCount;            //��ǰ������б��������

    _ObjectClass()
    {
        m_nBuffSize        = 0;
        m_nClassType       = 0;
        m_nClassID         = 0;
        m_nObjectListCount = 0;
    }

    vec_Object_Info m_vec_Object_Info;    //�����б�
};

typedef vector<_ObjectClass> vec_ObjectClass;   //�����ṹ

typedef vector<string> vec_Xml_File_Name;       //����XML�ļ���·��

struct _BaseType
{
    string m_strBaseTypeName;            //typedef���������
    string m_strTypeName;                //typedef�����ԭʼ��������
    string m_strClassName;               //�ǵ������󣬻������ݶ���
    int    m_nLen;                       //�����ܳ���(�ֽ���)
    int    m_nSaveSize;                  //����������Դ�еĴ�С

    _BaseType()
    {
        m_nLen      = 0;
        m_nSaveSize = 1;
    }
};

typedef vector<_BaseType> vec_Base_Type_List;

//�������ݶ�������
struct _Base_Type_List_info
{
    int m_nKeyPos;
    vec_Base_Type_List m_vec_Base_Type_List;

    _Base_Type_List_info()
    {
        m_nKeyPos = -1;
    }
};

//Mysql���ݽṹ
struct _Xml_Mysql_Table
{
    int    m_nClassID;                    //object������ΨһID
};

typedef vector<_Xml_Mysql_Table> vec_Xml_Mysql_Table;

//Mysql���ݽṹ
struct _Xml_Mysql_DB
{
    string m_strDBName;
    vec_Xml_Mysql_Table m_vec_Xml_Mysql_Table;  //�����ж��DB
};
typedef vector<_Xml_Mysql_DB> vec_Xml_Mysql_DB;

//�����ļ���ʵ��
static int copyFile(const char* src, const char* des)
{
    int nRet = 0;
    FILE* pSrc = NULL, *pDes = NULL;
    pSrc = fopen(src, "r");
    pDes = fopen(des, "w+");

    if (pSrc && pDes)
    {
        int nLen = 0;
        char szBuf[1024] = { 0 };

        while ((nLen = fread(szBuf, 1, sizeof szBuf, pSrc)) > 0)
        {
            fwrite(szBuf, 1, nLen, pDes);
        }
    }
    else
    {
        nRet = -1;
    }

    if (pSrc)
    {
        fclose(pSrc), pSrc = NULL;
    }

    if (pDes)
    {
        fclose(pDes), pDes = NULL;
    }

    return nRet;
}

//�����༶�ļ�
static void mkdirs(char* muldir)
{
    int i, len;
    char str[512];
    strncpy(str, muldir, 512);
    len = strlen(str);

    for (i = 0; i < len; i++)
    {
        if (str[i] == '/')
        {
            str[i] = '\0';

            if (access(str, 0) != 0)
            {
#ifdef WIN32
                mkdir(str);
#else
                mkdir(str, 0777);
#endif
            }

            str[i] = '/';
        }
    }

    if (len > 0 && access(str, 0) != 0)
    {
#ifdef WIN32
        mkdir(str);
#else
        mkdir(str, 0777);
#endif
    }

    return;
}

//�鿴ָ��Ŀ¼�Ƿ���ڣ��������򴴽�֮
static bool Create_Project_Path(const char* pPath)
{
    int nflag = 0;
    //�鿴�ļ�����û��дȨ��
    int nRet = access(pPath, 02);

    if (0 != nRet)
    {
        //��Ҫ�����ļ���
#ifdef WIN32
        nflag = mkdir(pPath);
#else
        nflag = mkdir(pPath, 0777);
#endif

        if (nflag == 0)
        {
            return true;
        }
        else
        {
            printf("[Create_Project_Path]Create Path(%s) error.\n", pPath);
            return false;
        }
    }

    return true;
}

//��֤��ǰMessage�Ƿ���
static bool Check_Message(string strMessage, vec_Message_Info& obj_vec_Message_Info)
{
    bool blRet = false;

    for (int i = 0; i < (int)obj_vec_Message_Info.size(); i++)
    {
        if (obj_vec_Message_Info[i].m_strMessageName == strMessage)
        {
            blRet = true;
            break;
        }
    }

    return blRet;
}

//��֤�����ǲ����Ѷ��������֮һ
static bool Check_Base_Type(string strType, vec_Base_Type_List& obj_vec_Base_Type_List)
{
    for (int i = 0; i < (int)obj_vec_Base_Type_List.size(); i++)
    {
        if (obj_vec_Base_Type_List[i].m_strBaseTypeName == strType)
        {
            return true;
        }
    }

    return false;
}

//�õ���ǰ�������ͳ���
static int Get_Base_Type_Size(string strType, vec_Base_Type_List& obj_vec_Base_Type_List)
{
    for (int i = 0; i < (int)obj_vec_Base_Type_List.size(); i++)
    {
        if (obj_vec_Base_Type_List[i].m_strBaseTypeName == strType)
        {
            return obj_vec_Base_Type_List[i].m_nLen;
        }
    }

    return 0;
}

//�õ���ǰ��������(�������ݣ�����)
static string Get_Base_Type_Class(string strType, vec_Base_Type_List& obj_vec_Base_Type_List)
{
    for (int i = 0; i < (int)obj_vec_Base_Type_List.size(); i++)
    {
        if (obj_vec_Base_Type_List[i].m_strBaseTypeName == strType)
        {
            return obj_vec_Base_Type_List[i].m_strClassName;
        }
    }

    return "";
}

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

//����Obect����
static bool Create_Base_Class_H()
{
    char szHFileName[200] = { '\0' };
    char szCodeLine[MAX_CODE_LINE_SIZE] = { '\0' };

    //������ӦĿ¼
    if (Create_Project_Path(OBJECT_OUTPUT_PATH) == false)
    {
        return false;
    }

    sprintf(szHFileName, "%s//%s", OBJECT_OUTPUT_PATH, OBJECT_BASECLASS_FILE);
    FILE* pFile = fopen(szHFileName, "w");

    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "//create BaseObject.h\n\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#ifndef _BASEOBJECT_H\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#define _BASEOBJECT_H\n\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    //�������ͷ�ļ�
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#include <stdio.h>\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#include <stdlib.h>\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#include <string.h>\n\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "class IObject\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "{\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "public:\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tvirtual ~IObject() {};\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tvirtual bool Get_Stream(char* pData, int& nLen) = 0;\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tvirtual bool Set_Stream(char* pData, int& nLen) = 0;\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "};\n\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#endif\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    fclose(pFile);
    return true;
}

//�����������Ͷ���ͷ�ļ�
static bool Create_Base_Type_H(vec_Base_Type_List& obj_vec_Base_Type_List)
{
    char szHFileName[200] = { '\0' };
    char szCodeLine[MAX_CODE_LINE_SIZE] = { '\0' };

    //������ӦĿ¼
    if (Create_Project_Path(OBJECT_OUTPUT_PATH) == false)
    {
        return false;
    }

    sprintf(szHFileName, "%s//%s", OBJECT_OUTPUT_PATH, OBJECT_BASETYPE_FILE);
    FILE* pFile = fopen(szHFileName, "w");

    if (NULL == pFile)
    {
        printf("[Create_Base_Type_H]fopen(%s) error.\n", szHFileName);
        return false;
    }

    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "//create BaseType.h\n\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#ifndef _BASETYPE_H\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#define _BASETYPE_H\n\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#include <stdio.h>\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#include <string>\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#include <vector>\n\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "using namespace std;\n\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    for (int i = 0; i < (int)obj_vec_Base_Type_List.size(); i++)
    {
        if (obj_vec_Base_Type_List[i].m_strClassName == "single")
        {
            //��������
            sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "typedef %s %s;\n",
                         obj_vec_Base_Type_List[i].m_strTypeName.c_str(),
                         obj_vec_Base_Type_List[i].m_strBaseTypeName.c_str());
            fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
        }
        else
        {
            //����
            sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "typedef %s %s[%d];\n",
                         obj_vec_Base_Type_List[i].m_strTypeName.c_str(),
                         obj_vec_Base_Type_List[i].m_strBaseTypeName.c_str(),
                         obj_vec_Base_Type_List[i].m_nLen);
            fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
        }
    }

    //������Ϣ���ͣ����ڸ�����Ĳ�������
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\nstruct _Object_Info\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "{\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tstring m_strName;\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tstring m_strType;\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tstring m_strMin;\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tstring m_strMax;\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tstring m_strInit;\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tint m_nSize;\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tint m_nStartPos;\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "};\n\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "typedef vector<_Object_Info> vec_Object_Info_List;\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#endif\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    fclose(pFile);
    return true;
}

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
