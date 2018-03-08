/*  main.c  - main */
/* edit lab 4 */

#include <xinu.h>
#define OURPORT 3

void proc1(umsg32,uint16);
void proc2(uint16);

//char *msg = "This is tagged with A";
int32 ptnum; 
sid32 mutex;

process	main(void)
{
    mutex = semcreate(1);
    //int32 ptnum; 
    ptinit(PT_MSGS);
    ptnum = ptcreate(5);
    
    //kprintf("ptnum %u \n",ptnum);
    
    
    sleep(1);
    wait(mutex);
    kprintf("*** First case msg '111' and tag 9 ***\n");
    signal(mutex);
    resume(create(proc1, 1124, 5, "proc1", 2, 111, 9));
    resume(create(proc2, 1124, 5, "proc2", 1, 9));
    
    sleep(1);
    wait(mutex);
    kprintf("\n\n*******\r\n 2nd msg '111' with tag8, \r\n * msg '222' tag7 \r\n * msg '333' tag 6 \n******\n");
    signal(mutex);
    resume(create(proc1, 1124, 5, "proc1", 2, 111, 8));
    resume(create(proc1, 1124, 5, "proc1", 2, 222, 7));
    resume(create(proc1, 1124, 5, "proc1", 2, 333, 6));
    resume(create(proc2, 1124, 5, "proc2", 1, 7));
    resume(create(proc2, 1124, 5, "proc2", 1, 8));
    resume(create(proc2, 1124, 5, "proc2", 1, 6));
    
    sleep(1);
    wait(mutex);
    kprintf("\n\n****Next Case msg '50' tag 7 \r\n *mag 55 tag 2 \r\n * with on rev of tag 0 \r\n**** \r\n ");
    signal(mutex);
    resume(create(proc1, 1124, 5, "proc1", 2, 50, 7));
    resume(create(proc1, 1124, 5, "proc1", 2, 55, 2));
    resume(create(proc2, 1124, 5, "proc2", 1, 7));
    resume(create(proc2, 1124, 5, "proc2", 1, 0));
    
    sleep(1);
    wait(mutex);
    kprintf("\n\n****Next Case wait for msg with tag 7 \r\n *mag 44 tag 2 \r\n *msg 77 tag 9 \r\n* mg 23 tag 5 \r\n*** \r\n ");
    signal(mutex);
    resume(create(proc2, 1124, 5, "proc2", 1, 7));//
    resume(create(proc1, 1124, 5, "proc1", 2, 44, 2));//
    resume(create(proc1, 1124, 5, "proc1", 2, 77, 9));//
    resume(create(proc1, 1124, 5, "proc1", 2, 99, 7));//
    //sleep(1);
    resume(create(proc1, 1124, 5, "proc1", 2, 23, 5));
    resume(create(proc2, 1124, 5, "proc2", 1, 5));
    resume(create(proc2, 1124, 5, "proc2", 1, 9));//
    resume(create(proc2, 1124, 5, "proc2", 1, 2));
    

    

}
/* used to send a message with a given message and given tag */
void proc1(umsg32 mymsg,uint16 tag)
{
    ptsend(ptnum, mymsg, tag);
}

/* used to recv a message for a given tag and print it out */
void proc2(uint16 tag)
{
    umsg32 msgrec;
    
    msgrec =ptrecv(ptnum,tag);    
    wait(mutex);
    kprintf("--msg of %u for tag %u -\n", msgrec, tag);
    signal(mutex);

}

