#include <fstream>
#include "../include/ClientProtocol.h"

//1
ClientProtocol::ClientProtocol():
waitingToLogin(false),
lastRqCode(-1),
dataBytesBuffer(),
receivingFileName(""),
previousReceivedBlock(-1),
lastSentBlockNum(0),
startReading(false),
sendingFileName(""),
uploadFileData(),
communicationCompleted(false),
readyToDisconnect(false)
{
    std::cout << "new protocol 333" << std::endl;
}

void ClientProtocol::process(BidiMessage& message, BidiMessage& reply) {

	short opcode = message.getOpcode();
    communicationCompleted = false;

    std::cout << "here1 LAST RQ?" << lastRqCode << std::endl;
    std::cout << "entered protocol process" << std::endl;
    switch(opcode) {

//        RRQ
        case 1: {

            reply = message;
            receivingFileName = message.getFileName();
            communicationCompleted = true;
            std::cout << "setting lastRqCode as: " << opcode << std::endl;

            setLastRqCode(opcode);
            std::cout << "set lastRqCode as: " << lastRqCode << std::endl;
            break;
        }

//        WRQ
        case 2: {

            reply = message;
            sendingFileName = message.getFileName();
            communicationCompleted = true;

            // read file
            std::ifstream fileReadStream(sendingFileName, std::ifstream::binary);

            // Find the length of the file
            fileReadStream.seekg(0, fileReadStream.end);
            std::streampos flength = fileReadStream.tellg();
            fileReadStream.seekg(0, fileReadStream.beg);

            std::cout << "4" << std::endl;


            // Create a vector to read it into
//            uploadFileData = std::vector<unsigned char>(flength);
            std::vector<unsigned char> uploadFileData1(flength);


            // Actually read data
            fileReadStream.read((char *)uploadFileData1.data(), flength);

            uploadFileData = uploadFileData1;

            std::cout << "456uploadFileData1.size(): " << uploadFileData1.size() << std::endl;
            std::cout << "123uploadFileData.size(): " << uploadFileData.size() << std::endl;

            fileReadStream.close();

            std::cout << "setting lastRqCode as: " << opcode << std::endl;

            setLastRqCode(opcode);
            std::cout << "set lastRqCode as: " << lastRqCode << std::endl;
            break;
        }
//        DATA
        case 3: {

//            if(lastRqCode == 2){
//
//                if(message.getPacketSize() < 512){
//
//                    lastRqCode = -1;
//                }
//                reply = message;
//                std::cout << "MY DATA PACKET" << std::endl;
//                std::cout << "reply.getOpcode(): " << reply.getOpcode() << std::endl;
//                break;
//            }

            std::cout << "I SOMEHOW ESCAPED!!!??? " << lastRqCode << std::endl;

//            TODO: deal with this reply on the server and check
//            if(previousReceivedBlock != message.getBlockNumber() + 1){
//
//                dataBytesBuffer.clear();
//                lastRqCode = -1;
//                communicationCompleted = true;
//                reply = BidiMessage::createAckMessage(0);
//                std::cout << "Received wrong data block from server" << std::endl;
//                break;
//            }
            std::cout << "packet size in protocol" << message.getPacketSize() << std::endl;
            addDataToBuffer(message);
            std::cout << "here2 LAST RQ?" << lastRqCode << std::endl;

            previousReceivedBlock = message.getBlockNumber();

            std::cout << "BLOCK# " << message.getBlockNumber() << std::endl;
            std::cout << "PSIZE " << message.getPacketSize() << message.getBlockNumber() << std::endl;


            std::cout << " data bytes buffer size" << dataBytesBuffer.size() << std::endl;
            if (message.getPacketSize() < 512) {

                std::cout << "here R LAST RQ?" << lastRqCode << std::endl;
                std::cout << "here? OPCODE" << opcode << std::endl;

                unsigned long dataSize = dataBytesBuffer.size();
                char receivedData[dataSize];

                switch (lastRqCode) {

//                    RRQ in data
                    case 1: {

                        std::cout << "are you here??" << std::endl;
                        for (unsigned long i = 0; i < dataSize; ++i) {

                            receivedData[i] = dataBytesBuffer.at(i);
                        }

                        if (dataSize != 0) {

                            std::cout << "or here??" << std::endl;
                            ofstream receivedFile(receivingFileName, ios::out | ios::binary);
                            receivedFile.write(receivedData, dataSize);
                            receivedFile.close();
                        } else {

                            std::cout << "write empty1" << std::endl;
                            ofstream receivedFile(receivingFileName, ios::out);
                            std::cout << "write empty2" << std::endl;
                            receivedFile.write(receivedData, dataSize);
                            std::cout << "write empty3" << std::endl;
                            receivedFile.close();
                            std::cout << "write empty4" << std::endl;
                        }

//                        DON'T REMOVE THIS COUT!!!!!!!!!!!!!!!!!!!!!!!!!!
                        std::cout << "RRQ " << receivingFileName << " complete" << std::endl;
                    }

//                    DIRQ in data
                    case 6: {

                        std::cout << "data size: " << dataSize << std::endl;


                        for (unsigned long i = 0; i < dataSize; ++i) {

                            if (dataBytesBuffer[i] == '\0') {
                                dataBytesBuffer[i] = '\n';
                            }

                            receivedData[i] = dataBytesBuffer.at(i);
                        }
                        std::cout << string(receivedData).substr(0, dataSize) << std::endl;

                    }

                    default: {

                    }
                }

                dataBytesBuffer.clear();

                std::cout << "BUFFERSIZE" << dataBytesBuffer.size() << std::endl;
                std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << std::endl;

                std::cout << "fileName: " << receivingFileName << std::endl;
                lastRqCode = -1;
                communicationCompleted = true;
            }

            reply = BidiMessage::createAckMessage(message.getBlockNumber());
            std::cout << "creating ACK from Data opcode:" << reply.getOpcode() << std::endl;
            break;
        }

//        ACK
		case 4: {

            std::cout << "ACK " << message.getBlockNumber() << std::endl;

            switch (lastRqCode){

//                RRQ in ACK
                case 1: {

                    //TODO: this field needs to be set to false in the first received data!!!
                    startReading = true;

                    break;
                }

//                WRQ in ACK
                case 2: {

                    std::cout << "WRQ in ACK 666" << std::endl;


//                    if(message.getBlockNumber() != lastSentBlockNum+1 && (message.getBlockNumber() != 0)) {
//
//                        std::cout << "1" << std::endl;
//
//                        sendingFileName = "";
//                        communicationCompleted = true;
//                        lastSentBlockNum = 0;
//                        fileReadStream.close();
//                    }
                    /*else*/ {

                        std::cout << "2" << std::endl;

                        int x = 512;  // chunk size


                        std::cout << "234uploadFileData.size(): " << uploadFileData.size() << std::endl;


                        if(uploadFileData.size() >= x) {
                            char data[x];
                            for (unsigned int i = 0; i < x; i++) {

                                std::cout << "uploadFileData.at(i): " << uploadFileData.at(0) << std::endl;


                                data[i] = uploadFileData.at(0);
                                std::cout << "after data[i]" << std::endl;

                                uploadFileData.erase(uploadFileData.begin());
                                std::cout << "after erase" << std::endl;

                            }

                            lastSentBlockNum++;
                            reply = BidiMessage::createDataMessage(x,lastSentBlockNum , data);
                        }
                        else{
                            int size = uploadFileData.size();
                            char data[size];
                            std::cout << "235uploadFileData.size(): " << uploadFileData.size() << std::endl;


                            for (unsigned int i = 0; i < uploadFileData.size(); i++) {

                                std::cout << "uploadFileData.at(i): " << uploadFileData.at(0) << std::endl;


                                data[i] = uploadFileData.at(0);
                                std::cout << "after data[i]" << std::endl;

                                uploadFileData.erase(uploadFileData.begin());
                                std::cout << "after erase" << std::endl;

                            }

                            lastSentBlockNum++;
                            reply = BidiMessage::createDataMessage(size,lastSentBlockNum , data);

                            std::cout << "WRQ " << sendingFileName << " complete" << std::endl;
                            lastRqCode = -1;
                            sendingFileName = "";
                            communicationCompleted = true;
                            lastSentBlockNum = 0;
                        }



                        std::cout << "lastsetBlockNum2 " << lastSentBlockNum << std::endl;
                        std::cout << "reply's blockNumber " << reply.getBlockNumber() << std::endl;
                        std::cout << "33reply.getOpcode(): " << reply.getOpcode() << std::endl;
                        std::cout << "33reply.getBytesLength(): " << reply.getBytesLength() << std::endl;



//
////                        for (unsigned int i = 0; i < uploadingDataLength - x + 1; i += x) {
////
////
////                            std::cout << "7" << std::endl;
////
////                            char newArray[x];
////
////                            for(int j = 0; j < x; j++) {
////
////                                std::cout << "8" << std::endl;
////
////                                newArray[j] = data[j];
////                            }
////
////                            reply = BidiMessage::createDataMessage(x, lastSentBlockNum++, newArray);
////                        }
//
//                        if (uploadingDataLength % x != 0) {


//                            char newArray[uploadingDataLength % x];

//                            for(int j = 0; j < x; j++) {
//                                std::cout << "10" << std::endl;
//
//                                newArray[j] = data[uploadingDataLength % x];
//                            }
//
//                            reply = BidiMessage::createDataMessage(uploadingDataLength % x, lastSentBlockNum++, newArray);
//

                        // Close the file explicitly, since we're finished with it


                    }

                    std::cout << "15" << std::endl;

                    break;
                }

//                DISC
                case 10: {

                    readyToDisconnect = true;
                    communicationCompleted = true;
                    lastRqCode = -1;

                    break;
                }

                default: {

                    std::cout << "16" << std::endl;

                    lastRqCode = -1;
                    receivingFileName = "";
                    communicationCompleted = true;

                    break;
                }
            }

            break;
        }

//        ERROR
        case 5: {

            std::cout << "Error " << message.getErrorCode() << std::endl;
            communicationCompleted = true;
            break;
        }

//        BCAST
        case 9: {

            reply = message;
            if(message.getFileName() == receivingFileName){

                communicationCompleted = false;
            }

            int broadReason = message.getDeletedAdded();

            if(broadReason != 0 && broadReason != 1) {

                //TODO: handle the case when the broadcast number is neither 0 nor 1
                std::cout << "Illegal broadcast" << std::endl;
            }
            else {
                std::cout << "BCAST " << broadReason << " " << message.getFileName() << std::endl;
            }

            break;
        }

//        DIRQ, DISQ
        case 6:
        case 10: {

            reply = message;
            communicationCompleted = true;
            std::cout << "setting lastRqCode as: " << opcode << std::endl;

            setLastRqCode(opcode);
            std::cout << "set lastRqCode as: " << lastRqCode << std::endl;
            break;
        }

        case 7: { //LOGRQ

            waitingToLogin = true;
            reply = message;
//            communicationCompleted = true;
            break;
        }


//        DELRQ
        case 8: {

            reply = message;
            receivingFileName = message.getFileName();
            communicationCompleted = true;
            std::cout << "setting lastRqCode as: " << opcode << std::endl;

            setLastRqCode(opcode);
            std::cout << "set lastRqCode as: " << lastRqCode << std::endl;
            break;
        }

        default:{

            std::cout << "Received unknown package from server" << std::endl;
            break;
        }

	}
}

void ClientProtocol::addDataToBuffer(BidiMessage message) {

    char data[message.getPacketSize()];
    message.copyData(data);
    int size = message.getPacketSize();

    for (int i = 0; i < size; ++i) {

        dataBytesBuffer.push_back(data[i]);
    }
}

void ClientProtocol::setLastRqCode(int lastRqCode) {
    this->lastRqCode = lastRqCode;
}

int ClientProtocol::getLastRqCode() const {
    return lastRqCode;
}

bool ClientProtocol::isComunicationCompleted() const {
    return communicationCompleted;
}

void ClientProtocol::setCommunicationCompleted(bool communicationCompleted) {
    ClientProtocol::communicationCompleted = communicationCompleted;
}

bool ClientProtocol::isStartReading() const {
    return startReading;
}

bool ClientProtocol::isReadyToDisconnect() const {
    return readyToDisconnect;
}
