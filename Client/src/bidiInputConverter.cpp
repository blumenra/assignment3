#include "../include/bidiInputConverter.h"
#include <stdio.h>
#include <string.h>
#include <sstream>	

//1
BidiMessage BidiInputConverter::convertInput(string input) {



	BidiMessage completeMessage;

	if(!splitToCommandAndArg(input))
		completeMessage = BidiMessage();

	if(command == "LOGRQ") {

        if(arg == "") {
            std::cout << "Empty login name" << std::endl;
            completeMessage = BidiMessage();
        }
        else {

            completeMessage = BidiMessage::createLoginMessage(arg);
        }
	}
	else if(command == "DELRQ") {

		completeMessage = BidiMessage::createDeleteMessage(arg);
	}
	else if(command == "RRQ") {

		completeMessage = BidiMessage::createRRQMessage(arg);
	}
	else if(command == "WRQ") {

		completeMessage = BidiMessage::createWRQMessage(arg);
	}
	else if(command == "DIRQ") {
		std::cout << "Inside DIRQ" << std::endl;

		completeMessage = BidiMessage::createDirMessage();
	}
	else if(command == "DISC") {

		completeMessage = BidiMessage::createDiscMessage();
	}
	else
		completeMessage = BidiMessage();

	return completeMessage;

}

bool BidiInputConverter::splitToCommandAndArg(string input) {

	std::cout << "1" << std::endl;


	vector<string> inputVec = split(input, ' ');

	std::cout << "inputVec size: " << inputVec.size() << std::endl;

	if((inputVec.size() == 0) || (inputVec.size() > 2))
		return false;

	std::cout << "8" << std::endl;

	this->command = inputVec[0];

	std::cout << "9" << std::endl;

	if(inputVec.size() == 2)
		this->arg = inputVec[1];

	return true;
}

vector<string> BidiInputConverter::split(const string &s, char delim) {
	std::cout << "2" << std::endl;

	stringstream ss(s);
	std::cout << "3" << std::endl;

    string item;
	std::cout << "4" << std::endl;

    vector<string> tokens;
	std::cout << "5" << std::endl;

    while (getline(ss, item, delim)) {
		std::cout << "6" << std::endl;

		tokens.push_back(item);
    }
    return tokens;
}
