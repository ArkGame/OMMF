#ifndef _MYSQL_OBJECT
#define _MYSQL_OBJECT

#include "Common.h"
#include "TemplateObjectList.h"

class CMysqlObject
{
public:
    CMysqlObject();
    ~CMysqlObject();

    bool WriteMysqlCode(vec_Xml_Mysql_DB objMysqlDBList, vec_ObjectClass objObjectList, _Base_Type_List_info obj_Base_Type_List_info);

private:
    _ObjectClass* Get_ObjectClass(int nClassID, vec_ObjectClass& objObjectList);
    int Get_Object_Size(string strName, _Base_Type_List_info obj_Base_Type_List_info);
    bool Create_Mysql_Code_H(vec_Xml_Mysql_DB objMysqlDBList, vec_ObjectClass objObjectList, _Base_Type_List_info obj_Base_Type_List_info);
    bool Create_Mysql_Code_Cpp(vec_Xml_Mysql_DB objMysqlDBList, vec_ObjectClass objObjectList, _Base_Type_List_info obj_Base_Type_List_info);
    bool Create_Mysql_Script(vec_Xml_Mysql_DB objMysqlDBList, vec_ObjectClass objObjectList, _Base_Type_List_info obj_Base_Type_List_info);
};
#endif