/*  main.c  - main */

#include <xinu.h>

#define K_MSGS  5  


//void mysend(pid32);
//void myrec(void);

process	main(void)
{
	
	pid32 pidsendto;
	pidsendto = create(myrec, 1124, 10, "recv1", 0);
	resume(pidsendto);
	resume(create(mysend, 1124, 10, "send1", 1, pidsendto));
	return;

}

