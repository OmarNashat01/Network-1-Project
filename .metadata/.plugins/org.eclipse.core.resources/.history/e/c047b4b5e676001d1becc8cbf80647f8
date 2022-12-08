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

#include <iostream>
#include <fstream>
#include <string.h>
using namespace std;
#include "coordinator.h"
#include "Message_m.h"

Define_Module(Coordinator);

void Coordinator::readInputFile(int &Node_id, float &starting_time)
{
    ifstream myfile;
    char temp;
    myfile.open("coordinator.txt");
    if (myfile.is_open())
    {
        if (myfile)
        {
            myfile >> temp;
            Node_id = int(temp);

            myfile >> temp;
            starting_time = float(temp);
        }
    }
    else
        cout << "Couldn't open file\n";
}

void Coordinator::initialize()
{
    int Node_id;
    float starting_time;
    readInputFile(Node_id, starting_time);
    Message *coordinator_msg= new Message("Coordinator"); //set message name as "Coordinator"
    coordinator_msg->setHeader(Node_id); //send Node_id in header
    coordinator_msg->setAck_nr(starting_time); //send starting_time in ack_nr
    send(coordinator_msg, "out0");
    send(coordinator_msg, "out1");
}

void Coordinator::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
}


