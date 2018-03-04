/* ptrecv.c - ptrecv */
/* edit lab 4*/
#include <xinu.h>

/*------------------------------------------------------------------------
 *  ptrecv  -  Receive a message from a port, blocking if port empty
 *------------------------------------------------------------------------
 */
uint32	ptrecv(
            int32       portid,		/* ID of port to use		*/
            uint16      tag             /* tag for message              */
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	ptentry	*ptptr;		/* Pointer to table entry	*/
	int32	seq;			/* Local copy of sequence num.	*/
	umsg32	msg;			/* Message to return		*/
	struct	ptnode	*msgnode;	/* First node on message list	*/
        struct	ptnode	*tailnode;	/* Last node in port or NULL	*/
        
        //struct ptnode *walk;            /*pointer to walk the message list */

	mask = disable();
	if ( isbadport(portid) ||
	     (ptptr= &porttab[portid])->ptstate != PT_ALLOC ) {
		restore(mask);
		return (uint32)SYSERR;
	}

	/* Wait for message and verify that the port is still allocated */

	
        /** To do: wait for msg with the tag */
        
        
        
        seq = ptptr->ptseq;		/* Record orignal sequence	*/
        
        
	if (wait(ptptr->ptrsem) == SYSERR || ptptr->ptstate != PT_ALLOC
	    || ptptr->ptseq != seq) {    
            restore(mask);
            return (uint32)SYSERR;
	}

	/* Dequeue first message that is waiting in the port */
        /** To do: Need to search for the tag and then dequeue when tag matches */    
	
        while(1)
        {
            
            //walk = ptptr->pthead;
            msgnode = ptptr->pthead;
            kprintf("my tag is %u \n",tag);
            
            if(msgnode == NULL)
            {
                kprintf("NULL %u\n",tag);
            }
            
            while(msgnode != NULL )
            {
                //kprintf("start loop\n");
                kprintf("--tag %u--\n\n",msgnode->pttag);
                if (msgnode->pttag == tag && msgnode->ptstate == PT_SEND)
                {
                    kprintf("tag match %u--\n",msgnode->pttag);
                    
                    //msgnode = ptptr->pthead;
                    /* our tag matched */
                    msg = msgnode->ptmsg;
                    if (ptptr->pthead == ptptr->pttail)	/* Delete last item	*/
                    {
                            ptptr->pthead = ptptr->pttail = NULL;
                    }
                    else
                    {
                        ptptr->pthead = msgnode->ptnext;
                    }
                    
                    msgnode->ptnext = ptfree;		/* Return to free list	*/
                    ptfree = msgnode;
                    signal(ptptr->ptssem); //??? is this needed here?? 

                    restore(mask);
                    return msg;
                
                }
                
                msgnode = msgnode->ptnext;
                kprintf("--loop-%u-\n", msgnode->pttag);
            }
            
            /* No msg found on this port with given tag  */
            
            msgnode = ptfree;		/* Point to first free node	*/
            ptfree  = msgnode->ptnext;	/* Unlink from the free list	*/
            msgnode->ptnext = NULL;		/* Set fields in the node	*/

            msgnode->ptstate  = PT_RECV;
            msgnode->pttag  = tag;          /* Set the tag field            */
            msgnode->ptrsem = semcreate(0);
            
            /* Link into queue for the specified port */
            
            tailnode = ptptr->pttail;
            if(tailnode == NULL) {
                ptptr->pttail = ptptr->pthead = msgnode;
            } else {
                tailnode->ptnext = msgnode;
                ptptr->pttail = msgnode;
            }
            
            
            kprintf("tag %u getting block \n", tag);
            signal(ptptr->ptssem); 
            kprintf("after msg blocked \n");
            wait(msgnode->ptrsem); /* block receiver till msg with correct tag is found */
            kprintf("after tag wait\n");
            
            
            
            kprintf("msg unblocked \n");
            wait(ptptr->ptssem); 
        }
            
            
            
            
	restore(mask);
	return msg;
}
