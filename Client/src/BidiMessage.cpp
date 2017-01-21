
#include "BidiMessage"



BidiMessage::BidiMessage createDataMessage(int packetSize, int blockNum, char* fileData){

    return new BidiMessage((short) 3, (short) packetSize, (short) blockNUM, fileData);
}

BidiMessage::BidiMessage createBcastMessage(int event, string fileName){

    return new BidiMessage((short) 9, (char) event, fileName, (char) 0);
}

BidiMessage::BidiMessage createAckMessage(int ackNum){

    return new BidiMessage((short) 4, (short) ackNum);
}

BidiMessage::BidiMessage createErrorMessage(int errNum, string errMsg){

    return new BidiMessage((short) 7, (short) errNum, errMsg, (char) 0);
}

//empty
BidiMessage::BidiMessage();

//copy
BidiMessage::BidiMessage(BidiMessage original){

    this.opcode = original.getOpcode();
    switch (opcode){
//            LOGRQ
        case 7: {
            this.userName = original.getUserName();
            this.aByte = original.getaByte();
            break;
        }

//            DELRQ,RRQ,WRQ
        case 1:
        case 2:
        case 8: {
            this.fileName = original.getFileName();
            this.aByte = original.getaByte();
            break;
        }

//            DATA
        case 3: {

            this.packetSize = original.getPacketSize();
            this.blockNumber = original.getBlockNumber();
            this.data = original.getData();
            break;
        }

//            ACK
        case 4: {
            this.blockNumber = original.getBlockNumber();
            break;
        }

//            BCAST
        case 9: {

            this.deletedAdded = original.getDeletedAdded();
            this.fileName = original.getFileName();
            this.aByte = original.getaByte();
            break;
        }

//            ERROR
        case 5: {

            this.errorCode = original.getErrorCode();
            this.errMsg = original.getErrMsg();
            this.aByte  = original.getaByte();
            break;
        }
    }
}


BidiMessage::BidiMessage(short opcode, string aString, char aByte){

    this.opcode = opcode;
    switch (opcode){

        case 7: {
            this.userName = aString;
            break;
        }

        case 1:
        case 2:
        case 8: {
            this.fileName = aString;
            break;
        }


    }
    this.fileName = aString;
    this.aByte = aByte;
}

BidiMessage::BidiMessage(short opcode){

    this.opcode = opcode;
}

BidiMessage::BidiMessage(short opcode, short packetSize, short blockNumber, char* data){

    this.opcode = opcode;
    this.packetSize = packetSize;
    this.blockNumber = blockNumber;
    this.data = data;
}

BidiMessage::BidiMessage(short opcode, short blockNumber){

    this.opcode = opcode;
    this.blockNumber = blockNumber;
}

BidiMessage::BidiMessage(short opcode, char deletedAdded, string fileName, char aByte){

    this.opcode = opcode;
    this.deletedAdded = deletedAdded;
    this.fileName = fileName;
    this.aByte = aByte;
}

BidiMessage::BidiMessage(short opcode, short errorCode, string errMsg, char aByte){

    this.opcode = opcode;
    this.errorCode = errorCode;
    this.errMsg = errMsg;
    this.aByte = aByte;
}


//getters

short BidiMessage::getOpcode(){

    return opcode;
}

string BidiMessage::getFileName(){

    return fileName + "";
}

char BidiMessage::getaByte(){

    return aByte;
}

string BidiMessage::getUserName(){

    return userName + "";
}

short BidiMessage::getPacketSize(){

    return packetSize;
}

short BidiMessage::getBlockNumber(){

    return blockNumber;
}

char*  BidiMessage::getData(){

    char* dataCopy;

    for(int i=0; i<packetSize-1; i++){

        dataCopy[i] = this.data[i];
    }

    return dataCopy;
}

char BidiMessage::getDeletedAdded(){

    return deletedAdded;
}

short BidiMessage::getErrorCode(){

    return errorCode;
}

string BidiMessage::getErrMsg(){

    return errMsg + "";
}


//setters

void BidiMessage::setOpcode(short opcode){

    this.opcode = opcode;
}

void BidiMessage::setFileName(string fileName){

    this.fileName = fileName;
}

void BidiMessage::setUserName(string userName){

    this.userName = userName;
}

void BidiMessage::setPacketSize(short packetSize){

    this.packetSize = packetSize;
}

void BidiMessage::setBlockNumber(short blockNumber){

    this.blockNumber = blockNumber;
}

void BidiMessage::setData(char* data){

    this.data = data;
}

void BidiMessage::setDeletedAdded(char deletedAdded){

    this.deletedAdded = deletedAdded;
}

void BidiMessage::setErrorCode(short errorCode){

    this.errorCode = errorCode;
}

void BidiMessage::setErrMsg(string errMsg){

    this.errMsg = errMsg;
}

void BidiMessage::setaByte(char aByte){

    this.aByte = aByte;
}


BidiMessage::~BidiMessage() {}