#include "../include/ClientProtocol.h"

//1
ClientProtocol::ClientProtocol():
waitingToLogin(false),
lastRqCode(-1),
dataBytesBuffer(),
currentFileName(""),
currentBlock(0),
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

//        DATA
        case 3: {

            std::cout << "packet size in protocol" << message.getPacketSize() << std::endl;
            addDataToBuffer(message);
            std::cout << "here2 LAST RQ?" << lastRqCode << std::endl;

            std::cout << " data bytes buffer size" << dataBytesBuffer.size() << std::endl;
            if (message.getPacketSize() < 512) {

                std::cout << "here R LAST RQ?" << lastRqCode << std::endl;
                std::cout << "here? OPCODE" << opcode << std::endl;
                switch(lastRqCode) {

//                    RRQ in data
                    case 1: {

                    }

//                    DIRQ in data
                    case 6: {

                        unsigned long dataSize = dataBytesBuffer.size();
                        char receivedData[dataSize];
                        std::cout << "data size: " << dataSize << std::endl;


                        for (unsigned long i = 0; i < dataSize; ++i) {

                            if(dataBytesBuffer[i] == '\0'){
                                dataBytesBuffer[i] = '\n';
                            }

                            receivedData[i] = dataBytesBuffer.at(i);
                        }
                        std::cout << string(receivedData).substr(0, dataSize) << std::endl;
                        dataBytesBuffer.clear();
                        std::cout << "BUFFERSIZE" << dataBytesBuffer.size() << std::endl;
                        std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << std::endl;
                    }

                    default: {

                    }
                }
                lastRqCode = -1;
                communicationCompleted = true;
            } else {

            }

            break;
        }

//        ACK
		case 4: {

            std::cout << "ACK " << message.getBlockNumber() << std::endl;

            switch (lastRqCode){

//                RRQ in ACK
                case 1: {


                }

//                WRQ in ACK
                case 2: {

                }

                default: {

                    lastRqCode = -1;
                    currentFileName = "";
                    communicationCompleted = true;
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
            if(message.getFileName() == currentFileName){

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
            currentFileName = message.getFileName();
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
