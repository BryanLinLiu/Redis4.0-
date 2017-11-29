/* A simple event-driven programming library. Originally I wrote this code
 * for the Jim's event-loop (Jim is a Tcl interpreter) but later translated
 * it in form of a library for easy reuse.
 * 事件驱动库
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

#ifndef __AE_H__
#define __AE_H__

#include <time.h>

#define AE_OK 0
#define AE_ERR -1

#define AE_NONE 0 //
#define AE_READABLE 1 //掩码-可读
#define AE_WRITABLE 2 //掩码-可写

#define AE_FILE_EVENTS 1 // 文件事件
#define AE_TIME_EVENTS 2 // 时间事件
#define AE_ALL_EVENTS (AE_FILE_EVENTS|AE_TIME_EVENTS) // 所有事件,包括文件和时间
#define AE_DONT_WAIT 4 // 不等待
#define AE_CALL_AFTER_SLEEP 8 //Sleep一段时间再唤醒

#define AE_NOMORE -1
#define AE_DELETED_EVENT_ID -1

/* Macros */
#define AE_NOTUSED(V) ((void) V)

struct aeEventLoop;

/* Types and data structures */
typedef void aeFileProc(struct aeEventLoop *eventLoop, int fd, void *clientData, int mask);
typedef int aeTimeProc(struct aeEventLoop *eventLoop, long long id, void *clientData);
typedef void aeEventFinalizerProc(struct aeEventLoop *eventLoop, void *clientData);
typedef void aeBeforeSleepProc(struct aeEventLoop *eventLoop);

/* File event structure 文件事件结构*/
typedef struct aeFileEvent {
    int mask; /* one of AE_(READABLE|WRITABLE)  掩码-读或写 */
    aeFileProc *rfileProc;// 读文件操作回调
    aeFileProc *wfileProc;// 写文件操作回调
    void *clientData; //数据
} aeFileEvent;

/* Time event structure 时间事件结构 */
typedef struct aeTimeEvent {
    long long id; /* time event identifier. 唯一标识*/
    long when_sec; /* seconds 秒时间*/
    long when_ms; /* milliseconds 微秒时间*/
    aeTimeProc *timeProc; //处理回调
    aeEventFinalizerProc *finalizerProc;
    void *clientData;
    struct aeTimeEvent *next;//指向下一个时间事件的指针
} aeTimeEvent;

/* A fired event 已经触发的时间*/
typedef struct aeFiredEvent {
    int fd;
    int mask;
} aeFiredEvent;

/* State of an event based program */
typedef struct aeEventLoop {
    int maxfd;   /* highest file descriptor currently registered  最大文件描述符*/
    int setsize; /* max number of file descriptors tracked 文件描述符的最大监听数目 */
    long long timeEventNextId; // 时间事件的唯一ID
    time_t lastTime;     /* Used to detect system clock skew 用于修正系统时间*/
    aeFileEvent *events; /* Registered events 已经注册的文件事件*/
    aeFiredEvent *fired; /* Fired events 以触发的文件事件*/
    aeTimeEvent *timeEventHead;
    int stop;
    void *apidata; /* This is used for polling API specific data */
    aeBeforeSleepProc *beforesleep; //Sleep之前调用的回调
    aeBeforeSleepProc *aftersleep; //Sleep之后调用的回调
} aeEventLoop;

/* Prototypes */
// 创建事件循环
aeEventLoop *aeCreateEventLoop(int setsize);
// 删除事件循环
void aeDeleteEventLoop(aeEventLoop *eventLoop);
// 暂停事件循环
void aeStop(aeEventLoop *eventLoop);
// 创建文件事件
int aeCreateFileEvent(aeEventLoop *eventLoop, int fd, int mask,
        aeFileProc *proc, void *clientData);
// 删除文件事件
void aeDeleteFileEvent(aeEventLoop *eventLoop, int fd, int mask);
// 从事件循环中取出指定的一个文件事件
int aeGetFileEvents(aeEventLoop *eventLoop, int fd);
// 创建时间事件
long long aeCreateTimeEvent(aeEventLoop *eventLoop, long long milliseconds,
        aeTimeProc *proc, void *clientData,
        aeEventFinalizerProc *finalizerProc);
// 删除文件事件
int aeDeleteTimeEvent(aeEventLoop *eventLoop, long long id);
// 处理事件
int aeProcessEvents(aeEventLoop *eventLoop, int flags);
// 等待
int aeWait(int fd, int mask, long long milliseconds);
// 主函数
void aeMain(aeEventLoop *eventLoop);
char *aeGetApiName(void);
// set事件循环Sleep前的回调
void aeSetBeforeSleepProc(aeEventLoop *eventLoop, aeBeforeSleepProc *beforesleep);
// set事件循环Sleep后的回调
void aeSetAfterSleepProc(aeEventLoop *eventLoop, aeBeforeSleepProc *aftersleep);
// 取事件循环的个数
int aeGetSetSize(aeEventLoop *eventLoop);
// Resize事件循环的个数
int aeResizeSetSize(aeEventLoop *eventLoop, int setsize);

#endif
