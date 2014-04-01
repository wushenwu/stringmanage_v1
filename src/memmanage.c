#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "memmanage.h"
#include "fit.h"
#include "table.h"
#include "ui.h"

/*storage for your input. can only be dealed within file*/
static char g_chBuf[MAX_SPACE];
static int g_indexMAX_buf;  //how many strings already stored into this buf.  

/*used for maintain*/
int g_add_table[MAX_SPACE];
int g_indexMAX_add;         //how many (size, start) are stored into table;

//del_table
int g_del_table[MAX_SPACE];
int g_indexMAX_del;         //how many (size, start) are stored into table;

/*char statistics ( those printable but not space) can only be dealed within file*/
static int g_char_table[MAX_CHAR];
static int g_totalchar;


int 
query(char *chInput, int *index)
{
    int i;
    int size;
    int result;

    result = NOT_EXIST;

    size = strlen(chInput) + 1;

    for (i = 0; i < g_indexMAX_add; i += 2)
    {
#ifdef ARY_VERSION
        if (size != g_add_table[i])
#else
        if (size != *(g_add_table + i) )
#endif
        {
            continue;
        }

#ifdef  ARY_VERSION
        if (0 == strcmp(&g_chBuf[g_add_table[i + 1]], chInput))
#else
        if (0 == strcmp(g_chBuf + (*( g_add_table + i + 1)), chInput))
#endif
        {
            *index = i;
            result = SUCCESS;

            break;
        }
    }

    return result;
}

int
query_about(char *chInput, int *indexary, int *num)
{
    int i;
    int size;
    int result;
    
    result = NOT_EXIST;
    
    size = strlen(chInput) + 1;
    *num = 0;
    
    for (i = 0; i < g_indexMAX_add; i += 2)
    {
#ifdef ARY_VERSION
        if (size > g_add_table[i])
#else
        if (size > *(g_add_table + i) )
#endif
        {
            continue;
        }
            
#ifdef  ARY_VERSION
        if (NULL != strstr(&g_chBuf[g_add_table[i + 1]], chInput))
#else
        if (NULL != strstr(g_chBuf + (*( g_add_table + i + 1)), chInput))
#endif
        {
            //
            *indexary = i;
            indexary++;

            //
            *num += 1;
         }
    }

    if (0 != *num)
    {
        result = SUCCESS;
    }
    
    return result;

}


int
store(char *chBuf, int start, char *chInput, int requestsize)
{
    int result = SUCCESS;

    //make sure g_indexMAX_buf updated
    if (start == g_indexMAX_buf)
    {
        if (MAX_SPACE == g_indexMAX_buf)
        {
            result = NO_SPACE;
            return result;
            
        }
        g_indexMAX_buf += requestsize;
    }

#ifdef ARY_VERSION
    strcpy(&chBuf[start], chInput);

    chBuf[start + requestsize - 1] = '\0';
#else
    strcpy(chBuf + start, chInput );

    *(chBuf + start + requestsize - 1) = '\0';
#endif  

    return result;
} 


int
add(char *chInput)
{
    int requestsize;
    int index;  //index of (size, start) within g_del_table
    int start;
    int size;

    int result;
    result = NO_SPACE;
        
    //we also store the '\0'
    requestsize = strlen(chInput) + 1;

    result = getsize(requestsize, &index);

    //no suitable size, we just store into g_chBuf[g_indexMAX_buf]
    if (NOT_EXIST == result)
    {
        result = store(g_chBuf, g_indexMAX_buf, chInput, requestsize);
       
        if (NO_SPACE == result)
        {
            //failed, no space
            return result;
        }

        start = g_indexMAX_buf - requestsize;

        updatetable(g_add_table, g_indexMAX_add, requestsize, g_indexMAX_buf - requestsize);

        g_indexMAX_add += 2;        
    }
    else
    {
#ifdef  ARY_VERSION
        size  = g_del_table[index];
        start = g_del_table[index + 1];
#else
        size  = *(g_del_table + index);
        start = *(g_del_table + index + 1);
#endif
        result = store(g_chBuf, start, chInput, requestsize);

        if (NO_SPACE == result)
        {
            //failed
            return result;
        }


        if (requestsize == size)
        {
            removefromtable(g_del_table, &g_indexMAX_del, index);
            insertintoaddtable(g_add_table, &g_indexMAX_add, size, start);
        }
        else
        {
            updatetable(g_del_table, index, size - requestsize, start + requestsize);
            insertintoaddtable(g_add_table, &g_indexMAX_add, requestsize, start);
        }
    }//end for else

    result = start;

    return result;
}


