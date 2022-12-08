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

#define MAX_SEQ 5



//int event_type;

Define_Module(Node0);

void Node0::initialize()
{
    // TODO - Generated method body
    isSender = 0;
    wStart = 0;
    wSize = 5;
    wEnd = wStart + wSize;
    filenames[0] = "../msgs.txt";
    filenames[1] = "../msgs.txt";

}

bool between(int ack)
{
    //because seq_nr is incremented circularly
    if (((wStart<=ack) && (ack<wEnd)) || ((wEnd<wStart) && (wStart<=ack)) || ((ack<wEnd) && (wEnd<wStart)))
        return true;
    else
        return false;
}

bool canRead()                          //checks if window has available space
{
    if(!isSender()) return false;
    else
    {
        //TODO
        return true;
    }

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

string Node0::getNextMsg()
{
    string line;
    inFile >> line;

    cout << line << endl;
    return line;
}


void Node0::handleMessage(cMessage *msg)
{

    Message *mymsg = check_and_cast<Message *>(msg);

    int frame_type = mymsg->getFrame_type();
    double delay = 0;

    switch (frame_type){
        case intialization:
        {
            if (mymsg->getNodeInd() == getIndex()){             //detect the sending node
                inFile.open(filenames[mymsg->getNodeInd()]);
                isSender = 1 ;
                delay = mymsg->getStartTime();
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
            //TODO: impelent
            break;
        }
        case timeout:
        {
            // TODO: implement
            break;
        }
//        case network_layer_ready:
//        {
//            // TODO: implement
//            break;
//        }
    }



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
