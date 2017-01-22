#include "../include/ClientProtocol.h"


ClientProtocol::ClientProtocol():
waitingToLogin(false),
upDownProcess(false)
{

}

BidiMessage ClientProtocol::process(BidiMessage message) {

	short opcode = message.getOpcode();

	switch(opcode) {

		case 3: //DATA
			if() {

			}


		case 4: //ACK

			if(waitingToLogin) {
				waitingToLogin = false;
			}
			else if(upDownProcess) {

				
			}

		case 9: //BCAST

	}
}