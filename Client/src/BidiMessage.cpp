
#include "../include/BidiMessage.h"


static BidiMessage BidiMessage::createDataMessage(int packetSize, int blockNum, char* fileData){

    return BidiMessage((short) 3, (short) packetSize, (short) blockNum, fileData);
}

static BidiMessage BidiMessage::createBcastMessage(int event, string fileName){

    return BidiMessage((short) 9, (char) event, fileName, (char) 0);
}

static BidiMessage BidiMessage::createAckMessage(int ackNum){

    return BidiMessage((short) 4, (short) ackNum);
}

static BidiMessage BidiMessage::createErrorMessage(int errNum, string errMsg){

    return BidiMessage((short) 7, (short) errNum, errMsg, (char) 0);
}

static BidiMessage BidiMessage::createLoginMessage(string userName) {

    return BidiMessage((short) 7, userName, (char) 0);
}


static BidiMessage BidiMessage::createDeleteMessage(string fileName) {

    return BidiMessage((short) 8, fileName, (char) 0);
}

static BidiMessage BidiMessage::createRRQMessage(string fileName) {

    return BidiMessage((short) 1, fileName, (char) 0);
}

static BidiMessage BidiMessage::createWRQMessage(string fileName) {

    return BidiMessage((short) 1, fileName, (char) 0);
}

static BidiMessage BidiMessage::createDirMessage() {

    return BidiMessage((short) 6);
}

static BidiMessage BidiMessage::createDiscMessage() {

    return BidiMessage((short) 10);
}


//empty
BidiMessage::BidiMessage(){

    this->complete = false;
}

//copy
BidiMessage::BidiMessage(BidiMessage original){

    this->opcode = original.getOpcode();
    this->complete = true;
    switch (opcode){
//            LOGRQ
        case 7: {
            this->userName = original.getUserName();
            this->aByte = original.getaByte();
            break;
        }

//            DELRQ,RRQ,WRQ
        case 1:
        case 2:
        case 8: {
            this->fileName = original.getFileName();
            this->aByte = original.getaByte();
            break;
        }

//            DATA
        case 3: {

            this->packetSize = original.getPacketSize();
            this->blockNumber = original.getBlockNumber();
            this->data = original.getData();
            break;
        }

//            ACK
        case 4: {
            this->blockNumber = original.getBlockNumber();
            break;
        }

//            BCAST
        case 9: {

            this->deletedAdded = original.getDeletedAdded();
            this->fileName = original.getFileName();
            this->aByte = original.getaByte();
            break;
        }

//            ERROR
        case 5: {

            this->errorCode = original.getErrorCode();
            this->errMsg = original.getErrMsg();
            this->aByte  = original.getaByte();
            break;
        }

        default:
            break;
    }
}


BidiMessage::BidiMessage(short opcode, string aString, char aByte){

    this->opcode = opcode;
    this->complete = true;

    switch (opcode){

        case 7: {
            this->userName = aString;
            break;
        }

        case 1:
        case 2:
        case 8: {
            this->fileName = aString;
            break;
        }

        default:
            break;
    }
    this->fileName = aString;
    this->aByte = aByte;
}

BidiMessage::BidiMessage(short opcode){

    this->opcode = opcode;
    this->complete = true;

}

BidiMessage::BidiMessage(short opcode, short packetSize, short blockNumber, char* data){

    this->opcode = opcode;
    this->complete = true;
    this->packetSize = packetSize;
    this->blockNumber = blockNumber;
    this->data = data;
}

BidiMessage::BidiMessage(short opcode, short blockNumber){

    this->opcode = opcode;
    this->complete = true;
    this->blockNumber = blockNumber;
}

BidiMessage::BidiMessage(short opcode, char deletedAdded, string fileName, char aByte){

    this->opcode = opcode;
    this->complete = true;
    this->deletedAdded = deletedAdded;
    this->fileName = fileName;
    this->aByte = aByte;
}

BidiMessage::BidiMessage(short opcode, short errorCode, string errMsg, char aByte){

    this->opcode = opcode;
    this->complete = true;
    this->errorCode = errorCode;
    this->errMsg = errMsg;
    this->aByte = aByte;
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

    char* dataCopy = (char*) "";

    for(int i=0; i<packetSize; i++){

        dataCopy[i] = this->data[i];
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

bool BidiMessage::isComplete() const {
    return complete;
}


//setters

void BidiMessage::setOpcode(short opcode){

    this->opcode = opcode;
}

void BidiMessage::setFileName(string fileName){

    this->fileName = fileName;
}

void BidiMessage::setUserName(string userName){

    this->userName = userName;
}

void BidiMessage::setPacketSize(short packetSize){

    this->packetSize = packetSize;
}

void BidiMessage::setBlockNumber(short blockNumber){

    this->blockNumber = blockNumber;
}

void BidiMessage::setData(char* data){

    this->data = data;
}

void BidiMessage::setDeletedAdded(char deletedAdded){

    this->deletedAdded = deletedAdded;
}

void BidiMessage::setErrorCode(short errorCode){

    this->errorCode = errorCode;
}

void BidiMessage::setErrMsg(string errMsg){

    this->errMsg = errMsg;
}

void BidiMessage::setaByte(char aByte){

    this->aByte = aByte;
}

void BidiMessage::setComplete(bool complete) {
    BidiMessage::complete = complete;
}

BidiMessage::~BidiMessage() {}



