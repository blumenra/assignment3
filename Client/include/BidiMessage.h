
#ifndef CLIENT_BIDIMESSAGE_H
#define CLIENT_BIDIMESSAGE_H

#include <string>
#include <iostream>

#endif //CLIENT_BIDIMESSAGE_H

using namespace std;

class BidiMessage {

private:
    short opcode;
    string fileName;
    string userName;
    short packetSize;
    short blockNumber;
    char* data;
    char deletedAdded;
    short errorCode;
    string errMsg;
    char aByte;

//
    BidiMessage(short opcode, string aString, char aByte);
    BidiMessage(short opcode);
    BidiMessage(short opcode, short packetSize, short blockNumber, char* data);
    BidiMessage(short opcode, short blockNumber);
    BidiMessage(short opcode, char deletedAdded, string fileName, char aByte);
    BidiMessage(short opcode, short errorCode, string errMsg, char aByte);

public:

    static BidiMessage createDataMessage(int packetSize, int blockNum, char* fileData);
    static BidiMessage createBcastMessage(int event, string fileName);
    static BidiMessage createAckMessage(int ackNum);
    static BidiMessage createErrorMessage(int errNum, string errMsg);
    static BidiMessage createLoginMessage(string userName);
    static BidiMessage createDeleteMessage(string fileName);
    static BidiMessage createRRQMessage(string fileName);
    static BidiMessage createWRQMessage(string fileName);
    static BidiMessage createDirMessage();
    static BidiMessage createDiscMessage();


//    empty
    BidiMessage();

//    copy
    BidiMessage(BidiMessage original);

//    getters
    short getOpcode();
    string getFileName();
    char getaByte();
    string getUserName();
    short getPacketSize();
    short getBlockNumber();
    char*  getData();
    char getDeletedAdded();
    short getErrorCode();
    string getErrMsg();


//    setters
    void setOpcode(short opcode);
    void setFileName(string fileName);
    void setUserName(string userName);
    void setPacketSize(short packetSize);
    void setBlockNumber(short blockNumber);
    void setData(char* data);
    void setDeletedAdded(char deletedAdded);
    void setErrorCode(short errorCode);
    void setErrMsg(string errMsg);
    void setaByte(char aByte);

    virtual ~BidiMessage();
};