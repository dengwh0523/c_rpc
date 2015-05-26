#ifndef _CRC32_H
#define _CRC32_H

unsigned int crc32( unsigned char *buf, int len);

int crc32_init(unsigned int * pdata);
int crc32_update(unsigned char *buf, int len, unsigned int * pdata);
int crc32_final(unsigned int * pdata);

#endif
