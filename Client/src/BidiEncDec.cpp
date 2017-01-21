
#include "../include/BidiEncDec.h"


bool BidiEncDec::delrqRrqWrqMessageCreator(char nextByte){

    bool done = false;

    if(currentMessageFieldNumber == 1){

        if(bytesToFileName(nextByte)){

            incomingMessage.setaByte(nextByte);
            done = true;
        }
    }
    return done;
}

bool BidiEncDec::dataMessageCreator(char nextByte){

    bool done = false;

    switch (currentMessageFieldNumber){

//            working on the packetSize
        case 1: {

            bytesToPacketSize(nextByte);
            break;
        }

//            working on blockNumber
        case 2: {

            bytesToBlockNumber(nextByte);
            break;
        }

//            working on data
        case 3: {

            done = bytesToData(nextByte);
            break;
        }

        default:
            break;
    }

    return done;
}

bool BidiEncDec::ackMessageCreator(char nextByte){

    bool done = false;

    if(currentMessageFieldNumber == 1){

        done = bytesToBlockNumber(nextByte);
    }
    return done;
}

bool BidiEncDec::errorMessageCreator(char nextByte){

    bool done = false;

    switch(currentMessageFieldNumber){

//            working on errorCode
        case 1: {

            bytesToErrorCode(nextByte);
            break;
        }

//            working on errMsg
        case 2: {

            done = bytesToErrMsg(nextByte);

            if(done){

                incomingMessage.setaByte(nextByte);
            }

            break;
        }

        default:
            break;
    }

    return done;
}

bool BidiEncDec::logrqMessageCreator(char nextByte){

    bool done = false;

    if(currentMessageFieldNumber == 1){

        if(bytesToUserName(nextByte)){

            incomingMessage.setaByte(nextByte);
            done = true;
        }
    }
    return done;
}

bool BidiEncDec::bcastMessageCreator(char nextByte){

    bool done = false;

    switch (currentMessageFieldNumber){

//            deletedAdded
        case 1: {

            incomingMessage.setDeletedAdded(nextByte);
            doneWithField();
            break;
        }

//            fileName
        case 2: {

            done = bytesToFileName(nextByte);

            if(done){

                incomingMessage.setaByte(nextByte);
            }

            break;
        }

        default: {

            break;
        }
    }

    return done;
}




void BidiEncDec::bytesToOpcode(char nextByte){

    short opcode = incomingBytesToShort(nextByte);

    if(opcode != -1){

        incomingMessage.setOpcode(opcode);
        doneWithField();
    }
}

bool BidiEncDec::bytesToFileName(char nextByte){

    string fileName = incomingBytesToString(nextByte);

    if(fileName != ""){

        incomingMessage.setFileName(fileName);
        doneWithField();
        return true;
    }

    return false;
}

bool BidiEncDec::bytesToBlockNumber(char nextByte){

    short blockNumber = incomingBytesToShort(nextByte);

    if(blockNumber != -1){

        incomingMessage.setBlockNumber(blockNumber);
        doneWithField();
        return true;
    }

    return false;
}

void BidiEncDec::bytesToPacketSize(char nextByte){

    short packetSize = incomingBytesToShort(nextByte);

    if(packetSize != -1){

        incomingMessage.setPacketSize(packetSize);
        doneWithField();
    }
}

bool BidiEncDec::bytesToData(char nextByte){

    incomingBytes.push_back(nextByte);

    if(incomingMessage.getPacketSize() != (short) 512){

        char* data = incomingBytesToArr();
        incomingMessage.setData(data);
        doneWithField();
        return true;
    }

    return false;
}

void BidiEncDec::bytesToErrorCode(char nextByte){

    short errorCode = incomingBytesToShort(nextByte);

    if(errorCode != -1){

        incomingMessage.setErrorCode(errorCode);
        doneWithField();
    }
}

