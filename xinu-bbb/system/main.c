/*  main.c  - main */

#include <xinu.h>

struct mytestfna_args {
	int32 firstarg;
	int32 argstwo;
};


void mytestfna(struct mytestfna_args *);
void mytestfnb(struct mytestfna_args *);
uint32 addcallout(uint32, void *, void *);
void removecallout(uint32);
void mytestfnc(void);
void dumpProctab(int32);


struct mytestfna_args *loc;
struct mytestfna_args *loc1;


process	main(void)
{

	struct mytestfna_args *locopyi;
	struct mytestfna_args *locopyx;
	uint32 mypid;
	int32 i = 12;

	
	sleep(1);
	locopyi = &loc;
	locopyx = &loc1;
	locopyi->firstarg = 12;	
	

	void (*testptn1)() = mytestfna;
	void(*testptn2)() = mytestfnb;
	void(*testptn3)() = mytestfnc;


	mypid = addcallout(2000, testptn1, locopyi);
	kprintf("pid is: %d\n", mypid);

	locopyx->firstarg = 10;
	locopyx->argstwo = 15;

	mypid = addcallout(3000, mytestfnb, locopyx);
	kprintf("pid is: %d \n", mypid);

	/* callout that will be removed using removecallout */
	mypid = addcallout(10000, mytestfnb, NULL);
	kprintf("pid is: %d that we will kill before its ran\n", mypid);

	sleep(1);
	resume(create(dumpProctab, 1024, 30, "myPros", 1, 0));
	

	sleep(3);
	removecallout(mypid);
	sleep(1);
	
	resume(create(dumpProctab, 1024, 30, "myProscount", 1, 1));


	return 0;
}


void mytestfna(struct mytestfna_args *arg)
{

	kprintf("--One argument of %d \n", arg->firstarg);
	return;
}

void mytestfnb(struct mytestfna_args *arg)
{
	kprintf("--Two arguments of %d and %d\n", arg->firstarg, arg->argstwo);
	return;
}

void mytestfnc(void)
{
	kprintf("This request should have been killed\n");
	return;
}



//struct callout coqueu; 

uint32 addcallout(uint32 msdelay, void *funcaddr, void *argp)
{

	uint32 pid;
	intmask	mask;			/* Saved interrupt mask		*/


	pid = create(funcaddr, 1024, 100, "calloutFNs", 1, argp);

	mask = disable();
	insertd(pid, sleepq, msdelay); /* put in the sleep queue*/
	proctab[pid].prstate = PR_SLEEP; /* set status to PR_SLEEP */
	restore(mask);

	return  pid;
}

void removecallout(uint32 cid)
{
	//remove the corresponding entry from the callout queu

	kill(cid);
}



char *prssb[] =
{
	"FREE ",
	"CURR ",
	"READY ",
	"RECV ",
	"SLEEP ",
	"SUSP ",
	"WAIT ",
	"RECTIM",
};

void dumpProctab(int32 freeflag)

{
	int32 i;

	struct procent *p;

	kprintf("Process table dump:\n\n");
	//kprintf("Num\tState\tPrio\tSem\tName\tmall\n"); 
	kprintf("Num\tState\tPrio\tSem\tName\n");
	kprintf("----------------------------------");
	kprintf("----------------------------------\n");

	for (i = 0; i < NPROC; i++)
	{

		p = &proctab[i];

		if (freeflag || (p->prstate != PR_FREE))
		{
			kprintf("%d\t%6s\t%d\t%d\t%s\t\n", i, prssb[p->prstate], p->prprio, p->prsem, p->prname);
			//kprintf("%d\t%6s\t%d\t%d\t%s\t%u\n", i, prssb[p->prstate], p->prprio, p->prsem, p->prname, p->malleable);
		}

	}
}

