#include "ReadObject.h"

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#endif

//���ɹ��ߣ���ָ����XML����ָ����Class���󣬲��ṩGet Set������
//add by freeeyes

//���õ�ǰ·��
//���õ�ǰ����·��
bool SetAppPath()
{
#ifndef WIN32
    int nSize = (int)pathconf(".",_PC_PATH_MAX);

    if (nSize <= 0)
    {
        printf("[SetAppPath]pathconf is error(%d).\n", nSize);
        return false;
    }

    char* pFilePath = new char[nSize];

    if(NULL != pFilePath)
    {
        char szPath[300] = { '\0' };
        memset(pFilePath, 0, nSize);
        sprintf(pFilePath,"/proc/%d/exe",getpid());

        //�ӷ��������л�õ�ǰ�ļ�ȫ·�����ļ���
        ssize_t stPathSize = readlink(pFilePath, szPath, 300 - 1);

        if (stPathSize <= 0)
        {
            printf("[SetAppPath](%s)no find work Path.\n", szPath);
            delete[] pFilePath;
            return false;
        }
        else
        {
            delete[] pFilePath;
        }

        while(szPath[stPathSize - 1]!='/')
        {
            stPathSize--;
        }

        szPath[stPathSize > 0 ? (stPathSize-1) : 0]= '\0';

        int nRet = chdir(szPath);

        if (-1 == nRet)
        {
            printf("[SetAppPath]Set work Path (%s) fail.\n", szPath);
        }
        else
        {
            printf("[SetAppPath]Set work Path (%s) OK.\n", szPath);
        }

        return true;
    }
    else
    {
        printf("[SetAppPath]Set work Path[null].\n");
        return false;
    }

#else
    return true;
#endif
}

//����ָ����Ŀ¼���������XML�ļ���
bool Read_Xml_Folder(string folderPath, vec_Xml_File_Name& obj_vec_Xml_File_Name)
{
#ifdef WIN32
    _finddata_t FileInfo;
    string strfind = folderPath + "\\*";
    long Handle = _findfirst(strfind.c_str(), &FileInfo);

    if (Handle == -1L)
    {
        return false;
    }

    do
    {
        //�ж��Ƿ�����Ŀ¼
        if (FileInfo.attrib & _A_SUBDIR)
        {
            //���������Ҫ
            if ((strcmp(FileInfo.name, ".") != 0) && (strcmp(FileInfo.name, "..") != 0))
            {
                //����֧����Ŀ¼����
                //string newPath = folderPath + "\\" + FileInfo.name;
                //dfsFolder(newPath);
            }
        }
        else
        {
            if (strcmp(FileInfo.name, "BaseType.xml") != 0)
            {
                string filename = folderPath + "\\" + FileInfo.name;
                obj_vec_Xml_File_Name.push_back(filename);
            }
        }
    }
    while (_findnext(Handle, &FileInfo) == 0);

    _findclose(Handle);
#else
    DIR* dp;
    struct dirent* entry;
    struct stat statbuf;

    if ((dp = opendir(folderPath.c_str())) == NULL)
    {
        printf("cannot open directory: %s\n", folderPath.c_str());
        return false;
    }

    //chdir(folderPath.c_str());

    while ((entry = readdir(dp)) != NULL)
    {
        lstat(entry->d_name, &statbuf);

        if (DT_REG == entry->d_type)
        {
            if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
            {
                continue;
            }

            //printf("[Read_Xml_Folder]%s.\n", entry->d_name);
            if (strcmp("BaseType.xml", entry->d_name) != 0)
            {
                string filename = folderPath + "/" + entry->d_name;
                obj_vec_Xml_File_Name.push_back(filename);
            }
        }
    }

    //chdir("..");
    closedir(dp);
#endif
    return true;
}

