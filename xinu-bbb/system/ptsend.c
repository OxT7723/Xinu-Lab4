/* ptsend.c - ptsend */
/* edit lab 4 */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  ptsend  -  Send a message to a port by adding it to the queue
 *------------------------------------------------------------------------
 */
syscall	ptsend(
	  int32		portid,		/* ID of port to use		*/
	  umsg32	msg,		/* Message to send		*/
          uint16        tag           /* tag for message              */
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	ptentry	*ptptr;		/* Pointer to table entry	*/
	int32	seq;			/* Local copy of sequence num.	*/
	struct	ptnode	*msgnode;	/* Allocated message node 	*/
	struct	ptnode	*tailnode;	/* Last node in port or NULL	*/

	mask = disable();
        //kprintf("here-1\n");
        
	if ( isbadport(portid) ||
	     (ptptr= &porttab[portid])->ptstate != PT_ALLOC ) {   
            restore(mask);
		return SYSERR;
	}
        //kprintf("here-2\n");

	/* Wait for space and verify port has not been reset */

	seq = ptptr->ptseq;		/* Record original sequence	*/
        //kprintf("here-3\n");
	if (wait(ptptr->ptssem) == SYSERR
	    || ptptr->ptstate != PT_ALLOC
	    || ptptr->ptseq != seq) {
		restore(mask);
		return SYSERR;
	}
        //kprintf("here-4\n");
	if (ptfree == NULL) {
		panic("Port system ran out of message nodes");
	}
        
	/* Obtain node from free list by unlinking */

	msgnode = ptfree;		/* Point to first free node	*/
	ptfree  = msgnode->ptnext;	/* Unlink from the free list	*/
	msgnode->ptnext = NULL;		/* Set fields in the node	*/

	msgnode->ptmsg  = msg;
        msgnode->pttag  = tag;          /* Set the tag field            */
        msgnode->ptstate = PT_SEND;
        
        
        
        //kprintf("here-5\n");

	/* Link into queue for the specified port */

	tailnode = ptptr->pttail;
	if (tailnode == NULL) {		/* Queue for port was empty	*/
		ptptr->pttail = ptptr->pthead = msgnode;
	} else {			/* Insert new node at tail	*/
		tailnode->ptnext = msgnode;
		ptptr->pttail = msgnode;
	}
	signal(ptptr->ptrsem);
        //kprintf("here-6\n");
	restore(mask);
        //ptwakeup(portid, tag);
        //kprintf("here-7\n");
	return OK;
}


syscall ptwakeup (
        int32		portid,		/* ID of port to use		*/
        uint16        tag           /* tag for message              */
)
{
    sleep(1);
        intmask	mask;			/* Saved interrupt mask		*/
    	struct	ptentry	*ptptr;		/* Pointer to table entry	*/
	//int32	seq;			/* Local copy of sequence num.	*/
	//umsg32	msg;			/* Message to return		*/
	struct	ptnode	*msgnode;	/* First node on message list	*/
        mask = disable();
        //kprintf("here-8\n");
        
        if ( isbadport(portid) ||
	     (ptptr= &porttab[portid])->ptstate != PT_ALLOC ) {
		restore(mask);
		return (uint32)SYSERR;
	}
        
        //kprintf("here-9\n");
        
        msgnode = ptptr->pthead;
        //kprintf("here-10\n");
            
        kprintf("msgHead-%x-msgT-%x\n",ptptr->pthead, ptptr->pttail);
            while(msgnode != NULL )
            {
                kprintf("here-11\n");
                kprintf("here-tag-%u-\n",msgnode->pttag);
                kprintf("here-state %u\n", msgnode->ptstate );
                if(msgnode->pttag == tag && msgnode->ptstate == PT_RECV)
                {
                    //signal(ptptr->ptssem);
                    kprintf("hear-99\n");
                    signal(ptptr->ptrsem);
                    signal(msgnode->ptrsem);
                    return OK;
                }
                kprintf("msgnode %x - %x\n",msgnode, msgnode->ptnext);
                msgnode = msgnode->ptnext;
                //kprintf("--loop-wakeup-\n");
            }
        signal(ptptr->ptrsem);
        //kprintf("here-end\n");
    restore(mask);
    return OK;

}