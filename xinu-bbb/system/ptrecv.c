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
        struct  ptnode  *prev;           /* */
        struct  ptnode  *curr;           /* */
        struct	ptnode	*tailnode;	/* Last node in port or NULL	*/
        int32   found = 0;
        
        //struct ptnode *walk;            /*pointer to walk the message list */

	mask = disable();
	if ( isbadport(portid) ||
	     (ptptr= &porttab[portid])->ptstate != PT_ALLOC ) {
		restore(mask);
		return (uint32)SYSERR;
	}

	/* Wait for message and verify that the port is still allocated */
        
        seq = ptptr->ptseq;		/* Record orignal sequence	*/
        
	if (wait(ptptr->ptrsem) == SYSERR || ptptr->ptstate != PT_ALLOC
	    || ptptr->ptseq != seq) {    
            restore(mask);
            return (uint32)SYSERR;
	}

	/* Dequeue first message that is waiting in the port */
        /** To do: Need to search for the tag and then dequeue when tag matches */    
	
        while(1) //keep looping till msg with tag is found
        {
            
            //wait(ptptr->ptssem);
            //wait(ptptr->ptrsem);
            if(msgnode ==NULL) //check if first instance looking for tag
            {
                kprintf("initFirstTime\n");
                msgnode = ptptr->pthead;
                //wait(msgnode->ptrsem);  
            }
            else {
                kprintf("**2ndlop***\n");
				kptintf("");
            } 
            
            
            //kprintf("my tag is %u \n",tag);
            
            while(msgnode != NULL && found == 0)
            {
                //kprintf("start loop\n");
                //kprintf("--tag %u--\n",msgnode->pttag);
                if (msgnode->pttag == tag && msgnode->ptstate == PT_SEND)
                {
                    //kprintf("tag match %u--\n",msgnode->pttag);
                    
                    //msgnode = ptptr->pthead;
                    /* our tag matched */
                    msg = msgnode->ptmsg;
                    if (ptptr->pthead == ptptr->pttail)	/* Delete last item	*/
                    {
                        //kprintf("head tail match \n");    
                        ptptr->pthead = ptptr->pttail = NULL;
                        found =1;
                    }
                    else if (ptptr->pthead == msgnode) /* head remove and update  */
                    {
                        //kprintf("matched head \n");
                        ptptr->pthead = msgnode->ptnext; 
                        found = 1;
                        
                    } else { /* not at the head or the tail of the list */
                        
                        //kprintf("head tail do not match \n");

                        //kprintf("set prev to head \n");
                        
                        /* start at the head of the list to remove item somewhere in the middle */
                        prev = ptptr->pthead;
                        //kprintf("after set \n");

                        /* remove node from list */
                        while(prev->ptnext != NULL && found == 0)
                        {
                            //kprintf("loop %x\n",prev->ptnext);

                            if(prev->ptnext == msgnode)
                            {
                                //kprintf("match\n");
                                prev->ptnext = msgnode->ptnext;
                                //kprintf("update prev\n");
                                found = 1;
                                //need to break out of the loop some way
                            } 
                            else
                            {
                                    //msgnode = msgnode->ptnext; // don't think I need this here
                                    //kprintf("valb4 %x\n",prev->ptnext);
                                    prev = prev->ptnext;
                                    //sleep(2);
                                    //kprintf("end loop\n");
                                    //kprintf("val %x\n",prev->ptnext);
                            }
                        
                        }
                            //sleep(1);
                        //kprintf("outside loop\n");

                    }
                    
                    if(msgnode->ptstate == PT_RECV)
                    {
                        semdelete(msgnode->ptrsem); 
                    }
                    //kprintf("setting ptfree \n");
                    msgnode->ptnext = ptfree;		/* Return to free list	*/
                    msgnode->pttag = NULL;
                    msgnode->ptstate = NULL;
                    //kprintf("after free \n");
                    ptfree = msgnode;
                    signal(ptptr->ptssem); 

                    restore(mask);
                    return msg;
                
                }
                /* tag not found walk the list and check again */
                //kprintf("didnt find %x \n", msgnode->ptnext);
                msgnode = msgnode->ptnext;
            }
            /* checked the hole list but no tag match was found */
            
            /* store the tag to the msgnode with state of PT_RECV and block 
             till process is awaken by a new tag being stored to the list
             */
            
            msgnode = ptfree;                   /* Point to first free node	*/
            ptfree  = msgnode->ptnext;          /* Unlink from the free list	*/
            msgnode->ptnext = NULL;		/* Set fields in the node	*/

            msgnode->ptstate  = PT_RECV;
            msgnode->pttag  = tag;              /* Set the tag field            */
            msgnode->ptrsem = semcreate(0);     /* */
            
            //curr = msgnode;                     /* store current */
            
            /* Link into queue for the specified port */
            
            tailnode = ptptr->pttail;
            if(tailnode == NULL) {
                ptptr->pttail = ptptr->pthead = msgnode;
            } else {
               tailnode->ptnext = msgnode;
                ptptr->pttail = msgnode;
            }
            
            
            kprintf("tag %u getting block \n", tag);
            
            signal(ptptr->ptssem);  /* unblock the port */
            kprintf("after msg blocked \n");
            
            //ptrsem   ptssem
            wait(msgnode->ptrsem); /* block receiver till msg with correct tag is found */
            kprintf("after tag wait\n");
            
            kprintf("msg unblocked \n");
            wait(ptptr->ptssem); /* block port while we check again */
        }
            
            
            
            
	restore(mask);
	return msg;
}
