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
//    if (type == 1)
//        return;
//    logs << "At time: " << simTime()
//         << ", Node id: " << getIndex();
//    if (type == 0){
//        logs << ", [sent]"
//                << ", seq no.= " << mymsg->getHeader()
//                << ", payload= " << mymsg->getPayload()
//                << ", trailer= " << bitset<8>(mymsg->getTrailer()).to_string()
//                << ", mod= " << (mymsg->getMod() ? 0: -1 )
//                << ", Lost= " << mymsg->getLost()
//                << ", Dup= " << (mymsg->getDuplicated() > 0)
//                << ", del= " << (mymsg->getDelay() > 1) << endl;
//
//    }
//    else if (type == 1){
//        logs << ", [processing] Packet number: " << mymsg->getHeader() << ", Introducing channel error with code= " << mymsg->getError() << endl;
//    }
    logs << "At time[" << simTime()
         << "], Node[" << getIndex();
    if (type == 0){
        logs << "] [sent] "
                << "frame with seq_num=[" << mymsg->getHeader()
                << "] and payload=[ " << mymsg->getPayload()
                << " ] and trailer=[ " << bitset<8>(mymsg->getTrailer()).to_string()
                << " ] , Modified [" << (mymsg->getMod() ? 0 : -1)
                << " ] , Lost [" << (mymsg->getLost() ? "Yes":"No")
                << "], Duplicate [" << (mymsg->getDuplicated() > 0)
                << "], Delay [" << (mymsg->getDelay() > 1) << "]." << endl;

    }
    else if (type == 1){
        logs << "] , Introducing channel error with code = [" << mymsg->getError() << "]." << endl;
    }

}


void Node0::initialize()
{
    // TODO - Generated method body
    filenames[0] = "../msgs.txt";

    filenames[1] = "../msgs.txt";

    sendWindow.inFile.open(filenames[getIndex()], ifstream::in);

    logs.open("Logs.txt", std::ios::app);

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

            // Start Timer for timeout
            if (sendWindow.Sender()){
                EV << "starting timers at node: " << getIndex() << ", ack: " << mymsg->getHeader() << endl;
                if (timers[mymsg->getHeader()].isScheduled())
                    cancelEvent(&timers[mymsg->getHeader()]);
                scheduleAt(simTime() + TO_DELAY, &timers[mymsg->getHeader()]);
            }

            if (mymsg->getAck_nr() == -1){
                mymsg->setFrame_type(frame_arrival);
                Printlog(0, mymsg);
            }
            else
                if (strcmp(mymsg->getName(),"NACK") == 0)
                    mymsg->setFrame_type(nack);
                else if (strcmp(mymsg->getName(),"CKSUM") == 0)
                    mymsg->setFrame_type(cksum_err);
                else
                    mymsg->setFrame_type(ack);


            if (!mymsg->getLost())
                sendDelayed(mymsg, mymsg->getDelay(), "out");
            else
                cancelAndDelete(mymsg);

            return;
            break;
        }
        case receiveProcessing:
        {
            char parity = Window::calcParity(mymsg->getPayload());
            mymsg->setDelay(1.0);
            mymsg->setPayload("");
            mymsg->setName("ACK");
            mymsg->setAck_nr(frame_expected);



            if (parity != mymsg->getTrailer())
               mymsg->setName("CKSUM");

            if (mymsg->getHeader() != frame_expected)
               mymsg->setName("NACK");

            if (strcmp(mymsg->getName(),"ACK") == 0)
               frame_expected = (frame_expected + 1) % MAX_SEQ;

            mymsg->setFrame_type(sendProcessing);
            scheduleAt(simTime(), mymsg);
            logs << "At time[" << simTime() << "], Node[" << getIndex()
                    << "] Sending [" << mymsg->getName() << "] with number [" << mymsg->getAck_nr()
                    << "] , loss [" << (mymsg->getLost() ? "Yes":"No") << "]." << endl;
            return;
            break;
        }
        case timeout:
        {
            cout << "time out event here...\n\n";
            // TODO: implement
            logs << "Time out event at time [" <<  simTime()
                    << "], at Node[" << getIndex() << "] for frame with seq_num=["
                    <<  mymsg->getHeader() << "]." << endl;

            sendWindow.TOFrame(mymsg->getAck_nr());
            for (int i=0; i < MAX_SEQ; i++)
            {
                if (timers[i].isScheduled())
                    cancelEvent(&timers[i]);
            }
            break;
        }
        case ack:
        {
            sendWindow.ackFrame(mymsg->getAck_nr());
            //            if (mymsg->getAck_nr() == 4)
//                cout<< "el ack wesel at :" << simTime() << endl;
//            EV << "stopping timers at node: " << getIndex() << ", ack: " << mymsg->getAck_nr() << endl;
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

        msgToSend->setName(msgToSend->getPayload());
        msgToSend->setFrame_type(sendProcessing);
        scheduleAt(simTime() + delay, msgToSend);
        msgToSend = sendWindow.getMsg(msgDelay);
        delay += 0.5; // processing delay between each message
    }


}
