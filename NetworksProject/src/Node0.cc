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
#include <iostream>
#include "string.h"
#include <bitset>
#include <vector>
#include "Message_m.h"
#define MAX_SEQ 5

enum {frame_arrival, cksum_err, timeout, network_layer_ready};

//int event_type;

Define_Module(Node0);

bool between(int a, int b, int c)
{
    //because seq_nr is incremented circularly
    if (((a<=b) && (b<c)) || ((c<a) && (a<=b)) || ((b<c) && (c<a)))
        return true;
    else
        return false;
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


void Node0::initialize()
{
    // TODO - Generated method body
}

void Node0::handleMessage(cMessage *msg)
{
    if (strcmp(msg->getName(),"Coordinator") == 0)
    {
        Message *coordinator_msg = check_and_cast<Message *>(msg);
        if (coordinator_msg->getHeader() == 0)
        {
            flag = 1;
            //set starting time
        }
    }
    //if sender(flag=1) implement the sender protocol, else receiver protocol

}
