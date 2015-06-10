#pragma once
#include <array>
#include "MsgGenerator.h"
#include "Message.h"
#include "PokerPlayer.h"
#include "Players.h"
#include "Pokers.h"
#include "circleList.h"
#include "Probability.h"
#include "SeatPoker.h"
#include <fstream>
#include "Strategy.h"



class Me
{
public:
	typedef Players::ref_type ref_type;
	typedef CircleList<ref_type>::iterator iter_type;
	friend class Strategy;
private:
	MsgGenerator &msgGen;
	PokerPlayer *me;
	Players &allPlayers;
	std::array<int, 4> roundCount;
	Probability proba;
	PID playerID;
	std::string MyName;
	Circle circle;
	ActionMsg nextMsg;
	std::ofstream &ofs;
	double winRate;
	Strategy normalStrategy;
	Strategy2 secStrategy;
	LosAgressStrategy losAgs;
	LosPassStrategy losPas;
	TighAgresStrategy tiAgs;
	TighPassStrategy tiPas;
	Strategy *strategy;
	int TotalPlaysCount;
public:
	Me(MsgGenerator &msgGen, Players &players, PID playerI, std::ofstream &of);
	~Me();
	void rigist();
	void MeGetMsg(Message &msg);
	void act();
	void selecteStrategy();
	
};

