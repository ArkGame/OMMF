#ifndef _READFILE_H
#define _READFILE_H

#include "Common.h"

class CReadObject
{
public:
    CReadObject();
    ~CReadObject();

    //����XML���ɴ�����������
    bool WriteFunction(_Function_Info& obj_Function_Info);
    //����XML���ɴ�����Ϣ����
    bool WriteMessage(_Message_Info objMessageInfo, _Base_Type_List_info obj_Base_Type_List_info);
    //����ĳһ�������ʵ�������
    bool WriteClass(int i, vec_ObjectClass objObjectClassList, _Base_Type_List_info obj_Base_Type_List_info);
    //����ObjectManager��
    bool WriteListManager(vec_ObjectClass objObjectClassList, _Base_Type_List_info obj_Base_Type_List_info);
    //���ɲ��������
    bool WriteTestManager(vec_ObjectClass objObjectClassList, _Base_Type_List_info obj_Base_Type_List_info);

private:
    //�ж�һ�������ǲ��ǻ�������
    bool Check_Type_In_Class(string strBaseTypeName, vec_Object_Info obj_vec_Object_Info);
    //���������h�ļ�
    bool Create_Object_H(int nIndex, vec_ObjectClass objObjectClassList, _Base_Type_List_info obj_Base_Type_List_info);
    //���������cpp�ļ�
    bool Create_Object_Cpp(int nIndex, vec_ObjectClass objObjectClassList, _Base_Type_List_info obj_Base_Type_List_info);
    //������Ϣ�����ļ�
    bool Create_Message_H(_Message_Info objMessageInfo, _Base_Type_List_info obj_Base_Type_List_info);
    //���ɺ�������h�ļ�
    bool Create_Function_H(_Function_Info& obj_Function_Info);
    //���ɺ�������cpp�ļ�
    bool Create_Function_Cpp(_Function_Info& obj_Function_Info);
    //����ObjectManager����h�ļ�
    bool Create_List_Manager_H(vec_ObjectClass objObjectClassList, vec_Base_Type_List& obj_vec_Base_Type_List);
    //����ObjectManager����cpp�ļ�
    bool Create_List_Manager_Cpp(vec_ObjectClass objObjectClassList, vec_Base_Type_List& obj_vec_Base_Type_List);
    //���ɲ���Test���̴���
    bool Create_Test_Manager_Cpp(vec_ObjectClass objObjectClassList, vec_Base_Type_List& obj_vec_Base_Type_List);
    //����Test���Ե�makefile����
    bool Create_Test_Make(vec_ObjectClass objObjectClassList, vec_Base_Type_List& obj_vec_Base_Type_List);
    //����Test���Ե�Make.Define����
    bool Create_Test_Make_Define(vec_ObjectClass objObjectClassList, vec_Base_Type_List& obj_vec_Base_Type_List);
};

#endif
