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


#define MAX_SEQ 5
#define DELAY_ERROR 4.0
#define DUP_DELAY 0.1
#define TO_DELAY 10

using namespace std;

enum {
    intialization,
    frame_arrival,
    cksum_err,
    timeout,
    network_layer_ready,
    ack,
    nack,
    sendProcessing,
    receiveProcessing,


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
    string error= "";

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
    string filename;
    bool isSender = 0;
    int wStart = 0;
    int indToSend = 0;
    int wEnd = 0;
    mesToSend buffer[MAX_SEQ];

public:
    ifstream inFile;
    bool Sender(){ return isSender;}
    bool openFile(string name){
//        inFile.open(name, ifstream::in);
        filename = name;
        isSender = 1;
        if (!inFile.is_open()) return 0;
        return 1;
    }

    bool inWindow(int ack){
        //because seq_nr is incremented circularly
        if (((wStart<=ack) && (ack<wEnd)) || ((wEnd<wStart) && (wStart<=ack)) || ((ack<wEnd) && (wEnd<wStart)))
            return true;
        else
            return false;
    }
    static char calcParity(string msg)
    {
        bitset<8> paritybyte = bitset<8> (0);
        for (int i=0; i < msg.size(); i++)
            paritybyte = (paritybyte ^ bitset<8>(msg[i]));

        return char(paritybyte.to_ulong());
    }
    bool canRead()  // Checks if there is space in sending window to store one more frame ( if not sender 0)
    {
        if(!isSender) return false;
        return calcFilledSlots(wStart) < MAX_SEQ-1;
    }

    bool readNext(){

        if (!canRead() || inFile.peek() == EOF)
            return 0;

        string line; inFile >> line;

        buffer[wEnd].error = line;

        buffer[wEnd].lost = (line[Loss] == '1');
        buffer[wEnd].delay = (line[Delay] == '1') * DELAY_ERROR;
        buffer[wEnd].dup = (line[Duplication] == '1');
        buffer[wEnd].mod = (line[Modification] == '1');

        inFile.ignore();
        getline(inFile, line);
        buffer[wEnd].payload = line;



        wEnd = ( wEnd + 1) % MAX_SEQ;
        return 1;
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

        msg->setLost(buffer[indToSend].lost);
        msg->setMod(buffer[indToSend].mod);

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
        msg->setAck_nr(-1); // packet number
        msg->setFrame_type(frame_arrival);
        msg->setPayload(payload.c_str());
        msg->setTrailer(calcParity(payload));

        msg->setError(buffer[indToSend].error.c_str());

        delay = buffer[indToSend].delay + 1;
        indToSend = (indToSend + 1) % MAX_SEQ;

        return msg;
    }
    void ackFrame(int ack_nr){
        if (inWindow(ack_nr))
            wStart = (ack_nr + 1) % MAX_SEQ;
    }
    void TOFrame(int fr_nr){ //Time out
        if (inWindow(fr_nr))
            indToSend = fr_nr;
    }


};

#endif /* GLOBAL_H_ */
