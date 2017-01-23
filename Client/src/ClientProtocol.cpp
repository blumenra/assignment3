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

            std::cout << "Error: " << message.getErrorCode() << std::endl;
            break;

        case 9: //BCAST

            int broadReason = message.getDeletedAdded();
            string occurrence = "";

            if(broadReason != 0 && broadReason != 1) {

                //TODO: handle the case when the broadcast number is neither 0 nor 1
                std::cout << "Illegal broadcast" << std::endl;
            }
            else {
                std::cout << "BCAST " << broadReason << " " << message.getFileName() << std::endl;
            }

            break;

        default:

            std::cerr << "Received unknown packet from server" << port << std::endl;
            break;

	}
}