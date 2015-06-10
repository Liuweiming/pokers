#include "stdafx.h"
#include <iostream>
#include "MainController.h"
using namespace std;
int main(int argc, char* argv[]){
	if (argc < 6)
		return false;
	string severAdrr(argv[1]);
	int severPort = atoi(argv[2]);
	string myAdrr (argv[3]);
	int myPort = atoi(argv[4]);
	string playerID = argv[5];
	MainController mainc(severAdrr, severPort, myAdrr, myPort, stoi(playerID));
	mainc.regist();
	mainc.runner();
	return 0;
}