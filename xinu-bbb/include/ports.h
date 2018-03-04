/* ports.h - isbadport */
/* edit lab 4 */

#define	NPORTS		30		/* Maximum number of ports	*/
#define	PT_MSGS		100		/* Total messages in system	*/
#define	PT_FREE		1		/* Port is free			*/
#define	PT_LIMBO	2		/* Port is being deleted/reset	*/
#define	PT_ALLOC	3		/* Port is allocated		*/
#define PT_WAIT         4               /* waiting on a message         */
#define PT_SEND         5
#define PT_RECV         6

struct	ptnode	{			/* Node on list of messages 	*/
	uint32	ptmsg;			/* A one-word message		*/
        uint16  pttag;                  /* tag for message              */
        uint16  ptstate;                /* port state (SEND/RECV)       */
        sid32   ptrsem;                 /* Receiver semaphore           */
	struct	ptnode	*ptnext;	/* Pointer to next node on list	*/
        
};

struct	ptentry	{			/* Entry in the port table	*/
	sid32	ptssem;			/* Sender semaphore		*/
	sid32	ptrsem;			/* Receiver semaphore		*/
	uint16	ptstate;		/* Port state (FREE/LIMBO/ALLOC)*/
	uint16	ptmaxcnt;		/* Max messages to be queued	*/
	int32	ptseq;			/* Sequence changed at creation	*/
	struct	ptnode	*pthead;	/* List of message pointers	*/
	struct	ptnode	*pttail;	/* Tail of message list		*/
        //struct  ptwait  *ptwhead;       /* List of waiting message pointers */
        //struct  ptwait  *ptwtail;	/* Tail of waiting list		*/
};

struct ptwait {
        sid32   ptsender;                /* Sender semaphore             */
        sid32   ptrecv;                  /* receiver semaphore */
        uint16  pttag;
        struct ptnode   *ptnext;        /* Pointer to next node on list */
    
};

extern	struct	ptnode	*ptfree;	/* List of free nodes		*/
extern	struct	ptentry	porttab[];	/* Port table			*/
extern	int32	ptnextid;		/* Next port ID to try when	*/
					/*   looking for a free slot	*/

#define	isbadport(portid)	( (portid)<0 || (portid)>=NPORTS )
