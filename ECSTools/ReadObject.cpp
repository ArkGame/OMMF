#include "ReadObject.h"

CReadObject::CReadObject()
{

}

CReadObject::~CReadObject()
{

}

bool CReadObject::WriteClass(int i, vec_ObjectClass objObjectClassList, vec_Base_Type_List& obj_vec_Base_Type_List)
{
    //���ȴ���ͷ�ļ�
    if (false == Create_Object_H(i, objObjectClassList, obj_vec_Base_Type_List))
    {
        printf("[CReadObject::WriteClass]Create (%s) H file error.\n", objObjectClassList[i].m_strClassName.c_str());
        return false;
    }

    if (false == Create_Object_Cpp(i, objObjectClassList, obj_vec_Base_Type_List))
    {
        printf("[CReadObject::WriteClass]Create (%s) Cpp file error.\n", objObjectClassList[i].m_strClassName.c_str());
        return false;
    }

    return true;
}

bool CReadObject::WriteListManager(vec_ObjectClass objObjectClassList, vec_Base_Type_List& obj_vec_Base_Type_List)
{
    if (false == Create_List_Manager_H(objObjectClassList, obj_vec_Base_Type_List))
    {
        printf("[CReadObject::WriteListManager]Create (%s) H file error.\n", OBJECT_LIST_MANAGER_NAME);
        return false;
    }

    if (false == Create_List_Manager_Cpp(objObjectClassList, obj_vec_Base_Type_List))
    {
        printf("[CReadObject::WriteListManager]Create (%s) Cpp file error.\n", OBJECT_LIST_MANAGER_NAME);
        return false;
    }

    return true;
}

bool CReadObject::Check_Type_In_Class(string strBaseTypeName, vec_Object_Info obj_vec_Object_Info)
{
    for (int i = 0; i < (int)obj_vec_Object_Info.size(); i++)
    {
        if (strBaseTypeName == obj_vec_Object_Info[i].m_strType)
        {
            return true;
        }
    }

    return false;
}

bool CReadObject::Create_Object_H(int nIndex, vec_ObjectClass objObjectClassList, vec_Base_Type_List& obj_vec_Base_Type_List)
{
    char szHFileName[200] = { '\0' };
    char szCodeLine[MAX_CODE_LINE_SIZE]  = { '\0' };

    sprintf(szHFileName, "%s//%s.h", OBJECT_OUTPUT_PATH, objObjectClassList[nIndex].m_strClassName.c_str());
    FILE* pFile = fopen(szHFileName, "w");

    if (NULL == pFile)
    {
        printf("[CReadObject::Create_Object_H]fopen(%s) error.\n", szHFileName);
        return false;
    }

    //������Ҫ���õ�������ͷ�ļ�
    for (int j = 0; j < (int)objObjectClassList[nIndex].m_vec_Object_Info.size(); j++)
    {
        _Object_Info& obj_Object_Info = objObjectClassList[nIndex].m_vec_Object_Info[j];

        if (false == Check_Base_Type(obj_Object_Info.m_strType, obj_vec_Base_Type_List))
        {
            printf("[CReadObject::Create_Object_H]no find base type(%s:%s) at [%s].\n",
                   obj_Object_Info.m_strName.c_str(),
                   obj_Object_Info.m_strType.c_str(),
                   objObjectClassList[nIndex].m_strClassName.c_str());
            return false;
        }
    }

    //���ͷ�ļ�
    char szDefine[200] = { '\0' };
    To_Upper_String(objObjectClassList[nIndex].m_strClassName.c_str(), szDefine);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "//create %s\n\n", objObjectClassList[nIndex].m_strClassName.c_str());
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#ifndef _%s_H\n", szDefine);
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#define _%s_H\n\n", szDefine);
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#include <stdio.h>\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#include \"%s\"\n", OBJECT_BASETYPE_FILE);
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#include \"%s\"\n", OBJECT_BASECLASS_FILE);
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "class C%s : public IObject\n", objObjectClassList[nIndex].m_strClassName.c_str());
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "{\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "public:\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tC%s();\n", objObjectClassList[nIndex].m_strClassName.c_str());
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t~C%s();\n", objObjectClassList[nIndex].m_strClassName.c_str());
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tvoid Load_Param();\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tint Get_Class_ID();\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tvoid Init();\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    //�������е�дBaseType����
    for (int j = 0; j < (int)obj_vec_Base_Type_List.size(); j++)
    {
        if(true == Check_Type_In_Class(obj_vec_Base_Type_List[j].m_strBaseTypeName, objObjectClassList[nIndex].m_vec_Object_Info))
        {
            sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tint Set_Data(string strValueName, %s& Value);\n", obj_vec_Base_Type_List[j].m_strBaseTypeName.c_str());
            fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
        }
    }

    //�������еĶ�BaseType����
    for (int j = 0; j < (int)obj_vec_Base_Type_List.size(); j++)
    {
        if (true == Check_Type_In_Class(obj_vec_Base_Type_List[j].m_strBaseTypeName, objObjectClassList[nIndex].m_vec_Object_Info))
        {
            sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tint Get_Data(string strValueName, %s& Value);\n", obj_vec_Base_Type_List[j].m_strBaseTypeName.c_str());
            fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
        }
    }

    //����Get_Stream����
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tbool Get_Stream(char* pData, int& nLen);\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    //����Set_Stream����
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tbool Set_Stream(char* pData, int& nLen);\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "private:\n", objObjectClassList[nIndex].m_strClassName.c_str());
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tint m_nClassID;\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tint m_nBuffPacketSize;\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tchar m_szBuffPacket[%d];\n", objObjectClassList[nIndex].m_nBuffSize);
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tvec_Object_Info_List m_vec_Object_Info_List;\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "};\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#endif\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    fclose(pFile);

    return true;
}

