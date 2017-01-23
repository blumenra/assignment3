#ifndef CLIENT_PROTOCOL_
#define CLIENT_PROTOCOL_
                                           
#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include "BidiMessage.h"


class ClientProtocol {
private:
	bool waitingToLogin;
	bool upDownProcess;
    vector<char> dataBytesBuffer;
    string downloadingFileName;
    int currentBlock;
	bool comunicationCompleted;

    void addDataToBuffer(BidiMessage message);

public:
    ClientProtocol();

	void process(BidiMessage& message, BidiMessage& reply);

	bool isComunicationCompleted() const;
}; //class ConnectionHandler
 
#endif