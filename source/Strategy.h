#pragma once
#include <fstream>
#include <ctime>
#include <random>
#include "Players.h"
#include "Probability.h"
#include "Message.h"
#include "SeatPoker.h"

class Me;
class Strategy
{
public:
	typedef PokerPlayer * ref_type;
protected:
	Players &players;
	std::array<int, 4> &roundCount;
	ref_type &me;
	Jetton &leastBet, &leastRaise;
	Jetton &shouldBet, &shouldRaise;
	Jetton &pot;
	SeatPoker seatPoker;
	double winRate;
	int myCardLevel;
	ref_type prePlayer;
	std::random_device rd;
	std::mt19937 gen;
	std::uniform_int_distribution<int> dis;
	int &totalplay;
	std::ofstream &ofs;
public:
	Strategy(Players &players, std::array<int, 4> &roundC,
		ref_type &me, std::ofstream &of, int &totalPlay);
	~Strategy();
	std::array<bool, 4> ifRaise;
	virtual ActionMsg inHold();
	virtual ActionMsg inHold2();
	virtual ActionMsg inFlop();
	virtual ActionMsg inTurn();
	virtual ActionMsg inRiver();
	ActionMsg GetStrategy(Circle cir, double winR);
	void getPrePlayer(const Players::iter_type &iter);
};

class Strategy2 : public Strategy
{
public:
	Strategy2(Players &players, std::array<int, 4> &roundC,
		ref_type &me, std::ofstream &of, int &totalPlay);
	~Strategy2(){}
	virtual ActionMsg inHold();
	virtual ActionMsg inFlop();
	virtual ActionMsg inTurn();
	virtual ActionMsg inRiver();
};

class LosPassStrategy : public Strategy
{
public:
	LosPassStrategy(Players &players, std::array<int, 4> &roundC,
		ref_type &me, std::ofstream &of, int &totalPlay);
	~LosPassStrategy(){}
	virtual ActionMsg inHold();
	virtual ActionMsg inFlop();
	virtual ActionMsg inTurn();
	virtual ActionMsg inRiver();

};

class LosAgressStrategy : public Strategy
{
public:
	LosAgressStrategy(Players &players, std::array<int, 4> &roundC,
		ref_type &me, std::ofstream &of, int &totalPlay);
	~LosAgressStrategy(){}
	virtual ActionMsg inHold();
	virtual ActionMsg inFlop();
	virtual ActionMsg inTurn();
	virtual ActionMsg inRiver();
};

class TighPassStrategy : public Strategy
{
public:
	TighPassStrategy(Players &players, std::array<int, 4> &roundC,
		ref_type &me, std::ofstream &of, int &totalPlay);
	~TighPassStrategy(){}
	virtual ActionMsg inHold();
	virtual ActionMsg inFlop();
	virtual ActionMsg inTurn();
	virtual ActionMsg inRiver();
};

class TighAgresStrategy : public Strategy
{
public:
	TighAgresStrategy(Players &players, std::array<int, 4> &roundC,
		ref_type &me, std::ofstream &of, int &totalPlay);
	~TighAgresStrategy(){}
	virtual ActionMsg inHold();
	virtual ActionMsg inFlop();
	virtual ActionMsg inTurn();
	virtual ActionMsg inRiver();
};