//��ȡ��������������
bool Read_Base_Type_XML_File(_Base_Type_List_info& obj_Base_Type_List_info)
{
    CXmlOpeation obj_MainConfig;

    obj_Base_Type_List_info.m_vec_Base_Type_List.clear();

    if (false == obj_MainConfig.Init(OBJECT_BASETYPE_PATH))
    {
        printf("[Read_Base_Type_XML_File]File Read Error = %s.\n", OBJECT_BASETYPE_PATH);
        return false;
    }

    //��������ز�����Ϣ
    char* pData = NULL;

    TiXmlElement* pNextTiXmlElementName  = NULL;
    TiXmlElement* pNextTiXmlElementType  = NULL;
    TiXmlElement* pNextTiXmlElementClass = NULL;
    TiXmlElement* pNextTiXmlElementSize  = NULL;
    TiXmlElement* pNextTiXmlElementKey   = NULL;

    while (true)
    {
        _BaseType obj_BaseType;
        pData = obj_MainConfig.GetData("CObject", "name", pNextTiXmlElementName);

        if (pData != NULL)
        {
            obj_BaseType.m_strBaseTypeName = (string)pData;
        }
        else
        {
            break;
        }

        pData = obj_MainConfig.GetData("CObject", "type", pNextTiXmlElementType);

        if (pData != NULL)
        {
            obj_BaseType.m_strTypeName = (string)pData;
        }
        else
        {
            break;
        }

        pData = obj_MainConfig.GetData("CObject", "class", pNextTiXmlElementClass);

        if (pData != NULL)
        {
            obj_BaseType.m_strClassName = (string)pData;
        }
        else
        {
            break;
        }

        pData = obj_MainConfig.GetData("CObject", "size", pNextTiXmlElementSize);

        if (pData != NULL)
        {
            obj_BaseType.m_nLen = atoi(pData);
        }
        else
        {
            break;
        }

        pData = obj_MainConfig.GetData("CObject", "isobjectkey", pNextTiXmlElementKey);

        if (pData != NULL)
        {
            if (strcmp(pData, "yes") == 0)
            {
                //�����ֶ�����
                if (obj_Base_Type_List_info.m_nKeyPos > 0)
                {
                    printf("[Read_Base_Type_XML_File]object key is exist.\n");
                    return false;
                }
                else
                {
                    obj_Base_Type_List_info.m_nKeyPos = (int)obj_Base_Type_List_info.m_vec_Base_Type_List.size();
                }
            }
        }

        obj_Base_Type_List_info.m_vec_Base_Type_List.push_back(obj_BaseType);
    }

    return true;
}

//������ȡMessage�ļ�����ԭ��ṹ
bool Read_Message_File(vec_Xml_File_Name& obj_vec_Xml_Message_Name, vec_Message_Info& obj_vec_Message_Info)
{
    obj_vec_Message_Info.clear();

    for (int i = 0; i < (int)obj_vec_Xml_Message_Name.size(); i++)
    {
        printf("[Read_Message_File]filename=%s.\n", obj_vec_Xml_Message_Name[i].c_str());

        _Message_Info obj_Message_Info;
        CXmlOpeation obj_MainConfig;

        if (false == obj_MainConfig.Init(obj_vec_Xml_Message_Name[i].c_str()))
        {
            printf("[Read_XML_File]File Read Error = %s.\n", obj_vec_Xml_Message_Name[i].c_str());
            return false;
        }

        //��������ز�����Ϣ
        char* pData = NULL;
        obj_Message_Info.m_strMessageName = Get_File_From_Path(obj_vec_Xml_Message_Name[i]);

        TiXmlElement* pNextTiXmlElementName = NULL;
        TiXmlElement* pNextTiXmlElementType = NULL;
        TiXmlElement* pNextTiXmlElementMin = NULL;
        TiXmlElement* pNextTiXmlElementMax = NULL;
        TiXmlElement* pNextTiXmlElementInit = NULL;

        while (true)
        {
            _Object_Info obj_Object_Info;
            pData = obj_MainConfig.GetData("CObject", "name", pNextTiXmlElementName);

            if (pData != NULL)
            {
                obj_Object_Info.m_strName = (string)pData;
            }
            else
            {
                break;
            }

            pData = obj_MainConfig.GetData("CObject", "type", pNextTiXmlElementType);

            if (pData != NULL)
            {
                obj_Object_Info.m_strType = (string)pData;
            }
            else
            {
                break;
            }

            pData = obj_MainConfig.GetData("CObject", "min", pNextTiXmlElementMin);

            if (pData != NULL)
            {
                obj_Object_Info.m_strMin = (string)pData;
            }
            else
            {
                obj_Object_Info.m_strMin = "";
            }

            pData = obj_MainConfig.GetData("CObject", "max", pNextTiXmlElementMax);

            if (pData != NULL)
            {
                obj_Object_Info.m_strMax = (string)pData;
            }
            else
            {
                obj_Object_Info.m_strMax = "";
            }

            pData = obj_MainConfig.GetData("CObject", "init", pNextTiXmlElementInit);

            if (pData != NULL)
            {
                obj_Object_Info.m_strInit = (string)pData;
            }
            else
            {
                obj_Object_Info.m_strInit = "";
            }

            obj_Message_Info.m_vec_Object_Info.push_back(obj_Object_Info);
        }

        obj_vec_Message_Info.push_back(obj_Message_Info);
    }

    return true;
}

