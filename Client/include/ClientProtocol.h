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
    
 
public:
    
    ClientProtocol();
    BidiMessage process();


}; //class ConnectionHandler
 
#endif