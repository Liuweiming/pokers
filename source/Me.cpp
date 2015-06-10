#include "stdafx.h"
#include "Me.h"

bool operator==(const PokerPair &lhs, const PokerPair &rhs){
	if (lhs.a == rhs.a && lhs.b == rhs.b)
		return true;
	return false;
}

Me::Me(MsgGenerator &msgGen, Players &players, PID playerI, std::ofstream &of) :
msgGen(msgGen), me(nullptr), allPlayers(players), proba(), playerID(playerI),
MyName("Rounders"), circle(seat_in), nextMsg(fold), ofs(of), 
normalStrategy(players, roundCount, me, of, TotalPlaysCount),
secStrategy(players, roundCount, me, of, TotalPlaysCount),
losAgs(players, roundCount, me, of, TotalPlaysCount),
losPas(players, roundCount, me, of, TotalPlaysCount),
tiAgs(players, roundCount, me, of, TotalPlaysCount),
tiPas(players, roundCount, me, of, TotalPlaysCount),
strategy(nullptr), TotalPlaysCount(0){}
Me::~Me(){ }
void Me::rigist(){
	RegMsg reg(playerID, MyName);
	msgGen.sendMessage(reg);
}
void Me::MeGetMsg(Message &msg){
	if (msg.msg_type == seat_msg){
		me = allPlayers.getMe(playerID);
		for (int i = 0; i != 4; ++i){
			roundCount[i] = 0;
		}
		circle = seat_in;
		++TotalPlaysCount;
		ofs << "totalPlay " << TotalPlaysCount << std::endl;
	}
	else if (msg.msg_type == hold_cards_msg){
		ofs << "me_hold" << std::endl;
		HoldCardMsg &holdMsg = static_cast<HoldCardMsg &>(msg);
		me->playerCards[0] = holdMsg.card1;
		me->playerCards[1] = holdMsg.card2;
		circle = hold_card;
		//查表确定胜率；
	}
	else if (msg.msg_type == flop_msg){
		ofs << "me_flop" << std::endl;
		circle = flop_card;
		proba.setMyCard(me->playerCards);
	}
	else if (msg.msg_type == turn_msg){
		ofs << "me_turn" << std::endl;
		circle = turn_card;
		proba.setTurn(me->playerCards[5]);
	}
	else if (msg.msg_type == river_msg){
		ofs << "me_river" << std::endl;
		circle = river_card;
		proba.setRiver(me->playerCards[6]);
	}
	else if (msg.msg_type == inquire_msg){
		ofs << "me_inquire" << std::endl;
		act();
		msgGen.sendMessage(nextMsg);
	}
}
void Me::act(){
	allPlayers.getBetAndRaise();
	selecteStrategy();
	switch (circle)
	{
	case seat_in:
		break;
	case hold_card:
		break;
	case pre_flop:
		break;
	case flop_card: 
		winRate = proba.calFlop(allPlayers.getPlayersNum());
		break;
	case pre_turn: 
		break;
	case turn_card: 
		winRate = proba.calTurn(allPlayers.getPlayersNum());
		break;
	case pre_river: 
		break;
	case river_card: 
		winRate = proba.calRiver(allPlayers.getPlayersNum());
		break;
	default:
		break;
	}
	ofs << "winRate " << winRate << std::endl;
	nextMsg = strategy->GetStrategy(circle, winRate);
	if (nextMsg.action == raise){
		if (circle == hold_card)
			strategy->ifRaise[0]++;
		else if (circle == flop_card){
			strategy->ifRaise[1]++;
		}
		else if (circle == turn_card){
			strategy->ifRaise[2]++;
		}
		else if (circle == river_card){
			strategy->ifRaise[3]++;
		}
	}
}
void Me::selecteStrategy(){
	auto q = allPlayers.playerInTable.find(me);
	q++;
	auto myiter = allPlayers.playerInTable.find(me);
	int countloose = 0, counttight = 0, countmiddle = 0,
		existplayer = allPlayers.getPlayersNum();
	bool preaggress = false, afteraggress = false;
	for (; q != myiter; q++){
		{
			if ((*q)->VPIP < 0.14) counttight++;
			else if ((*q)->VPIP > 0.23) countloose++;
			else countmiddle++;
		}
		if ((*q)->AF > 3 || ((*q)->PFR / (*q)->VPIP > 0.42)) {
			preaggress = true;
		}
		if ((*q)->AF > 2.7 && ((*q)->Cbet > 0.6)) {
			afteraggress = true;
		}
	}
	if (TotalPlaysCount < 10){
		strategy = &normalStrategy;
	}
	else{
		if (circle == hold_card){
			if (countloose > 0.5*existplayer){
				if (!preaggress){
					//loose&weak
					strategy = &losPas;
				}
				else{
					//loose&fierce
					strategy = &losAgs;
				}
			}
			else if (counttight > 0.5*existplayer){
				if (!preaggress){
					//tight&weak
					strategy = &tiPas;
				}
				else{
					//tight&fierce
					strategy = &tiAgs;
				}
			}
			else{
				strategy = &secStrategy;
			}
		}
		else{
			strategy = &secStrategy;
		}
	}
	//strategy = &normalStrategy;
}
