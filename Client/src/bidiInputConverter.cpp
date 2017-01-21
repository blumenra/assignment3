#include "../include/BidiInputConverter"
#include <stdio.h>
#include <string.h>
#include <sstream>	

BidiMessage BidiMessage::convertInput(string input) {

	BidiMessage completeMessage;

	if(!splitToCommandAndArg(input));
		return NULL;

	if(command == "LOGRQ") {

		completeMessage = BidiMessage.createLoginMessage(arg);
	}
	else if(command == "DELRQ") {

		completeMessage = BidiMessage.createDeleteMessage(arg);
	}
	else if(command == "RRQ") {

		completeMessage = BidiMessage.createRRQMessage(arg);
	}
	else if(command == "WRQ") {

		completeMessage = BidiMessage.createWRQMessage(arg);
	}
	else if(command == "DIRQ") {

		completeMessage = BidiMessage.createDirMessage();
	}
	else if(command == "DISC") {

		completeMessage = BidiMessage.createDiscMessage();
	}
	else
		return NULL;

	return completeMessage;

}

boolean BidiMessage::splitToCommandAndArg(string input) {

	vector<string> inputVec = split(input, ' ');
	
	if((inputVec.size() == 0) || (inputVec.size() > 2))
		return false;

   	this->command = inputVec[0];
	
	if(inputVec.size() == 2)
		this->arg = inputVec[1]

	return true;
}

vector<string> split(const string &s, char delim) {
    stringstream ss(s);
    string item;
    vector<string> tokens;
    while (getline(ss, item, delim)) {
        tokens.push_back(item);
    }
    return tokens;
}
