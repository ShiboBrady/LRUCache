//
// Created by zhangshibo on 17-2-23.
//

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "LRUCache.h"
#include "LRUCacheImpl.h"

/********************************************************
* 缓存块创建销毁相关接口及实现
********************************************************/

/*
 * 创建缓存块
 */
static CacheEntryS* NewCacheEntry(char key, char data) {
    CacheEntryS* entry = (CacheEntryS*)malloc(sizeof(CacheEntryS));
    if (entry == NULL) {
        perror("malloc");
        return NULL;
    }
    bzero(entry, 0);
    entry->key = key;
    entry->data = data;
    return entry;
}

/*
 * 销毁缓存块
 */
static void FreeCacheEntry(CacheEntryS* entry) {
    if (NULL == entry) {
        return;
    }
    free(entry);
}

/********************************************************
* 双向链表相关接口及实现
********************************************************/

/*
 * 删除一个节点
 */
static void RemoveFromList(LRUCacheS* cache, CacheEntryS* entry) {
    if (cache->lruListSize == 0) {
        return;
    }

    if (entry == cache->lruListHead && entry == cache->lruListTail) {
        //仅剩一个节点
        cache->lruListHead = cache->lruListTail = NULL;
    } else if (entry == cache->lruListHead) {
        //位于表头
        cache->lruListHead = cache->lruListHead->lruListNext;
        cache->lruListHead->lruListPrev = NULL;
    } else if (entry == cache->lruListTail) {
        //位于表尾
        cache->lruListTail = entry->lruListPrev;
        cache->lruListTail->lruListNext = NULL;
    } else {
        //一般情况
        entry->lruListPrev->lruListNext = entry->lruListNext;
        entry->lruListNext->lruListPrev = entry->lruListPrev;
    }

    //数量减1
    cache->lruListSize--;
}

/*
 * 将节点插入到链表表头
 */
static CacheEntryS* InsertToListHead(LRUCacheS* cache, CacheEntryS* entry) {
    CacheEntryS* removedEntry = NULL;

    //如果存满了，将最后的元素删除
    if (++cache->lruListSize > cache->cacheCapacity) {
        removedEntry = cache->lruListTail;
        RemoveFromList(cache, cache->lruListTail);
    }

    //如果当前链表为空
    if (cache->lruListTail == NULL && cache->lruListTail == NULL) {
        cache->lruListHead = cache->lruListTail = entry;
    } else {
        //非空，则插入表头
        entry->lruListNext = cache->lruListHead;
        entry->lruListPrev = NULL;
        cache->lruListHead->lruListPrev = entry;
        cache->lruListHead = entry;
    }

    return removedEntry;
}

/*
 * 释放链表
 */
static void FreeList(LRUCacheS* cache) {
    if (cache->lruListSize == 0) {
        return;
    }
    CacheEntryS* entry = cache->lruListHead;
    while (entry) {
        CacheEntryS* temp = entry->lruListNext;
        FreeCacheEntry(entry);
        entry = temp;
    }
    cache->lruListSize = 0;
}

/*
 * 将节点置于链表头部
 */
static void UpdateLRUList(LRUCacheS* cache, CacheEntryS* entryS) {
    //将节点从链表中删除
    RemoveFromList(cache, entryS);

    //将节点插入链表头部
    InsertToListHead(cache, entryS);
}

/********************************************************
* 哈希表相关接口及实现
********************************************************/
/*
 * 哈希函数
 */
static int HashKey(LRUCacheS* cacheS, char key) {
    return (int)key % cacheS->cacheCapacity;
}

/*
 * 从哈希表中获取值
 */
static CacheEntryS* GetValueFromHashMap(LRUCacheS* cache, int key) {
    //定位元素的位置
    CacheEntryS* entry = cache->hashMap[HashKey(cache, key)];

    //遍历哈希链表，找到相应元素
    while (entry) {
        if (entry->key == key) {
            break;
        }
        entry = entry->hashListNext;
    }

    return entry;
}

/*
 * 向哈希表中插入元素
 */
