#ifndef CLIENT_PROTOCOL_
#define CLIENT_PROTOCOL_
                                           
#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include "BidiMessage.h"


class ClientProtocol {
private:
	bool waitingToLogin;
	int lastRqCode;
    vector<char> dataBytesBuffer;
    string downloadingFileName;
    int currentBlock;
	bool comunicationCompleted;

    void addDataToBuffer(BidiMessage message);

public:
    ClientProtocol();

    int getLastRqCode() const;

    void setLastRqCode(int lastRqCode);

    void process(BidiMessage& message, BidiMessage& reply);

	bool isComunicationCompleted() const;
}; //class ConnectionHandler
 
#endif