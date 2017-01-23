#include "../include/ClientProtocol.h"


ClientProtocol::ClientProtocol():
waitingToLogin(false),
lastRqCode(-1),
dataBytesBuffer(),
downloadingFileName(""),
currentBlock(0),
communicationCompleted(false)
{

}

void ClientProtocol::process(BidiMessage& message, BidiMessage& reply) {

	short opcode = message.getOpcode();
    communicationCompleted = false;

    std::cout << "entered protocol process" << std::endl;
	switch(opcode) {

//        DATA
		case 3: {

            std::cout << "packet size in protocol" << message.getPacketSize() << std::endl;
            addDataToBuffer(message);

            std::cout << " data bytes buffer size" << dataBytesBuffer.size() << std::endl;
            if (message.getPacketSize() < 512) {

                std::cout << "here LAST RQ?" << lastRqCode << std::endl;
                std::cout << "here? OPCODE" << opcode << std::endl;
                switch(lastRqCode) {

//                    RRQ
                    case 1: {

                    }

//                    DIRQ
                    case 6: {

                        std::cout << "-1" << std::endl;
                        unsigned long dataSize = dataBytesBuffer.size();
                        char receivedData[dataSize];

                        std::cout << "0" << std::endl;
                        for (unsigned long i = 0; i < dataSize; ++i) {

                            std::cout << "1" << std::endl;
                            if(dataBytesBuffer[i] == '\0'){
                                std::cout << "2" << std::endl;
                                dataBytesBuffer[i] = '\n';
                                std::cout << "3" << std::endl;
                            }

                            std::cout << "4" << std::endl;
                            receivedData[i] = dataBytesBuffer.at(i);
                        }
                        std::cout << string(receivedData) << std::endl << std::endl;
                        std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << std::endl;
                    }

                    default: {

                    }
                }

                communicationCompleted = true;
            } else {

            }

            break;
        }

//        ACK
		case 4: {




            // Needs to be fixed
            communicationCompleted = true;
            if(waitingToLogin) {
                waitingToLogin = false;

			}
//			else if() {
//
//			}
            // Needs to be fixed




            break;
        }

//        ERROR
        case 5: {

            std::cout << "++++++++++++++++++++++++++++++++++++++++++Error: " << message.getErrorCode() << std::endl;
            break;
        }

//        BCAST
        case 9: {

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
            communicationCompleted = true;
            break;
        }

//        DIRQ, DELRQ, DISQ
        case 6:
        case 8:
        case 10: {

            reply = message;
            communicationCompleted = true;
            lastRqCode = opcode;
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
    int size = message.getPacketSize();

    for (int i = 0; i < size; ++i) {

        dataBytesBuffer.push_back(data[i]);
    }
}

void ClientProtocol::setLastRqCode(int lastRqCode) {
    ClientProtocol::lastRqCode = lastRqCode;
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
