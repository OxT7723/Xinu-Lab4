# Xinu-Lab4
Ports with tags and Filters in Xinu

**ptsend()**

ptsend() is called with a port, message, and tag. 
It stores the message, tag and ptstate of PT_SEND to msgnode. The msgnode is then pushed into the ptptr.
Then ptwakeup is called to wake up any sleeping receiving process that may be waiting for a message on that port with that given tag. 

**ptwakeup()**

Accepts  a portid and a tag to check if any receiving process is waiting for a message with that tag. 
First disable interrupts then error check if the port is bad and the state is PT_ALLOC. 
Set the msgnode for the current port and walk looking for a tag that matches with a ptstate of PT_RECV. If a match is found then signal the waiting process. 

**ptrecv()**

Accepts a port, message and tag from a receiving process 
msgnode is set to the pthead of ptptr (the start of the list).

1. Then loop over the list while it is not at the end of the list.
    1. Check to see if the tag matches pptag and the ptstate is PT_SEND
       1. If a match is found get the message, remove it from the list and return it
    1. If a match if not found then walk list and try again till the end of the list.
  
1.	If the end of the list is reached without a match being found found then store the tag into pttag, set ptstate to PT_RECV, create semaphore and push it to the list. 

1.	Signal the ptptr sender semaphore. 

1.	Call wait so the process will wait till a new message is sent before the process will be signaled 

1.	Once the process is signaled start back at step 1 to find the new message. 

**The issue**
When the 2nd process calling ptrecv gets to the loop that walk the msgnode list, msgnode is null and the process gets blocked. That is at least what I can tell from my testing and I haven’t been able to track down why the msgnode list is null at that time when it should have another message.
