//
// Created by zhangshibo on 17-2-23.
//

#ifndef LRUCACHE_LRUCACHEIMPL_H
#define LRUCACHE_LRUCACHEIMPL_H

/*
 * 定义缓存块
 */
typedef struct CacheEntryS {
    char key;
    char data;

    struct CacheEntryS* hashListPrev;
    struct CacheEntryS* hashListNext;

    struct CacheEntryS* lruListPrev;
    struct CacheEntryS* lruListNext;
} CacheEntryS;

/*
 * LRU
 */
typedef struct LRUCacheS {
    int cacheCapacity;
    CacheEntryS** hashMap;

    CacheEntryS* lruListHead;
    CacheEntryS* lruListTail;
    int lruListSize;
} LRUCacheS;

#endif //LRUCACHE_LRUCACHEIMPL_H
