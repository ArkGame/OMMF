#ifndef _TAMPLATEOBJECT_H
#define _TAMPLATEOBJECT_H

#include "BaseObject.h"
#include <time.h>


#define GUID_SIZE 24

//����һ��ObjectID
static int Create_Object_UID(char* pObjectUID, int& nLen, int nType, int nPos)
{
    if (nLen < GUID_SIZE)
    {
        //����GUID�ַ����ڴ治��
        printf("[Create_Object_UID]no enough GUID_SIZE(%d).\n", nLen);
        return -1;
    }

    //��������ʱ��+ClassID+�����±�
    time_t ttCurrentTime;
    time(&ttCurrentTime);
    struct tm tmTime;
#ifdef WIN32
    localtime_s(&tmTime, &ttCurrentTime);
#else
    localtime_r(&ttCurrentTime, &tmTime);
#endif
    char szTimeNow[50] = { '\0' };
    sprintf(szTimeNow, "%04d%02d%02d%02d%02d%02d",
            tmTime.tm_year + 1900,
            tmTime.tm_mon + 1,
            tmTime.tm_mday,
            tmTime.tm_hour,
            tmTime.tm_min,
            tmTime.tm_sec);

    sprintf(pObjectUID, "%s%04d%06d", szTimeNow, nType, nPos);
    nLen = GUID_SIZE;
    return 0;
}

//��һ��ObjectUID�л�õ�ǰ����ID
static int Get_Object_UID_Info(char* pObjectUID, int nLen, int& nType, int& nPos)
{
    if (GUID_SIZE != nLen)
    {
        printf("[Get_Object_UID_Info]error GUID_SIZE(%d).\n", nLen);
        return -1;
    }

    char szType[10] = { '\0' };
    char szPos[10] = { '\0' };

    memcpy(szType, &pObjectUID[14], 4);
    memcpy(szPos, &pObjectUID[18], 6);

    nType = atoi(szType);
    nPos = atoi(szPos);

    return 0;
}

//��Ӧ����Ĳ�λ
struct _Object_Data_Solt
{
    IObject*  m_pObject;            //ָ�����
    int       m_nState;             //��ǰָ��״̬��0�������ã�1δʹ�ã�2����ʹ��
    char      m_szUUID[GUID_SIZE];  //��ǰ�����ΨһID

    _Object_Data_Solt()
    {
        Init();
    }

    void Init()
    {
        m_pObject              = NULL;
        m_nState               = 0;
        m_szUUID[0]            = '\0';
    }

    void Set_UUID(const char* pUUID)
    {
        sprintf(m_szUUID, "%s", pUUID);
    }

    void Clear()
    {
        m_nState = 1;
        m_szUUID[0] = '\0';
    }
};

template <class T>
class COjectList
{
public:
    COjectList()
    {
        m_nType      = 0;
        m_nCount     = 0;
        m_objectList = NULL;

        Create_Object_UID_Fn = Create_Object_UID;
        Get_Object_UID_Info_Fn = Get_Object_UID_Info;
    };

    ~COjectList()
    {
        Close();
    };

    void Close()
    {
        delete[] m_objectList;
        m_objectList = NULL;
    }

    int Init(int nType, int nCount)
    {
        Close();

        m_nType    = nType;
        m_nCount   = nCount;

        m_objectList = new _Object_Data_Solt[nCount];

        for (int i = 0; i < m_nCount; i++)
        {
            m_objectList[i].m_pObject = (IObject* )new T();
            m_objectList[i].m_nState  = 1;
        }

        m_nUsedCount = 0;
        m_nCurrIndex = 0;

        return 0;
    }

    //���ڴ���л�ȡһ�����еĶ���
    T* Create(char* pObjectUID, int& nLen)
    {
        //��m_nCurrIndex������
        for (int i = m_nCurrIndex; i < m_nCount; i++)
        {
            if (m_objectList[i].m_nState == 1)
            {
                m_objectList[i].m_nState = 2; //���λ����ʹ��
                m_nCurrIndex = i;
                Create_Object_UID_Fn(pObjectUID, nLen, m_nType, i);
                m_objectList[i].Set_UUID(pObjectUID);   //�ڲ�λ��¼��ǰ��UUID
                return (T*)m_objectList[i].m_pObject;
            }
        }

        //���Ѱ�ҵ�ĩβ��û�ҵ�����ͷ��ʼ��
        for (int i = 0; i < m_nCurrIndex; i++)
        {
            if (m_objectList[i].m_nState == 1)
            {
                m_objectList[i].m_nState = 2; //���λ����ʹ��
                m_nCurrIndex = i;
                Create_Object_UID_Fn(pObjectUID, nLen, m_nType, i);
                m_objectList[i].Set_UUID(pObjectUID);   //�ڲ�λ��¼��ǰ��UUID
                return (T*)m_objectList[i].m_pObject;
            }
        }

        //�����û���ҵ�
        printf("[Create]Object no can use object.\n");
        return NULL;
    }

    //�黹һ������ʹ�õĶ���
    bool Delete(char* pObjectUID, int nLen, T* pt)
    {
        int nType = 0;
        int nPos  = 0;

        if (NULL == pt)
        {
            printf("[Delete]pObjectUID is NULL.\n");
            return false;
        }

        if (0 == Get_Object_UID_Info_Fn(pObjectUID, nLen, nType, nPos))
        {
            if (nType != m_nType || nPos < 0 || nPos >= m_nCount)
            {
                printf("[Delete]error nType=%d, nPos=%d.\n", nType, nPos);
                return false;
            }

            //���ն���
            m_objectList[nPos].Clear();
            return true;
        }
        else
        {
            return false;
        }
    }

    //�ҵ�һ��ָ���Ķ���
    T* Get_Object(char* pObjectUID, int& nLen)
    {
        int nType = 0;
        int nPos = 0;

        if (0 == Get_Object_UID_Info_Fn(pObjectUID, nLen, nType, nPos))
        {
            if (nType != m_nType || nPos < 0 || nPos >= m_nCount)
            {
                printf("[Get_Object]error nType=%d, nPos=%d.\n", nType, nPos);
                return NULL;
            }

            if (2 == m_objectList[nPos].m_nState)
            {
                return (T*)m_objectList[nPos].m_pObject;
            }
            else
            {
                return NULL;
            }
        }
        else
        {
            return NULL;
        }
    }

    //�õ���ǰ�Ļ�����е�Count
    int Get_Count()
    {
        return m_nCount;
    }

private:
    int (*Create_Object_UID_Fn)(char* pObjectUID, int& nLen, int nType, int nPos);               //��������UID����Ϣ����ָ��
    int (*Get_Object_UID_Info_Fn)(char* pObjectUID, int nLen, int& nType, int& nPos);       //���ָ������UID���ຯ��ָ��

    int                m_nType;           //��ǰ�����������
    int                m_nCount;          //��ǰ�����������ĸ���
    int                m_nUsedCount;      //��ǰ����ʹ�ö���ĸ���
    int                m_nCurrIndex;      //��ǰָ�룬����Createʹ��
    _Object_Data_Solt* m_objectList;      //����ָ������
};

#endif
