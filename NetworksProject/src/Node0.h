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


class Node0 : public cSimpleModule
{
  protected:
    string filenames[2];
    Window sendWindow;
    Message timers[MAX_SEQ];
    int frame_expected;

    ofstream logs;

    void Printlog(int type, Message * mymsg);
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

#endif
