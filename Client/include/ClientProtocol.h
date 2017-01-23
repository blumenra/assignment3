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


    void addDataToBuffer(BidiMessage message);

public:
    ClientProtocol();

    int getLastRqCode() const;

    void setLastRqCode(int lastRqCode);

    BidiMessage process();

    BidiMessage process(BidiMessage message);
}; //class ConnectionHandler
 
#endif