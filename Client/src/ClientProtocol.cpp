#include <fstream>
#include "../include/ClientProtocol.h"

//1
ClientProtocol::ClientProtocol():
waitingToLogin(false),
lastRqCode(-1),
dataBytesBuffer(),
receivingFileName(""),
previousReceivedBlock(0),
lastSentBlockNum(0),
startReading(false),
sendingFileName(""),
fileReadStream(NULL),
communicationCompleted(false)
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
//            TODO: change to sendingFileName
            receivingFileName = message.getFileName();
            communicationCompleted = true;
            std::cout << "setting lastRqCode as: " << opcode << std::endl;

            setLastRqCode(opcode);
            std::cout << "set lastRqCode as: " << lastRqCode << std::endl;
            break;
        }
//        DATA
        case 3: {

//            TODO: deal with this reply on the server and check
            if(previousReceivedBlock == message.getBlockNumber() + 1){

                dataBytesBuffer.clear();
                lastRqCode = -1;
                communicationCompleted = true;
                reply = BidiMessage::createAckMessage(0);
                std::cout << "Received wrong data block from server" << std::endl;
                break;
            }
            std::cout << "packet size in protocol" << message.getPacketSize() << std::endl;
            addDataToBuffer(message);
            std::cout << "here2 LAST RQ?" << lastRqCode << std::endl;

            previousReceivedBlock = message.getBlockNumber();

            std::cout << " data bytes buffer size" << dataBytesBuffer.size() << std::endl;
            if (message.getPacketSize() < 512) {

                std::cout << "here R LAST RQ?" << lastRqCode << std::endl;
                std::cout << "here? OPCODE" << opcode << std::endl;

                unsigned long dataSize = dataBytesBuffer.size();
                char receivedData[dataSize];

                switch(lastRqCode) {

//                    RRQ in data
                    case 1: {

                        for (unsigned long i = 0; i < dataSize; ++i) {

                            receivedData[i] = dataBytesBuffer.at(i);
                        }

                        ofstream receivedFile (receivingFileName, ios::out | ios::binary);
                        receivedFile.write(receivedData, dataSize);
                        receivedFile.close();
                    }

//                    DIRQ in data
                    case 6: {

                        std::cout << "data size: " << dataSize << std::endl;


                        for (unsigned long i = 0; i < dataSize; ++i) {

                            if(dataBytesBuffer[i] == '\0'){
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

                    if(message.getBlockNumber() != lastSentBlockNum+1) {

                        lastRqCode = -1;
                        sendingFileName = "";
                        communicationCompleted = true;
                        lastSentBlockNum = 0;
                        fileReadStream.close();
                    }
                    else {

                        fileReadStream = std::ifstream(sendingFileName, std::ifstream::binary);

                        // Find the length of the file
                        fileReadStream.seekg(0, fileReadStream.end);
                        std::streampos flength = fileReadStream.tellg();
                        fileReadStream.seekg(0, fileReadStream.beg);

                        // Create a vector to read it into
                        std::vector<unsigned char> bytes(flength);

                        // Actually read data
                        fileReadStream.read((char *)bytes.data(), flength);

                        unsigned int length = (unsigned int) flength;

                        char data[length];

                        for (unsigned int i = 0; i < length; ++i) {

                            data[i] = bytes.at(i);
                        }


                        int x = 512;  // chunk size

                        for (unsigned int i = 0; i < length - x + 1; i += x) {

                            char newArray[x];

                            for(int j = 0; j < x; j++) {
                                newArray[j] = data[j];
                            }

                            reply = BidiMessage::createDataMessage(x, lastSentBlockNum++, newArray);
                        }

                        if (length % x != 0) {

                            char newArray[length % x];

                            for(int j = 0; j < x; j++) {
                                newArray[j] = data[length % x];
                            }

                            reply = BidiMessage::createDataMessage(length % x, lastSentBlockNum++, newArray);


                            // Close the file explicitly, since we're finished with it
                            fileReadStream.close();

                            lastRqCode = -1;
                            sendingFileName = "";
                            communicationCompleted = true;
                            lastSentBlockNum = 0;
                        }
                    }

                    break;
                }

                default: {

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
