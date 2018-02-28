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
        
        struct ptnode *walk;            /*pointer to walk the message list */

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
	
            
            walk = ptptr->pthead;
            msgnode = ptptr->pthead;
            
            while(walk != NULL )
            {
                kprintf("start loop\n");
                if (msgnode->pttag == tag)
                {
                    kprintf("tag match\n");
                    
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
                    signal(ptptr->ptssem);

                    restore(mask);
                    return msg;
                
                }
                
                walk = walk->ptnext;
                msgnode = msgnode->ptnext;
                kprintf("--loop--\n");
            }
            /** to do: if tag is not found make process wait. **/
            
            
        //if(msgnode->pttag == tag)
        //{
            //msg = msgnode->ptmsg;
        
            
        
            //if (ptptr->pthead == ptptr->pttail)	/* Delete last item	*/
//                    ptptr->pthead = ptptr->pttail = NULL;
  //          else
    //                ptptr->pthead = msgnode->ptnext;
      //      msgnode->ptnext = ptfree;		/* Return to free list	*/
        //    ptfree = msgnode;
          //  signal(ptptr->ptssem);
        
        //}
	restore(mask);
	return msg;
}
