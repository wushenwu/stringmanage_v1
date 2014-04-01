#include <stdio.h>
#include "memmanage.h"
#include "fit.h"

//the way you can get suitable size
//can only be dealed within this file
static int g_fitWAY = FIT_EQUAL;

/*
Function: ��g_del_table���ҵ�һ��(size, start)��¼��ʹ��size = requestsize
Param	: requestsize ��������Ŀռ��С�� 
����      index���ڽ��շ�������(size, start) ��g_del_table�е��±�
Return  :  NOT_EXIST ʧ�ܣ� SUCCESS�ɹ���������index.
*/
int 
fitequal(int requestsize, int *index)
{
    int i;
    int result;

    result = NOT_EXIST;

    for (i = 0; i < g_indexMAX_del; i += 2)
    {
#ifdef ARY_VERSION
        if (requestsize == g_del_table[i])
#else
        if (requestsize == *(g_del_table + i))
#endif
        {
            *index = i;
            result = SUCCESS;

            break;
        }
    }

    return result;
}

/*
Function: ��g_del_table���ҵ�һ��(size, start)��¼��ʹ��size �պ�>= 
����       requestsize��
Param	: requestsize ��������Ŀռ��С�� 
          index���ڽ��շ�������(size, start) ��g_del_table�е��±�
Return  :  NOT_EXIST ʧ�ܣ� SUCCESS�ɹ���������index.
*/
int 
fitenough(int requestsize, int *index)
{
    int i;
    int min;
    int result;

    result = NOT_EXIST;

    for (i = 0, min = -1; i < g_indexMAX_del; i += 2)
    {
#ifdef ARY_VERSION
        if (requestsize <= g_del_table[i])
#else
        if (requestsize <= *(g_del_table + i))
#endif
        {
#ifdef ARY_VERSION
            if ((-1 == min) || (g_del_table[i] < g_del_table[min]))
#else
            if ((-1 == min) || (*(g_del_table + i) < *(g_del_table + min)))
#endif
            {
                min = i;
            }
        }
    }

    if (-1 != min)
    {
        *index = min;
        result = SUCCESS;
    }

    return result;
}

/*
Function: ��g_del_table���ҵ�һ��(size, start)��¼��ʹ��size �ǵ�һ��>= 
����        requestsize��
Param	: requestsize ��������Ŀռ��С�� 
          index���ڽ��շ�������(size, start) ��g_del_table�е��±�
Return  :  NOT_EXIST ʧ�ܣ� SUCCESS�ɹ���������index.
*/
int 
fitfirst(int requestsize, int *index)
{
    int i;
    int result;

    result = NOT_EXIST;

    for (i = 0; i < g_indexMAX_del; i += 2)
    {
#ifdef ARY_VERSION
        if (requestsize <= g_del_table[i])
#else
        if (requestsize <= *(g_del_table + i))
#endif
        {
            *index = i;
            result = SUCCESS;

            break;
        }
    }

    return result;
}

/*
Function: ����g_fitWAY����g_del_table���ҵ�һ�����ʵ�(size, start)��¼��
����      �Ƕ�����fitxxx�����ķ�װ��
���� 
Param	: requestsize ��������Ŀռ��С�� 
          index���ڽ��շ�������(size, start) ��g_del_table�е��±�
Return  :  NOT_EXIST ʧ�ܣ� SUCCESS �ɹ���������index.
*/
int 
getsize(int requestsize, int *index)
{
    int result;

    result = NOT_EXIST;

    switch (g_fitWAY)
    {
    case FIT_EQUAL:
        result = fitequal(requestsize, index);
        break;

    case FIT_ENOUGH:
        result = fitenough(requestsize, index);
        break;

    case FIT_FIRST:
        result = fitfirst(requestsize, index);
        break;

    case FIT_EQUAL_ENOUGH:
        result = fitequal(requestsize, index);
        if (NOT_EXIST == result)
        {
            result = fitenough(requestsize, index);
        }
        break;

    case FIT_EQUAL_FIRST:
        result = fitequal(requestsize, index);
        if (NOT_EXIST == result)
        {
            result = fitfirst(requestsize, index);
        }
        break;

    default:
        //sth wrong ?
        break;
    }

    return result;
}

void
setfit(int select)
{
   g_fitWAY = select;
}