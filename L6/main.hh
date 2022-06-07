#include <iostream>

void printError(std::string errorType, std::string errorMsg);
bool checkArguments(int c, char* v[], std::string &arg1, std::string &arg2, size_t &arg3);
void encodeTGA(std::string inName, std::string outName, size_t qBits);
void decodeTGA(std::string inName, std::string outName);
void compareImages(std::string filename1, std::string filename2);
