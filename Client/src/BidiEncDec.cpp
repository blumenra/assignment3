
#include "BidiEncDec"


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

    if(fileName != null){

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

    incomingBytes.add(nextByte);

    if(incomingMessage.getPacketSize() != (short) 512){

        char* data = byteListToArr();
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

    if(errMsg != null){

        incomingMessage.setErrMsg(errMsg);
        doneWithField();
        return true;
    }

    return  false;
}

bool BidiEncDec::bytesToUserName(char nextByte){

    string userName = incomingBytesToString(nextByte);

    if(userName != null){

        incomingMessage.setUserName(userName);
        doneWithField();
        return true;
    }

    return false;
}



string BidiEncDec::incomingBytesToString(char nextByte){

    if(nextByte == (byte) 0){

        char* fileNameBytes = byteListToArr();

        String string = new String(fileNameBytes);

        return string;
    }
    else{

        incomingBytes.add(nextByte);
        return null;
    }
}

char* BidiEncDec::byteListToArr(){

}

void BidiEncDec::doneWithMessage(){

    this.incomingBytes = null;
    this.incomingMessage = null;
    currentMessageFieldNumber = 0;
}

void BidiEncDec::doneWithField(){

    emptyIncomingBytes();
    currentMessageFieldNumber++;
}

void BidiEncDec::emptyIncomingBytes(){

    while(!incomingBytes.isEmpty()){

        incomingBytes.remove(0);
    }
}




short BidiEncDec::incomingBytesToShort(char nextByte){

    incomingBytes.add(nextByte);

    if(incomingBytes.size() == 2){

        char* shortBytes = {incomingBytes.get(0), incomingBytes.get(1)};
        short num = bytesToShort(shortBytes);

        return num;
    }
    return (short) -1;
}


char* BidiEncDec::shortToBytes(short num, char* byteArr){

    byteArr[0] = ((num >> 8) & 0xFF);
    byteArr[1] = (num & 0xFF);
}

short BidiEncDec::bytesToShort(char* byteArr){

    short result = (short)((byteArr[0] & 0xff) << 8);
    result += (short)(byteArr[1] & 0xff);
    return result;
}

void BidiEncDec::putInByteArray(char* toPut, char* container, int fromIndex){

}



BidiMessage BidiEncDec::decodeNextByte(char nextByte){

    bool done = false;

    if(incomingBytes == null){

        incomingBytes = new vector<>();
        incomingMessage = new BidiMessage();
        currentMessageFieldNumber = 0;
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
}



char* BidiEncDec::encode(BidiMessage message){

    char* encoded;

    short msgType = message.getOpcode();
    char* msgTypeBytes = shortToBytes(msgType);

    switch (msgType){

//            DELRQ, RRQ, WRQ
        case 1:
        case 2:
        case 8:{

            string fileName = message.getFileName();
            char* fileNameBytes = fileName.getBytes();

            char* aByte = {message.getaByte()};

            encoded = new byte[fileNameBytes.length + 3];

            putInByteArray(msgTypeBytes, encoded, 0);
            putInByteArray(fileNameBytes, encoded, 2);
            putInByteArray(aByte, encoded, encoded.length-1);

            break;
        }

//            DATA
        case 3: {

            short packetSize = message.getPacketSize();
            char* packetSizeBytes = shortToBytes(packetSize);

            short blockNumber = message.getBlockNumber();
            char* blockNumberBytes = shortToBytes(blockNumber);

            char* data = message.getData();

            encoded = new byte[data.length + 7];

            putInByteArray(msgTypeBytes, encoded, 0);
            putInByteArray(packetSizeBytes, encoded, 2);
            putInByteArray(blockNumberBytes, encoded, 4);
            putInByteArray(data, encoded, 6);

            break;
        }

//            ACK
        case 4: {

            short blockNumber = message.getBlockNumber();
            char* blockNumberBytes = shortToBytes(blockNumber);

            encoded = new byte[4];

            putInByteArray(msgTypeBytes, encoded, 0);
            putInByteArray(blockNumberBytes, encoded, 2);

            break;
        }

//            ERROR
        case 5: {

            short errorCode = message.getErrorCode();
            char* errorCodeBytes = shortToBytes(errorCode);

            string errMsg = message.getErrMsg();
            char* errMsgBytes = errMsg.getBytes();

            char* aByte = {message.getaByte()};

            encoded = new byte[errMsgBytes.length + 5];

            putInByteArray(msgTypeBytes, encoded, 0);
            putInByteArray(errorCodeBytes, encoded, 2);
            putInByteArray(errMsgBytes, encoded, 4);
            putInByteArray(aByte, encoded, encoded.length-1);

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
            char* userNameBytes = userName.getBytes();

            char* aByte = {message.getaByte()};

            encoded = new byte[userNameBytes.length + 3];

            putInByteArray(msgTypeBytes, encoded, 0);
            putInByteArray(userNameBytes, encoded, 2);
            putInByteArray(aByte, encoded, encoded.length-1);

            break;
        }

//            BCAST
        case 9: {

            char* deletedAdded = {message.getDeletedAdded()};

            string fileName = message.getFileName();
            char* fileNameBytes = fileName.getBytes();

            char* aByte = {message.getaByte()};

            encoded = new byte[fileNameBytes.length + 4];

            putInByteArray(msgTypeBytes, encoded, 0);
            putInByteArray(deletedAdded, encoded, 2);
            putInByteArray(fileNameBytes, encoded, 3);
            putInByteArray(aByte, encoded, encoded.length-1);

            break;
        }

        default:{
            encoded = new byte[0];
        }
    }

    return encoded;
}


BidiEncDec::~BidiEncDec() {}


