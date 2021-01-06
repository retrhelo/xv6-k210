#ifndef __STRING_H
#define __STRING_H 

#include "types.h"

int memcmp(const void*, const void*, uint);

void *memmove(void*, const void*, uint);

/* 
 * fill memory with c in length of len 
 */
void *memset(void *buf, int c, uint len);

char *safestrcpy(char *dst, const char *src, int len);

int strlen(const char *s);

int strncmp(const char *dst, const char *src, uint len);

char *strncpy(char *dst, const char *src, int len);

/* 
 * Convert uchar string into wchar string 
 */
void wnstr(wchar *dst, uchar const *src, int len);

/* 
 * Convert wchar string into uchar string 
 */
void snstr(uchar *dst, wchar const *src, int len);

/* 
 * Compare two wchar strings 
 */
int wcsncmp(wchar const *s1, wchar const *s2, int len);

#endif 
