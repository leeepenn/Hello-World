#include <stdio.h>
#include <string.h>

int checkReply(char *recBuff)
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
// C prototype : void ASCIIToHexStr(unsigned char *pbDest, unsigned cha *pbSrc, int nLen)
// parameter(s): [OUT] pbDest - Êä³ö»º³åÇø
// [IN] pbSrc - ×Ö·û´®ÊäÈë
// [IN] nLen -ý××Ö·û×Ö½ÚÊý
// return value: 
// remarks : ASCII×Ö·û×ªÎª16½øÖÆ×Ö·û
*/
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
int main(int argc, char const *argv[])
{
    int a = 0x30;
    char b = '0';
    char ascii[] = "\r\n,0,ERROR\r\n"; //ERROR->4552524F52
    char hexStr[] = "";
    short RecBuff[] = {0x45, 0x52, 0x52, 0x4F, 0x52};
    // char RecBuff[] = {0x45, 0x52, 0x52, 0x4F, 0x52};
    ASCIIToHexStr(hexStr, ascii, strlen(ascii));
    // printf("%s", hexStr);
    if (checkReply(RecBuff))
    {
        printf("got!");
    }

    else
    {
        printf("lose!");
    }

    return 0;
}
