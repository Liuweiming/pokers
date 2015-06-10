#include "stdafx.h"
#include "Message.h"
#include "PokerPlayer.h"
std::ostream &operator << (std::ostream &os, const Message &msg){
	std::string buffer;
	switch (msg.msg_type)
	{
	case seat_msg: buffer = "seat";
		break;
	case game_over_msg: buffer = "game_over";
		break;
	case blind_msg: buffer = "blind";
		break;
	case hold_cards_msg: buffer = "hold_cards";
		break;
	case inquire_msg: buffer = "inquire";
		break;
	case notify_msg: buffer = "notify";
		break;
	case total_pot_msg: buffer = "total_pot";
		break;
	case flop_msg: buffer = "flop";
		break;
	case turn_msg: buffer = "turn";
		break;
	case river_msg: buffer = "river";
		break;
	case show_down_msg: buffer = "show_down";
		break;
	case pot_win_msg: buffer = "pot_win";
		break;
	case error_msg: buffer = "error";
		break;
	case action_msg: buffer = "action";
		break;
	default:
		break;
	}
	os << buffer << " " << msg.playerID;
	return os;
}

Message::Message(MessageType msg_t, PID playerI) : msg_type(msg_t), playerID(playerI){}
void Message::getPlayerMsg(PokerPlayer &player){}
Message::~Message(){}

ErrorMsg::ErrorMsg() : Message(error_msg, PID()){}
ErrorMsg::ErrorMsg(const std::string &err) : Message(error_msg, PID()), error(err){}

RegMsg::RegMsg(PID playerI, const std::string &playerN) : Message(reg_msg, playerI),
			 playerName(playerN){}

SeatMsg::SeatMsg(PID playerI, Seat sea, Jetton jett, Money mon) : Message(seat_msg, playerI),
seat(sea), jetton(jett), money(mon){}
void SeatMsg::getPlayerMsg(PokerPlayer &player){
#ifndef NDEBUG
	if (player.playerID != playerID){
		std::cerr << "error playerID when seat!";
		return;
	}
#endif
	player.seat = seat;
	player.money = money;
	player.jetton = jetton;
	++player.totalPlay;
	player.winRate = 0;
	player.bet = 0;
	player.lastAction = attend;
	player.isShow = false;
	player.isRaise = false;
}

GameOverMsg::GameOverMsg() : Message(game_over_msg, PID()){}

BlindMsg::BlindMsg(Jetton sb, Jetton bb) : Message(blind_msg, PID()),
smallBlind(sb), bigBlind(bb){}

HoldCardMsg::HoldCardMsg(Poker card1, Poker card2) : Message(hold_cards_msg, PID()),
card1(card1), card2(card2) {}

InquireMsg::InquireMsg(PID pid, Jetton jett, Jetton be, Money mon, Action act) :
Message(inquire_msg, pid), jetton(jett), bet(be),
money(mon), action(act){}
void InquireMsg::getPlayerMsg(PokerPlayer &player){
#ifndef NDEBUG
	if (player.playerID != playerID){
		std::cerr << "error playerID when inquire!";
		return;
	}
#endif
	player.jetton = jetton;
	player.money = money;
	player.bet = bet;
	player.lastAction = action;
	switch (action)
	{
	case blind:
		break;
	case check: ++player.checkC;
		break;
	case call: ++player.callC;
		break;
	case raise: ++player.raiseC;
		break;
	case all_in: ++player.all_inC;
		break;
	case fold: ++player.foldC;
		break;
	case lose:
		break;
	default:
		break;
	}
}

NotifyMsg::NotifyMsg(PID pid, Jetton jett, Jetton be, Money mon, Action act) :
Message(notify_msg, pid), jetton(jett), bet(be),
money(mon), action(act){}
void NotifyMsg::getPlayerMsg(PokerPlayer &player){
#ifndef NDEBUG
	if (player.playerID != playerID){
		std::cerr << "error playerID when notify!";
		return;
	}
#endif
	player.jetton = jetton;
	player.money = money;
	player.bet = bet;
	player.lastAction = action;
	switch (action)
	{
	case blind:
		break;
	case check: ++player.checkC;
		break;
	case call: ++player.callC;
		break;
	case raise: ++player.raiseC;
		break;
	case all_in: ++player.all_inC;
		break;
	case fold: ++player.foldC;
		break;
	case lose:
		break;
	default:
		break;
	}
}

TotalPotMsg::TotalPotMsg(Jetton total) : Message(total_pot_msg, 0),
totalPot(total){}

ActionMsg::ActionMsg(Action action) : Message(action_msg, PID()),
action(action), add_msg(0){}
ActionMsg::ActionMsg(Action action, Jetton add_msg) : Message(action_msg, PID()),
action(action), add_msg(add_msg){}

FlopMsg::FlopMsg(Poker ca1, Poker ca2, Poker ca3) : Message(flop_msg, PID()),
card1(ca1), card2(ca2), card3(ca3){}
void FlopMsg::getPlayerMsg(PokerPlayer &player){
	player.playerCards[2] = card1;
	player.playerCards[3] = card2;
	player.playerCards[4] = card3;
}

TurnMsg::TurnMsg(Poker ca1) : Message(turn_msg, PID()),
card1(ca1){}
void TurnMsg::getPlayerMsg(PokerPlayer &player){
	player.playerCards[5] = card1;
}

RiverMsg::RiverMsg(Poker ca1) : Message(river_msg, PID()),
card1(ca1){}
void RiverMsg::getPlayerMsg(PokerPlayer &player){
	player.playerCards[6] = card1;
}

ShowDownMsg::ShowDownMsg(PID playerI, Poker hand1, Poker hand2, CardType car_t) :
Message(show_down_msg, playerI), card1(hand1), card2(hand2), cardType(car_t){}
void ShowDownMsg::getPlayerMsg(PokerPlayer &player){
#ifndef NDEBUG
	if (player.playerID != playerID){
		std::cerr << "error playerID when showDown!";
		return;
	}
#endif
	player.playerCards[0] = card1;
	player.playerCards[1] = card2;
	++player.handCardType[cardType];
	++player.showC;
	player.isShow = true;
}

PotWinMsg::PotWinMsg(PID playerI, Jetton wi) : Message(pot_win_msg, playerI), 
win(wi){}
void PotWinMsg::getPlayerMsg(PokerPlayer &player){
	++player.winC;
	if (player.isShow){
		++player.WTSDC;
		player.isShow = false;
	}
}