bool CReadObject::Create_Object_Cpp(int nIndex, vec_ObjectClass objObjectClassList, vec_Base_Type_List& obj_vec_Base_Type_List)
{
    char szHFileName[200] = { '\0' };
    char szCodeLine[MAX_CODE_LINE_SIZE] = { '\0' };

    sprintf(szHFileName, "%s//%s.cpp", OBJECT_OUTPUT_PATH, objObjectClassList[nIndex].m_strClassName.c_str());
    FILE* pFile = fopen(szHFileName, "w");

    if (NULL == pFile)
    {
        printf("[CReadObject::Create_Object_Cpp]fopen(%s) error.\n", szHFileName);
        return false;
    }

    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#include \"%s.h\"\n\n", objObjectClassList[nIndex].m_strClassName.c_str());
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    //���칹�캯��
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "C%s::C%s()\n", objObjectClassList[nIndex].m_strClassName.c_str(), objObjectClassList[nIndex].m_strClassName.c_str());
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "{\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t Load_Param();\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "}\n\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    //��������
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "C%s::~C%s()\n", objObjectClassList[nIndex].m_strClassName.c_str(), objObjectClassList[nIndex].m_strClassName.c_str());
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "{\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "}\n\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    //���Get_Class_ID()����
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "int C%s::Get_Class_ID()\n", objObjectClassList[nIndex].m_strClassName.c_str());
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "{\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\treturn m_nClassID;\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "}\n\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    //��ʼ������
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "void C%s::Init()\n", objObjectClassList[nIndex].m_strClassName.c_str());
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "{\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    for (int i = 0; i < (int)objObjectClassList[nIndex].m_vec_Object_Info.size(); i++)
    {
        int nSize = Get_Base_Type_Size(objObjectClassList[nIndex].m_vec_Object_Info[i].m_strType, obj_vec_Base_Type_List);
        string strClass = Get_Base_Type_Class(objObjectClassList[nIndex].m_vec_Object_Info[i].m_strType, obj_vec_Base_Type_List);

        if ("single" == strClass)
        {
            sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t%s obj%s = %s;\n", objObjectClassList[nIndex].m_vec_Object_Info[i].m_strType.c_str(),
                         objObjectClassList[nIndex].m_vec_Object_Info[i].m_strName.c_str(),
                         objObjectClassList[nIndex].m_vec_Object_Info[i].m_strInit.c_str());
            fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
            sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tSet_Data(\"%s\", obj%s);\n", objObjectClassList[nIndex].m_vec_Object_Info[i].m_strName.c_str(),
                         objObjectClassList[nIndex].m_vec_Object_Info[i].m_strName.c_str());
            fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
        }
        else
        {
            sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t%s obj%s;\n", objObjectClassList[nIndex].m_vec_Object_Info[i].m_strType.c_str(),
                         objObjectClassList[nIndex].m_vec_Object_Info[i].m_strName.c_str());
            fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
            sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tmemcpy(&obj%s, (char* )\"%s\", %d);\n", objObjectClassList[nIndex].m_vec_Object_Info[i].m_strName.c_str(),
                         objObjectClassList[nIndex].m_vec_Object_Info[i].m_strInit.c_str(),
                         nSize);
            fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
            sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tSet_Data(\"%s\", obj%s);\n", objObjectClassList[nIndex].m_vec_Object_Info[i].m_strName.c_str(),
                         objObjectClassList[nIndex].m_vec_Object_Info[i].m_strName.c_str());
            fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
        }
    }

    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "}\n\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    //��ʼ���������Զ�����
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "void C%s::Load_Param()\n", objObjectClassList[nIndex].m_strClassName.c_str());
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "{\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tm_nClassID = %d;\n", objObjectClassList[nIndex].m_nClassID);
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t_Object_Info obj_Object_Info;\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    int nStartPos = 0;

    for (int i = 0; i < (int)objObjectClassList[nIndex].m_vec_Object_Info.size(); i++)
    {
        int nSize = Get_Base_Type_Size(objObjectClassList[nIndex].m_vec_Object_Info[i].m_strType, obj_vec_Base_Type_List);
        sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tobj_Object_Info.m_strName=\"%s\";\n", objObjectClassList[nIndex].m_vec_Object_Info[i].m_strName.c_str());
        fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
        sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tobj_Object_Info.m_strType=\"%s\";\n", objObjectClassList[nIndex].m_vec_Object_Info[i].m_strType.c_str());
        fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
        sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tobj_Object_Info.m_strMin=\"%s\";\n", objObjectClassList[nIndex].m_vec_Object_Info[i].m_strMin.c_str());
        fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
        sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tobj_Object_Info.m_strMax=\"%s\";\n", objObjectClassList[nIndex].m_vec_Object_Info[i].m_strMax.c_str());
        fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
        sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tobj_Object_Info.m_strInit=\"%s\";\n", objObjectClassList[nIndex].m_vec_Object_Info[i].m_strInit.c_str());
        fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
        sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tobj_Object_Info.m_nSize=%d;\n", nSize);
        fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
        sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tobj_Object_Info.m_nStartPos=%d;\n", nStartPos);
        fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
        nStartPos += nSize;
        sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tm_vec_Object_Info_List.push_back(obj_Object_Info);\n\n");
        fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    }

    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tm_nBuffPacketSize = %d;\n", objObjectClassList[nIndex].m_nBuffSize);
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "}\n\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    //�������е�дBaseType����
    for (int i = 0; i < (int)obj_vec_Base_Type_List.size(); i++)
    {
        if (true == Check_Type_In_Class(obj_vec_Base_Type_List[i].m_strBaseTypeName, objObjectClassList[nIndex].m_vec_Object_Info))
        {
            sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "int C%s::Set_Data(string strValueName, %s& Value)\n",
                         objObjectClassList[nIndex].m_strClassName.c_str(),
                         obj_vec_Base_Type_List[i].m_strBaseTypeName.c_str());
            fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
            sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "{\n");
            fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

            for (int j = 0; j < (int)objObjectClassList[nIndex].m_vec_Object_Info.size(); j++)
            {
                if (objObjectClassList[nIndex].m_vec_Object_Info[j].m_strType == obj_vec_Base_Type_List[i].m_strBaseTypeName)
                {
                    int nSize = Get_Base_Type_Size(objObjectClassList[nIndex].m_vec_Object_Info[j].m_strType, obj_vec_Base_Type_List);
                    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tif(strValueName == \"%s\")\n", objObjectClassList[nIndex].m_vec_Object_Info[j].m_strName.c_str());
                    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t{\n");
                    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t\tmemcpy(&m_szBuffPacket[m_vec_Object_Info_List[%d].m_nStartPos], (char* )&Value, m_vec_Object_Info_List[%d].m_nSize);\n", j, j);
                    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t\treturn %d;\n", nSize);
                    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t}\n");
                    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                }
            }

            sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\treturn 0;\n");
            fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
            sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "}\n");
            fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
        }
    }

    //�������ж�BaseType����
    for (int i = 0; i < (int)obj_vec_Base_Type_List.size(); i++)
    {
        if (true == Check_Type_In_Class(obj_vec_Base_Type_List[i].m_strBaseTypeName, objObjectClassList[nIndex].m_vec_Object_Info))
        {
            sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "int C%s::Get_Data(string strValueName, %s& Value)\n",
                         objObjectClassList[nIndex].m_strClassName.c_str(),
                         obj_vec_Base_Type_List[i].m_strBaseTypeName.c_str());
            fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
            sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "{\n");
            fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

            for (int j = 0; j < (int)objObjectClassList[nIndex].m_vec_Object_Info.size(); j++)
            {
                if (objObjectClassList[nIndex].m_vec_Object_Info[j].m_strType == obj_vec_Base_Type_List[i].m_strBaseTypeName)
                {
                    int nSize = Get_Base_Type_Size(objObjectClassList[nIndex].m_vec_Object_Info[j].m_strType, obj_vec_Base_Type_List);
                    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tif(strValueName == \"%s\")\n", objObjectClassList[nIndex].m_vec_Object_Info[j].m_strName.c_str());
                    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t{\n");
                    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t\tmemcpy((char* )&Value, &m_szBuffPacket[m_vec_Object_Info_List[%d].m_nStartPos], m_vec_Object_Info_List[%d].m_nSize);\n", j, j);
                    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t\treturn %d;\n", nSize);
                    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t}\n");
                    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
                }
            }

            sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\treturn 0;\n");
            fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
            sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "}\n");
            fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
        }
    }

    //����Get_Stream����
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "bool C%s::Get_Stream(char* pData, int& nLen)\n", objObjectClassList[nIndex].m_strClassName.c_str());
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "{\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tif(nLen < 0 || nLen > m_nBuffPacketSize)\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t{\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t\treturn false;\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t}\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tmemcpy(pData, m_szBuffPacket, m_nBuffPacketSize);\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tnLen = m_nBuffPacketSize;\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\treturn true;\n\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "}\n\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    //����Set_Stream����
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "bool C%s::Set_Stream(char* pData, int& nLen)\n", objObjectClassList[nIndex].m_strClassName.c_str());
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "{\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tif(nLen < 0 || nLen > m_nBuffPacketSize)\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t{\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t\treturn false;\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t}\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tmemcpy(m_szBuffPacket, pData, m_nBuffPacketSize);\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tnLen = m_nBuffPacketSize;\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\treturn true;\n\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "}\n\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    fclose(pFile);

    return true;
}

