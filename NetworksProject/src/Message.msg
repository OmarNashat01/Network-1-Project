
packet Message {
    int header; //data sequence number
    string payload; //payload with byte stuffing (flag=$ , Escape=/)
    char trailer; //parity byte
    int frame_type; //Data=0,ACK=1,NACK=2
    int ack_nr;
}
