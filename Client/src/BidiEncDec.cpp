
#include "../include/BidiEncDec.h"


bool BidiEncDec::delrqRrqWrqMessageCreator(char nextByte){

    bool done = false;

    if(currentMessageFieldNumber == 1){

        if(bytesToFileName(nextByte)){

            incomingMessage.setaByte(nextByte);
            int bytesLength = (int) (incomingMessage.getFileName().length() + 3);
            incomingMessage.setBytesLength(bytesLength);
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
            int bytesLength = incomingMessage.getPacketSize() + 6;
            incomingMessage.setBytesLength(bytesLength);
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

        incomingMessage.setBytesLength(4);
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

                int bytesLength = (int) (incomingMessage.getErrMsg().length() + 5);
                incomingMessage.setBytesLength(bytesLength);
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

            int bytesLength = (int) (incomingMessage.getUserName().length() + 3);
            incomingMessage.setBytesLength(bytesLength);
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

                int bytesLength = (int) (incomingMessage.getFileName().length() + 4);
                incomingMessage.setBytesLength(bytesLength);
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
        std::cout << "*******************opcode:  "<< opcode << std::endl;

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

        char data[incomingMessage.getPacketSize()];
        incomingBytesToArr(data);
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

        char fileNameBytes[incomingBytes.size()];
        incomingBytesToArr(fileNameBytes);

        string aString = string(fileNameBytes);

        return aString;
    }
    else{

        incomingBytes.push_back(nextByte);
        return "";
    }
}

void BidiEncDec::incomingBytesToArr(char* shortBytes){

//    char byteArr[incomingBytes.size()];

    for(unsigned int i=0; i<incomingBytes.size(); i++){

        shortBytes[i] = incomingBytes.at((unsigned long) i);
//        byteArr[i] = incomingBytes.at((unsigned long) i);
    }

//    shortBytes = byteArr;
    std::cout << "ba0 "<< (int)shortBytes[0] << std::endl;
    std::cout << "ba1 "<< (int)shortBytes[1] << std::endl;

}

void BidiEncDec::doneWithMessage(){

    this->incomingBytes.clear();
    this->incomingMessage = BidiMessage();
    currentMessageFieldNumber = 0;
    this->isNewMessage = true;
    incomingMessage.setComplete(true);
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
    std::cout << "INSIDE vector char: " << incomingBytes[incomingBytes.size()-1] << std::endl;

    if(incomingBytes.size() == 2){

        std::cout << "incomingBytesToShort incomingBytes 0:  "<< incomingBytes.at(0) << std::endl;
        std::cout << "incomingBytesToShort incomingBytes 1:  "<< incomingBytes.at(1) << std::endl;
        char shortBytes[2];
        incomingBytesToArr(shortBytes);
        std::cout << "incomingBytesToShort shortBytes0:  "<< shortBytes[0] << std::endl;
        std::cout << "incomingBytesToShort shortBytes1:  "<< shortBytes[1] << std::endl;
        short num = bytesToShort(shortBytes);
        char* g = (char*) "04";
        short gg = bytesToShort(g);
        std::cout << "incomingBytesToShort result:  "<< num << std::endl;
        std::cout << "incomingBytesToShort result g:  "<< gg << std::endl;

        return num;
    }
    return (short) -1;
}


void BidiEncDec::shortToBytes(short num, char* byteArr){

    std::cout << "where" << std::endl;
    byteArr[0] = (char) ((num >> 8) & 0xFF);
    std::cout << "is" << std::endl;
    byteArr[1] = (char) (num & 0xFF);
    std::cout << "the fault" << std::endl;
}

short BidiEncDec::bytesToShort(char* byteArr){

    short result = (short)((byteArr[0] & 0xff) << 8);
    std::cout << "inside bytesToShort byteArr[0]:  "<< byteArr[0] << std::endl;
    std::cout << "inside bytesToShort byteArr[0]&0xff:  "<< ((short)((byteArr[0] & 0xff))) << std::endl;
    std::cout << "inside bytesToShort 0&0xff:  "<< ((short)(((char)0 & 0xff))) << std::endl;
    std::cout << "inside bytesToShort 0xff:  "<< ((short)((0xff))) << std::endl;
    std::cout << "inside bytesToShort result1:  "<< result << std::endl;
    result += (short)(byteArr[1] & 0xff);
    std::cout << "inside bytesToShort result2:  "<< result << std::endl;
    std::cout << "inside bytesToShort byteArr[1]:  "<< byteArr[1] << std::endl;
    return result;
}

void BidiEncDec::putInByteArray(char* toPut, int toPutLength, char* container, int fromIndex){

    for(int putIndex = 0, containerIndex = fromIndex; putIndex < toPutLength ; putIndex++, containerIndex++){

        container[containerIndex] = toPut[putIndex];
    }
}



BidiMessage BidiEncDec::decodeNextByte(char nextByte){

    bool done = false;
    std::cout << "before decoder newMessage? "<< isNewMessage << std::endl;

    if(isNewMessage){
        std::cout << "ENTERING vector char: " << nextByte << std::endl;
        incomingBytes = vector<char>();
        incomingMessage = BidiMessage();
        currentMessageFieldNumber = 0;
        this->isNewMessage = false;
    }
    std::cout << "before if else FIELDnUMBER "<< currentMessageFieldNumber << std::endl;
    if(currentMessageFieldNumber == 0){

        std::cout << "before bytesToOpcode" << std::endl;

        bytesToOpcode(nextByte);
        std::cout << "after bytesToOpcode" << std::endl;
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
                std::cout << "inside case 4: " << std::endl;

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

                incomingMessage.setBytesLength(2);
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


                std::cout << "default:  "<< opcode << std::endl;
                incomingMessage.setOpcode((short) -1);
                return incomingMessage;
            }
        }
    }
    std::cout << "after if else doen:  "<< done << std::endl;
    std::cout << "after if else FIELDnUMBER "<< currentMessageFieldNumber << std::endl;


    if(done){
        BidiMessage newMessage = BidiMessage(this->incomingMessage);

        doneWithMessage();
        return newMessage;
    }
    else {
        std::cout << "isComplete? "<< incomingMessage.isComplete() << std::endl;
        BidiMessage newMessage = BidiMessage(this->incomingMessage);

        return newMessage;
//        return incomingMessage;
    }
}



