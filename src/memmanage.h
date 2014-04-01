#ifndef MEMMANAGE_H_CF771D01_FB96_4a42_BA47_C7B9C76D8B18    //��ֹͷ�ļ����ظ�����

#define MEMMANAGE_H_CF771D01_FB96_4a42_BA47_C7B9C76D8B18


//
#define 	MAX_SPACE	            10000
#define 	MAX_INPUT		        256

//which table: g_add_table, or g_del_table
#define		ADD		                1
#define		DEL			            -1

//error process
#define     NOT_EXIST               -1
#define     NO_SPACE                -2
#define     SUCCESS                 0                


//for memory space statistics
#define     SPLIT                   8

//for char statistics(printable but not space)
#define     MAX_CHAR                0x7E
#define     MIN_CHAR                0x21

//used to choose between ARRY and POINTER VERSION
#define     ARY_VERSION


/*���ᳫʹ��ȫ�ֱ����� ������Ҫ��ͷ�ļ���ʹ��extern int value ��������*/

/*used for maintain*/
extern int g_add_table[MAX_SPACE];
extern int g_indexMAX_add;         //how many (size, start) are stored into table;

//del_table
extern int g_del_table[MAX_SPACE];
extern int g_indexMAX_del;         //how many (size, start) are stored into table;

/*
Function: ��ȷ��ѯ������ַ����Ƿ������g_chBuf�С� 
����
Param	: chInput ��ʱ����û�������ַ��� 
           index ���ڽ��ո��ַ�����Ӧ��(size, start)��g_add_table�е��±�
           (���Ψһ)
           
Return  :  NOT_EXIST�����ڣ� SUCCESS���ڣ�������index.
*/
int 
query(char *chInput, int *index);

/*
Function: ģ����ѯ������ַ����Ƿ������g_chBuf��

Param   : chInput ��ʱ����û�������ַ���
          indexary���ڽ��ղ�ѯ�����ַ�����Ӧ��(size, start)��g_add_table�е��±�
          (�����ж�������Դ���ʱ��Ӧ��Ϊ������).

           num ���ڽ��ղ�ѯ�����ܸ���

Return  : NOT_EXIST �����ڣ� SUCCESS���ڣ�������indexary.
          
*/
int
query_about(char *chInput, int *indexary, int *num);


/*
Function: ���û�����chInput[]�洢��g_chBuf[]�С�
����  
Param	: chInput[]Ϊ�û��������ʱ�洢
����        
Return  :  NO_SPACE ���ʧ�ܣ� ������ʾ��ӵ�λ��.

Effect  : ����g_add_table, g_indexMAX_add, �����������
          g_del_table,g_indexMAX_buf


  add(string):
  ����sizereal = len(string)
  �������û�д�g_del_table�в�ѯ�����ʣ� �ں����������(size, start)��¼�������charBuf[10000]�������ѷ��������±�indexMax, ��indexMax������string, ����g_add_table��׷��(size, indexMax)��¼��
  ����
  ��������g_del_table�в�ѯ��һ�����ʵģ��ں������) size�ļ�¼(size, start)����string������charBuf[start]��ʼ��λ�á�����sizereal��size�Ĺ�ϵ�������¼��������
  ����sizereal = size, ��(size, start)��g_del_table��ɾ����ͬʱ��(size, start)����Ĳ��뵽g_add_table��
      sizereal < size, ��g_del_table�еĸ�(size, start)��Ϊ(size - sizereal, start + sizereal), ͬʱ����(sizereal, start)����Ĳ��뵽g_add_table�С�

*/
int
add(char *chInput);

/*
Function: ��ͼ��g_chBuf[]��ɾ���û�����chInput[]��
����  
Param	: chInput[]Ϊ�û��������ʱ�洢
����        
Return  :  NOT_EXIST for failed�� SUCCESS �ɹ�.

Effect  : ����g_add_table, 
           ����g_del_table�� ��������Կ��пռ�������ϣ�Ӱ��g_del_table

del(string):
����index = query(string)
����(size, start) = (g_add_table[index], g_add_table[index+1])
����
�������index == -1 ��ѯʧ�ܣ��򷵻�-1��ʾɾ��ʧ�ܡ�
����
�������index != -1, ��(size, start)��g_add_table��ɾ����ͬʱ���Ǹ��������g_del_table. ���ǵ����ܵ��������пռ�����ϣ���Ϊ���������
�������(size, start)�к��ʵ�ǰ��(size1, start1) ���� start1 + size1 = size, ������������пռ���������Ӧ��(size1, start1)����Ϊ(size1+size, start1)
�������(size,start)�к��ʵĺ��(size2, start2)���� start + size = start2, ��(size2, start2)Ӧ����Ϊ(size+size2, start)
*/
int
del(char *chInput);

/*
Function: ��ͼ��g_chBuf[]�н�ԭ������origin����Ϊ����������new��
����  
Param	: originָ����޸�����
           newָ��������
����        
Return  :  NOT_EXIST for not exist, NO_SPACE for no space, SUCCESS for success

Effect  : ����g_add_table, 
           ����g_del_table�� ��������Կ��пռ�������ϣ�Ӱ��g_del_table

modify(origin, new):
����index = query(origin)
�������index == -1 ����������origin, �޸�ʧ�ܡ�
����oldsize = len(origin)
����newsize = len(new)
�������index != -1, ����ʾorigin��(size, start)��g_add_table�е��±꣬ȡ��start.
��������oldsize��newsize�Ĵ�С����Ϊ���������
������� newsize < oldsize, �����g_add_table�е�(size, start) Ϊ(newsize, start), ͬʱ��g_del_table������Ĳ���(oldsize - newsize, start + newsize)
������� newsize = oldsize, �򲻷������
�������newsize > oldsize, ���ʱ���Լ򵥵��õ�del(origin)��add(string), ��ɿռ���ͷź������ϣ��Լ��ٷ��䡣
����Ȼ��Ҳ���ܳ��ָ�startǰ�����ߺ�̱����Ѿ�Ϊ���е��������ô��ʱ���п��е���������õģ��������ǻ��ǽ�������һ������̽��д�����
*/
int
modify(char *origin, char *newinput);


/*
Function: ��chInput[]�洢��chBuf[start]
����  
Param	 : chBuf, start, chInput, requestsize          
����        
Return  :  NO_SPACE for failed, SUCESS for success

Effect   : ����chBuf[],������Ҫ����g_indexMAX_buf
*/
int
store(char *chBuf, int start, char *chInput, int requestsize);

/*
Function: ����g_add_table, ���g_chBuf, ������д洢���ַ������롣
����  
Param	 : None         
����        
Return  : None.

Effect  : None
*/
void
display(void);


/*
Function: ͳ��g_chBuf[] 0 ��g_indexMAX_buf ���ֽڵ�ʹ�����,
����  
Param	 : None          
����        
Return  : None
Effect   : None
*/
void
space_stat(void);

/*
Function: ������Ƭ����
*/
void
defrag(void);

/*
Function: ͳ�Ƹ��ַ����ֵĴ�����Ƶ��
*/
void
char_stat(void);

/*
Function: ���szText[]��action, ����g_char_table�и��ַ����ֵĴ���

        called whenever add, del, modify success.
*/
static void
updatechar(char *szText, int action);


#endif