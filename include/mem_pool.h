#ifndef _MEM_PALLOC_H_INCLUDED_
#define _MEM_PALLOC_H_INCLUDED_

/*
 * 包含一些头文件
 */
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

/*
 * 预定义一些常用类型
 */
typedef unsigned char u_char;
typedef intptr_t mem_int_t;
typedef uintptr_t mem_uint_t;

#define MEM_ALIGNMENT sizeof(unsigned long)

/*
 * 预定义一些函数
 */
#define mem_memalign(alignment, size)   malloc(size)
#define mem_free                        free

#define mem_align(d, a)     (((d) + (a - 1)) & ~(a - 1))
#define mem_align_ptr(p, a)                                                   \
    (u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))


/*
 * MEM_MAX_ALLOC_FROM_POOL should be (mem_pagesize - 1), i.e. 4095 on x86.
 * On Windows NT it decreases a number of locked pages in a kernel.
 */
#define MEM_MAX_ALLOC_FROM_POOL  (mem_pagesize - 1)

#define MEM_DEFAULT_POOL_SIZE    (16 * 1024)

#define MEM_POOL_ALIGNMENT       16
#define MEM_MIN_POOL_SIZE                                                     \
    mem_align((sizeof(mem_pool) + 2 * sizeof(mem_pool_large_t)),            \
              MEM_POOL_ALIGNMENT)

typedef void (*mem_pool_cleanup_pt)(void *data);

typedef struct mem_pool_cleanup_s  mem_pool_cleanup_t;

struct mem_pool_cleanup_s {
    mem_pool_cleanup_pt   handler;
    void                 *data;
    mem_pool_cleanup_t   *next;
};

typedef struct mem_pool_large_s  mem_pool_large_t;

struct mem_pool_large_s {
    mem_pool_large_t     *next;
    void                 *alloc;
};

typedef struct mem_pool_s  mem_pool;

typedef struct {
    u_char               *last;
    u_char               *end;
    mem_pool             *next;
    mem_uint_t            failed;
} mem_pool_data_t;


struct mem_pool_s {
    mem_pool_data_t       d;
    size_t                max;
    mem_pool           *current;
    mem_pool_large_t     *large;
    mem_pool_cleanup_t   *cleanup;
};

void *mem_calloc(size_t size);

mem_pool *mem_create_pool(size_t size);
void mem_destroy_pool(mem_pool *pool);
void mem_reset_pool(mem_pool *pool);

void *mem_palloc(mem_pool *pool, size_t size);
void *mem_pnalloc(mem_pool *pool, size_t size);


#endif /* _MEM_PALLOC_H_INCLUDED_ */