int
del(char *chInput)
{
    //indexpre(sizepre, startpre), index(size, start), indexaft(sizeaft, startaft)
    int index;
    int size;
    int start;
    int result;

    result = query(chInput, &index);

    if (NOT_EXIST == result)
    {
        //failed; not exist
        return result;
    }
    else
    {
#ifdef ARY_VERSION
        size  = g_add_table[index];
        start = g_add_table[index + 1];
#else
        size  = *(g_add_table + index);
        start = *(g_add_table + index + 1);
#endif

        insertintodeltable(g_del_table, &g_indexMAX_del, size, start);
        removefromtable(g_add_table, &g_indexMAX_add, index); 
        

    }//end for result != NOT_EXIST

    return result;
}


int
modify(char *origin, char *newinput)
{
    int index;
    int oldsize, newsize;
    int start;

    int result;

    result = query(origin, &index);

    if (NOT_EXIST == result)
    {
        //not exist
        return result;
    }
    else    
    {
        oldsize = strlen(origin) + 1;
        newsize = strlen(newinput) + 1;

#ifdef ARY_VERSION
        start = g_add_table[index + 1];
#else
        start = *(g_add_table + index + 1);
#endif

       
        if (newsize < oldsize)
        {
            // this will generate unused space, we need to update g_add_table and g_del_table;
            result = store(g_chBuf, start, newinput, newsize);
            if (NO_SPACE == result)
            {
                //failed, no more space
                return result;
            }

            updatetable(g_add_table, index, newsize, start);
            insertintodeltable(g_del_table, &g_indexMAX_del, oldsize - newsize, start + newsize);
        }
        else if (newsize == oldsize)
        {
            //just replace directly
            result = store(g_chBuf, start, newinput, newsize);
            if (NO_SPACE == result)
            {
                //failed, no space.
                return result;
            }

        }
        else
        {
            del(origin);
            result = add(newinput);
        }
    }//end for result != NOT_EXIST

    return result;
}

void
display(void)
{
    int i;
    int size;
    int start;
    char szhitinfo[MAX_INPUT] = {0};

    for (i = 0; i < g_indexMAX_add; i +=2)
    {
#ifdef ARY_VERSION
        size  = g_add_table[i];
        start = g_add_table[i + 1];

        sprintf(szhitinfo, "%s  start: %d, size: %d\r\n", &g_chBuf[start], start, size);
#else
        size  = *(g_add_table + i);
        start = *(g_add_table + i + 1);

        sprintf(szhitinfo, "%s  start: %d, size: %d\r\n", (g_chBuf + start), start, size);
#endif  

        
        showinfo(szhitinfo);
    }
}



void
showbuf(int index, int len)
{
    int i;
    char szhitinfo[8] = {0};

    if (index < 0)
    {
        index = 0;
    }

    for (i = index; i < index + len; i++)
    {
#ifdef ARY_VERSION
        if ('\0' == g_chBuf[i])
#else
        if ('\0' == *(g_chBuf + i))
#endif
        {
            showinfo("\\0 ");
        }
        else
        {
#ifdef  ARY_VERSION
        sprintf(szhitinfo, "%c ", g_chBuf[i]); 
#else
        sprintf(szhitinfo, "%c ", *(g_chBuf + i));
#endif
            
            showinfo(szhitinfo);
        }
    }
}

