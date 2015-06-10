#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "Pokers.h"
#include "Message.h"
enum Circle{ seat_in, hold_card, pre_flop, flop_card, pre_turn, turn_card, pre_river, river_card };
enum PlayersType{ loose_pass, loose_aggre, normal, tight_pass, tight_aggre};
class PokerPlayer
{
public:
	PID playerID;
	Seat seat;
	Money money;
	Jetton jetton, bet;
	Action lastAction;
	std::array<Poker, 7> playerCards;
	std::array<std::size_t, 9> handCardType;
	double winRate;
	ActionCounter foldC, raiseC, all_inC, checkC, callC,
		joinC, winC, showC, should_loseC, true_winC;
	ActionCounter F23BetC, NF23BetC, _3BetC, N3BetC,
		PFRC, CBetC, NCBetC, WTSDC, FBetC, NFBetC;
	double VPIP, PFR, AF, _3Bet, FBet, F23Bet, Cbet, WTSD;
	ActionCounter totalPlay;
	PlayersType type;
	bool isShow, isRaise;
	friend bool operator < (const PokerPlayer &lhs, const PokerPlayer &rhs);
	friend bool operator == (const PokerPlayer &lhs, const PokerPlayer &rhs);
	friend std::ostream &operator<<(std::ostream &os, const PokerPlayer &hs);
public:
	PokerPlayer();
	PokerPlayer(PID playerID);
	void getPlayerMsg(Message &msg);
	CardType getMyCardType(Circle cir);
	bool HoldCardIn(){
		Pokers myPokers(playerCards);
		return myPokers.HoldCardIn();
	}
	~PokerPlayer();
};

