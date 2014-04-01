#ifndef  TABLE_H_6AEDF8D6_CF88_44ce_9E06_CCF5E6129889

#define  TABLE_H_6AEDF8D6_CF88_44ce_9E06_CCF5E6129889


/*
Function: ��(size, start)��¼����Ĳ��뵽table[*max]��
����  
Param	: table[max]
����        size, start
����        
Return  :  None.

Effect   : ����*max;
*/
void
insertintoaddtable(int *table, int *max, int size, int start);

/*
Function: ��(size, start)��¼����Ĳ��뵽table[*max]��, ����g_del_tableָʾ�������пռ��������
����  
Param	:  table[*max]
����        size, start
����        
Return  :  None.

Effect   : ����*max, ��g_del_tableָʾ���������пռ�������ϣ������ٷ���
*/
void
insertintodeltable(int *table, int *max, int size, int start);



/*
Function: ��index ����ʶ��(size, start)��table[*max]��ɾ��, ����֤table�������ԡ�
����  
Param	: table[*max]
����        
Return  :  None.

  Effect  : ����*max;
*/
void
removefromtable(int *table, int *max, int index);

/*
Function: ��table[index]����ʶ��(size, start)����Ϊ�´����ֵ��
����  
Param	: 
����        
Return  :  None.

Effect  :  ����table
*/
void
updatetable(int *table, int index, int size, int start);


/*
Function: get the table that index belongs to, and set the max value of this range.
*/
int
gettable(int index, int *max);


#endif