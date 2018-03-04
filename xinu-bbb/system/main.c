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
    //sleep(2);
    resume(create(proc2, 1124, 5, "proc2", 0));
    //resume(create(proctest, 1124, 5, "proc3", 0));
    
}


void proc1(void)
{
    //case 1     
    ptsend(ptnum, 234, 9); 
    ptsend(ptnum, 123, 5); 
    
    //sleep(3);
    
    //ptsend(ptnum, 111, 6); 
    //case 2 
    //sleep(4);
    //kprintf("----case 2 ------\n");
    //ptsend(ptnum, 456, 9); 
    
    //sleep(8);
    //kprintf("---- sending to tag 5--\n");
    //ptsend(ptnum, 567, 5); 
    
    
}

void proc2(void)
{
    
    
    
    umsg32 msgrec;
    //case 1
    sleep(1);
    msgrec =ptrecv(ptnum,5);    
    kprintf("first %u \r\n\n", msgrec);
    msgrec = 0;
    msgrec =ptrecv(ptnum,9);
    kprintf("second %u \r\n", msgrec);   
    
    
    //case 2
    //msgrec = 0;
//    kprintf("-wait on tag 5--\n");
//    msgrec =ptrecv(ptnum,5);    
//    kprintf("--case 2 msg: %u \n", msgrec);
    
}