static void InsertEntryToHashMap(LRUCacheS* cache, CacheEntryS* entry) {
    //定位元素位置
    CacheEntryS* pos = cache->hashMap[HashKey(cache, entry->key)];

    //如果当前槽内有元素，则将该元素头插到槽内的链表中
    if (pos != NULL) {
        entry->hashListNext = pos;
        pos->hashListPrev = entry;
    }

    //更新槽
    cache->hashMap[HashKey(cache, entry->key)] = entry;
}

/*
 * 从哈希表中删除元素
 */
static void RemoveEntryFromHashMap(LRUCacheS* cache, CacheEntryS* entry) {
    if (NULL == entry || NULL == cache || NULL == cache->hashMap) {
        return;
    }
    //定位元素位置
    CacheEntryS* pos = cache->hashMap[HashKey(cache, entry->key)];
    while (pos) {

        //找到要删除的元素
        if (pos->key == entry->key) {
            if (pos->hashListPrev) {
                pos->hashListPrev->hashListNext = pos->hashListNext;
            } else {
                cache->hashMap[HashKey(cache, entry->key)] = pos->hashListNext;
            }
            if (pos->hashListNext) {
                pos->hashListNext->hashListPrev = pos->hashListPrev;
            }
            return;
        }
        pos = pos->hashListNext;
    }

}

int LRUCacheCreate(int capacity, void** lruCache) {
    LRUCacheS* cache = (LRUCacheS*)malloc(sizeof(LRUCacheS));
    if (NULL == cache) {
        perror("malloc");
        return -1;
    }

    bzero(cache, sizeof(LRUCacheS));
    cache->cacheCapacity = capacity;
    cache->hashMap = (CacheEntryS**)malloc(sizeof(CacheEntryS) * capacity);
    if (NULL == cache->hashMap) {
        free(cache);
        perror("malloc");
        return -1;
    }
    bzero(cache->hashMap, sizeof(CacheEntryS) * capacity);
    *lruCache = cache;
    return 0;
}

int LRUCacheDestory(void* lruCache) {
    LRUCacheS* cache = (LRUCacheS*) lruCache;
    if (NULL == cache) {
        return 0;
    }

    //释放hashmap
    if (cache->hashMap) {
        free(cache->hashMap);
    }
    //释放链表
    FreeList(cache);
    free(cache);
    return 0;
}

int LRUCacheSet(void* lruCache, char key, char data) {
    LRUCacheS* cache = (LRUCacheS*) lruCache;
    CacheEntryS* entry = GetValueFromHashMap(cache, key);
    if (entry != NULL) {
        //缓存过的数据
        entry->data = data;
        UpdateLRUList(cache, entry);
    } else {
        //未缓存的数据
        entry = NewCacheEntry(key, data);

        CacheEntryS* removedEntry = InsertToListHead(cache, entry);
        if (NULL != removedEntry) {
            //插入过程中发生了溢出
            RemoveEntryFromHashMap(cache, removedEntry);
            FreeCacheEntry(removedEntry);
        }
        //插入哈希表
        InsertEntryToHashMap(cache, entry);
    }
    return 0;
}

char LRUCacheGet(void* lruCache, char key) {
    LRUCacheS* cache = (LRUCacheS*)lruCache;
    //从哈希表中查找
    CacheEntryS* entry = GetValueFromHashMap(cache, key);
    if (NULL != entry) {
        //找到该数据
        UpdateLRUList(cache, entry);
        return entry->data;
    } else {
        //未找到
        return '\0';
    }
}

void LRUCachePrint(void* lruCache) {
    LRUCacheS* cache = (LRUCacheS*)lruCache;
    if (NULL == cache || 0 == cache->lruListSize) {
        return;
    }
    fprintf(stdout, "\n>>>>>>>>>>>>\n");
    fprintf(stdout, "cache (key data):\n");
    CacheEntryS* entry = cache->lruListHead;
    while (entry) {
        fprintf(stdout, "(%c, %c)", entry->key, entry->data);
        entry = entry->lruListNext;
    }
    fprintf(stdout, "\n<<<<<<<<<<<<\n");
}

