CFLAGS = -g -Wall -Weffc++ -std=c++11
CC = g++
BOOST  = -lboost_locale -lboost_system


# All Targets
all: client

# Executable "hello" depends on the files hello.o and run.o.
client: bin/client.o  bin/connectionHandler.o  bin/BidiEncDec.o  bin/BidiInputConverter.o  bin/BidiMessage.o
	@echo 'Building target: client'
	@echo 'Invoking: C++ Linker'
	$(CC) -o bin/client  bin/client.o  bin/connectionHandler.o  bin/BidiEncDec.o  bin/BidiInputConverter.o  bin/BidiMessage.o $(BOOST)
	@echo 'Finished building target: client'
	@echo '********************************** '
	@echo ' '

# Depends on the source and header files
bin/client.o:   src/client.cpp
	$(CC) $(CFLAGS)  -c  -Iinclude  -o  bin/client.o   src/client.cpp

bin/connectionHandler.o:   src/connectionHandler.cpp   include/connectionHandler.h
	$(CC) $(CFLAGS)  -c  -Iinclude  -o  bin/connectionHandler.o   src/connectionHandler.cpp

bin/BidiEncDec.o:   src/BidiEncDec.cpp   include/BidiEncDec.h
	$(CC) $(CFLAGS)  -c  -Iinclude  -o  bin/BidiEncDec.o   src/BidiEncDec.cpp

bin/BidiInputConverter.o:   src/BidiInputConverter.cpp   include/BidiInputConverter.h
	$(CC) $(CFLAGS)  -c  -Iinclude  -o  bin/BidiInputConverter.o   src/BidiInputConverter.cpp

bin/BidiMessage.o:   src/BidiMessage.cpp   include/BidiMessage.h
	$(CC) $(CFLAGS)  -c  -Iinclude  -o  bin/BidiMessage.o   src/BidiMessage.cpp

#Clean the build directory
clean: 
	rm -rf bin/*

	