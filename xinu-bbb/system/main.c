/*  main.c  - main */

#include <xinu.h>

/* * Declare any global variables here */
#define NPHIL 5
sid32 utensils[NPHIL];
#define leftutensil(i) (utensils[i]) 
#define rightutensil(i) (utensils[(i+1)%NPHIL])

/* * Declare the philosopher function */
void philosopher(int32, sid32, sid32); 
void eat(int32); 
void think(int32);
void grab_utensils(int);
void put_down_utensils(int);

sid32 mutex;

/* * main: create the philosophers. */
process	main(void)
{

/* Declare other local variables here */
    mutex = semcreate(1);
    sleep(2);
    int32 i; 
    char philname[PNMLEN];
    for (i = 0; i < NPHIL; i++) 
    {
        utensils[i] = semcreate(1); 
    }
    
    for (i = 0; i < NPHIL; i++) 
    { 
        sprintf(philname, "Philosopher %d", i);
        /* Add other arguments as needed to the create call */ 

        resume(create(philosopher, 1024, 20, philname, 3, i, leftutensil(i), rightutensil(i)));
    }
    
    return OK;

}

void grab_utensils(int32 i) 
{ 
    wait(leftutensil(i)); 
    wait(rightutensil(i)); 
}

void put_down_utensils(int32 i) 
{
    signal(leftutensil(i)); 
    signal(rightutensil(i)); 
}

/* * philosopher: implement a single philosopher */
void philosopher(int32 me, sid32 left, sid32 right)
{
    while (1) 
    {
        grab_utensils(me);
        
        wait(mutex);
            kprintf("philosopher %u picks up L-%u R-%u \r\n",me, left, right);
        signal(mutex);
        
        eat(me);
        
        wait(mutex);
            kprintf("philosopher %u puts down L-%u R-%u \r\n",me, left , right);
        signal(mutex);
        put_down_utensils(me);
        /* ponder the ineffable mysteries of life */ 
        think(me);
    }
}

void think(int32 me)
{
    wait(mutex);
        kprintf("philosopher %u is thinking \r\n", me);
    signal(mutex);
    sleep(4);
    
    return;
}

void eat(int32 me)
{
    wait(mutex);
        kprintf("philosopher %u is eating \r\n", me);
    signal(mutex);
    sleep(3);
    return;
}