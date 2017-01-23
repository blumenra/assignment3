#include <stdlib.h>
#include "../include/connectionHandler.h"
#include "../include/bidiInputConverter.h"

/**
* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
*/
int main (int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " host port" << std::endl << std::endl;
        return -1;
    }
    std::string host = argv[1];
    short port = atoi(argv[2]);

    BidiInputConverter converter;
    BidiEncDec encDec = BidiEncDec();
    ClientProtocol protocol = ClientProtocol();
//
//    protocol.setLastRqCode(5);
//

    ConnectionHandler connectionHandler(host, port, encDec, protocol);
    if (!connectionHandler.connect()) {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return 1;
    }



    //From here we will see the rest of the ehco client implementation:
    while (1) {
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        std::string line(buf);

        BidiMessage messagePre = converter.convertInput(line);
        BidiMessage messagePost = BidiMessage();

        std::cout << "before client process lastrq" << protocol.getLastRqCode() << std::endl;
        protocol.process(messagePre, messagePost);
        std::cout << "after client process lastrq" << protocol.getLastRqCode() << std::endl;

        if(messagePost.isComplete()) {
//        if(messagePre.isComplete()) {
            std::cout << "1after client process lastrq" << protocol.getLastRqCode() << std::endl;

            if(!connectionHandler.sendMessage(messagePost)) {
//            if(!connectionHandler.sendMessage(messagePre)) {
                std::cout << "Disconnected. Exiting1...\n" << std::endl;
                break;
            }
            std::cout << "2after client process lastrq" << protocol.getLastRqCode() << std::endl;

            if(!connectionHandler.processMessage()){
                std::cout << "Disconnected. Exiting2...\n" << std::endl;
                break;
            }

        } else{

            std::cout << "Not complete message" << std::endl;
        }
    }

    return 0;
}