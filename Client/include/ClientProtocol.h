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
    
 
public:
    
    ClientProtocol();
    BidiMessage process();


    BidiMessage process(BidiMessage message);
}; //class ConnectionHandler
 
#endif