bool CReadObject::Create_List_Manager_H(vec_ObjectClass objObjectClassList, vec_Base_Type_List& obj_vec_Base_Type_List)
{
    //�������������
    char szHFileName[200] = { '\0' };
    char szCodeLine[MAX_CODE_LINE_SIZE] = { '\0' };

    sprintf(szHFileName, "%s//%s.h", OBJECT_OUTPUT_PATH, OBJECT_LIST_MANAGER_NAME);
    FILE* pFile = fopen(szHFileName, "w");

    if (NULL == pFile)
    {
        printf("[CReadObject::Create_List_Manager_H]fopen(%s) error.\n", szHFileName);
        return false;
    }

    //���������Ҫ�Ķ���ͷ�ļ�
    char szDefine[200] = { '\0' };
    To_Upper_String(OBJECT_LIST_MANAGER_NAME, szDefine);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#ifndef _%s_H\n", szDefine);
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#define _%s_H\n\n", szDefine);
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    for (int i = 0; i < (int)objObjectClassList.size(); i++)
    {
        sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#include \"%s.h\"\n", objObjectClassList[i].m_strClassName.c_str());
        fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    }

    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#include \"TemplateObjectList.h\"\n\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "class C%s\n", OBJECT_LIST_MANAGER_NAME);
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "{\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "public:\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tC%s();\n", OBJECT_LIST_MANAGER_NAME);
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t~C%s();\n", OBJECT_LIST_MANAGER_NAME);
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tvoid Init();\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tIObject* CreateObject(int nClassID, char* szUID, int& nUIDSize);\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tbool DeleteObject(int nClassID, char* szUID, int& nUIDSize, IObject* pObject);\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tIObject* GetObject(char* szUID, int& nUIDSize);\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "private:\n", OBJECT_LIST_MANAGER_NAME);
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    for (int i = 0; i < (int)objObjectClassList.size(); i++)
    {
        sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tCOjectList<C%s> m_obj%sList;\n",
                     objObjectClassList[i].m_strClassName.c_str(),
                     objObjectClassList[i].m_strClassName.c_str());
        fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    }

    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "};\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#endif\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    fclose(pFile);

    return true;
}

