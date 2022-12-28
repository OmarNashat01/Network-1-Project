
packet Message {
    int nodeInd;// index of the node in initialization
    int startTime; // start timer in initialization
    
    int header; //data sequence number 
    string payload; //payload with byte stuffing (flag=$ , Escape=/)
    char trailer; //parity byte
    int frame_type; //Data=0,ACK=1,NACK=2
  
    int ack_nr;
    
    double delay;
    bool mod;
    bool lost;
    bool duplicated;
    string error;
}
