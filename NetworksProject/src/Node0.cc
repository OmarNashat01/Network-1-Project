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

void Node0::Printlog(int type, Message* mymsg){
    logs << "At time: " << simTime()
         << ", Node id: " << getIndex();
    if (type == 0){
        logs << ", [sent]"
                << ", seq number= " << mymsg->getHeader()
                << ", and payload= " << mymsg->getPayload()
                << ", and trailer= " << bitset<8>(mymsg->getTrailer()).to_string()
                << ", modified= " << -1 * mymsg->getMod()
                << ", Lost= " << mymsg->getLost()
                << ", Duplicate= " << (mymsg->getDuplicated() > 0)
                << ", delay= " << (mymsg->getDelay() > 0) << endl;

    }
    else if (type == 1){
        logs << ", [processing] Packet number: " << mymsg->getHeader() << ", Introducing channel error with code= " << mymsg->getError() << endl;
    }

}

void Node0::initialize()
{
    // TODO - Generated method body
    filenames[0] = "../msgs.txt";

    filenames[1] = "../msgs.txt";

    sendWindow.inFile.open(filenames[getIndex()], ifstream::in);

    logs.open("Logs.txt", ofstream::out);

    frame_expected = 0;

    for (int i = 0; i < MAX_SEQ; i++){
        timers[i].setFrame_type(timeout);
        timers[i].setAck_nr(i);
    }
}


void Node0::handleMessage(cMessage *msg)
{
    Message *mymsg = check_and_cast<Message *>(msg);

    int frame_type = mymsg->getFrame_type();
    double delay = 0.5; // processing delay


    // Handle received event
    switch (frame_type)
    {
        case intialization:
        {

            if (mymsg->getNodeInd() == getIndex()){  //detect the sending node

                if (!sendWindow.openFile(filenames[mymsg->getNodeInd()]))
                    cout << "Can't read from file\n";
                delay += mymsg->getStartTime();
            }
            else
                cancelAndDelete(mymsg);
            break;
        }
        case frame_arrival:
        {
            Printlog(1,mymsg);
            mymsg->setFrame_type(receiveProcessing);

            EV << "Frame arrived here at node: " << getIndex() << endl;
            scheduleAt(simTime() + 0.5, mymsg);


            break;
        }
        case sendProcessing:
        {

            if (mymsg->getMod()){
                string payload = mymsg->getPayload();
                payload[payload.size()/2] = char((bitset<8>(8) ^ bitset<8>(payload[payload.size()/2])).to_ulong());
                mymsg->setPayload(payload.c_str());
            }

            Printlog(0, mymsg);

            // Start Timer for timeout
            if (sendWindow.Sender()){
                EV << "starting timers at node: " << getIndex() << ", ack: " << mymsg->getHeader() << endl;
                scheduleAt(simTime() + TO_DELAY, &timers[mymsg->getHeader()]);
            }

            if (mymsg->getAck_nr() == -1)
                mymsg->setFrame_type(frame_arrival);
            else
                if (mymsg->getHeader() != mymsg->getAck_nr())
                    mymsg->setFrame_type(nack);
                else
                    mymsg->setFrame_type(ack);

            if (!mymsg->getLost())
                sendDelayed(mymsg, mymsg->getDelay(), "out");
            else
                cancelAndDelete(mymsg);

            break;
        }
        case receiveProcessing:
        {
            char parity = Window::calcParity(mymsg->getPayload());
            mymsg->setDelay(1.0);
            mymsg->setPayload("");
            mymsg->setName("ack");
            mymsg->setAck_nr(frame_expected);

            if (mymsg->getHeader() != frame_expected)
               mymsg->setName("Nack");
            else
               frame_expected = (frame_expected + 1) % MAX_SEQ;

            if (parity != mymsg->getTrailer()){
               mymsg->setFrame_type(cksum_err);
               mymsg->setName("check sum error");
            }
            mymsg->setFrame_type(sendProcessing);
            scheduleAt(simTime() + 0.5, mymsg);

            break;
        }
        case timeout:
        {
            // TODO: implement
            sendWindow.TOFrame(mymsg->getAck_nr());
            break;
        }
        case ack:
        {
            sendWindow.ackFrame(mymsg->getAck_nr());
            EV << "stopping timers at node: " << getIndex() << ", ack: " << mymsg->getAck_nr() << endl;
            cancelEvent(&timers[mymsg->getAck_nr()]);
            break;
        }
        case cksum_err:
        {
            //TODO: implement
            cancelAndDelete(mymsg);
            break;
        }

        case nack:
        {
            EV << "debug: ind: " << getIndex() << ", frametype: " << mymsg->getFrame_type() << endl;
            cancelAndDelete(mymsg);
            break;
        }

    }


    // Read more messages if available in sender

    while (sendWindow.readNext());




    // Send messages if available
    int msgDelay;
    Message * msgToSend = sendWindow.getMsg(msgDelay);
    while (msgToSend != NULL){
        Printlog(1,msgToSend);

        mymsg->setDelay(mymsg->getDelay() + msgDelay);
        msgToSend->setName(msgToSend->getPayload());
        msgToSend->setFrame_type(sendProcessing);
        scheduleAt(simTime() + delay, msgToSend);
        msgToSend = sendWindow.getMsg(msgDelay);
        delay += 0.5; // processing delay between each message
        if (msgToSend == NULL)
            cancelAndDelete(mymsg);
    }


}
