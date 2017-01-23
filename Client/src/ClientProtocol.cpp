#include "../include/ClientProtocol.h"


ClientProtocol::ClientProtocol():
waitingToLogin(false),
upDownProcess(false),
dataBytesBuffer(),
downloadingFileName(""),
currentBlock(0),
comunicationCompleted(false)
{

}

void ClientProtocol::process(BidiMessage& message, BidiMessage& reply) {

	short opcode = message.getOpcode();
    comunicationCompleted = false;

	switch(opcode) {

        case 1: { //RRQ

        }

        case 2: { //WRQ

        }

		case 3: //DATA

//            addDataToBuffer();

			if(message.getPacketSize() < 512) {

			}
            else{

            }

            break;


		case 4: //ACK




            // Needs to be fixed
            comunicationCompleted = true;
            if(waitingToLogin) {
                waitingToLogin = false;

			}
			else if(upDownProcess) {

			}
            // Needs to be fixed




            break;

        case 5: { //ERROR

            std::cout << "++++++++++++++++++++++++++++++++++++++++++Error: " << message.getErrorCode() << std::endl;
            break;
        }


        case 9: { //BCAST

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
            comunicationCompleted = true;
            break;
        }
        case 6: //DIRQ
        case 8: //DELRQ
        case 10: { //DISC

            break;
        }

        default:

            std::cout << "Received unknown package from server" << std::endl;
            break;

	}
}

void ClientProtocol::addDataToBuffer(BidiMessage message) {

    char data[message.getPacketSize()];

//    dataBytesBuffer.push_back(message.copyData((char*)data));
}

bool ClientProtocol::isComunicationCompleted() const {
    return comunicationCompleted;
}