//������ȡXML�ļ�����ԭ��ṹ
bool Read_XML_File(vec_Xml_File_Name& obj_vec_Xml_File_Name, vec_ObjectClass& obj_vec_ObjectClass)
{
    obj_vec_ObjectClass.clear();

    for (int i = 0; i < (int)obj_vec_Xml_File_Name.size(); i++)
    {
        printf("[Read_XML_File]filename=%s.\n", obj_vec_Xml_File_Name[i].c_str());

        _ObjectClass obj_ObjectClass;
        CXmlOpeation obj_MainConfig;

        if (false == obj_MainConfig.Init(obj_vec_Xml_File_Name[i].c_str()))
        {
            printf("[Read_XML_File]File Read Error = %s.\n", obj_vec_Xml_File_Name[i].c_str());
            return false;
        }

        //��������ز�����Ϣ
        char* pData = NULL;
        obj_ObjectClass.m_strClassName = Get_File_From_Path(obj_vec_Xml_File_Name[i]);
        pData = obj_MainConfig.GetData("ClassInfo", "BuffSize");

        if (NULL != pData)
        {
            obj_ObjectClass.m_nBuffSize = atoi(pData);
        }

        pData = obj_MainConfig.GetData("ClassInfo", "ClassType");

        if (NULL != pData)
        {
            obj_ObjectClass.m_nClassType = atoi(pData);
        }

        pData = obj_MainConfig.GetData("ClassInfo", "ClassID");

        if (NULL != pData)
        {
            obj_ObjectClass.m_nClassID = atoi(pData);
        }
        else
        {
            printf("[Read_XML_File](%s)Class ID no exist.\n", obj_vec_Xml_File_Name[i].c_str());
            return false;
        }

        pData = obj_MainConfig.GetData("ClassInfo", "ListCount");

        if (NULL != pData)
        {
            obj_ObjectClass.m_nObjectListCount = atoi(pData);
        }
        else
        {
            printf("[Read_XML_File](%s)ListCount no exist.\n", obj_vec_Xml_File_Name[i].c_str());
            return false;
        }

        TiXmlElement* pNextTiXmlElementName = NULL;
        TiXmlElement* pNextTiXmlElementType = NULL;
        TiXmlElement* pNextTiXmlElementMin  = NULL;
        TiXmlElement* pNextTiXmlElementMax  = NULL;
        TiXmlElement* pNextTiXmlElementInit = NULL;

        while (true)
        {
            _Object_Info obj_Object_Info;
            pData = obj_MainConfig.GetData("CObject", "name", pNextTiXmlElementName);

            if (pData != NULL)
            {
                obj_Object_Info.m_strName = (string)pData;
            }
            else
            {
                break;
            }

            pData = obj_MainConfig.GetData("CObject", "type", pNextTiXmlElementType);

            if (pData != NULL)
            {
                obj_Object_Info.m_strType = (string)pData;
            }
            else
            {
                break;
            }

            pData = obj_MainConfig.GetData("CObject", "min", pNextTiXmlElementMin);

            if (pData != NULL)
            {
                obj_Object_Info.m_strMin = (string)pData;
            }
            else
            {
                obj_Object_Info.m_strMin = "";
            }

            pData = obj_MainConfig.GetData("CObject", "max", pNextTiXmlElementMax);

            if (pData != NULL)
            {
                obj_Object_Info.m_strMax = (string)pData;
            }
            else
            {
                obj_Object_Info.m_strMax = "";
            }

            pData = obj_MainConfig.GetData("CObject", "init", pNextTiXmlElementInit);

            if (pData != NULL)
            {
                obj_Object_Info.m_strInit = (string)pData;
            }
            else
            {
                obj_Object_Info.m_strInit = "";
            }

            obj_ObjectClass.m_vec_Object_Info.push_back(obj_Object_Info);
        }

        obj_vec_ObjectClass.push_back(obj_ObjectClass);

    }

    return true;
}

