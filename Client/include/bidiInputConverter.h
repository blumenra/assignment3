
#ifndef BIDI_CONVERTER_H
#define BIDI_CONVERTER_H

#include "BidiMessage.h"
#include <string>
#include <iostream>

#endif //CLIENT_BIDIMESSAGE_H

using namespace std;

class BidiInputConverter {

private:

    string input = "";
    string command = "";
    string arg = "";

    void split(const string& s, char delim,vector<string>& v);
    void splitToCommandAndArg(string input);

public:

    BidiInputConverter();
    BidiMessage convertInput(string input);

 };