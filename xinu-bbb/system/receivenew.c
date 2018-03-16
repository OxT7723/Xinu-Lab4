/* receive.c - receive */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  receive  -  Wait for a message and return the message to the caller
 *------------------------------------------------------------------------
 */
umsg32	receivenew(void)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	umsg32	msg;			/* Message to return		*/
	struct msgque* quemsg;

	mask = disable();
	prptr = &proctab[currpid];
	//if (prptr->prhasmsg == FALSE) {
	//	prptr->prstate = PR_RECV;
	//	resched();		/* Block until message arrives	*/
	//}

	quemsg = &massageque[currpid];
	if (quemsg->msgcount > 0)
	{
		int32 len = quemsg->msgcount; 
		int32 i;
		msg = quemsg->msg[0];

		for (i = 0; i < len - 1; i++) 
		{
			quemsg->msg[i] = quemsg->msg[i + 1];
		}
		quemsg->msgcount--;

		if (semcount(quemsg->semwait) < 0)
		{
			signal(quemsg->semwait); 
		}
	}
	//msg = prptr->prmsg;		/* Retrieve message		*/
	//prptr->prhasmsg = FALSE;	/* Reset message flag		*/
	restore(mask);
	return msg;
}
