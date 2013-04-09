#include <FreeRTOS_AVR.h>
#include "Queue2.h"

char	read_buff[BUFF_SIZE];
char    write_buff[BUFF_SIZE];
int		riHead, riTail;
int             wiHead, wiTail;

void rdbClear()
{
	riHead = riTail = 0;
	memset(read_buff, 0, BUFF_SIZE);
}

void wrbClear()
{
	wiHead = wiTail = 0;
	memset(write_buff, 0, BUFF_SIZE);
}


int rdbGetSize()
{
	return (riHead - riTail + BUFF_SIZE) % BUFF_SIZE;
}

int wrbGetSize()
{
	return (wiHead - wiTail + BUFF_SIZE) % BUFF_SIZE;
}

BOOL rdbPutbyte(char b)
{
//  taskENTER_CRITICAL();
	if (rdbGetSize() == (BUFF_SIZE-1)) return FALSE;
	read_buff[riHead++] = b;
	riHead %= BUFF_SIZE;
//  taskEXIT_CRITICAL();
	return TRUE;
}

BOOL wrbPutbyte(char b)
{
//  taskENTER_CRITICAL();
	if (wrbGetSize() == (BUFF_SIZE-1)) return FALSE;
	write_buff[wiHead++] = b;
	wiHead %= BUFF_SIZE;
//  taskEXIT_CRITICAL();
	return TRUE;
}


BOOL rdbGetbyte(char* pb)
{
 //  taskENTER_CRITICAL();
	if (rdbGetSize() == 0) return FALSE;
	*pb = read_buff[riTail++];
	riTail %= BUFF_SIZE;
//   taskEXIT_CRITICAL();
	return TRUE;
}

BOOL wrbGetbyte(char* pb)
{
 //  taskENTER_CRITICAL();
	if (wrbGetSize() == 0) return FALSE;
	*pb = write_buff[wiTail++];
	wiTail %= BUFF_SIZE;
//   taskEXIT_CRITICAL();
	return TRUE;
}

