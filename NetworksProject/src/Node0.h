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

#ifndef __NETWORKSPROJECT_NODE0_H_
#define __NETWORKSPROJECT_NODE0_H_

#include <omnetpp.h>
#include "global.h"

using namespace omnetpp;
using namespace std;


/**
 * TODO - Generated class
 */

struct mesToSend // saved in sending window
{
    bool isNone = 0;
    double delay = 0;
    string payload;
    bool dup = 0;
    bool lost = 0;
    bool mod = 0;

    void set(mesToSend mes){
        isNone = mes.isNone;
        delay = mes.delay;
        payload = mes.payload;
        dup = mes.dup;
        lost = mes.lost;
        mod = mes.mod;
    }

};
class Node0 : public cSimpleModule
{
  protected:
    int flag;
    ifstream inFile;
    bool isSender;
    int wStart;
    int indToSend;
    int wEnd;
    string filenames[2];
    mesToSend window[MAX_SEQ];


    int startTime;
    void send_data(int frame_nr, int frame_expected, char data[]);
    virtual void initialize();
    mesToSend getNextMsg();
    bool between(int ack); // Take ack number and check if the ack is for a frame that is currently in my sending window or not
    bool canRead(); // Checks if there is space in sending window to store one more frame ( if not sender 0)
    int calcFilledSlots(int start); // Calculate number of full slots in sending window
    Message* bufToFrame(mesToSend mes); // takes mesToSend from the buffer and returns frame carrying its info
    char calcParity(string msg); // Calculate parity byte of the payload
    virtual void handleMessage(cMessage *msg);
};

#endif