bool BidiEncDec::bytesToErrMsg(char nextByte){

    string errMsg = incomingBytesToString(nextByte);

    if(errMsg != ""){

        incomingMessage.setErrMsg(errMsg);
        doneWithField();
        return true;
    }

    return  false;
}

bool BidiEncDec::bytesToUserName(char nextByte){

    string userName = incomingBytesToString(nextByte);

    if(userName != ""){

        incomingMessage.setUserName(userName);
        doneWithField();
        return true;
    }

    return false;
}



string BidiEncDec::incomingBytesToString(char nextByte){

    if(nextByte == (char) 0){

        char* fileNameBytes = incomingBytesToArr();

        string aString = string(fileNameBytes);

        return aString;
    }
    else{

        incomingBytes.push_back(nextByte);
        return "";
    }
}

char* BidiEncDec::incomingBytesToArr(){

    char* byteArr = (char*)"";

    for(unsigned int i=0; i<incomingBytes.size(); i++){

        byteArr[i] = incomingBytes.at((unsigned long) i);
    }
    return byteArr;
}

void BidiEncDec::doneWithMessage(){

    this->incomingBytes.clear();
    this->incomingMessage = BidiMessage();
    currentMessageFieldNumber = 0;
    this->isNewMessage = true;
}

void BidiEncDec::doneWithField(){

    emptyIncomingBytes();
    currentMessageFieldNumber++;
}

void BidiEncDec::emptyIncomingBytes(){

    incomingBytes.clear();
}




short BidiEncDec::incomingBytesToShort(char nextByte){

    incomingBytes.push_back(nextByte);

    if(incomingBytes.size() == 2){

        char* shortBytes = incomingBytesToArr();
        short num = bytesToShort(shortBytes);

        return num;
    }
    return (short) -1;
}


char* BidiEncDec::shortToBytes(short num){

    char* byteArr = (char *) "";
    byteArr[0] = (char) ((num >> 8) & 0xFF);
    byteArr[1] = (char) (num & 0xFF);

    return byteArr;
}

short BidiEncDec::bytesToShort(char* byteArr){

    short result = (short)((byteArr[0] & 0xff) << 8);
    result += (short)(byteArr[1] & 0xff);
    return result;
}

void BidiEncDec::putInByteArray(char* toPut, int toPutLength, char* container, int fromIndex){

    for(int putIndex = 0, containerIndex = fromIndex; putIndex < toPutLength ; putIndex++, containerIndex++){

        container[containerIndex] = toPut[putIndex];
    }
}



BidiMessage BidiEncDec::decodeNextByte(char nextByte){

    bool done = false;

    if(isNewMessage){

        incomingBytes = vector<char>();
        incomingMessage = BidiMessage();
        currentMessageFieldNumber = 0;
        this->isNewMessage = false;
    }

    if(currentMessageFieldNumber == 0){

        bytesToOpcode(nextByte);
    }
    else {

        short opcode = incomingMessage.getOpcode();

        switch (opcode) {

//                DELRQ, RRQ, WRQ
            case 1:
            case 2:
            case 8: {

                done = delrqRrqWrqMessageCreator(nextByte);
                break;
            }

//                DATA
            case 3: {

                done = dataMessageCreator(nextByte);
                break;
            }

//                ACK
            case 4: {

                done = ackMessageCreator(nextByte);
                break;
            }

//                ERROR
            case 5: {

                done = errorMessageCreator(nextByte);
                break;
            }

//                DIRQ, DISC
            case 6:
            case 10: {

                return incomingMessage;
            }

//                LOGRQ
            case 7: {

                done = logrqMessageCreator(nextByte);
                break;
            }

//                BCAST
            case 9: {

                done = bcastMessageCreator(nextByte);
                break;
            }

//                unKnown opcode
            default: {

                incomingMessage.setOpcode((short) -1);
                return incomingMessage;
            }
        }
    }

    if(done){
        BidiMessage newMessage = BidiMessage(this->incomingMessage);

        doneWithMessage();
        return newMessage;
    }
    else {

        return incomingMessage;
    }
}



