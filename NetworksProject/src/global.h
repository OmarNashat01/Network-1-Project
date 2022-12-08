/*
 * global.h
 *
 *  Created on: Dec 8, 2022
 *      Author: ranaa
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <iostream>
#include <string>
#include <bitset>
#include <vector>
#include <fstream>
#include <bitset>
#include "Message_m.h"


#define MAX_SEQ 5
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




#endif /* GLOBAL_H_ */
