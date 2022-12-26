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


Define_Module(Node0);

void Node0::initialize()
{
    // TODO - Generated method body
    filenames[0] = "../msgs.txt";

    filenames[1] = "../msgs.txt";

    frame_expected = 0;
}

void Node0::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage())
    {
        Message *mymsg = check_and_cast<Message *>(msg);
        if (mymsg->getMod()){
            string payload = mymsg->getPayload();
            payload[payload.size()/2] = char((bitset<8>(8) ^ bitset<8>(payload[payload.size()/2])).to_ulong());
            mymsg->setPayload(payload.c_str());
        }

        if (!mymsg->getLost())
            sendDelayed(mymsg, mymsg->getDelay(), "out");

        return;
    }
    Message *mymsg = check_and_cast<Message *>(msg);


    int frame_type = mymsg->getFrame_type();
    double delay = 0.5; // processing delay

    // Handle received event
    switch (frame_type)
    {
        case intialization:
        {
            cout << "nodeind: " << mymsg->getNodeInd() << ", getind: " << getIndex() << endl;
            if (mymsg->getNodeInd() == getIndex()){             //detect the sending node
                if (!sendWindow.openFile(filenames[mymsg->getNodeInd()]))
                    cout << "Can't read from file\n";
                delay += mymsg->getStartTime();
            }
            break;
        }
        case frame_arrival:
        {
            // TODO: Implement
            char parity = Window::calcParity(mymsg->getPayload());

            mymsg->setDelay(1.0);
            mymsg->setPayload("");
            mymsg->setFrame_type(ack);


            if (mymsg->getHeader() != frame_expected){
                mymsg->setFrame_type(nack);
                mymsg->setName("Nack");
            }
            else
                frame_expected = (frame_expected + 1) % MAX_SEQ;

            mymsg->setAck_nr(frame_expected);

            if (parity != mymsg->getTrailer()){
                mymsg->setFrame_type(cksum_err);
                mymsg->setName("Nack");
            }

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

    while (sendWindow.readNext());



    // Send messages if available
    if (sendWindow.Sender())
    {
        int msgDelay;
        Message * msgToSend = sendWindow.getMsg(msgDelay);
        while (msgToSend){
            mymsg->setDelay(mymsg->getDelay() + msgDelay);
            scheduleAt(simTime() + delay, msgToSend);
            msgToSend = sendWindow.getMsg(msgDelay);
            delay += 0.5; // processing delay between each message
        }
    }
    else
        scheduleAt(simTime() + delay, mymsg);

}
