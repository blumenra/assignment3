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
    
    ConnectionHandler connectionHandler(host, port);
    if (!connectionHandler.connect()) {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return 1;
    }

    BidiInputConverter converter;
    BidiEncDec encDec = BidiEncDec();

    //From here we will see the rest of the ehco client implementation:
    while (1) {
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        std::string line(buf);
//        unsigned long len=line.length();

        std::cout << "Before convert" << std::endl;

        BidiMessage message = converter.convertInput(line);

        std::cout << "After convert" << std::endl;

        if(message.isComplete()) {

            std::cout << "After complete message" << std::endl;

            std::cout << "message opcode: " << message.getOpcode() << std::endl;

//            std::cout << "ENCODED: " << encDec.encode(message) <<std::endl;
            std::cout << "bytesLength: " << message.getBytesLength() <<std::endl;
            char encoded[message.getBytesLength()];
            encDec.encode(message, encoded);
            bool result = connectionHandler.sendBytes(encoded, message.getBytesLength());
            std::cout << "after result" << std::endl;
            if(!result) {
                std::cout << "Disconnected. Exiting...\n" << std::endl;
                break;
            }

//            // connectionHandler.sendLine(line) appends '\n' to the message. Therefor we send len+1 bytes.
//            std::cout << "Sent " << len+1 << " bytes to server" << std::endl;
//            TODO: find out what is supposed to be printed out and implement cout.

            // We can use one of three options to read data from the server:
            // 1. Read a fixed number of characters
            // 2. Read a line (up to the newline character using the getline() buffered reader
            // 3. Read up to the null character
//            std::string answer;
            // Get back an answer: by using the expected number of bytes (len bytes + newline delimiter)
            // We could also use: connectionHandler.getline(answer) and then get the answer without the newline char at the end
//            if (!connectionHandler.getLine(answer)) {
//                std::cout << "Disconnected. Exiting...\n" << std::endl;
//                break;
//            }

            BidiMessage answer = BidiMessage();

            if(!connectionHandler.getMessage(answer, encDec)){
                std::cout << "Disconnected. Exiting...\n" << std::endl;
                break;
            }

//            TODO: send answer to protocol for a response. What is under here isn't needed.

//            len=answer.length();
            // A C string must end with a 0 char delimiter.  When we filled the answer buffer from the socket
            // we filled up to the \n char - we must make sure now that a 0 char is also present. So we truncate last character.
//            answer.resize(len-1);
            std::cout << "**************Reply Opcode: " << answer.getOpcode() << std::endl << std::endl;

//            if (answer == "bye") {
//                std::cout << "Exiting...\n" << std::endl;
//                break;
//            }
        }

        std::cout << "Not complete message" << std::endl;








    }
    return 0;
}