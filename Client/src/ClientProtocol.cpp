#include "../include/ClientProtocol.h"


ClientProtocol::ClientProtocol():
waitingToLogin(false),
upDownProcess(false),
dataBytesBuffer(),
downloadingFileName(""),
currentBlock(0)
{

}

BidiMessage ClientProtocol::process(BidiMessage message) {

	short opcode = message.getOpcode();

	switch(opcode) {

		case 3: //DATA

            addDataToBuffer();

			if(message.getPacketSize() < 512) {

			}
            else{

            }

            break;


		case 4: //ACK

			if(waitingToLogin) {
				waitingToLogin = false;
			}
			else if(upDownProcess) {

				
			}

            break;

        case 5: //ERROR


            break;

		case 9: //BCAST


            break;

        default:

            std::cout << "Received unknown package from server" << std::endl;

            break;

	}
}

void ClientProtocol::addDataToBuffer(BidiMessage message) {

    char data[message.getPacketSize()];

    dataBytesBuffer.push_back(message.copyData((char*)data));
}