void
space_stat(void)
{
    int index_buf;
    int col;  
    int max;
    int table;
    int start;
    int len;
    int remain;
    int i;
    char szhitinfo[8] = {0};

    char flag;

    index_buf = 0;
    col       = 0;

    table     = ADD;
    max       = -1;

    for ( ; index_buf < g_indexMAX_buf; index_buf++)
    {
        //this index belong to which one? g_add_table, g_del_table ?
        if (index_buf >= max )
        {
            table = gettable(index_buf, &max);            
        }
        else
        {
            //that is to keep the table result last time unchanged.
            //so no need to gettable again.
        }
        

        if (ADD == table)       //used
        {
            flag = 'U';
        }
        else if (DEL == table)  //free
        {
            flag = 'F';
        }
        else
        {
            //sth wrong?
            //printf("Sth wrong?");
            getchar();
        }

        if (0 == col % (SPLIT))
        {
            sprintf(szhitinfo, "%d: ", col);
            showinfo(szhitinfo);
        }
       
        sprintf(szhitinfo, "%c ", flag);
        showinfo(szhitinfo);
        

        if ((col != 0)
            &&(0 == (col + 1) % (SPLIT)) )
        {
            showinfo("    ");
            //
            start = col + 1 - (SPLIT);
            len   = SPLIT;
            showbuf(start, len);

            showinfo("\r\n\r\n");

        }

        col += 1;
    } 
    
    //
    remain = (SPLIT) - col % (SPLIT);
    for (i = 0; i < remain; i++)
    {
        showinfo("  ");
    }
    showinfo("  ");
    
    len   = (SPLIT) - remain;
    start = col - len;
    
    showbuf(start, len);
    
}

/*
Function: used for debug, show detail info about g_add_table, g_del_table
*/
void
showtable(void)
{
    int i;
    char szhitinfo[64] = {0};
    
    showinfo("\r\n[ Debug ]showing g_add_table, g_del_table  (size, start) ......\r\n");
    
    showinfo("g_add_table:\r\n");
    for (i = 0; i < g_indexMAX_add; i += 2)
    {
#ifdef ARY_VERSION
        sprintf(szhitinfo, "(%d %d) ", g_add_table[i], g_add_table[i+1]);
#else
        sprintf(szhitinfo, "(%d %d) ", *(g_add_table + i), *(g_add_table + i + 1));
#endif
        showinfo(szhitinfo);
    }
    showinfo("\r\n\r\n");
    
    showinfo("g_del_table:\r\n");
    for (i = 0; i < g_indexMAX_del; i += 2)
    {
#ifdef ARY_VERSION
        sprintf(szhitinfo, "(%d %d) ", g_del_table[i], g_del_table[i+1]);
#else
        sprintf(szhitinfo, "(%d %d) ", *(g_del_table + i), *(g_del_table + i + 1));
#endif
        showinfo(szhitinfo);
    }
    showinfo("\r\n\r\n"); 
}

