#ifndef _TAMPLATEOBJECT_H
#define _TAMPLATEOBJECT_H

#include "BaseObject.h"
#include <stdio.h>
#include <time.h>

#define GUID_SIZE 24

struct _Object_Data_Info
{
    IObject*  m_pObject;       //ָ�����
    int       m_nState;       //��ǰָ��״̬��0�������ã�1δʹ�ã�2����ʹ��

    _Object_Data_Info()
    {
        Init();
    }

    void Init()
    {
        m_pObject = NULL;
        m_nState  = 0;
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

        m_objectList = new _Object_Data_Info[nCount];

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
                Create_Object_UID(pObjectUID, nLen, i);
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
                Create_Object_UID(pObjectUID, nLen, i);
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

        if (0 == Get_Object_UID_Info(pObjectUID, nLen, nType, nPos))
        {
            //���ն���
            m_objectList[nPos].m_nState = 1; //���Ϊδʹ��
            return true;
        }
        else
        {
            return false;
        }
    }

    //�õ���ǰ�Ļ�����е�Count
    int Get_Count()
    {
        return m_nCount;
    }

private:
    //����һ��ObjectID
    int Create_Object_UID(char* pObjectUID, int& nLen, int nPos)
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
        localtime_r(&tmTime, &ttCurrentTime);
#endif
        char szTimeNow[50] = { '\0' };
        sprintf(szTimeNow, "%04d%02d%02d%02d%02d%02d",
                tmTime.tm_year + 1900,
                tmTime.tm_mon + 1,
                tmTime.tm_mday,
                tmTime.tm_hour,
                tmTime.tm_min,
                tmTime.tm_sec);

        sprintf(pObjectUID, "%s%04d%06d", szTimeNow, m_nType, nPos);
        nLen = GUID_SIZE;
        return 0;
    }

    //��һ��ObjectUID�л�õ�ǰ����ID
    int Get_Object_UID_Info(char* pObjectUID, int nLen, int& nType, int& nPos)
    {
        if (GUID_SIZE != nLen)
        {
            printf("[Get_Object_UID_Info]error GUID_SIZE(%d).\n", nLen);
            return -1;
        }

        char szType[10] = { '\0' };
        char szPos[10]  = { '\0' };

        memcpy(szType, &pObjectUID[14], 4);
        memcpy(szPos, &pObjectUID[18], 6);

        nType = atoi(szType);
        nPos  = atoi(szPos);

        if (nType != m_nType || nPos < 0 || nPos >= m_nCount)
        {
            printf("[Get_Object_UID_Info]error nType=%d, nPos=%d.\n", nType, nPos);
            return -1;
        }

        return 0;
    }

private:
    int                m_nType;           //��ǰ�����������
    int                m_nCount;          //��ǰ�����������ĸ���
    int                m_nUsedCount;      //��ǰ����ʹ�ö���ĸ���
    int                m_nCurrIndex;      //��ǰָ�룬����Createʹ��
    _Object_Data_Info* m_objectList;      //����ָ������
};

#endif
