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
    int msgDelay;
    Message * msgToSend = sendWindow.getMsg(msgDelay);
    while (msgToSend){
        sendDelayed(msgToSend, msgDelay + delay, "out");
        msgToSend = sendWindow.getMsg(msgDelay);
    }

}
