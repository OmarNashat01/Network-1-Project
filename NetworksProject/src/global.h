/*
 * global.h
 *
 *  Created on: Dec 8, 2022
 *      Author: ranaa
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <iostream>
#include <stdio.h>
#include <string>
#include <bitset>
#include <vector>
#include <fstream>
#include <bitset>
#include "Message_m.h"


#define MAX_SEQ 6
#define DELAY_ERROR 4.0
#define DUP_DELAY 0.1


using namespace std;

enum {
    intialization,
    frame_arrival,
    cksum_err,
    timeout,
    network_layer_ready,

};

enum {
    Modification,
    Loss,
    Duplication,
    Delay
};

struct mesToSend // saved in sending window
{
    bool isNone = 0;
    double delay = 0;
    string payload;
    bool dup = 0;
    bool lost = 0;
    bool mod = 0;

    void set(mesToSend mes){
        if (isNone) {
            isNone = 1;
            return;
        }

        delay = mes.delay;
        payload = mes.payload;
        dup = mes.dup;
        lost = mes.lost;
        mod = mes.mod;
    }

};

class Window
{
protected:
    ifstream inFile;
    bool isSender = 0;
    int wStart = 0;
    int indToSend = 0;
    int wEnd = 0;
    mesToSend buffer[MAX_SEQ];

public:
    bool openFile(string filename){
        inFile.open(filename);
        isSender = 1;
        if (!inFile.is_open()) return 0;
        return 1;
    }

    bool ackFrame(int ack){
        //because seq_nr is incremented circularly
        if (((wStart<=ack) && (ack<wEnd)) || ((wEnd<wStart) && (wStart<=ack)) || ((ack<wEnd) && (wEnd<wStart)))
            return true;
        else
            return false;
    }

    bool canRead()  // Checks if there is space in sending window to store one more frame ( if not sender 0)
    {
        if(!isSender) return false;
        return calcFilledSlots(wStart) < MAX_SEQ-1;
    }

    bool readNext(){
        if (!canRead() || inFile.peek() == EOF)
            return 0;
        cout << "only haw haw \n Filed slots: " << calcFilledSlots(wStart) << endl;
        string line; inFile >> line;
        cout << "read line: " << line << endl;

        buffer[wEnd].lost = (line[Loss] == '1');
        buffer[wEnd].delay = (line[Delay] == '1') * DELAY_ERROR;
        buffer[wEnd].dup = (line[Duplication] == '1');
        buffer[wEnd].mod = (line[Modification] == '1');

        getline(inFile, line, '\n');
        buffer[wEnd].payload = line;

        wEnd = ( wEnd + 1) % MAX_SEQ;
        return 1;
    }

    char calcParity(string msg)
    {
        bitset<8> paritybyte = bitset<8> (0);
        for (int i=0; i < msg.size(); i++)
            paritybyte = (paritybyte ^ bitset<8>(msg[i]));

        return char(paritybyte.to_ulong());
    }

    int calcFilledSlots(int start) // Receiver will always return false since sending window is empty
    {
        if (wEnd >= start)
            return wEnd - start;
        else
            return MAX_SEQ - start + wEnd;
    }

    Message* getMsg(int &delay){
        if (calcFilledSlots(indToSend) <= 0)
            return NULL;

        Message* msg = new Message("normal msg");

        // Byte stuffing loop
        string payload = "$";
        for( int i = 0; i < buffer[indToSend].payload.size(); i++)
        {
            if (buffer[indToSend].payload[i] == '$' || buffer[indToSend].payload[i] == '/')
                payload += '/';
            payload += buffer[indToSend].payload[i];
        }
        payload += '$';


        msg->setHeader(indToSend); // packet number
        msg->setFrame_type(frame_arrival);
        msg->setPayload(payload.c_str());
        msg->setTrailer(calcParity(payload));

        delay = buffer[indToSend].delay;
        indToSend = (indToSend + 1) % MAX_SEQ;
        return msg;
    }


};

#endif /* GLOBAL_H_ */
