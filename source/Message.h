#pragma once
#include <string>
#include "Pokers.h"

typedef  unsigned PID;
typedef int Money;
typedef int Jetton;
typedef int ActionCounter;

enum Seat{ small_blind, big_blind, second_p, third_p, fourth_p, fifth_p, sixth_p, button };

enum Action{ attend, blind, check, call, raise, all_in, fold, lose };

enum MessageType{
	reg_msg, seat_msg, game_over_msg, blind_msg, hold_cards_msg,
	inquire_msg, notify_msg, total_pot_msg, action_msg, flop_msg, turn_msg, river_msg,
	show_down_msg, pot_win_msg, error_msg
};

class PokerPlayer;
class Message;
std::ostream &operator << (std::ostream &os, const Message &msg);
class Message
{
public:
	MessageType msg_type;
	PID playerID;
	friend std::ostream &operator << (std::ostream &os, const Message &msg);
	Message(MessageType msg_t, PID playerI);
	virtual void getPlayerMsg(PokerPlayer &player);
	~Message();
};



class ErrorMsg : public Message{
public:
	std::string error;
	ErrorMsg();
	ErrorMsg(const std::string &err);
};

class RegMsg :public Message{
public:
	std::string playerName;
	RegMsg(PID playerI, const std::string &playerN);
};

class SeatMsg : public Message{
public:
	Seat seat;
	Jetton jetton;
	Money money;
	SeatMsg(PID playerI, Seat sea, Jetton jett, Money mon);
	virtual void getPlayerMsg(PokerPlayer &player);
};

class GameOverMsg : public Message{
public:
	GameOverMsg();
};

class BlindMsg : public Message{
public:
	Jetton smallBlind, bigBlind;
	BlindMsg(Jetton sb, Jetton bb);
};

class HoldCardMsg : public Message{
public:
	Poker card1, card2;
	HoldCardMsg(Poker card1, Poker card2);
};

class InquireMsg : public Message{
public:
	Jetton jetton, bet;
	Money money;
	Action action;
	InquireMsg(PID pid, Jetton jett, Jetton be, Money mon, Action act);
	virtual void getPlayerMsg(PokerPlayer &player);
};

class NotifyMsg : public Message{
public:
	Jetton jetton, bet;
	Money money;
	Action action;
	NotifyMsg(PID pid, Jetton jett, Jetton be, Money mon, Action act);
	virtual void getPlayerMsg(PokerPlayer &player);
};

class TotalPotMsg : public Message{
public:
	Jetton totalPot;
	TotalPotMsg(Jetton total);
};
class ActionMsg : public Message{
public:
	Action action;
	Jetton add_msg;
	ActionMsg(Action action);
	ActionMsg(Action action, Jetton add_msg);
};

class FlopMsg : public Message{
public:
	Poker card1, card2, card3;
	FlopMsg(Poker ca1, Poker ca2, Poker ca3);
	virtual void getPlayerMsg(PokerPlayer &player);
};

class TurnMsg : public Message{
public:
	Poker card1;
	TurnMsg(Poker ca1);
	virtual void getPlayerMsg(PokerPlayer &player);
};

class RiverMsg : public Message{
public:
	Poker card1;
	RiverMsg(Poker ca1);
	virtual void getPlayerMsg(PokerPlayer &player);
};

class ShowDownMsg : public Message{
public:
	Poker card1, card2;
	CardType cardType;
	ShowDownMsg(PID playerI, Poker hand1, Poker hand2, CardType car_t);
	virtual void getPlayerMsg(PokerPlayer &player);
};

class PotWinMsg : public Message{
public:
	Jetton win;
	PotWinMsg(PID playerI, Jetton wi);
	virtual void getPlayerMsg(PokerPlayer &player);
};