char* BidiEncDec::encode(BidiMessage message){

    char* encoded = (char*)"";

    short msgType = message.getOpcode();
    char* msgTypeBytes = shortToBytes(msgType);

    switch (msgType){

//            DELRQ, RRQ, WRQ
        case 1:
        case 2:
        case 8:{

            string fileName = message.getFileName();
            char* fileNameBytes = (char *) fileName.c_str();

//            vector<char> aByte;
//            aByte.push_back(message.getaByte());

            char* aByte = (char*) "";
            aByte[0] = message.getaByte();

            int encodedLength = (int)fileName.length() + 3;

            putInByteArray(msgTypeBytes, 2, encoded, 0);
            putInByteArray(fileNameBytes, (int) fileName.length(), encoded, 2);
            putInByteArray(aByte, 1, encoded, encodedLength-1);

            break;
        }

//            DATA
        case 3: {

            short packetSize = message.getPacketSize();
            char* packetSizeBytes = shortToBytes(packetSize);

            short blockNumber = message.getBlockNumber();
            char* blockNumberBytes = shortToBytes(blockNumber);

            char* data = message.getData();

            putInByteArray(msgTypeBytes, 2, encoded, 0);
            putInByteArray(packetSizeBytes, 2, encoded, 2);
            putInByteArray(blockNumberBytes, 2, encoded, 4);
            putInByteArray(data, (int) packetSize, encoded, 6);

            break;
        }

//            ACK
        case 4: {

            short blockNumber = message.getBlockNumber();
            char* blockNumberBytes = shortToBytes(blockNumber);

            putInByteArray(msgTypeBytes, 2, encoded, 0);
            putInByteArray(blockNumberBytes, 2, encoded, 2);

            break;
        }

//            ERROR
        case 5: {

            short errorCode = message.getErrorCode();
            char* errorCodeBytes = shortToBytes(errorCode);

            string errMsg = message.getErrMsg();
            char* errMsgBytes = (char *) errMsg.c_str();

            char* aByte = (char*) "";
            aByte[0] = message.getaByte();

            int encodedLength = (int)errMsg.length() + 5;

            putInByteArray(msgTypeBytes, 2, encoded, 0);
            putInByteArray(errorCodeBytes, 2, encoded, 2);
            putInByteArray(errMsgBytes, (int) errMsg.length(), encoded, 4);
            putInByteArray(aByte, 1, encoded, encodedLength-1);

            break;
        }

//            DIRQ, DISC
        case 6:
        case 10: {

            encoded = msgTypeBytes;
            break;
        }

//            LOGRQ
        case 7: {

            string userName = message.getUserName();
            char* userNameBytes = (char *) userName.c_str();

            char* aByte = (char*) "";
            aByte[0] = message.getaByte();

            int encodedLength = (int) userName.length() + 3;

            putInByteArray(msgTypeBytes, 2, encoded, 0);
            putInByteArray(userNameBytes, (int) userName.length(), encoded, 2);
            putInByteArray(aByte, 1, encoded, encodedLength-1);

            break;
        }

//            BCAST
        case 9: {

            char* deletedAdded = (char*) "";
            deletedAdded[0] = message.getDeletedAdded();

            string fileName = message.getFileName();
            char* fileNameBytes = (char *) fileName.c_str();

            char* aByte = (char*) "";
            aByte[0] = message.getaByte();

            int encodedLength = (int)fileName.length() + 4;

            putInByteArray(msgTypeBytes, 2, encoded, 0);
            putInByteArray(deletedAdded, 1, encoded, 2);
            putInByteArray(fileNameBytes, (int) fileName.length(), encoded, 3);
            putInByteArray(aByte, 1, encoded, encodedLength-1);

            break;
        }

        default:{
            break;
        }
    }

    return encoded;
}


BidiEncDec::~BidiEncDec() {}


