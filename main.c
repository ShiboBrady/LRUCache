#include <stdio.h>
#include <stdlib.h>
#include "LRUCache.h"

/*
 * 错误处理宏
 */
#define HANDLE_ERROR(msg) \
    do { \
        fprintf(stderr, "%s fail.\n", msg); \
        exit(-1); \
    } while(0)

/*封装缓存数据储存接口，此处我们让data同时充当key的角色*/
#define LRUCACHE_PUTDATA(cache, data) \
do {\
    if (0 != LRUCacheSet(cache, data, data)) \
        fprintf(stderr, "put (%c,%c) to cache fail.\n", data, data);\
    else \
        fprintf(stdout, "put (%c,%c) to cache success.\n", data, data); \
}while(0)

/*封装缓存数据储存接口*/
#define LRUCACHE_GETDATA(cache, key) \
do{\
    char data = LRUCacheGet(cache, key);\
    if ('\0' == data) \
        fprintf(stderr, "get data (Key:%c) from cache fail.\n", key);\
    else if (key == data) \
        fprintf(stdout, "got (%c,%c) from cache\n", key, data);\
}while(0)

void testcase1(void) {
    fprintf(stdout, "=========================\n");
    fprintf(stdout, "In testcase1....\n");
    fprintf(stdout, "=========================\n");
    void *lruCache;
    if (0 != LRUCacheCreate(5, &lruCache))
        HANDLE_ERROR("LRUCacheCreate");
    /*ABC!*/
    LRUCACHE_PUTDATA(lruCache, 'A');
    LRUCACHE_GETDATA(lruCache, 'A');
    LRUCACHE_PUTDATA(lruCache, 'B');
    LRUCACHE_GETDATA(lruCache, 'B');
    LRUCACHE_PUTDATA(lruCache, 'C');
    LRUCACHE_GETDATA(lruCache, 'C');
    LRUCachePrint(lruCache);/*CBA*/

    /*DEAF!*/
    LRUCACHE_PUTDATA(lruCache, 'D');
    LRUCACHE_GETDATA(lruCache, 'D');
    LRUCACHE_PUTDATA(lruCache, 'E');
    LRUCACHE_GETDATA(lruCache, 'E');
    LRUCACHE_PUTDATA(lruCache, 'A');
    LRUCACHE_GETDATA(lruCache, 'A');
    LRUCACHE_PUTDATA(lruCache, 'F');
    LRUCACHE_GETDATA(lruCache, 'F');
    LRUCachePrint(lruCache); /*FAEDC*/

    /*B!*/
    LRUCACHE_PUTDATA(lruCache, 'F');
    LRUCACHE_GETDATA(lruCache, 'F');
    LRUCachePrint(lruCache); /*BFAED*/

    if (0 != LRUCacheDestory(lruCache))
        HANDLE_ERROR("LRUCacheDestory");
    fprintf(stdout, "\n\ntestcase1 finished\n");
    fprintf(stdout, "=========================\n\n");
}

int main() {
    testcase1();
    return 0;
}