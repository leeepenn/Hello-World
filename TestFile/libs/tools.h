#ifndef __TOOLS_H
#define __TOOLS_H

void delay_ms(int ms);
void ASCIIToHexStr(unsigned char *pbDest, unsigned char *pbSrc, int nLen);
unsigned int CRC16_Checkout(unsigned char *puchMsg, unsigned int usDataLen);
int includeError(char *theArray);
int checkConnect(char *recBuff);
#endif