void BidiEncDec::encode(BidiMessage message, char* encoded){

//    char encoded[message.getBytesLength()];

    short msgType = message.getOpcode();
    std::cout << "Before SHORTtObYTES IN ENCODE" << std::endl;
    char msgTypeBytes[2];
    shortToBytes(msgType, msgTypeBytes);
    std::cout << "AFTER SHORTtObYTES IN ENCODE" << std::endl;

    switch (msgType){

//            DELRQ, RRQ, WRQ
        case 1:
        case 2:
        case 8:{

            string fileName = message.getFileName();
            char* fileNameBytes = (char *) fileName.c_str();

//            vector<char> aByte;
//            aByte.push_back(message.getaByte());

            char aByte[] = "";
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
            char packetSizeBytes[] = "";
            shortToBytes(packetSize, packetSizeBytes);

            short blockNumber = message.getBlockNumber();
            char blockNumberBytes[] = "";
            shortToBytes(blockNumber, blockNumberBytes);

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
            char blockNumberBytes[] = "";
            shortToBytes(blockNumber, blockNumberBytes);

            putInByteArray(msgTypeBytes, 2, encoded, 0);
            putInByteArray(blockNumberBytes, 2, encoded, 2);

            break;
        }

//            ERROR
        case 5: {

            short errorCode = message.getErrorCode();
            char errorCodeBytes[] = "";
            shortToBytes(errorCode, errorCodeBytes);

            string errMsg = message.getErrMsg();
            char* errMsgBytes = (char *) errMsg.c_str();

            char aByte[] = "";
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

            putInByteArray(msgTypeBytes, 2, encoded, 0);
            break;
        }

//            LOGRQ
        case 7: {

            string userName = message.getUserName();
            char* userNameBytes = (char *) userName.c_str();
            char aByte[] = "";
            aByte[0] = message.getaByte();

            int encodedLength = (int) userName.length() + 3;

//            char encodedT[] = "";

            std::cout << "in case 8 1" << std::endl;
            putInByteArray(msgTypeBytes, 2, encoded, 0);
            std::cout << "in case 8 2" << std::endl;
            putInByteArray(userNameBytes, (int) userName.length(), encoded, 2);
            std::cout << "in case 8 3" << std::endl;
            putInByteArray(aByte, 1, encoded, encodedLength-1);
            std::cout << "in case 8 4" << std::endl;
//            encoded = encodedT;
            std::cout << "in case 8 5" << std::endl;
            break;
        }

//            BCAST
        case 9: {

            char deletedAdded[] = "";
            deletedAdded[0] = message.getDeletedAdded();

            string fileName = message.getFileName();
            char* fileNameBytes = (char *) fileName.c_str();

            char aByte[] = "";
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

    std::cout << "in after cases" << std::endl;

//    return encoded;
}


BidiEncDec::~BidiEncDec() {}

BidiEncDec::BidiEncDec():
incomingBytes(),
isNewMessage(true),
incomingMessage(),
currentMessageFieldNumber(0)
{}


