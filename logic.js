function ptrecv(portid, tag)
{
    var msg;
    var msgnode = [];
    var prev = [];
    var curr = [];
    var tailnode = [];

    doErroCheking();

    var TagNotFound = true;
    while (TagNotFound)
    {
        checkfortag();
    }

}

function checkfortag()
{
    while (msgnode != null) //loop over the msgnode
    {
        if (msgnode.pttag == tag && msgnode.ptstate == PT_SEND) //return tag and remove from the list
        {
            msg = msgnode.msg; 
            if (ptptr.pthead == ptptr.pttail) // only item in the list and remove it
            {
                //clear the head and tail
                ptptr.pthead = ptptr.tailnode = null;
            } else if (ptptr.pthead == msgnode) // at the front of the list with other itmes behind it
            {
                ptptr.pthead == msgnode.ptnext; //set head to next in list
            } else { // not the head or the tail, remove from somewhere in the middle of the list

                removeFromMiddleOfList();
                
                returnNodeToFreeList();

                return msg;
            }
        }
        //node not found increment list and loop again till end of the list
        msgnode = msgnode.ptnext;
    }
    //tag not found in the list, store the ptrecv process to the msgnode.

    tailnode = ptptr.pttail;
    if (tailnode == null) {
        ptptr.pttail = ptptr.pthead = msgnode;
    } else {
        tailnode.ptnext = msgnode;
        ptptr.pttail = msgnode;
    }

    signal(ptptr.ptssem); // unlock port 

    wait(msgnode.ptrsem); //wait on a new message to be sent. Once a new message is sent it will unblock the waiting process

}
function removeFromMiddleOfList()
{
    var loopOverList = ptptr.pthead;

    while (loopOverList != null)
    {
        if (loopOverList.ptnext == msgnode) //if the next item in the list matches the node to remove
        {
            //set the next item in the to the next node of msgnode
            loopOverList.ptnext = msgnode.ptnext;
            return; //break out of while loop and return 

        } else { // increment list
            loopOverlist = loopOverlist.ptnext;
        }
    }
}

function returnNodeToFreeList()
{
    msgnode.ptnext = ptfree;
    msgnode.pttag = null;
    msgnode.ptstate = null;
    ptfree = msgnode;
    signal(ptptr.ptsssem);
}