void
defrag(void)
{
    int size_del, start_del;
    int size_add, start_add;

    int i, j, k;

    i = g_indexMAX_del; 
    j = g_indexMAX_add;

    //walk through del_table
    for (; i >= 2 ; i -= 2)
    {
#ifdef ARY_VERSION
        size_del  = g_del_table[i - 2];
        start_del = g_del_table[i - 1];
#else
        size_del  = *(g_del_table + i - 2);
        start_del = *(g_del_table + i - 1);
#endif
        

        //find the proper index (size, start) in add_table
        for ( ; j >= 2; j -= 2)
        {
#ifdef ARY_VERSION
            size_add  = g_add_table[j - 2];
            start_add = g_add_table[j - 1];
#else
            size_add  = *(g_add_table + j - 2);
            start_add = *(g_add_table + j - 1);
#endif
            
            if (start_add == start_del + size_del)
            {
                //we've already gotten the index
                break;
            }
        }

        //remove (size_del, start_del) from del_table, update g_indexMAX_del
#ifdef ARY_VERSION
        g_del_table[i - 2] = 0; //size
        g_del_table[i - 1] = 0; //start
#else
        *(g_del_table + i - 2) = 0;
        *(g_del_table + i - 1) = 0;
#endif

        g_indexMAX_del    -= 2;


        //update (size, start) in add_table to (size, start - size_del)
        for (k = g_indexMAX_add; (j >= 2) && (k >= j); k -= 2)
        {
#ifdef ARY_VERSION
            g_add_table[k - 1] = g_add_table[k - 1] - size_del;            
#else
            *(g_add_table + k - 1) = *(g_add_table + k - 1) - size_del;
#endif
        }

        //move char in g_chBuf[start_add,.. ,g_indexMAX_buf] forward
        // step is size_del
        for (k = start_add; (j >= 2) && (k < g_indexMAX_buf); k++)
        {
#ifdef ARY_VERSION
            g_chBuf[k - size_del] = g_chBuf[k];
#else
            *(g_chBuf + k - size_del) = *(g_chBuf + k);
#endif
        }

        //update g_indexMAX_buf
        g_indexMAX_buf -= size_del;                
    }
}

void
char_stat(void)
{
    double percent = 0.0;
    char ch;
    int  num;
    int i;
    char szhitinfo[64] = {0};

    showinfo("Show char info......\r\n");

    sprintf(szhitinfo, "Total: %d\r\n", g_totalchar);
    showinfo(szhitinfo);

    for (i = 0; i < (MAX_CHAR - MIN_CHAR + 1) ; i++ )
    {
#ifdef  ARY_VERSION
        if (0 != g_char_table[i])
#else
        if( 0 != *(g_char_table + i))
#endif        
        {
            //0x21 is the first printable(exclude space) within ascii
            ch  = i + 0x21;
#ifdef  ARY_VERSION
            num = g_char_table[i];
#else
            num = *(g_char_table + i);
#endif
            percent = num * 1.0 / g_totalchar;

            sprintf(szhitinfo, "%c %d %f\r\n", ch, num, percent);
            showinfo(szhitinfo);
        }
    }
}

static void
updatechar(char *szText, int action)
{
    int index;
    int contribute;
    size_t i;
    size_t length;

    length = strlen(szText);

    if (ADD == action)
    {
        contribute = 1;
    }
    else if (DEL == action)
    {
        contribute = -1;
    }
    else
    {
        //sth wrong??
    }

    for (i = 0; i < length; i++ )
    {
#ifdef ARY_VERSION
        index = szText[i] - MIN_CHAR;

        g_char_table[index] += contribute;
#else
        index = *(szText + i) - MIN_CHAR;

        *(g_char_table + index) += contribute;
#endif

        g_totalchar += contribute;

    }
}

void
process_add(void)
{
    char szhitinfo[MAX_INPUT] = {0};    //输出的提示性信息
    char chInput[MAX_INPUT];
    int result;

    //Add
    getinput(chInput, "Add what ? ");
    
    result = add(chInput);
    if (NO_SPACE == result)
    {
        showinfo("Add failed, for No More Space\r\n");
    }
    else
    {
        sprintf(szhitinfo, "ADD: store into %d\r\n", result);
        showinfo(szhitinfo);
        
        //
        updatechar(chInput, ADD);
    }
}

