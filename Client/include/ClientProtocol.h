#ifndef CLIENT_PROTOCOL_
#define CLIENT_PROTOCOL_
                                           
#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include "BidiMessage.h"

//1
class ClientProtocol {
private:
	bool waitingToLogin;
	int lastRqCode;
    vector<char> dataBytesBuffer;
    string currentFileName;
    int currentBlock;
	bool communicationCompleted;

    void addDataToBuffer(BidiMessage message);

public:
    ClientProtocol();

    int getLastRqCode() const;

    void setLastRqCode(int lastRqCode);

    void setCommunicationCompleted(bool communicationCompleted);

    void process(BidiMessage& message, BidiMessage& reply);

	bool isComunicationCompleted() const;
}; //class ConnectionHandler
 
#endif