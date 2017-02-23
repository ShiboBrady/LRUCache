cmake_minimum_required(VERSION 3.6)
project(LRUCache)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c LRUCacheImpl.c)
add_executable(LRUCache ${SOURCE_FILES})