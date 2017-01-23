#include "../include/connectionHandler.h"

using boost::asio::ip::tcp;
 
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
 
ConnectionHandler::ConnectionHandler(string host, short port, BidiEncDec& encDec, ClientProtocol& protocol):
host_(host), port_(port), io_service_(), socket_(io_service_), encDec(encDec), protocol(protocol){}
    
ConnectionHandler::~ConnectionHandler() {
    close();
}
 
bool ConnectionHandler::connect() {
    std::cout << "Starting connect to " 
        << host_ << ":" << port_ << std::endl;
    try {
        tcp::endpoint endpoint(boost::asio::ip::address::from_string(host_), port_); // the server endpoint
        boost::system::error_code error;
        socket_.connect(endpoint, error);
        if (error)
            throw boost::system::system_error(error);
    }
    catch (std::exception& e) {
        std::cerr << "Connection failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}
 
bool ConnectionHandler::getBytes(char bytes[], unsigned int bytesToRead) {
    size_t tmp = 0;
    boost::system::error_code error;
    try {
        while (!error && bytesToRead > tmp ) {
            std::cout << "before SOCKET BYTESTOREAD "<< bytesToRead << std::endl;
            std::cout << "before socket" << std::endl;
            tmp += socket_.read_some(boost::asio::buffer(bytes+tmp, bytesToRead-tmp), error);
            std::cout << "after SOCKET bytes[0] "<< (int)bytes[0] << std::endl;
            std::cout << "after SOCKET bytes[1] "<< (int)bytes[1] << std::endl;
            std::cout << "after socket tmp" << tmp << std::endl;
        }
        if(error)
            throw boost::system::system_error(error);
    } catch (std::exception& e) {
        std::cerr << "recv failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}
 
bool ConnectionHandler::sendBytes(const char bytes[], int bytesToWrite) {
    int tmp = 0;
    std::cout << "before try" << std::endl;
    boost::system::error_code error;
    try {
        std::cout << "before while error : "<< error << std::endl;
        std::cout << "before while bytesToWrite: "<< bytesToWrite << std::endl;
        while (!error && bytesToWrite > tmp ) {
            std::cout << "before while bytesToWrite in: "<< bytesToWrite << std::endl;
            std::cout << "before while tmp: "<< tmp << std::endl;
            std::cout << "Inside sendBytes while" << std::endl;
            tmp += socket_.write_some(boost::asio::buffer(bytes + tmp, bytesToWrite - tmp), error);
            std::cout << "before while bytesToWrite in a: "<< bytesToWrite << std::endl;
            std::cout << "before while tmp a: "<< tmp << std::endl;
        }
        if(error)
            throw boost::system::system_error(error);
    } catch (std::exception& e) {
        std::cerr << "recv failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}
 
bool ConnectionHandler::getLine(std::string& line) {

//    return getFrameAscii(line, '\n');
    return getFrameAscii(line, '\0');
}
 
bool ConnectionHandler::sendLine(std::string& line) {
    return sendFrameAscii(line, '\n');
}
 
bool ConnectionHandler::getFrameAscii(std::string& frame, char delimiter) {
    char ch;
    // Stop when we encounter the null character. 
    // Notice that the null character is not appended to the frame string.
    try {
        do{
            getBytes(&ch, 1);
            frame.append(1, ch);
        }while (delimiter != ch);
    } catch (std::exception& e) {
        std::cerr << "recv failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}
 
bool ConnectionHandler::sendFrameAscii(const std::string& frame, char delimiter) {
    bool result=sendBytes(frame.c_str(),frame.length());
    if(!result) return false;
    return sendBytes(&delimiter,1);
}
 
// Close down the connection properly.
void ConnectionHandler::close() {
    try{
        socket_.close();
    } catch (...) {
        std::cout << "closing failed: connection already closed" << std::endl;
    }
}

bool ConnectionHandler::getMessage(BidiMessage& message) {

    char ch[1];

    try {
        do{
            std::cout << "BEFORE getBytes" << std::endl;
            getBytes(ch, 1);
            std::cout << "BEFORE decoder" << std::endl;
            std::cout << "before decoder BYTE "<< ch[0] << std::endl;
            std::cout << ""<< std::endl;
            std::cout << "before decoder isComplete "<< message.isComplete() << std::endl;
            encDec.decodeNextByte(ch[0], message);
            std::cout << "after decoder isComplete "<< message.isComplete() << std::endl;
            std::cout << "after decoder opcodeXXXXXXXXXXXXX "<< message.getOpcode() << std::endl;
            std::cout << "after decoder blockNumberXXXXXXXXXXXXX "<< message.getBlockNumber() << std::endl;
        }while(!message.isComplete());
    } catch (std::exception& e) {
        std::cerr << "recv failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }

    return true;
}

bool ConnectionHandler::sendMessage(BidiMessage &message) {

    char encoded[message.getBytesLength()];
    encDec.encode(message, encoded);

    return sendBytes(encoded, message.getBytesLength());
}

bool ConnectionHandler::processMessage() {

    bool result;

    while(!protocol.isComunicationCompleted()) {

        BidiMessage answer = BidiMessage();
        BidiMessage reply = BidiMessage();

        result = getMessage(answer);

        if(!result) {
            return result;
        }

        protocol.process(answer, reply);

        if(reply.getOpcode() == -1){

            break;
        }

        sendMessage(reply);
    }

//    if(result){
//
//        std::cout << "**************Reply Opcode: " << answer.getOpcode() << std::endl << std::endl;
//        if(answer.getOpcode() == 3){
//
//            int ps = answer.getPacketSize();
//            char receivedData[ps];
//            answer.copyData(receivedData);
//            std::cout << "DATA: " << string(receivedData) << std::endl << std::endl;
//        }
//    }


    return result;









//            TODO: send answer to protocol for a response. What is under here isn't needed.

}