bool CReadObject::Create_List_Manager_Cpp(vec_ObjectClass objObjectClassList, vec_Base_Type_List& obj_vec_Base_Type_List)
{
    //�������������
    char szHFileName[200] = { '\0' };
    char szCodeLine[MAX_CODE_LINE_SIZE] = { '\0' };

    sprintf(szHFileName, "%s//%s.cpp", OBJECT_OUTPUT_PATH, OBJECT_LIST_MANAGER_NAME);
    FILE* pFile = fopen(szHFileName, "w");

    if (NULL == pFile)
    {
        printf("[CReadObject::Create_List_Manager_Cpp]fopen(%s) error.\n", szHFileName);
        return false;
    }

    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "#include \"%s.h\"\n\n", OBJECT_LIST_MANAGER_NAME);
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    //�������캯��
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "C%s::C%s()\n", OBJECT_LIST_MANAGER_NAME, OBJECT_LIST_MANAGER_NAME);
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "{\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "}\n\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    //������������
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "C%s::~C%s()\n", OBJECT_LIST_MANAGER_NAME, OBJECT_LIST_MANAGER_NAME);
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "{\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "}\n\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    //������ʼ������
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "void C%s::Init()\n", OBJECT_LIST_MANAGER_NAME);
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "{\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    for (int i = 0; i < (int)objObjectClassList.size(); i++)
    {
        sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tm_obj%sList.Init(%d, %d);\n",
                     objObjectClassList[i].m_strClassName.c_str(),
                     objObjectClassList[i].m_nClassID,
                     objObjectClassList[i].m_nObjectListCount);
        fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    }

    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "}\n\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    //������ȡ������
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "IObject* C%s::CreateObject(int nClassID, char* szUID, int& nUIDSize)\n", OBJECT_LIST_MANAGER_NAME);
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "{\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    for (int i = 0; i < (int)objObjectClassList.size(); i++)
    {
        sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tif(nClassID == %d)\n",
                     objObjectClassList[i].m_nClassID);
        fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
        sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t{\n");
        fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
        sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t\treturn (IObject* )m_obj%sList.Create(szUID, nUIDSize);\n",
                     objObjectClassList[i].m_strClassName.c_str());
        fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
        sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t}\n");
        fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    }

    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\treturn NULL;\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "}\n\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    //�������պ���
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "bool C%s::DeleteObject(int nClassID, char* szUID, int& nUIDSize, IObject* pObject)\n",
                 OBJECT_LIST_MANAGER_NAME);
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "{\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    for (int i = 0; i < (int)objObjectClassList.size(); i++)
    {
        sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tif(nClassID == %d)\n",
                     objObjectClassList[i].m_nClassID);
        fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
        sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t{\n");
        fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
        sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t\treturn m_obj%sList.Delete(szUID, nUIDSize, (C%s* )pObject);\n",
                     objObjectClassList[i].m_strClassName.c_str(),
                     objObjectClassList[i].m_strClassName.c_str());
        fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
        sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t}\n");
        fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    }

    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\treturn false;\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "}\n\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    //������ö�����
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "IObject* C%s::GetObject(char* szUID, int& nUIDSize)\n", OBJECT_LIST_MANAGER_NAME);
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "{\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tint nClassID = 0;\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tint nPos = 0;\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tif (0 == Get_Object_UID_Info(szUID, nUIDSize, nClassID, nPos))\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t{\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t\r return NULL;\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t}\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    for (int i = 0; i < (int)objObjectClassList.size(); i++)
    {
        sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\tif(nClassID == %d)\n",
                     objObjectClassList[i].m_nClassID);
        fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
        sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t{\n");
        fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
        sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t\treturn (IObject* )m_obj%sList.Get_Object(szUID, nUIDSize);\n",
                     objObjectClassList[i].m_strClassName.c_str());
        fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
        sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\t}\n");
        fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    }

    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "\treturn NULL;\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);
    sprintf_safe(szCodeLine, MAX_CODE_LINE_SIZE, "}\n\n");
    fwrite(szCodeLine, strlen(szCodeLine), sizeof(char), pFile);

    fclose(pFile);
    return true;
}

