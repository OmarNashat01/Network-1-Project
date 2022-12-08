//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "Node0.h"
using namespace std;

//#define MAX_SEQ 5



//int event_type;

Define_Module(Node0);

void Node0::initialize()
{
    // TODO - Generated method body
    isSender = 0;
    wStart = 0;
    indToSend = 0;
    wEnd = wStart;
    filenames[0] = "../msgs.txt";
    filenames[1] = "../msgs.txt";

}

bool Node0::between(int ack)
{
    //because seq_nr is incremented circularly
    if (((wStart<=ack) && (ack<wEnd)) || ((wEnd<wStart) && (wStart<=ack)) || ((ack<wEnd) && (wEnd<wStart)))
        return true;
    else
        return false;
}

int Node0::calcFilledSlots(int start) // Receiver will always return false since sending window is empty
{
    if (wEnd >= start)
        return wEnd - start;
    else
        return MAX_SEQ - start + wEnd;
}

bool Node0::canRead()
{
    if(!isSender) return false;

    return calcFilledSlots(wStart) < 5;
}

mesToSend Node0::getNextMsg()
{

    mesToSend msg;

    if (!canRead() || inFile.peek() == EOF)
    {
        msg.isNone = 1;
        return msg;
    }

    string line; inFile >> line;

    msg.lost = (line[Loss] == '1');
    msg.delay = (line[Delay] == '1') * DELAY_ERROR;
    msg.dup = (line[Duplication] == '1');
    msg.mod = (line[Modification] == '1');

    msg.payload = line.substr(4,line.size() - 4);

    return msg;
}


void Node0::send_data(int frame_nr, int frame_expected, char data[])
{
    Message* s = new Message("");
    s->setPayload(data);
    s->setHeader(frame_nr);
    //s->setAck_nr((frame_expected+MAX_SEQ)%(MAX_SEQ+1)); //No piggybacking
    send(s,"out");
    //scheduleAt(simTime()+interval, s);

    //start_timer(frame_nr); //need to implement start_timer
}

Message* Node0::bufToFrame(mesToSend mes)
{
    Message* msg = new Message("normal msg");

    // Byte stuffing loop
    string payload = "$";
    for( int i = 0; i < mes.payload.size(); i++)
    {
        if (mes.payload[i] == '$' || mes.payload[i] == '/')
            payload += '/';
        payload += mes.payload[i];
    }
    payload += '$';


    msg->setHeader(indToSend); // packet number
    msg->setFrame_type(frame_arrival);
    msg->setPayload(payload.c_str());
    msg->setTrailer(calcParity(payload));

    return msg;
}

char Node0::calcParity(string msg)
{
    bitset<8> paritybyte = bitset<8> (0);
    for (int i=0; i < msg.size(); i++)
        paritybyte = (paritybyte ^ bitset<8>(msg[i]));

    return char(paritybyte.to_ulong());
}


void Node0::handleMessage(cMessage *msg)
{

    Message *mymsg = check_and_cast<Message *>(msg);

    int frame_type = mymsg->getFrame_type();
    double delay = 1.5; // processing and transmission delay

    // Handle received event
    switch (frame_type)
    {
        case intialization:
        {
            if (mymsg->getNodeInd() == getIndex()){             //detect the sending node
                inFile.open(filenames[mymsg->getNodeInd()]);
                isSender = 1 ;
                delay += mymsg->getStartTime();
            }
            break;
        }
        case frame_arrival:
        {
            // TODO: Implement

            break;
        }
        case cksum_err:
        {
            //TODO: implement
            break;
        }
        case timeout:
        {
            // TODO: implement
            break;
        }
    }


    // Read more messages if available in sender
    window[wEnd++].set(getNextMsg());

    // TODO: CHECKK IF THIS WORKS
    if (window[wEnd].isNone)
        wEnd--; // if msg read was NULL discard it

    // Send messages if available
    while (window[indToSend].lost && calcFilledSlots(indToSend) > 0) indToSend++;

    if (calcFilledSlots(indToSend) > 0)
        sendDelayed(bufToFrame(window[indToSend]), SimTime() + delay, "out");



//    if (strcmp(msg->getName(),"Coordinator") == 0)
//    {
//        Message *coordinator_msg = check_and_cast<Message *>(msg);
//        if (coordinator_msg->getHeader() == 0)
//        {
//            flag = 1;
//            //set starting time
//        }
//    }

    //if sender(flag=1) implement the sender protocol, else receiver protocol

}
