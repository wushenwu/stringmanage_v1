#include <stdio.h>
#include "memmanage.h"
#include "table.h"

void
insertintoaddtable(int *table, int *max,  int size, int start)
{
    int i, j;

    //find the proper position
    for (i = *max; i >= 2; i -= 2)
    {
#ifdef ARY_VERSION
        if (table[i - 1] <= start)
#else
        if (*(table + i - 1) <= start)
#endif
        {
            break;
        }
    }

    //now we need to move some data foreward
    for (j = *max; j >= i + 2; j -= 2)
    {
#ifdef ARY_VERSION
        table[j + 1] = table[j - 1];
        table[j] = table[j - 2];
#else
        *(table + j + 1) = *(table + j - 1);
        *(table + j)     = *(table + j - 2);
#endif
    }

    //now insert (size, start)
#ifdef ARY_VERSION
    table[i]     = size;
    table[i + 1] = start;
#else
    *(table + i)     = size;
    *(table + i + 1) = start;
#endif

    *max += 2;
}

/*
Function: 考虑g_del_table中，index与其前驱、后继是否为连续的空闲空间，进行整合，
           以获取更大的空闲空间，便于后续再分配
Param   : index

Return  : None

Effect  : 可能调用removefromtable，间接更新g_indexMAX_del

Attention: this function can only be called by insertintodeltable
*/
void
reorganize(int index)
{
    int indexpre, indexaft;
    int sizepre,  sizeaft;
    int startpre, startaft;

    int size, start;

    
    //now we need to consider the probable free space reorganize.
#ifdef ARY_VERSION    
    size  = g_del_table[index];
    start = g_del_table[index + 1];
#else
    size  = *(g_del_table + index);
    start = *(g_del_table + index + 1);
#endif
   
    
    //aft
    if ((index + 2) != g_indexMAX_del)
    {
        indexaft = index + 2;  
#ifdef ARY_VERSION
        sizeaft  = g_del_table[indexaft];
        startaft = g_del_table[indexaft + 1];
#else
        sizeaft  = *(g_del_table + indexaft);
        startaft = *(g_del_table + indexaft + 1);
#endif

        if (startaft == start + size)
        {
            updatetable(g_del_table, index, size + sizeaft, start);
            removefromtable(g_del_table, &g_indexMAX_del, indexaft);
            
            printf("reorganize space aft happens\r\n");
        }
    }
    
    //pre
    if (index != 0)
    {
        indexpre = index - 2;   
#ifdef ARY_VERSION
        sizepre  = g_del_table[indexpre];
        startpre = g_del_table[indexpre + 1];
#else
        sizepre  = *(g_del_table + indexpre);
        startpre = *(g_del_table + indexpre + 1);
#endif
        
        if (start == startpre + sizepre)
        {
            //take care, may size already updated, we need to use g_del_table[index]
#ifdef ARY_VERSION
            updatetable(g_del_table, indexpre, sizepre + g_del_table[index], startpre);
            removefromtable(g_del_table, &g_indexMAX_del, index);
#else
            updatetable(g_del_table, indexpre, sizepre + (*(g_del_table + index)), startpre);
            removefromtable(g_del_table, &g_indexMAX_del, index);
#endif
            
            printf("reorganize space pre happens\r\n");
        }   
    } 
}



void
insertintodeltable(int *table, int *max, int size, int start)
{
    int i, j;
    
    //find the proper position
    for (i = *max; i >= 2; i -= 2)
    {
#ifdef ARY_VERSION
        if (table[i - 1] <= start)
#else
        if (*(table + i - 1) <= start)
#endif
        {
            break;
        }
    }
    
    //now we need to move some data foreward
    for (j = *max; j >= i + 2; j -= 2)
    {
#ifdef ARY_VERSION
        table[j + 1] = table[j - 1];
        table[j]     = table[j - 2];
#else
        *(table + j + 1) = *(table + j - 1);
        *(table + j)     = *(table + j - 2);
#endif
    }
    
    //now insert (size, start)
#ifdef  ARY_VERSION
    table[i]     = size;
    table[i + 1] = start;
#else
    *(table + i)     = size;
    *(table + i + 1) = start;
#endif
        
    *max += 2; 

    //reorganize
    reorganize(i);
}


void
removefromtable(int *table, int *max, int index)
{
    int i;
    for (i = index; i <= *max - 4; i += 2)
    {
#ifdef ARY_VERSION
        table[i]     = table[i + 2];
        table[i + 1] = table[i + 3];  
#else
        *(table + i)        = *(table + i + 2);
        *(table + i + 1)    = *(table + i + 3);
#endif
    }

#ifdef ARY_VERSION
    table[*max - 1] = 0;
    table[*max - 2] = 0;
#else
    *(table + *max - 1) = 0;
    *(table + *max - 2) = 0;
#endif

    *max -= 2;
}


void
updatetable(int *table, int index, int size, int start)
{
#ifdef ARY_VERSION
    table[index]        = size;
    table[index + 1]    = start;
#else
    *(table + index)        = size;
    *(table + index + 1)    = start;
#endif
}


int
gettable(int index, int *max)
{
    static int s_index_add = 0;
    static int s_index_del = 0;

    int table = -1;

    int size_add, start_add;
    int size_del, start_del;

    size_add    = -1;
    start_add   = -1;
    size_del    = -1;
    start_del   = -1;


    if (0 == index)
    {
        s_index_add = 0;
        s_index_del = 0;
    }

    if (0 != g_indexMAX_add)
    {
#ifdef ARY_VERSION
        size_add        = g_add_table[s_index_add];
        start_add       = g_add_table[s_index_add + 1];
#else
        size_add        = *(g_add_table + s_index_add);
        start_add       = *(g_add_table + s_index_add + 1);
#endif
    }
        
    if (0 != g_indexMAX_del)
    {
#ifdef  ARY_VERSION
        size_del        = g_del_table[s_index_del];
        start_del       = g_del_table[s_index_del + 1];
#else
        size_del        = *(g_del_table + s_index_del);
        start_del       = *(g_del_table + s_index_del + 1);
#endif
    }
        
    if ((index >= start_add) && (index <= start_add + size_add))
    {
        *max = start_add + size_add;

        //now need to move toward
        s_index_add += 2;
        return ADD;
    }
    
    if ((index >= start_del) && (index <= start_del + size_del))
    {
        *max = start_del + size_del;

        //
        s_index_del += 2;
        return DEL;
    }

    //should not come to this point.
    return table;
}