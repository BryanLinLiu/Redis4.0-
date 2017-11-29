/* adlist.h - A generic doubly linked list implementation
 *
 * Copyright (c) 2006-2012, Salvatore Sanfilippo <antirez at gmail dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Redis nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __ADLIST_H__
#define __ADLIST_H__

/* Node, List, and Iterator are the only data structures used currently. */
// 链表节点
typedef struct listNode {
    struct listNode *prev; // 指向前一节点的指针
    struct listNode *next; // 指向后一节点的指针
    void *value; // 值
} listNode;
// 链表迭代器
typedef struct listIter {
    listNode *next; // 下一节点的指针
    int direction; // 方向
} listIter;
// 链表
typedef struct list {
    listNode *head; // 头
    listNode *tail; // 尾
    void *(*dup)(void *ptr); // 拷贝回调函数
    void (*free)(void *ptr); // 释放回调函数
    int (*match)(void *ptr, void *key); // 匹配回调函数
    unsigned long len; //链表长度
} list;

/* Functions implemented as macros */
#define listLength(l) ((l)->len) // 返回链表l长度
#define listFirst(l) ((l)->head) // 返回l的表头
#define listLast(l) ((l)->tail)  // 返回l的表尾
#define listPrevNode(n) ((n)->prev) // 返回节点n的前一节点
#define listNextNode(n) ((n)->next) // 返回节点n的后一节点
#define listNodeValue(n) ((n)->value) // 返回节点n的值

#define listSetDupMethod(l,m) ((l)->dup = (m)) // set l的拷贝回调
#define listSetFreeMethod(l,m) ((l)->free = (m)) // set l的释放回调
#define listSetMatchMethod(l,m) ((l)->match = (m)) // set l的匹配回调

#define listGetDupMethod(l) ((l)->dup) // get l的拷贝回调
#define listGetFree(l) ((l)->free) // get l的释放回调
#define listGetMatchMethod(l) ((l)->match) // get l的匹配回调

/* Prototypes */
list *listCreate(void); // 创建一个新链表
void listRelease(list *list); // 释放链表
void listEmpty(list *list); // 清空链表
list *listAddNodeHead(list *list, void *value); // 插入新节点到表头
list *listAddNodeTail(list *list, void *value); // 插入新节点到表尾
list *listInsertNode(list *list, listNode *old_node, void *value, int after); // 在制定的节点处插入新节点
void listDelNode(list *list, listNode *node); // 删除链表节点
listIter *listGetIterator(list *list, int direction); // 取链表迭代器（指向表头或表尾）
listNode *listNext(listIter *iter); // 去迭代器的下一个节点
void listReleaseIterator(listIter *iter); // 释放迭代器
list *listDup(list *orig); // 链表拷贝
listNode *listSearchKey(list *list, void *key); // 查找value为key的节点
listNode *listIndex(list *list, long index); // 取index下表的节点
void listRewind(list *list, listIter *li); // 指向表头
void listRewindTail(list *list, listIter *li); // 指向表尾
void listRotate(list *list); // 链表反向
void listJoin(list *l, list *o); // 链表合并

/* Directions for iterators */
#define AL_START_HEAD 0 // 方向从表头到表尾
#define AL_START_TAIL 1 // 方向从表尾到表头

#endif /* __ADLIST_H__ */
