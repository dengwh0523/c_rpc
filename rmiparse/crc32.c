#include <stdio.h>
#include "crc32.h"

static unsigned int   CRC32[256];
static char   init = 0;

//初始化表
static void init_table()
{
    int   i,j;
    unsigned int   crc;
    for(i = 0;i < 256;i++)
    {
        crc = i;
        for(j = 0;j < 8;j++)
        {
            if(crc & 1)
            {
                 crc = (crc >> 1) ^ 0xEDB88320;
            }
            else
            {
                 crc = crc >> 1;
            }
        }
        CRC32[i] = crc;
    }
}

//crc32实现函数
unsigned int crc32( unsigned char *buf, int len)
{
    unsigned int ret = 0xFFFFFFFF;
    int   i;
    if( !init )
    {
         init_table();
         init = 1;
    }
    for(i = 0; i < len;i++)
    {
         ret = CRC32[((ret & 0xFF) ^ buf[i])] ^ (ret >> 8);
    }
    ret = ~ret;
    return ret;
}

int crc32_init(unsigned int * pdata) {
	init_table();

	init = 1;
	*pdata = 0xFFFFFFFF;
	return 0;
}

int crc32_update(unsigned char *buf, int len, unsigned int * pdata) {
    unsigned int ret = 0xFFFFFFFF;
    int   i;
    if( !init )
    {
         init_table();
         init = 1;
    }
	ret = *pdata;
    for(i = 0; i < len;i++)
    {
         ret = CRC32[((ret & 0xFF) ^ buf[i])] ^ (ret >> 8);
    }

	*pdata = ret;

	return 0;
}

int crc32_final(unsigned int * pdata) {
	*pdata = ~*pdata;

	return 0;
}
