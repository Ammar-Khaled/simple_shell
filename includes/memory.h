#ifndef __MEMORY_H__
#define __MEMORY_H__


int min(int a, int b);
void *_memset(void *ptr, char byte, unsigned int nbytes);
void *_memcpy(void *dest, void *src, unsigned int nbytes);
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);

#endif