int main()
{
    bool blRet = false;
    vec_Xml_File_Name obj_vec_Xml_File_Name;
    vec_Xml_File_Name obj_vec_Xml_Message_Name;
    _Base_Type_List_info obj_Base_Type_List_info;
    vec_ObjectClass obj_vec_ObjectClass;
    vec_Message_Info obj_vec_Message_Info;

    SetAppPath();

    //��û���������
    blRet = Read_Base_Type_XML_File(obj_Base_Type_List_info);

    if (false == blRet)
    {
        printf("[Main]Read_Base_Type_XML_File is fail.\n");
        return 0;
    }

    //������û���������ͣ�û���򲻽��д�������
    if (obj_Base_Type_List_info.m_nKeyPos == -1)
    {
        printf("[Main]Read_Base_Type_XML_File no find isobjectkey,you must set one.\n");
        return 0;
    }

    //������е�xml�����ļ���
    blRet = Read_Xml_Folder(OBJECT_CONFIG_PATH, obj_vec_Xml_File_Name);

    if (false == blRet)
    {
        printf("[Main]Read_Xml_Folder is fail.\n");
        return 0;
    }

    //������е�Message�����ļ�
    blRet = Read_Xml_Folder(OBJECT_MESSAGE_PATH, obj_vec_Xml_Message_Name);

    if (false == blRet)
    {
        printf("[Main]Read_Xml_Message is fail.\n");
        return 0;
    }

    printf("[main]obj_vec_Xml_File_Name count=%d.\n", (int)obj_vec_Xml_File_Name.size());

    //�����е�xmlת����ObjectClass�������ݽṹ
    blRet = Read_XML_File(obj_vec_Xml_File_Name, obj_vec_ObjectClass);

    if (false == blRet)
    {
        printf("[Main]Read_XML_File is fail.\n");
        return 0;
    }

    //�����е�xmlת����message���������ݽṹ
    blRet = Read_Message_File(obj_vec_Xml_Message_Name, obj_vec_Message_Info);

    if (false == blRet)
    {
        printf("[Main]Read_Message_File is fail.\n");
        return 0;
    }

    //��������ͷ�ļ�
    if(false == Create_Base_Type_H(obj_Base_Type_List_info.m_vec_Base_Type_List))
    {
        return 0;
    }

    //���������ļ�
    Create_Base_Class_H();

    //��ʼ�����ļ�
    CReadObject objReadObject;

    for (int i = 0; i < (int)obj_vec_ObjectClass.size(); i++)
    {
        objReadObject.WriteClass(i, obj_vec_ObjectClass, obj_Base_Type_List_info);
    }

    for (int i = 0; i < (int)obj_vec_Message_Info.size(); i++)
    {
        objReadObject.WriteMessage(obj_vec_Message_Info[i], obj_Base_Type_List_info);
    }

    objReadObject.WriteListManager(obj_vec_ObjectClass, obj_Base_Type_List_info);
    objReadObject.WriteTestManager(obj_vec_ObjectClass, obj_Base_Type_List_info);

#ifdef WIN32
    getchar();
#endif
    return 0;
}
