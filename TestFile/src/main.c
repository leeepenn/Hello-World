#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define lenthRec 128
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
int NB_CONNECTED_FLAG = 0;
char dataTemp[200] = "##";
int packageSize = 0;
int crc_reg = 0;
char recBuff0[lenthRec] = "0,ok";
char recBuff1[lenthRec] = "0,ok";
char recBuff2[lenthRec] = "0,ok";
char recBuff3[lenthRec] = "0,ok";
char recBuff4[lenthRec] = "0,ERRO";
unsigned char dataSegment[] = "QN=20181214102157223;ST=22;CN=2011;PW=123456;MN=010000A8900016F000169DC0;Flag=5;CP=&&DataTime=20180531102157;a05002-Rtd=7.11,a05002-Flag=N;a05002-SampleTime=20180531104100&&"; //

int main()
{
	unsigned char dataToSend[400] = {0};
	unsigned int len = strlen(dataSegment);		//temporary save the lenth of dataSegment,later convert to char array
	crc_reg = CRC16_Checkout(dataSegment, len); //save the result of crcCheck,later convert to char array
	char crc_temp[5] = {0};
	char lenth_seg[5];
	sprintf(crc_temp, "%x", crc_reg);
	sprintf(lenth_seg, "%04d", len);
	char temp1[200] = {0};
	strcpy(temp1, dataSegment);
	strcat(temp1, crc_temp); //
	strcat(dataTemp, lenth_seg);
	strcat(dataTemp, temp1);
	//  include crc:	b681->62363831
	packageSize = strlen(dataTemp) + 2; //add 2 byte because of "0d0a"
	ASCIIToHexStr(dataToSend, dataTemp, strlen(dataTemp));
	// while (!NB_CONNECTED_FLAG)
	// {
	// 	printf("AT+NRB\r\n"); //restart nb module
	// 	delay_ms(10000);
	// 	printf("AT+BSSERVER=0\r\n");
	// 	delay_ms(5000);
	// 	if (checkReply(recBuff0))
	// 	{
	// 		continue;
	// 	}

	// 	printf("AT+NSOCR=STREAM,6,10000,1,AF_INET\r\n"); //create socket
	// 	delay_ms(5000);
	// 	// }
	// 	if (checkReply(recBuff2))
	// 	{
	// 		continue;
	// 	}

	// 	printf("AT+NSOCO=0,120.27.105.150,10000\r\n"); //create tcp
	// 	delay_ms(5000);
	// 	if (checkReply(recBuff4))
	// 	{
	// 		continue;
	// 	}
	// 	printf("AT+NSORF=0,128\r\n"); //read socket
	// 	delay_ms(5000);
	// 	NB_CONNECTED_FLAG = 1;
	// }
	printf("AT+NSOSD=0,%d,%s%s\r\n", packageSize, dataToSend, "0D0A");
	return 0;
}
void setTime(char *dataSeg, char *time)
{
}
void setGasValue(char *dataSeg, char *value)
{
}