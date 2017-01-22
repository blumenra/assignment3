#ifndef CLIENT_BIDIENCDEC_H
#define CLIENT_BIDIENCDEC_H

#include <string>
#include <iostream>
#include <vector>
#include "BidiMessage.h"

#endif //CLIENT_BIDIENCDEC_H

using namespace std;

class BidiEncDec {

private:
    vector<char> incomingBytes;
    bool isNewMessage = true;
    BidiMessage incomingMessage;
    int currentMessageFieldNumber;


    bool delrqRrqWrqMessageCreator(char nextByte);
    bool dataMessageCreator(char nextByte);
    bool ackMessageCreator(char nextByte);
    bool errorMessageCreator(char nextByte);
    bool logrqMessageCreator(char nextByte);
    bool bcastMessageCreator(char nextByte);



    void bytesToOpcode(char nextByte);
    bool bytesToFileName(char nextByte);
    bool bytesToBlockNumber(char nextByte);
    void bytesToPacketSize(char nextByte);
    bool bytesToData(char nextByte);
    void bytesToErrorCode(char nextByte);
    bool bytesToErrMsg(char nextByte);
    bool bytesToUserName(char nextByte);
    string incomingBytesToString(char nextByte);
    char* incomingBytesToArr();
    void doneWithMessage();
    void doneWithField();
    void emptyIncomingBytes();



    short incomingBytesToShort(char nextByte);

    void shortToBytes(short num, char* byteArr);
    short bytesToShort(char* byteArr);
    void putInByteArray(char* toPut, int toPutLength, char* container, int fromIndex);

public:
    BidiEncDec();

    BidiMessage decodeNextByte(char nextByte);

    void encode(BidiMessage message, char* encoded);

    virtual ~BidiEncDec();
};
