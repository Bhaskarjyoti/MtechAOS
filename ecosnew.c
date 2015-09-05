/*
 * ecosnew.c
 * 
 * 
 * 
 * 
 */

#include<cyg/kernel/kapi.c>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>

#define STACK_MAX 4096
#define THREADS_MAX 2
#define RES_MAX 2
char stack[THREADS_MAX][STACK_MAX];
typedef int res_t;

cyg_thread thread_s[THREAD_MAX];
cyg_handle_t simple_threadA, simple_threadB;
cyg_thread_entry_t simple_programA;
cyg_thread_entry_t simple_programB;
cyg_mutex_t mut_t;
cyg_mutex_t res_lock;
cyg_cond_t res_wait;
res_t res_pool[RES_MAX];
res_t resAllocated;
int res_count = RES_MAX;

void resinit(void)
{
cyg_mutex_init(&res_lock);
cyg_cond_init(&res_wait, &res_lock);
}

void resFree(res_t res)
{
cyg_mutex_lock(&res_lock);
res_pool[res_count]=res;
res_count++;
cyg_cond_signal(&res_wait);
cyg_mutex_unlock(&res_lock);
}

res_t resAllocate(void)
{
res_t res;
cyg_mutex_lock(&res_lock);
while(res_count == 0)
cyg_cond_wait(&res_wait);
res_count--;
res = res_pool[res_count];
cyg_mutex_unlock(&res_lock);
return res;
}

void cyg_user_start(void)
{
cyg_mutex_init(&mut_t);
resinit();
cyg_thread_create(5,simple_programA,(cyg_addrword_t) 0,
"THREAD A", (void *)stack[0], STACK_MAX, &simple_threadA,
&thread_s[0]);
cyg_thread_create(4,simple_programB,(cyg_addrword_t) 1,
"THREAD B", (void *)stack[1], STACK_MAX, &simple_threadB,
&thread_s[1]);
cyg_thread_resume(simple_threadA);
cyg_thread_resume(simple_threadA);
}

void simple_programA(cyg_addrword_t data)
{
int msg = (int) data;
int delay;
printf("Starting thread : %d\n",msg);
cyg_thread_delay(200);
for(;;)
{
delay = 200 + (rand()%50);
resAllocated = resAllocate();
cyg_mutex_lock(&mut_t);
printf("Allocating resource for thread %d\n", msg);
cyg_mutex_unlock(&mut_t);
cyg_thread_delay(delay);
}
}

void simple_programB(cyg_addrword_t data)
{
int msg = (int) data;
int delay;
printf("Starting thread : %d\n",msg);
cyg_thread_delay(200);
for(;;)
{
delay = 200 + (rand()%50);
resFree(resAllocated);
cyg_mutex_lock(&mut_t);
printf("Allocating resource for thread %d\n", msg);
cyg_mutex_unlock(&mut_t);
cyg_thread_delay(delay);
}
}
