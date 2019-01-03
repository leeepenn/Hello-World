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

/*
	judge the reply from NB module
	judge by keyword "ERROR" of "ok"
*/
int checkConnect(char *recBuff)
{
	char find[] = "ERROR";
	if (strstr(recBuff, find) != NULL || strlen(recBuff) == 0)
	{
		return 1;
	}

	else
	{
		return 0;
	}
}

/* 
	function to set value,judged by the label, such as "QN" "a05002-Rtd",etc.
 * @param char * dataSeg The original string
 * @param char * label The data's label to search for
 * @param char * value The replace string value
 */
void setValue(char *dataSeg, char *label, char *value)
{
	//the lenth of real data segment,do not count label and symbols "=", ";" etc.
	unsigned int len = 0;
	//offset equals the lenth of label string appending symbol "="
	unsigned int offset = 0;
	//a buffer variable
	char buffer[256] = "";
	//store the pointer returned from strstr
	char *ch = "";

	//get the position of the string to replace

	if (!(ch = strstr(dataSeg, label)))
	{
		return;
	}

	if (strcmp(label, "QN") == 0)
	{
		offset = 3;
		len = 17;
	}
	else if (strcmp(label, "a05002-Rtd") == 0)
	{
		offset = 11;
		len = 4;
	}
	else if (strcmp(label, "DataTime") == 0)
	{
		offset = 9;
		len = 14;
	}
	else if (strcmp(label, "a05002-SampleTime") == 0)
	{
		offset = 18;
		len = 14;
	}
	ch += offset;
	//copy all the content to buffer before the
	strncpy(buffer, dataSeg, ch - dataSeg);
	//prepare the buffer for appending by adding a null to the end
	buffer[ch - dataSeg] = 0;
	//append the rep string to the end of buffer useing sprinf fuction
	sprintf(buffer + (ch - dataSeg), "%s%s", value, ch + len);
	//empty the original string to save the new data
	dataSeg[0] = 0;
	strcpy(dataSeg, buffer);
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
char dataSegment[] = "QN=20181214102157223;ST=22;CN=2011;PW=123456;MN=010000A8900016F000169DC0;Flag=5;CP=&&DataTime=20180531102157;a05002-Rtd=7.11,a05002-Flag=N;a05002-SampleTime=20180531104100&&"; //

// typedef struct 
// {
// 	 char *qn;
// 	 char *gasValue;
// 	 char *dataTime;
// 	 char *sampleTime;
// }dataLabel;

int main()
{
	char *label_qn = "QN";
	char *label_gasValue = "a05002-Rtd";
	char *label_dataTime= "DataTime";
	char *label_sampleTime = "a05002-SampleTime";
	char newQnCode[] = "20191114102157223";
	char newGasValue[] = "9.99";
	char newDataTime[] = "20110531102154";
	char newSampleTime[] = "20140831104109";
	printf("the old dataSegment is: %s\n", dataSegment);
	setValue(dataSegment, label_qn, newQnCode);
	setValue(dataSegment, label_gasValue, newGasValue);
	setValue(dataSegment, label_dataTime, newDataTime);
	setValue(dataSegment, label_sampleTime, newSampleTime);
	printf("the new dataSegment is: %s\n", dataSegment);
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
	// printf("AT+NSOSD=0,%d,%s%s\r\n", packageSize, dataToSend, "0D0A");
	return 0;
}
