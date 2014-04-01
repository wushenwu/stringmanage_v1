#include <stdio.h>
#include "memmanage.h"
#include "fit.h"

//the way you can get suitable size
//can only be dealed within this file
static int g_fitWAY = FIT_EQUAL;

/*
Function: 从g_del_table中找到一个(size, start)记录，使得size = requestsize
Param	: requestsize 是想申请的空间大小， 
　　      index用于接收符合条件(size, start) 在g_del_table中的下标
Return  :  NOT_EXIST 失败， SUCCESS成功，并设置index.
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
Function: 从g_del_table中找到一个(size, start)记录，使得size 刚好>= 
　　       requestsize，
Param	: requestsize 是想申请的空间大小， 
          index用于接收符合条件(size, start) 在g_del_table中的下标
Return  :  NOT_EXIST 失败， SUCCESS成功，并设置index.
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
Function: 从g_del_table中找到一个(size, start)记录，使得size 是第一个>= 
　　        requestsize，
Param	: requestsize 是想申请的空间大小， 
          index用于接收符合条件(size, start) 在g_del_table中的下标
Return  :  NOT_EXIST 失败， SUCCESS成功，并设置index.
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
Function: 根据g_fitWAY，从g_del_table中找到一个合适的(size, start)记录，
　　      是对上面fitxxx函数的封装。
　　 
Param	: requestsize 是想申请的空间大小， 
          index用于接收符合条件(size, start) 在g_del_table中的下标
Return  :  NOT_EXIST 失败， SUCCESS 成功，并设置index.
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