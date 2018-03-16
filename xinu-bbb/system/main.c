/*  main.c  - main */

#include <xinu.h>

#define K_MSGS  5 


void mysend(pid32);
void myrec(void);

process	main(void)
{
	pid32 pidsendto;
	pidsendto = create(myrec, 1124, 10, "recv1", 0);
	resume(pidsendto);
	resume(create(mysend, 1124, 10, "send1", 1, pidsendto));
	return;

}

void mysend(pid32 pid)
{
		sendnew(pid, 111);
//		sleep(1);
		sendnew(pid, 222);
		//sleep(1);
		sendnew(pid, 333);
		//sleep(1);
		sendnew(pid, 444);
		//sleep(1);
		sendnew(pid, 555);
		//sleep(1);
		sendnew(pid, 666);

}

void myrec(void)
{
	umsg32	mymsg;

	sleep(1);
	mymsg = receivenew();
	
	kprintf("my msg is %u on pid of %d named %s \n", mymsg, currpid, proctab[currpid].prname );

	//sleep(1);
	mymsg = 0;
	mymsg = receivenew();
	kprintf("my msg2 is %u on pid of %d named %s \n", mymsg, currpid, proctab[currpid].prname);
	
	//sleep(1);
	mymsg = 0;
	mymsg = receivenew();
	kprintf("my msg3 is %u on pid of %d named %s \n", mymsg, currpid, proctab[currpid].prname);

	//sleep(1);
	mymsg = 0;
	mymsg = receivenew();
	kprintf("my msg4 is %u on pid of %d named %s \n", mymsg, currpid, proctab[currpid].prname);

	//sleep(1);
	mymsg = 0;
	mymsg = receivenew();
	kprintf("my msg5 is %u on pid of %d named %s \n", mymsg, currpid, proctab[currpid].prname);

	//sleep(1);
	mymsg = 0;
	mymsg = receivenew();
	kprintf("my msg6 is %u on pid of %d named %s \n", mymsg, currpid, proctab[currpid].prname);
}

/* NOT BEING USED RIGHT NOW */
void initmsgque(void) {
	struct msgque* quemsg;
	quemsg->msgcount = 0;
}