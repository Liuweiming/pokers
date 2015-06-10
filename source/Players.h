#pragma once
#include <map>
#include <array>
#include "circleList.h"
#include <vector>
#include <algorithm>
#include "Message.h"
#include "PokerPlayer.h"
#include <fstream>


//enum int{ at_head, at_middle, at_tail, at_button };
class MainController;
class Players
{
public:
	typedef PokerPlayer * ref_type;
	typedef CircleList<ref_type>::iterator iter_type;
	friend class MainController;
	Jetton leastBet, leastRaise;
	Jetton shouldBet, shouldRaise;
	Jetton pot;
	std::ofstream &ofs;
	
private:
	std::map<PID, PokerPlayer> players;
	void iniPlayers(std::vector<Message *> &msgs);
	bool playerInSeat(ref_type player);
	bool isJion, isUpdate, canJion, shouldDe, alwaysDe,
		notAlways, hasNotify, isNotify;
	PID meID;
	ref_type raisePlayer, lastRaisePlayer, me;
	int totalSeatIn;
	int roundC;
public:
	CircleList<ref_type> playerInTable;
	std::array<Jetton, 9> playersBet;
	iter_type myiter;
	iter_type maxiter, miniter;
	int playerAction[8][8];
	int playersRaiseC;
	int playersAllC;
	Circle circle;
	int playerRaiseBefor(int myseat);
	int playerRaiseBefor();
	int playerAllIn(int myseat);
	int playerAllIn();
	ref_type getRaisePlayer();
	bool playerLastRaise[8];
	Players(std::ofstream &of, PID meID);
	~Players();
	int mySeat();
	void getPlayersMsg(std::vector<Message *> &msgs);
	ref_type getMe(PID playerID);
	std::size_t getPlayersNum();
	void getBetAndRaise();
	PlayersType getPlayersType(iter_type &ret);
	bool mySituation();
	Jetton maxJetton();
	Jetton minJetton();
};

