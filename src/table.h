#ifndef  TABLE_H_6AEDF8D6_CF88_44ce_9E06_CCF5E6129889

#define  TABLE_H_6AEDF8D6_CF88_44ce_9E06_CCF5E6129889


/*
Function: 将(size, start)记录有序的插入到table[*max]中
　　  
Param	: table[max]
　　        size, start
　　        
Return  :  None.

Effect   : 更新*max;
*/
void
insertintoaddtable(int *table, int *max, int size, int start);

/*
Function: 将(size, start)记录有序的插入到table[*max]中, 并对g_del_table指示连续空闲空间进行整合
　　  
Param	:  table[*max]
　　        size, start
　　        
Return  :  None.

Effect   : 更新*max, 对g_del_table指示的连续空闲空间进行整合，进行再分配
*/
void
insertintodeltable(int *table, int *max, int size, int start);



/*
Function: 将index 所标识的(size, start)从table[*max]中删除, 并保证table的有序性。
　　  
Param	: table[*max]
　　        
Return  :  None.

  Effect  : 更新*max;
*/
void
removefromtable(int *table, int *max, int index);

/*
Function: 将table[index]所标识的(size, start)更新为新传入的值。
　　  
Param	: 
　　        
Return  :  None.

Effect  :  更新table
*/
void
updatetable(int *table, int index, int size, int start);


/*
Function: get the table that index belongs to, and set the max value of this range.
*/
int
gettable(int index, int *max);


#endif