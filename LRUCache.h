//
// Created by zhangshibo on 17-2-23.
//

#ifndef LRUCACHE_LRUCACHE_H
#define LRUCACHE_LRUCACHE_H

/**
 * 创建LRU
 * @param capacity：容量
 * @param lruCache：LRU
 * @return
 */
int LRUCacheCreate(int capacity, void** lruCache);

/**
 * 销毁LRU
 * @param lruCache
 * @return
 */
int LRUCacheDestory(void* lruCache);

/**
 * 写操作
 * @param lruCache: LRU
 * @param key: key
 * @param data: value
 * @return
 */
int LRUCacheSet(void* lruCache, char key, char data);

/**
 * 读操作
 * @param lruCache
 * @param key
 * @return
 */
char LRUCacheGet(void* lruCache, char key);

/**
 * 查看所有缓存信息
 * @param lruCache
 */
void LRUCachePrint(void* lruCache);

#endif //LRUCACHE_LRUCACHE_H