void
process_query(void)
{
    char szhitinfo[MAX_INPUT] = {0};    //输出的提示性信息
    char chInput[MAX_INPUT];
    char szqueryed[MAX_INPUT];          //模糊查询匹配的字符串
    int indexary[MAX_INPUT];            //用于接收模糊查询的结果
    int num;                            //模糊查收所得结果的数目
    int index;    
    int result;
    int i;

    //query
    getinput(chInput, "Query what ? ");
    
#ifdef ACCURATE_QUERY
    result = query(chInput, &index);
#else
    result = query_about(chInput, indexary, &num);
#endif
    
    
    if (NOT_EXIST == result)
    {
        showinfo("Query: not exist\r\n");
    }
    else
    {
#ifdef  ACCURATE_QUERY 
#ifdef ARY_VERSION
        sprintf(szhitinfo, "Query: at %d\r\n", g_add_table[index + 1]);
#else
        sprintf(szhitinfo, "Query: at %d\r\n", *(g_add_table + index + 1));
#endif
        showinfo(szhitinfo);
#else
        //模糊查询结果输出
        for (i = 0; i < num; i++)
        {
#ifdef ARY_VERSION
            //查询到的字符串的(size, start)记录在g_add_table中的下标
            index = indexary[i];  
            strcpy(szqueryed, &g_chBuf[g_add_table[index + 1]]);
            sprintf(szhitinfo, "Query: %s at %d\r\n", 
                szqueryed,                   //查询到的字符串
                g_add_table[index + 1]       //查询到的字符串在g_chBuf中的下标
                );    
#else
            index = *(indexary + i);
            strcpy(szqueryed, g_chBuf + (*(g_add_table + index + 1)));
            sprintf(szhitinfo, "Query: %s at %d\r\n", 
                szqueryed,                   
                *(g_add_table + index + 1)
                );   
            
#endif
            
            showinfo(szhitinfo);
        }
#endif
    }
}

void
process_del(void)
{
    char szhitinfo[MAX_INPUT] = {0};    //输出的提示性信息
    char chInput[MAX_INPUT];
    int result;

    getinput(chInput, "Del what ? ");
    
    result = del(chInput);
    
    if (-1 == result)
    {
        sprintf(szhitinfo, "%s does not exist\r\n", chInput);
        showinfo(szhitinfo);
    }
    else
    {
        showinfo("Del ok\r\n");
        
        //
        updatechar(chInput, DEL);
    }
}

void
process_modify(void)
{
    char szhitinfo[MAX_INPUT] = {0};    //输出的提示性信息
    char chInput[MAX_INPUT];
    char newinput[MAX_INPUT];
    int result;
    
    //modify
    getinput(chInput, "Modify, origin ? ");
    
    getinput(newinput, "newinput ? ");
    
    result = modify(chInput, newinput);
    if (NOT_EXIST == result)
    {
        sprintf(szhitinfo, "Modify: %s not exist\r\n", chInput);
        showinfo(szhitinfo);
    }
    else if (NO_SPACE == result)
    {
        showinfo("Modify: no more space to store newinput\r\n");
    }
    else
    {
        showinfo("Modify ok\r\n");
        
        //
        updatechar(chInput, DEL);
        updatechar(newinput, ADD);
    }
}

void
process_fit(void)
{
    int select;

    //set fit
    showinfo("Please choose the fit way\r\n");
    
    showfit();
    
    getselect(&select);
    
    if (select < FIT_EQUAL || select > FIT_EQUAL_FIRST )
    {
        showinfo("Please make the right decision ^_^\r\n");
    }
    else
    {
        setfit(select);
    }
}

void
dispatch(int select)
{
    switch (select)
    {
    default:
        {
            showinfo("Please make the right decision ^_^\r\n");
            break;
        }

    case 1:
        {
            process_add();
            break;
        }
  
    case 2:
        {
            process_query();
            break;
        }

    case 3:
        {
            process_del();
            break;
        }

    case 4:
        {
            process_modify();
            break;
        }

    case 5:
        {
            //display:
            showinfo("Displaying......\r\n");
            
            display();            
            break;
        }
        
    case 6:
        {
            //statistics
            showinfo("Statisticsing......U for Used, F for Free\r\n\r\n");
            
            space_stat();
            break;
        }

    case 7:
        {
            process_fit();
            break;
        }

    case 8:
        {
            defrag();
            break;
        }

    case 9:
        {
            char_stat();
            break;
        }
    }//end for switch
}

int
main(void)
{
    int select;

    while (1)
    {
        showmenu();

        getselect(&select);

        if (0 == select)
        {
            break;
        }

        dispatch(select);

        //used for debug, can be removed
        showtable();
    }

    return 0;
}