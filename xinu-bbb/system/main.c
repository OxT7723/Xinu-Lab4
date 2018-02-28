/*  main.c  - main */
/* edit lab 4 */

#include <xinu.h>
#define OURPORT 3

void proc1(void);
void proc2(void);

//char *msg = "This is tagged with A";
int32 ptnum; 

process	main(void)
{
    //int32 ptnum; 
    ptinit(PT_MSGS);
    ptnum = ptcreate(5);
    
    kprintf("ptnum %u \n",ptnum);
    
    
    sleep(1);
    resume(create(proc1, 1124, 5, "proc1", 0));
    sleep(2);
    resume(create(proc2, 1124, 5, "proc2", 0));
    //resume(create(proctest, 1124, 5, "proc3", 0));
    
}


void proc1(void)
{
    //kprintf("proc2 %s \r\n", msg);
    
    ptsend(ptnum, 234, 9); 

    ptsend(ptnum, 123, 5); 
    
    //sleep(2);
    
    
}

void proc2(void)
{
    
    
    
    umsg32 msgrec;
    
    sleep(2);
    msgrec =ptrecv(ptnum,5);    
    kprintf("first %u \r\n", msgrec);
    
    msgrec =ptrecv(ptnum,9);
    kprintf("second %u \r\n", msgrec);   
    
    
    
    
}

