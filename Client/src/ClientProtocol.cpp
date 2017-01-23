#include "../include/ClientProtocol.h"


ClientProtocol::ClientProtocol():
waitingToLogin(false),
upDownProcess(false),
incomingDataBytes(),
downloadingFileName(""),
currentBlock(0)
{

}

BidiMessage ClientProtocol::process(BidiMessage message) {

	short opcode = message.getOpcode();

	switch(opcode) {

		case 3: //DATA
//			if() {
//
//			}

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

            std::cerr << "Received unknown packet from server" << port << std::endl;
            break;

	}
}