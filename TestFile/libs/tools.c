#include <time.h>
#include <string.h>
#include "tools.h"
void delay_ms(int ms) 
{ 
	// Converting time into milli_seconds 
	int milli_seconds = ms; 

	// Stroing start time 
	clock_t start_time = clock(); 

	// looping till required time is not acheived 
	while (clock() < start_time + milli_seconds) 
		; 
} 

void ASCIIToHexStr(unsigned char *pbDest, unsigned char *pbSrc, int nLen)
{
	char ddl, ddh;
	int i;
	for (i = 0; i < nLen; i++)
	{
		ddh = 48 + pbSrc[i] / 16;
		ddl = 48 + pbSrc[i] % 16;
		if (ddh > 57)
		{
			ddh = ddh + 7;
		}
		if (ddl > 57)
		{
			ddl = ddl + 7;
		}
		pbDest[i * 2] = ddh;
		pbDest[i * 2 + 1] = ddl;
	}
}

unsigned int CRC16_Checkout(unsigned char *puchMsg, unsigned int usDataLen)
{
	unsigned int i, j, crc_reg, check;
	crc_reg = 0xFFFF;
	for (i = 0; i < usDataLen; i++)
	{
		crc_reg = (crc_reg >> 8) ^ puchMsg[i];
		for (j = 0; j < 8; j++)
		{
			check = crc_reg & 0x0001;
			crc_reg >>= 1;
			if (check == 0x0001)
			{
				crc_reg ^= 0xA001;
			}
		}
	}
	return crc_reg;
}

int includeError(char *theArray)
{
	return 0;
}
/*
	judge the reply from NB module
	judge by keyword "ERROR" of "ok"
*/
int checkConnect(char *recBuff)
{
	if (includeError(recBuff) || strlen(recBuff) == 0)
	{
		return 0;
	}

	else
	{
		return 1;
	}
}