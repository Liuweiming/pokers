#pragma once
#include "PokerPlayer.h"
#include "Message.h"
#include "ClientSocket.h"
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
class MsgGenerator
{
private:
	ClientSocket mySocket;
	std::vector<Message *> msgs;

	std::vector<Message *> &getSeatMsg();
	std::vector<Message *> &getGameOverMsg();
	std::vector<Message *> &getblindMsg();
	Poker getPoker(std::istringstream &strs);
	std::vector<Message *> &getHoldMsg();
	Action getAction(std::istringstream &strs);
	std::vector<Message *> &getInquireMsg();
	std::vector<Message *> &getNotifyMsg();
	std::vector<Message *> &getFlopMsg();
	std::vector<Message *> &getTurnMsg();
	std::vector<Message *> &getRiverMsg();
	CardType getCardType(std::istringstream &strs);
	std::vector<Message *> &getShowDownMsg();
	std::vector<Message *> &getPotWinMsg();
public:
	MsgGenerator(const std::string &host, int port,
		const std::string &myAdrr, int myPort);
	~MsgGenerator();
	std::vector<Message *> &getMessage();
	void sendMessage(Message &msg);
};

