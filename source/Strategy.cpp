#include "stdafx.h"
#include "Strategy.h"
Strategy::Strategy(Players &players, std::array<int, 4> &roundC,
	ref_type &me, std::ofstream &of, int &totalPlay) : players(players),
	roundCount(roundC), me(me),
	leastBet(players.leastBet), leastRaise(players.leastRaise),
	shouldBet(players.shouldBet), shouldRaise(players.shouldRaise),
	pot(players.pot), myCardLevel(11), prePlayer(nullptr), gen(rd()), dis(0, 10), ofs(of),
	totalplay(totalPlay){
	for (int i = 0; i != 4; ++i){
		ifRaise[i] = 0;
	}
}
Strategy::~Strategy(){}
/*ActionMsg Strategy::inHold(){
ofs << "inHold" << std::endl;
ActionMsg nextMsg(fold);
if (players.getPlayersNum() == 1){
nextMsg.action = call;
}
else{
PokerPair myCard;
nextMsg.action = fold;
if (me->playerCards[0] < me->playerCards[1]){
myCard.a = me->playerCards[0];
myCard.b = me->playerCards[1];
}
else{
myCard.a = me->playerCards[1];
myCard.b = me->playerCards[0];
}
int mySeat = players.mySeat();
myCardLevel = seatPoker.holdCardLevel(myCard);
switch (mySeat)
{
case at_head:
if (myCardLevel <= 1 && shouldBet - me->bet < 5 * leastBet){
if (players.playerRaiseBefor() < 3){
nextMsg.action = raise;
nextMsg.add_msg = (2 + dis(gen) % 3) * leastBet;
}
else{
nextMsg.action = call;
}
}
break;
case at_middle:
if (myCardLevel <= 3 && shouldBet - me->bet < 5 * leastBet){
if (players.playerRaiseBefor() < 3){
nextMsg.action = raise;
nextMsg.add_msg = (2 + dis(gen) % 3) * leastBet;
}
else{
nextMsg.action = call;
}
}
else if (myCardLevel <= 0 && players.getPlayersNum() == 2){
nextMsg.action = call;
}
break;
case at_tail:
if (myCardLevel <= 3 && shouldBet - me->bet < 5 * leastBet){
if (players.playerRaiseBefor() < 3){
nextMsg.action = raise;
nextMsg.add_msg = (2 + dis(gen) % 3) * leastBet;
}
else{
nextMsg.action = call;
}
}
else if (myCardLevel <= 0 && players.getPlayersNum() == 2){
nextMsg.action = call;
}
break;
case at_button:
if (myCardLevel <= 5 && shouldBet - me->bet < 5 * leastBet){
if (players.playerRaiseBefor() < 3){
nextMsg.action = raise;
nextMsg.add_msg = (2 + dis(gen) % 3) * leastBet;
}
else{
nextMsg.action = call;
}
}
else if (myCardLevel <= 0 && players.getPlayersNum() == 2){
nextMsg.action = call;
}
break;
default:
break;
}
}
++roundCount[0];
return nextMsg;
}*/

ActionMsg Strategy::inHold(){
	ofs << "inHold" << std::endl;
	ActionMsg nextMsg(fold);
	if (players.getPlayersNum() == 1){
		nextMsg.action = call;
	}
	else{
		PokerPair myCard;
		nextMsg.action = fold;
		if (me->playerCards[0] < me->playerCards[1]){
			myCard.a = me->playerCards[0];
			myCard.b = me->playerCards[1];
		}
		else{
			myCard.a = me->playerCards[1];
			myCard.b = me->playerCards[0];
		}
		int mySeat = players.mySeat();
		myCardLevel = seatPoker.holdCardLevel(myCard);
		if (me->seat == big_blind && shouldBet == leastBet && myCardLevel >= 8) {
			nextMsg.action = call;
		}
		else
		{
			switch (myCardLevel)
			{
			case 0:
				if (mySeat < 8 && mySeat > 3){
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = raise;
						nextMsg.add_msg = (4 + dis(gen) % 3) * leastBet;
					}
					else if (players.playerAllIn() > 0){
						if (myCard.a==_A && myCard.b==_A && players.getPlayersNum()<4) nextMsg.action = call;
						else nextMsg.action = fold;
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = raise;
							nextMsg.add_msg = (3 + dis(gen) % 3) * leastBet;
						}
						else{
							if (myCard.a != _Q) nextMsg.action = call;
							else nextMsg.action = fold;
						}
					}
					else{
						if (myCard.a == _A && myCard.b == _A && players.getPlayersNum()<4){
							nextMsg.action = all_in;
						}
						else{
							nextMsg.action = call;
						}
					}
				}
				else{
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = raise;
						nextMsg.add_msg = (3 + dis(gen) % 3) * leastBet;
					}
					else if (players.playerAllIn() > 0){
						if (myCard.a == _A && myCard.b == _A && players.getPlayersNum()<4) nextMsg.action = call;
						else nextMsg.action = fold;
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = raise;
							nextMsg.add_msg = (3 + dis(gen) % 3) * leastBet;
						}
						else{
							nextMsg.action = call;
						}
					}
					else{
						if ((!(myCard.a == _A && myCard.b == _K && myCard.a.color != myCard.b.color)) &&
							myCard.a != _Q) nextMsg.action = all_in;
						else nextMsg.action = call;
					}
				}
				break;
			case 1:
				if (mySeat < 8 && mySeat > 3){
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = raise;
						nextMsg.add_msg = (4 + dis(gen) % 3) * leastBet;
					}
					else if (players.playerAllIn() > 0){
						nextMsg.action = fold;
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
					else{
						nextMsg.action = fold;
					}
				}
				else{
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = raise;
						nextMsg.add_msg = (3 + dis(gen) % 3) * leastBet;
					}
					else if (players.playerAllIn() > 0){
						nextMsg.action = fold;
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
					else{
						nextMsg.action = fold;
					}
				}
				break;
			case 2:
				if (mySeat < 8 && mySeat > 3){
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = raise;
						nextMsg.add_msg = (3 + dis(gen) % 3) * leastBet;
					}
					else if (players.playerAllIn() > 0){
						nextMsg.action = fold;
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
					else{
						if (shouldBet - me->bet <= 5 * leastBet  && roundCount[0] <4){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
				}
				else{
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = raise;
						nextMsg.add_msg = (4 + dis(gen) % 3) * leastBet;
					}
					else if (players.playerAllIn() > 0){
						nextMsg.action = fold;
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = raise;
							nextMsg.add_msg = (3 + dis(gen) % 3) * leastBet;
						}
						else{
							nextMsg.action = call;
						}
					}
					else{
						if (shouldBet - me->bet <= 5 * leastBet && roundCount[0] <4){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
				}
				break;
			case 3:
				if (mySeat < 8 && mySeat > 3){
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = raise;
						nextMsg.add_msg = (2 + dis(gen) % 3) * leastBet;
					}
					else if (players.playerAllIn() > 0){
						nextMsg.action = fold;
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
					else{
						if (shouldBet - me->bet <= 3 * leastBet && roundCount[0] <3){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
				}
				else{
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = raise;
						nextMsg.add_msg = (2 + dis(gen) % 3) * leastBet;
					}
					else if (players.playerAllIn() > 0){
						nextMsg.action = fold;
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 8 * leastBet){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
					else{
						if (shouldBet - me->bet <= 5 * leastBet&& roundCount[0] <4){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
				}
				break;
			case 4:
				if (mySeat < 8 && mySeat > 3){
					/*if (players.playerRaiseBefor() == 0){
					nextMsg.action = raise;
					nextMsg.add_msg = (1 + dis(gen) % 2) * leastBet;
					}
					else if (players.playerAllIn() > 0){
					nextMsg.action = fold;
					}

					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
					if (shouldBet - me->bet <= 3 * leastBet){
					nextMsg.action = call;
					}
					else{
					nextMsg.action = fold;
					}
					}
					else{
					nextMsg.action = fold;
					}*/
					nextMsg.action = fold;
				}
				else{
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = raise;
						nextMsg.add_msg = (2 + dis(gen) % 2) * leastBet;
					}
					else if (players.playerAllIn() > 0){
						nextMsg.action = fold;
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = raise;
							nextMsg.add_msg = 3 + dis(gen) % 3;
						}
						else{
							nextMsg.action = call;
						}
					}
					else{
						if (shouldBet - me->bet <= 3 * leastBet&& roundCount[0] <2){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
				}
				break;
			case 5:
				if (mySeat < 6 && mySeat > 3){
					/*if (players.playerRaiseBefor() == 0){
					nextMsg.action = raise;
					nextMsg.add_msg = (1 + dis(gen) % 2) * leastBet;
					}
					else{
					nextMsg.action = fold;
					}*/
					nextMsg.action = fold;
				}
				else if(mySeat<4){
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = raise;
						nextMsg.add_msg = (2 + dis(gen) % 3) * leastBet;
					}
					else if (players.playerAllIn() > 0){
						nextMsg.action = fold;
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
					else{
						if (shouldBet - me->bet <= 3 * leastBet&& roundCount[0] <2){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
				}
				break;
			case 6:
				if (mySeat < 8 && mySeat > 3){
					nextMsg.action = fold;
				}
				else{
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = call;
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
					else{
						nextMsg.action = fold;
					}
				}
				break;
			case 7:
				if (mySeat < 8 && mySeat > 3){
					nextMsg.action = fold;
				}
				else{
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = raise;
						nextMsg.add_msg = (1 + (dis(gen) % 3)) * leastBet;
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 3 * leastBet){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
					else{
						nextMsg.action = fold;
					}
				}
				break;
			case 8:
				if (mySeat < 8 && mySeat > 3){
					nextMsg.action = fold;
				}
				else{
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = call;
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 2 * leastBet){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
					else{
						nextMsg.action = fold;
					}
				}
				break;
			case 9:
				if (mySeat<3){
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = call;
					}
					else{
						nextMsg.action = fold;
					}
				}
				else{
					nextMsg.action = fold;
				}
				break;
			case 10:
				if (mySeat<2){
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = call;
					}
					else{
						nextMsg.action = fold;
					}
				}
				else{
					nextMsg.action = fold;
				}
				break;
			default:
				break;
			}
		}
	}
	++roundCount[0];
	return nextMsg;
}
ActionMsg Strategy::inFlop(){
	ofs << "inFlop" << std::endl;
	ActionMsg nextMsg(fold);
	if (players.getPlayersNum() == 1){
		nextMsg.action = call;
		return nextMsg;
	}
	auto raiseNum = players.playerRaiseBefor();
	if (raiseNum == 0){
		if (winRate <0.7 && ifRaise[0] == 1 && ((dis(gen) % 3 < 1) || (me->seat == button))){
			nextMsg.action = raise;
			nextMsg.add_msg = static_cast<Jetton>(0.5*pot);
		}
		else if (winRate > 0.7){
			nextMsg.action = raise;
			nextMsg.add_msg = static_cast<Jetton>((0.3*(dis(gen) % 2) + 0.7)*pot);
		}
		else {
			nextMsg.action = check;
		}
	}
	else if (raiseNum == 1){
		if ((static_cast<double>(shouldBet - me->bet) / (pot + shouldBet - me->bet)
			< winRate) && (winRate < 0.7)){
			nextMsg.action = call;
		}
		else if (winRate>0.98 && (players.playerAllIn() > 0)){
			nextMsg.action = call;
		}
		else if (winRate>0.95 && ((players.playerAllIn() == 0) || roundCount[1] >= 1)){
			nextMsg.action = raise;
			nextMsg.add_msg = shouldRaise;
		}
		else if (winRate>0.85 && ((players.playerAllIn() == 0) || roundCount[1] >= 1) &&
			(shouldBet - me->bet) / (pot + shouldBet - me->bet) < winRate){
			nextMsg.action = call;
		}
		else if (winRate>0.85 && (players.playerAllIn() == 0) && (shouldRaise <= 0.125*me->jetton) && roundCount[1]<1){
			nextMsg.action = raise;
			nextMsg.add_msg = shouldRaise;
		}
		else{
			nextMsg.action = fold;
		}
	}
	else if (raiseNum >= 2){
		if (winRate > 0.95){
			nextMsg.action = raise;
			nextMsg.add_msg = shouldRaise;
		}
		else if (winRate > 0.9){
			nextMsg.action = call;
		}
		else
			nextMsg.action = fold;
	}

	++roundCount[1];
	return nextMsg;
}
ActionMsg Strategy::inTurn(){
	ofs << "inTurn" << std::endl;
	ofs << "winRate " << winRate << std::endl;
	ActionMsg nextMsg(fold);
	CardType myCardType_pre = me->getMyCardType(flop_card);
	CardType myCardType = me->getMyCardType(turn_card);
	if (players.getPlayersNum() == 1){
		nextMsg.action = call;
		return nextMsg;
	}
	auto raiseNum = players.playerRaiseBefor();
	if (raiseNum == 0){
		if (myCardType_pre != myCardType && me->HoldCardIn() && winRate>0.7){
			nextMsg.action = raise;
			nextMsg.add_msg = static_cast<Jetton>(1.0*pot);
		}
		else if (winRate > 0.85){
			nextMsg.action = raise;
			nextMsg.add_msg = static_cast<Jetton>((0.15*(dis(gen) % 2) + 0.85)*pot);
		}
		else {
			nextMsg.action = check;
		}
	}
	else if (raiseNum == 1){
		if ((static_cast<double>(shouldBet - me->bet) / (pot + shouldBet - me->bet)< winRate)
			&& (winRate < 0.7) && (winRate>0.50)){
			nextMsg.action = call;
		}
		else if (winRate>0.98 && (players.playerAllIn() > 0)){
			nextMsg.action = call;
		}
		else if (winRate>0.95 && ((players.playerAllIn() == 0) || roundCount[2]>1)){
			nextMsg.action = raise;
			nextMsg.add_msg = shouldRaise;
		}
		else if (winRate>0.8 && (players.playerAllIn() == 0) &&
			(shouldRaise <= 0.125*me->jetton) && roundCount[2] <= 1){
			nextMsg.action = raise;
			nextMsg.add_msg = shouldRaise;
		}
		else{
			nextMsg.action = fold;
		}
	}
	else if (raiseNum >= 2){
		if (winRate > 0.95&& me->HoldCardIn()){
			nextMsg.action = raise;
			nextMsg.add_msg = shouldRaise;
		}
		else if (winRate > 0.9){
			nextMsg.action = call;
		}
		else
			nextMsg.action = fold;
	}
	++roundCount[2];
	return nextMsg;
}
ActionMsg Strategy::inRiver(){
	ofs << "inRiver" << std::endl;
	ActionMsg nextMsg(fold);
	CardType myCardType_pre = me->getMyCardType(turn_card);
	CardType myCardType = me->getMyCardType(river_card);
	if (players.getPlayersNum() == 1){
		nextMsg.action = call;
		return nextMsg;
	}
	auto raiseNum = players.playerRaiseBefor();
	if (raiseNum == 0){
		if (myCardType_pre != myCardType && me->HoldCardIn() && winRate>0.8){
			nextMsg.action = raise;
			nextMsg.add_msg = static_cast<Jetton>(1.0*pot);
		}
		else if (winRate > 0.7){
			nextMsg.action = raise;
			nextMsg.add_msg = static_cast<Jetton>((0.2*(dis(gen) % 2) + 0.5)*pot);
		}
		else {
			nextMsg.action = check;
		}
	}
	else if (raiseNum == 1){
		if (winRate > 0.75){
			nextMsg.action = call;
		}
		else if (winRate>0.98 && (players.playerAllIn() > 0)){
			nextMsg.action = call;
		}
		else if (winRate>0.95 && ((players.playerAllIn() == 0) || roundCount[3] >= 1)){
			nextMsg.action = raise;
			nextMsg.add_msg = shouldRaise;
		}
		else if (winRate>0.85 && (players.playerAllIn() == 0) &&
			(shouldRaise <= 0.125*me->jetton) && roundCount[3] <1){
			nextMsg.action = raise;
			nextMsg.add_msg = shouldRaise;
		}
		else{
			nextMsg.action = fold;
		}
	}
	else if (raiseNum >= 2){
		if (winRate > 0.95 && me->HoldCardIn()){
			nextMsg.action = raise;
			nextMsg.add_msg = shouldRaise;
		}
		else if (winRate > 0.9){
			nextMsg.action = call;
		}
		else
			nextMsg.action = fold;
	}
	++roundCount[3];
	return nextMsg;
}
ActionMsg Strategy::inHold2(){
	ofs << "in second Hold" << std::endl;
	ActionMsg nextMsg(fold);
	/*auto p = players.playerInTable.begin();
	auto q = players.playerInTable.find(me);
	q++;
	(*q)->AF;
	*/
	if (players.getPlayersNum() == 1){
		nextMsg.action = call;
	}
	else{
		PokerPair myCard;
		nextMsg.action = fold;
		if (me->playerCards[0] < me->playerCards[1]){
			myCard.a = me->playerCards[0];
			myCard.b = me->playerCards[1];
		}
		else{
			myCard.a = me->playerCards[1];
			myCard.b = me->playerCards[0];
		}
		int mySeat = players.mySeat();
		myCardLevel = seatPoker.holdCardLevel(myCard);
		if (me->seat == big_blind && shouldBet == leastBet && myCardLevel >= 8) {
			nextMsg.action = call;
		}
		else
		{
			switch (myCardLevel)
			{
			case 0:
				if (mySeat < 8 && mySeat > 3){
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = raise;
						nextMsg.add_msg = (4 + dis(gen) % 3) * leastBet;
					}
					else if (players.playerAllIn() > 0){
						if (shouldBet - me->bet <= 5 * leastBet) nextMsg.action = call;
						else nextMsg.action = fold;
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = raise;
							nextMsg.add_msg = (3 + dis(gen) % 3) * leastBet;
						}
						else{
							if (myCard.a != _Q) nextMsg.action = call;
							else nextMsg.action = fold;
						}
					}
					else{
						nextMsg.action = call;
					}
				}
				else{
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = raise;
						nextMsg.add_msg = (3 + dis(gen) % 3) * leastBet;
					}
					else if (players.playerAllIn() > 0){
						if ((!(myCard.a == _A && myCard.b == _K && myCard.a.color != myCard.b.color)) &&
							myCard.a != _Q && players.getPlayersNum()<4) nextMsg.action = call;
						else nextMsg.action = fold;
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = raise;
							nextMsg.add_msg = (3 + dis(gen) % 3) * leastBet;
						}
						else{
							nextMsg.action = call;
						}
					}
					else{
						if ((!(myCard.a == _A && myCard.b == _K && myCard.a.color != myCard.b.color)) &&
							myCard.a != _Q) nextMsg.action = all_in;
						else nextMsg.action = call;
					}
				}
				break;
			case 1:
				if (mySeat < 8 && mySeat > 3){
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = raise;
						nextMsg.add_msg = (4 + dis(gen) % 3) * leastBet;
					}
					else if (players.playerAllIn() > 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = call;
						}
						else{ nextMsg.action = fold; }
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = call;
						}
						else if ((players.getRaisePlayer()->VPIP > 0.23 || players.getRaisePlayer()->PFR > 0.1) && (shouldBet - me->bet <= 10 * leastBet)){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
					else{
						if ((players.getRaisePlayer()->VPIP > 0.23 || players.getRaisePlayer()->PFR > 0.1) && (shouldBet - me->bet <= 10 * leastBet)){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
				}
				else{
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = raise;
						nextMsg.add_msg = (3 + dis(gen) % 3) * leastBet;
					}
					else if (players.playerAllIn() > 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = call;
						}
						else{ nextMsg.action = fold; }
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = call;
						}
						else if ((players.getRaisePlayer()->VPIP > 0.23 || players.getRaisePlayer()->PFR > 0.1) && (shouldBet - me->bet <= 10 * leastBet)){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
					else{
						if ((players.getRaisePlayer()->VPIP > 0.23 || players.getRaisePlayer()->PFR > 0.1) && (shouldBet - me->bet <= 10 * leastBet)){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
				}
				break;
			case 2:
				if (mySeat < 8 && mySeat > 3){
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = raise;
						nextMsg.add_msg = (3 + dis(gen) % 3) * leastBet;
					}
					else if (players.playerAllIn() > 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = call;
						}
						else{ nextMsg.action = fold; }
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = call;
						}
						else if ((players.getRaisePlayer()->VPIP > 0.23 || players.getRaisePlayer()->PFR > 0.1) && (shouldBet - me->bet <= 8 * leastBet)){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
					else{
						if (shouldBet - me->bet <= 5 * leastBet && roundCount[0]<4){
							nextMsg.action = call;
						}
						else if ((players.getRaisePlayer()->VPIP > 0.23 || players.getRaisePlayer()->PFR > 0.1) && (shouldBet - me->bet <= 8 * leastBet) && roundCount[0]<4){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
				}
				else{
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = raise;
						nextMsg.add_msg = (4 + dis(gen) % 3) * leastBet;
					}
					else if (players.playerAllIn() > 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = call;
						}
						else{ nextMsg.action = fold; }
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = raise;
							nextMsg.add_msg = (3 + dis(gen) % 3) * leastBet;
						}
						else if ((players.getRaisePlayer()->VPIP > 0.23 || players.getRaisePlayer()->PFR > 0.1) && (shouldBet - me->bet <= 8 * leastBet)){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = call;
						}
					}
					else{
						if (shouldBet - me->bet <= 5 * leastBet&& roundCount[0]<4){
							nextMsg.action = call;
						}
						else if ((players.getRaisePlayer()->VPIP > 0.23 || players.getRaisePlayer()->PFR > 0.1) && (shouldBet - me->bet <= 8 * leastBet) && roundCount[0]<4){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
				}
				break;
			case 3:
				if (mySeat < 8 && mySeat > 3){
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = raise;
						nextMsg.add_msg = (2 + dis(gen) % 3) * leastBet;
					}
					else if (players.playerAllIn() > 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = call;
						}
						else{ nextMsg.action = fold; }
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = call;
						}
						else if ((players.getRaisePlayer()->VPIP > 0.23 || players.getRaisePlayer()->PFR > 0.1) && (shouldBet - me->bet <= 7 * leastBet)){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
					else{
						if (shouldBet - me->bet <= 3 * leastBet&& roundCount[0]<4){
							nextMsg.action = call;
						}
						else if ((players.getRaisePlayer()->VPIP > 0.23 || players.getRaisePlayer()->PFR > 0.1) && (shouldBet - me->bet <= 5 * leastBet) && roundCount[0]<4){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
				}
				else{
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = raise;
						nextMsg.add_msg = (2 + dis(gen) % 3) * leastBet;
					}
					else if (players.playerAllIn() > 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = call;
						}
						else{ nextMsg.action = fold; }
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 8 * leastBet){
							nextMsg.action = call;
						}
						else if ((players.getRaisePlayer()->VPIP > 0.23 || players.getRaisePlayer()->PFR > 0.1) && (shouldBet - me->bet <= 10 * leastBet)){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
					else{
						if (shouldBet - me->bet <= 5 * leastBet&& roundCount[0]<4){
							nextMsg.action = call;
						}
						else if ((players.getRaisePlayer()->VPIP > 0.23 || players.getRaisePlayer()->PFR > 0.1) && (shouldBet - me->bet <= 8 * leastBet) && roundCount[0]<4){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
				}
				break;
			case 4:
				if (mySeat < 8 && mySeat > 3){
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = raise;
						nextMsg.add_msg = (1 + dis(gen) % 2) * leastBet;
					}
					else if (players.playerAllIn() > 0){
						nextMsg.action = fold;
					}

					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 3 * leastBet){
							nextMsg.action = call;
						}
						else if ((players.getRaisePlayer()->VPIP > 0.23 || players.getRaisePlayer()->PFR > 0.1) && (shouldBet - me->bet <= 5 * leastBet)){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
					else{
						nextMsg.action = fold;
					}
					//nextMsg.action = fold;
				}
				else{
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = raise;
						nextMsg.add_msg = (2 + dis(gen) % 2) * leastBet;
					}
					else if (players.playerAllIn() > 0){
						nextMsg.action = fold;
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = raise;
							nextMsg.add_msg = 3 + dis(gen) % 3;
						}
						else if ((players.getRaisePlayer()->VPIP > 0.23 || players.getRaisePlayer()->PFR > 0.1) && (shouldBet - me->bet <= 5 * leastBet)){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = call;
						}
					}
					else{
						if (shouldBet - me->bet <= 3 * leastBet&& roundCount[0]<4){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
				}
				break;
			case 5:
				if (mySeat < 6 && mySeat > 3){
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = raise;
						nextMsg.add_msg = (1 + dis(gen) % 2) * leastBet;
					}
					else{
						nextMsg.action = fold;
					}
					//nextMsg.action = fold;
				}
				else if (mySeat<4){
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = raise;
						nextMsg.add_msg = (2 + dis(gen) % 3) * leastBet;
					}
					else if (players.playerAllIn() > 0){
						nextMsg.action = fold;
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
					else{
						if (shouldBet - me->bet <= 3 * leastBet&& roundCount[0]<4){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
				}
				break;
			case 6:
				if (mySeat < 8 && mySeat > 3){
					nextMsg.action = fold;
				}
				else{
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = call;
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
					else{
						nextMsg.action = fold;
					}
				}
				break;
			case 7:
				if (mySeat < 8 && mySeat > 3){
					nextMsg.action = fold;
				}
				else{
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = raise;
						nextMsg.add_msg = (1 + (dis(gen) % 3)) * leastBet;
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 3 * leastBet){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
					else{
						nextMsg.action = fold;
					}
				}
				break;
			case 8:
				if (mySeat < 8 && mySeat > 3){
					nextMsg.action = fold;
				}
				else{
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = call;
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 2 * leastBet){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
					else{
						nextMsg.action = fold;
					}
				}
				break;
			case 9:
				if (mySeat<3){
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = call;
					}
					else{
						nextMsg.action = fold;
					}
				}
				else{
					nextMsg.action = fold;
				}
				break;
			case 10:
				if (mySeat<2){
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = call;
					}
					else{
						nextMsg.action = fold;
					}
				}
				else{
					nextMsg.action = fold;
				}
				break;
			default:
				break;
			}
		}
	}
	++roundCount[0];
	return nextMsg;
}


ActionMsg Strategy::GetStrategy(Circle cir, double winR){
	ofs << "should Bet raise myBet, pot: " << shouldBet << " " << shouldRaise
		<< " " << me->bet << " " << pot << std::endl;
	winRate = winR;
	ActionMsg nextMsg(fold);
	if (cir == hold_card){
		nextMsg = inHold();
	}
	else if (cir == flop_card){
		nextMsg = inFlop();
	}
	else if (cir == turn_card){
		nextMsg = inTurn();
	}
	else if (cir == river_card){
		nextMsg = inRiver();
	}
	return nextMsg;
}
void Strategy::getPrePlayer(const Players::iter_type &iter){
	prePlayer = *(iter);
}


Strategy2::Strategy2(Players &players, std::array<int, 4> &roundC,
	ref_type &me, std::ofstream &of, int &totalPlay) :
	Strategy(players, roundC, me, of, totalPlay){
}
ActionMsg Strategy2::inHold(){
	ofs << "in second Hold" << std::endl;
	ActionMsg nextMsg(fold);
	/*auto p = players.playerInTable.begin();
	auto q = players.playerInTable.find(me);
	q++;
	(*q)->AF;
	*/
	if (players.getPlayersNum() == 1){
		nextMsg.action = call;
	}
	else{
		PokerPair myCard;
		nextMsg.action = fold;
		if (me->playerCards[0] < me->playerCards[1]){
			myCard.a = me->playerCards[0];
			myCard.b = me->playerCards[1];
		}
		else{
			myCard.a = me->playerCards[1];
			myCard.b = me->playerCards[0];
		}
		int mySeat = players.mySeat();
		myCardLevel = seatPoker.holdCardLevel(myCard);
		if (me->seat == big_blind && shouldBet == leastBet && myCardLevel >= 8) {
			nextMsg.action = call;
		}
		else
		{
			switch (myCardLevel)
			{
			case 0:
				if (mySeat < 8 && mySeat > 3){
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = raise;
						nextMsg.add_msg = (4 + dis(gen) % 3) * leastBet;
					}
					else if (players.playerAllIn() > 0){
						if (shouldBet - me->bet <= 5 * leastBet) nextMsg.action = call;
						else nextMsg.action = fold;
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = raise;
							nextMsg.add_msg = (3 + dis(gen) % 3) * leastBet;
						}
						else{
							if (myCard.a != _Q) nextMsg.action = call;
							else nextMsg.action = fold;
						}
					}
					else{
							nextMsg.action = call;
					}
				}
				else{
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = raise;
						nextMsg.add_msg = (3 + dis(gen) % 3) * leastBet;
					}
					else if (players.playerAllIn() > 0){
						if ((!(myCard.a == _A && myCard.b == _K && myCard.a.color != myCard.b.color)) &&
							myCard.a != _Q && players.getPlayersNum()<4) nextMsg.action = call;
						else nextMsg.action = fold;
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = raise;
							nextMsg.add_msg = (3 + dis(gen) % 3) * leastBet;
						}
						else{
							nextMsg.action = call;
						}
					}
					else{
						if ((!(myCard.a == _A && myCard.b == _K && myCard.a.color != myCard.b.color)) &&
							myCard.a != _Q) nextMsg.action = all_in;
						else nextMsg.action = call;
					}
				}
				break;
			case 1:
				if (mySeat < 8 && mySeat > 3){
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = raise;
						nextMsg.add_msg = (4 + dis(gen) % 3) * leastBet;
					}
					else if (players.playerAllIn() > 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = call;
						}
						else{ nextMsg.action = fold; }
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = call;
						}
						else if ((players.getRaisePlayer()->VPIP > 0.23 || players.getRaisePlayer()->PFR > 0.1) && (shouldBet - me->bet <= 10 * leastBet)){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
					else{
						if ((players.getRaisePlayer()->VPIP > 0.23 || players.getRaisePlayer()->PFR > 0.1) && (shouldBet - me->bet <= 10 * leastBet)){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
				}
				else{
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = raise;
						nextMsg.add_msg = (3 + dis(gen) % 3) * leastBet;
					}
					else if (players.playerAllIn() > 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = call;
						}
						else{ nextMsg.action = fold; }
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = call;
						}
						else if ((players.getRaisePlayer()->VPIP > 0.23 || players.getRaisePlayer()->PFR > 0.1) && (shouldBet - me->bet <= 10 * leastBet)){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
					else{
						if ((players.getRaisePlayer()->VPIP > 0.23 || players.getRaisePlayer()->PFR > 0.1) && (shouldBet - me->bet <= 10 * leastBet)){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
				}
				break;
			case 2:
				if (mySeat < 8 && mySeat > 3){
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = raise;
						nextMsg.add_msg = (3 + dis(gen) % 3) * leastBet;
					}
					else if (players.playerAllIn() > 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = call;
						}
						else{ nextMsg.action = fold; }
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = call;
						}
						else if ((players.getRaisePlayer()->VPIP > 0.23 || players.getRaisePlayer()->PFR > 0.1) && (shouldBet - me->bet <= 8 * leastBet)){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
					else{
						if (shouldBet - me->bet <= 5 * leastBet && roundCount[0]<4){
							nextMsg.action = call;
						}
						else if ((players.getRaisePlayer()->VPIP > 0.23 || players.getRaisePlayer()->PFR > 0.1) && (shouldBet - me->bet <= 8 * leastBet) && roundCount[0]<4){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
				}
				else{
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = raise;
						nextMsg.add_msg = (4 + dis(gen) % 3) * leastBet;
					}
					else if (players.playerAllIn() > 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = call;
						}
						else{ nextMsg.action = fold; }
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = raise;
							nextMsg.add_msg = (3 + dis(gen) % 3) * leastBet;
						}
						else if ((players.getRaisePlayer()->VPIP > 0.23 || players.getRaisePlayer()->PFR > 0.1) && (shouldBet - me->bet <= 8 * leastBet)){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = call;
						}
					}
					else{
						if (shouldBet - me->bet <= 5 * leastBet&& roundCount[0]<4){
							nextMsg.action = call;
						}
						else if ((players.getRaisePlayer()->VPIP > 0.23 || players.getRaisePlayer()->PFR > 0.1) && (shouldBet - me->bet <= 8 * leastBet)&& roundCount[0]<4){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
				}
				break;
			case 3:
				if (mySeat < 8 && mySeat > 3){
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = raise;
						nextMsg.add_msg = (2 + dis(gen) % 3) * leastBet;
					}
					else if (players.playerAllIn() > 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = call;
						}
						else{ nextMsg.action = fold; }
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = call;
						}
						else if ((players.getRaisePlayer()->VPIP > 0.23 || players.getRaisePlayer()->PFR > 0.1) && (shouldBet - me->bet <= 7 * leastBet)){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
					else{
						if (shouldBet - me->bet <= 3 * leastBet&& roundCount[0]<4){
							nextMsg.action = call;
						}
						else if ((players.getRaisePlayer()->VPIP > 0.23 || players.getRaisePlayer()->PFR > 0.1) && (shouldBet - me->bet <= 5 * leastBet) && roundCount[0]<4){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
				}
				else{
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = raise;
						nextMsg.add_msg = (2 + dis(gen) % 3) * leastBet;
					}
					else if (players.playerAllIn() > 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = call;
						}
						else{ nextMsg.action = fold; }
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 8 * leastBet){
							nextMsg.action = call;
						}
						else if ((players.getRaisePlayer()->VPIP > 0.23 || players.getRaisePlayer()->PFR > 0.1) && (shouldBet - me->bet <= 10 * leastBet)){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
					else{
						if (shouldBet - me->bet <= 5 * leastBet&& roundCount[0]<4){
							nextMsg.action = call;
						}
						else if ((players.getRaisePlayer()->VPIP > 0.23 || players.getRaisePlayer()->PFR > 0.1) && (shouldBet - me->bet <= 8 * leastBet) && roundCount[0]<4){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
				}
				break;
			case 4:
				if (mySeat < 8 && mySeat > 3){
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = raise;
						nextMsg.add_msg = (1 + dis(gen) % 2) * leastBet;
					}
					else if (players.playerAllIn() > 0){
						nextMsg.action = fold;
					}

					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 3 * leastBet){
							nextMsg.action = call;
						}
						else if ((players.getRaisePlayer()->VPIP > 0.23 || players.getRaisePlayer()->PFR > 0.1) && (shouldBet - me->bet <= 5 * leastBet)){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
					else{
						nextMsg.action = fold;
					}
					//nextMsg.action = fold;
				}
				else{
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = raise;
						nextMsg.add_msg = (2 + dis(gen) % 2) * leastBet;
					}
					else if (players.playerAllIn() > 0){
						nextMsg.action = fold;
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = raise;
							nextMsg.add_msg = 3 + dis(gen) % 3;
						}
						else if ((players.getRaisePlayer()->VPIP > 0.23 || players.getRaisePlayer()->PFR > 0.1) && (shouldBet - me->bet <= 5 * leastBet)){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = call;
						}
					}
					else{
						if (shouldBet - me->bet <= 3 * leastBet&& roundCount[0]<4){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
				}
				break;
			case 5:
				if (mySeat < 6 && mySeat > 3){
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = raise;
						nextMsg.add_msg = (1 + dis(gen) % 2) * leastBet;
					}
					else{
						nextMsg.action = fold;
					}
					//nextMsg.action = fold;
				}
				else if(mySeat<4){
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = raise;
						nextMsg.add_msg = (2 + dis(gen) % 3) * leastBet;
					}
					else if (players.playerAllIn() > 0){
						nextMsg.action = fold;
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
					else{
						if (shouldBet - me->bet <= 3 * leastBet&& roundCount[0]<4){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
				}
				break;
			case 6:
				if (mySeat < 8 && mySeat > 3){
					nextMsg.action = fold;
				}
				else{
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = call;
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 5 * leastBet){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
					else{
						nextMsg.action = fold;
					}
				}
				break;
			case 7:
				if (mySeat < 8 && mySeat > 3){
					nextMsg.action = fold;
				}
				else{
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = raise;
						nextMsg.add_msg = (1 + (dis(gen) % 3)) * leastBet;
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 3 * leastBet){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
					else{
						nextMsg.action = fold;
					}
				}
				break;
			case 8:
				if (mySeat < 8 && mySeat > 3){
					nextMsg.action = fold;
				}
				else{
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = call;
					}
					else if (players.playerRaiseBefor() > 0 && roundCount[0] == 0){
						if (shouldBet - me->bet <= 2 * leastBet){
							nextMsg.action = call;
						}
						else{
							nextMsg.action = fold;
						}
					}
					else{
						nextMsg.action = fold;
					}
				}
				break;
			case 9:
				if (mySeat<3){
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = call;
					}
					else{
						nextMsg.action = fold;
					}
				}
				else{
					nextMsg.action = fold;
				}
				break;
			case 10:
				if (mySeat<2){
					if (players.playerRaiseBefor() == 0){
						nextMsg.action = call;
					}
					else{
						nextMsg.action = fold;
					}
				}
				else{
					nextMsg.action = fold;
				}
				break;
			default:
				break;
			}
		}
	}
	++roundCount[0];
	return nextMsg;
}
ActionMsg Strategy2::inFlop(){
	ofs << "in second Flop" << std::endl;
	ActionMsg nextMsg(fold);
	if (players.getPlayersNum() == 1){
		nextMsg.action = call;
		return nextMsg;
	}
	auto raiseNum = players.playerRaiseBefor();
	auto q = players.playerInTable.find(me);
	q++;
	auto myiter = players.playerInTable.find(me);
	bool afteraggress = false;
	for (; q != myiter; q++){
		if ((*q)->AF > 2.7 && ((*q)->Cbet > 0.6)) {
			afteraggress = true;
		}
	}
	if (raiseNum == 0){
		if (winRate <0.7 && ifRaise[0] == 1 && (me->seat == button)){
			nextMsg.action = raise;
			nextMsg.add_msg = static_cast<Jetton>(0.5*pot);
		}
		else if((winRate > 0.7&&(!afteraggress))||(winRate>0.8 && afteraggress)){
			nextMsg.action = raise;
			nextMsg.add_msg = static_cast<Jetton>((0.3*(dis(gen) % 2) + 0.7)*pot);
		}
		else {
			nextMsg.action = check;
		}
	}
	else if (raiseNum == 1){
		if (((static_cast<double>(shouldBet - me->bet) / (pot + shouldBet - me->bet)
			< winRate) && (winRate < 0.7))){
			nextMsg.action = call;
		}
		else if (winRate>0.98 && (players.playerAllIn() > 0)){
			nextMsg.action = call;
		}
		else if (winRate>0.95 && ((players.playerAllIn() == 0) || roundCount[1] >= 1)){
			nextMsg.action = raise;
			nextMsg.add_msg = shouldRaise;
		}
		else if (winRate>0.85 && ((players.playerAllIn() == 0) || roundCount[1] >= 1) &&
			(shouldBet - me->bet) / (pot + shouldBet - me->bet)	< winRate){
			nextMsg.action = call;
		}
		else if (winRate>0.85 && (players.playerAllIn() == 0) && (shouldRaise <= 0.125*me->jetton) && roundCount[1]<1){
			nextMsg.action = raise;
			nextMsg.add_msg = shouldRaise;
		}
		else if ((static_cast<double>(shouldBet - me->bet) / (pot + shouldBet - me->bet)
			< (winRate + 0.1)) && (winRate > 0.7) && players.getRaisePlayer()->Cbet > 0.65){
			nextMsg.action = call;
		}
		else{
			nextMsg.action = fold;
		}
	}
	else if (raiseNum >= 2){
		if (winRate > 0.95){
			nextMsg.action = raise;
			nextMsg.add_msg = shouldRaise;
		}
		else if (winRate > 0.9){
			nextMsg.action = call;
		}
		else
			nextMsg.action = fold;
	}

	++roundCount[1];
	return nextMsg;
}
ActionMsg Strategy2::inTurn(){
	ofs << "in second Turn" << std::endl;
	ofs << "winRate " << winRate << std::endl;
	ActionMsg nextMsg(fold);
	CardType myCardType_pre = me->getMyCardType(flop_card);
	CardType myCardType = me->getMyCardType(turn_card);
	if (players.getPlayersNum() == 1){
		nextMsg.action = call;
		return nextMsg;
	}
	auto raiseNum = players.playerRaiseBefor();
	if (raiseNum == 0){
		if (myCardType_pre != myCardType && me->HoldCardIn() && winRate>0.8){
			nextMsg.action = raise;
			nextMsg.add_msg = static_cast<Jetton>(1.0*pot);
		}
		else if (winRate > 0.85){
			nextMsg.action = raise;
			nextMsg.add_msg = static_cast<Jetton>((0.15*(dis(gen) % 2) + 0.85)*pot);
		}
		else {
			nextMsg.action = check;
		}
	}
	else if (raiseNum == 1){
		if ((static_cast<double>(shouldBet - me->bet) / (pot + shouldBet - me->bet)< winRate)
			&& (winRate < 0.7) && (winRate>0.55)){
			nextMsg.action = call;
		}
		else if (winRate>0.98 && (players.playerAllIn() > 0)){
			nextMsg.action = call;
		}
		else if (winRate>0.95 && ((players.playerAllIn() == 0) || roundCount[2]>1)){
			nextMsg.action = raise;
			nextMsg.add_msg = shouldRaise;
		}
		else if (winRate>0.8 && (players.playerAllIn() == 0) &&
			(shouldRaise <= 0.125*me->jetton) && roundCount[2] <= 1){
			nextMsg.action = raise;
			nextMsg.add_msg = shouldRaise;
		}
		else if ((static_cast<double>(shouldBet - me->bet) / (pot + shouldBet - me->bet)
			< (winRate + 0.1)) && (winRate > 0.7) && players.getRaisePlayer()->AF > 2.7){
			nextMsg.action = call;
		}
		else{
			nextMsg.action = fold;
		}
	}
	else if (raiseNum >= 2){
		if (winRate > 0.95&& me->HoldCardIn()){
			nextMsg.action = raise;
			nextMsg.add_msg = shouldRaise;
		}
		else if (winRate > 0.9){
			nextMsg.action = call;
		}
		else
			nextMsg.action = fold;
	}
	++roundCount[2];
	return nextMsg;
}
ActionMsg Strategy2::inRiver(){
	ofs << "in second river" << std::endl;
	ActionMsg nextMsg(fold);
	CardType myCardType_pre = me->getMyCardType(turn_card);
	CardType myCardType = me->getMyCardType(river_card);
	if (players.getPlayersNum() == 1){
		nextMsg.action = call;
		return nextMsg;
	}
	auto raiseNum = players.playerRaiseBefor();
	if (raiseNum == 0){
		if (myCardType_pre != myCardType && me->HoldCardIn() && winRate>0.8){
			nextMsg.action = raise;
			nextMsg.add_msg = static_cast<Jetton>(1.0*pot);
		}
		else if (winRate > 0.7){
			nextMsg.action = raise;
			nextMsg.add_msg = static_cast<Jetton>((0.2*(dis(gen) % 2) + 0.5)*pot);
		}
		else {
			nextMsg.action = check;
		}
	}
	else if (raiseNum == 1){
		if (winRate > 0.75){
			nextMsg.action = call;
		}
		else if (winRate>0.98 && (players.playerAllIn() > 0)){
			nextMsg.action = call;
		}
		else if (winRate>0.95 && ((players.playerAllIn() == 0) || roundCount[3] >= 1)){
			nextMsg.action = raise;
			nextMsg.add_msg = shouldRaise;
		}
		else if (winRate>0.85 && (players.playerAllIn() == 0) &&
			(shouldRaise <= 0.125*me->jetton) && roundCount[3] <1){
			nextMsg.action = raise;
			nextMsg.add_msg = shouldRaise;
		}
		else if ((winRate > 0.7) && players.getRaisePlayer()->WTSD <0.5 && (static_cast<double>(shouldBet - me->bet) / (pot + shouldBet - me->bet)
			< winRate)){
			nextMsg.action = call;
		}
		else{
			nextMsg.action = fold;
		}
	}
	else if (raiseNum >= 2){
		if (winRate > 0.95 && me->HoldCardIn()){
			nextMsg.action = raise;
			nextMsg.add_msg = shouldRaise;
		}
		else if (winRate > 0.9){
			nextMsg.action = call;
		}
		else
			nextMsg.action = fold;
	}
	++roundCount[3];
	return nextMsg;
}


LosPassStrategy::LosPassStrategy(Players &players, std::array<int, 4> &roundC,
	ref_type &me, std::ofstream &of, int &totalPlay) :
	Strategy(players, roundC, me, of, totalPlay){
}
/*ActionMsg LosPassStrategy::inHold(){
ofs << "in Losse pass strategy Hold" << std::endl;
auto p = players.playerInTable.find(me);
p--;
prePlayer = *p;
ActionMsg nextMsg(fold);
if (players.getPlayersNum() == 1){
nextMsg.action = call;
}
else{
PokerPair myCard;
nextMsg.action = fold;
if (me->playerCards[0] < me->playerCards[1]){
myCard.a = me->playerCards[0];
myCard.b = me->playerCards[1];
}
else{
myCard.a = me->playerCards[1];
myCard.b = me->playerCards[0];
}
int mySeat = players.mySeat();
myCardLevel = seatPoker.holdCardLevel(myCard);
if (((mySeat == at_head) &&
(myCardLevel <= 2)) ||
((mySeat == at_tail) &&
(myCardLevel <= 5))){
if (players.playerRaiseBefor() == 0){
if (prePlayer->AF < 2.7 ||
prePlayer->FBet > 0.5 ||
prePlayer->F23Bet > 0.75){
nextMsg.action = raise;
nextMsg.add_msg = (3 + dis(gen) % 3) * leastBet;
}
else if (prePlayer->AF > 2.7 &&
prePlayer->FBet < 0.5 &&
prePlayer->F23Bet < 0.75){
if (myCardLevel <= 1 && shouldBet - me->bet < 5 * leastBet){
if (players.playerRaiseBefor() < 3){
nextMsg.action = raise;
nextMsg.add_msg = (2 + dis(gen) % 3) * leastBet;
}
else{
nextMsg.action = call;
}
}
}
}
else if (players.playerAllIn() >= 1){
if (myCardLevel == 0){
nextMsg.action = call;
}
}
else if (players.playerRaiseBefor() == 1
&& roundCount[0] == 0){
if (shouldBet - me->bet < 5 * leastBet || myCardLevel <= 1){
nextMsg.action = call;
}
}
else if (players.playerRaiseBefor() >= 1 &&
players.playerRaiseBefor() <= 5){
if (myCardLevel <= 1){
nextMsg.action = call;
}
}
}
}
++roundCount[0];
return nextMsg;
}*/
ActionMsg LosPassStrategy::inHold(){
	ofs << "in los pass Hold" << std::endl;
	ActionMsg nextMsg(fold);
	if (players.getPlayersNum() == 1){
		nextMsg.action = call;
	}
	else{
		PokerPair myCard;
		nextMsg.action = fold;
		if (me->playerCards[0] < me->playerCards[1]){
			myCard.a = me->playerCards[0];
			myCard.b = me->playerCards[1];
		}
		else{
			myCard.a = me->playerCards[1];
			myCard.b = me->playerCards[0];
		}
		int mySeat = players.mySeat();
		myCardLevel = seatPoker.holdCardLevel(myCard);
		if ((mySeat > 3) && (myCardLevel <= 2)){
			if (prePlayer->F23Bet > 0.6 || prePlayer->FBet > 0.6){
				nextMsg = Strategy::inHold2();
				if (nextMsg.action == raise){
					nextMsg.add_msg -= (1 + dis(gen) % 2) * leastBet;
				}
			}
		}
		else if ((mySeat <= 3)){
			if (myCardLevel <= 6 && players.minJetton() != me->jetton && players.playerRaiseBefor() == 0){
				nextMsg = Strategy::inHold2();
			}
			else if (myCardLevel <= 3){
				nextMsg = Strategy::inHold2();
			}
		}
	}
	return nextMsg;
}
ActionMsg LosPassStrategy::inFlop(){
	ofs << "in in Losse pass strategy Flop" << std::endl;
	ActionMsg nextMsg(fold);
	auto p = players.playerInTable.find(me);
	p--;
	prePlayer = *p;
	if (players.getPlayersNum() == 1){
		nextMsg.action = call;
		return nextMsg;
	}
	if (winRate < 0.5){
		Probability proba(me->playerCards);
		if (proba.isDry() && prePlayer->F23Bet > 0.6
			&& players.playerRaiseBefor() == 0){
			nextMsg.action = raise;
			nextMsg.add_msg = static_cast<Jetton>(0.75 * pot);
		}
	}
	else if (prePlayer->AF < 2){
		if (players.getPlayersNum() == 1){
			nextMsg.action = call;
			return nextMsg;
		}
		auto raiseNum = players.playerRaiseBefor();
		if (raiseNum == 1){
			if (static_cast<double>(shouldBet - me->bet) / (pot + shouldBet - me->bet)
				< winRate / 1.2){
				nextMsg.action = raise;
				nextMsg.add_msg = shouldRaise;
			}
			else if (static_cast<double>(shouldBet - me->bet) / (pot + shouldBet - me->bet)
				< winRate && me->jetton > shouldBet - me->bet){
				nextMsg.action = call;
			}
			else if (static_cast<double>(me->jetton) / (pot + me->jetton)
				< winRate / 2){
				nextMsg.action = call;
			}
			else{
				nextMsg.action = fold;
			}
		}
		else if (raiseNum >= 2){
			if (static_cast<double>(shouldBet - me->bet) / (pot + shouldBet - me->bet)
				< winRate / raiseNum){
				nextMsg.action = call;
			}
			else
				nextMsg.action = fold;
		}
		else{
			Jetton myBet = static_cast<Jetton>((winRate) / (1.5 - (winRate)) * pot);
			Jetton myRaise = static_cast<Jetton>(myBet + me->bet - shouldBet);
			ofs << "mybet, myraise " << myBet << " " << myRaise << std::endl;
			if (myRaise > shouldRaise || myRaise > me->jetton){
				nextMsg.action = raise;
				nextMsg.add_msg = myRaise;
			}
			else{
				nextMsg.action = fold;
			}
		}
		++roundCount[1];
		return nextMsg;
	}
	else if (winRate > 0.85){
		nextMsg.action = call;
	}
	++roundCount[1];
	return nextMsg;
}
ActionMsg LosPassStrategy::inTurn(){
	ofs << "in Losse pass strategy Turn" << std::endl;
	auto p = players.playerInTable.find(me);
	p--;
	prePlayer = *p;
	ActionMsg nextMsg(fold);
	if (players.getPlayersNum() == 1){
		nextMsg.action = call;
		return nextMsg;
	}
	if (winRate < 0.4){
		nextMsg.action = fold;
	}
	else
		if (roundCount[2] == 0 || roundCount[2] == 1){
			if ((shouldBet - me->bet) / (pot + shouldBet - me->bet) < winRate){
				nextMsg.action = call;
			}
			else if ((me->jetton) / (pot + me->jetton) < winRate){
				nextMsg.action = call;
			}
		}
	++roundCount[2];
	return nextMsg;
}
ActionMsg LosPassStrategy::inRiver(){
	ofs << "in Losse pass strategy Flop" << std::endl;
	ActionMsg nextMsg(fold);
	auto p = players.playerInTable.find(me);
	p--;
	prePlayer = *p;
	if (players.getPlayersNum() == 1){
		nextMsg.action = call;
		return nextMsg;
	}
	if (winRate < 0.5){
		Probability proba(me->playerCards);
		if (proba.isDry() && prePlayer->F23Bet > 0.6
			&& players.playerRaiseBefor() == 0){
			nextMsg.action = raise;
			nextMsg.add_msg = static_cast<Jetton>(0.75 * pot);
		}
	}
	else if (prePlayer->AF < 2){
		if (players.getPlayersNum() == 1){
			nextMsg.action = call;
			return nextMsg;
		}
		auto raiseNum = players.playerRaiseBefor();
		if (raiseNum == 1){
			if (static_cast<double>(shouldBet - me->bet) / (pot + shouldBet - me->bet)
				< winRate / 1.5){
				nextMsg.action = raise;
				nextMsg.add_msg = shouldRaise;
			}
			else if (static_cast<double>(shouldBet - me->bet) / (pot + shouldBet - me->bet)
				< winRate / 1.5 && me->jetton > shouldBet - me->bet){
				nextMsg.action = call;
			}
			else if (static_cast<double>(me->jetton) / (pot + me->jetton)
				< winRate / 2){
				nextMsg.action = call;
			}
			else{
				nextMsg.action = fold;
			}
		}
		else if (raiseNum >= 2){
			if (static_cast<double>(shouldBet - me->bet) / (pot + shouldBet - me->bet)
				< winRate / raiseNum){
				nextMsg.action = call;
			}
			else
				nextMsg.action = fold;
		}
	}
	else if (winRate > 0.90){
		Jetton myBet = static_cast<Jetton>(winRate / (1.5 - winRate) * pot);
		Jetton myRaise = static_cast<Jetton>(myBet + me->bet - shouldBet);
		ofs << "mybet, myraise " << myBet << " " << myRaise << std::endl;
		if (myRaise > shouldRaise || myRaise > me->jetton){
			nextMsg.action = raise;
			nextMsg.add_msg = myRaise;
		}
		else{
			nextMsg.action = fold;
		}
	}
	++roundCount[3];
	return nextMsg;
}

LosAgressStrategy::LosAgressStrategy(Players &players, std::array<int, 4> &roundC,
	ref_type &me, std::ofstream &of, int &totalPlay) :
	Strategy(players, roundC, me, of, totalPlay){
}

/*ActionMsg LosAgressStrategy::inHold(){
ofs << "in Losse agree strategy Hold" << std::endl;
auto p = players.playerInTable.find(me);
p--;
prePlayer = *p;
ActionMsg nextMsg(fold);
if (players.getPlayersNum() == 1){
nextMsg.action = call;
}
else{
PokerPair myCard;
nextMsg.action = fold;
if (me->playerCards[0] < me->playerCards[1]){
myCard.a = me->playerCards[0];
myCard.b = me->playerCards[1];
}
else{
myCard.a = me->playerCards[1];
myCard.b = me->playerCards[0];
}
int mySeat = players.mySeat();
myCardLevel = seatPoker.holdCardLevel(myCard);
if (((mySeat == at_head) &&
(myCardLevel <= 2)) ||
((mySeat == at_tail) &&
(myCardLevel <= 5))){
if (players.playerAllIn() > 0
&& players.getPlayersNum() <= 2){
if (myCardLevel == 0){
nextMsg.action = call;
}
}
else if (players.playerRaiseBefor() <= 2){
if (prePlayer->F23Bet > 0.5){
nextMsg.action = raise;
nextMsg.add_msg = (5 + dis(gen) % 6) * leastBet;
}
else if (shouldBet - me->bet < 5 * leastBet){
nextMsg.action = call;
}
}
else if (players.playerRaiseBefor() >= 3 &&
players.playerRaiseBefor() <= 5){
if (myCardLevel <= 0){
nextMsg.action = call;
}
else if (myCardLevel == 1){
if (dis(gen) % 2 == 0){
nextMsg.action = call;
}
}
}
}
}
++roundCount[0];
return nextMsg;
}*/

ActionMsg LosAgressStrategy::inHold(){
	ofs << "in los agress Hold" << std::endl;
	ActionMsg nextMsg(fold);
	if (players.getPlayersNum() == 1){
		nextMsg.action = call;
	}
	else{
		PokerPair myCard;
		nextMsg.action = fold;
		if (me->playerCards[0] < me->playerCards[1]){
			myCard.a = me->playerCards[0];
			myCard.b = me->playerCards[1];
		}
		else{
			myCard.a = me->playerCards[1];
			myCard.b = me->playerCards[0];
		}
		int mySeat = players.mySeat();
		myCardLevel = seatPoker.holdCardLevel(myCard);
		if ((mySeat > 3) && (myCardLevel <= 2)){
			nextMsg = Strategy::inHold2();
		}
		else if ((mySeat <= 3)){
			if (myCardLevel <= 3){
				nextMsg = Strategy::inHold2();
			}
		}
	}
	return nextMsg;
}
ActionMsg LosAgressStrategy::inFlop(){
	ofs << "inFlop" << std::endl;
	ActionMsg nextMsg(fold);
	if (players.getPlayersNum() == 1){
		nextMsg.action = call;
		return nextMsg;
	}
	auto raiseNum = players.playerRaiseBefor();
	if (winRate < 0.4){
		nextMsg.action = fold;
	}
	else if (raiseNum == 1){
		if (static_cast<double>(shouldBet - me->bet) / (pot + shouldBet - me->bet)
			< winRate / 3){
			nextMsg.action = raise;
			nextMsg.add_msg = shouldRaise;
		}
		else if (static_cast<double>(shouldBet - me->bet) / (pot + shouldBet - me->bet)
			< winRate && me->jetton > shouldBet - me->bet){
			nextMsg.action = call;
		}
		else if (static_cast<double>(me->jetton) / (pot + me->jetton)
			< winRate / 1.5){
			nextMsg.action = call;
		}
		else{
			nextMsg.action = fold;
		}
	}
	else if (raiseNum >= 2){
		if (static_cast<double>(shouldBet - me->bet) / (pot + shouldBet - me->bet)
			< 2 * winRate / (raiseNum + 2)){
			nextMsg.action = call;
		}
		else
			nextMsg.action = fold;
	}
	else{
		Jetton myBet = static_cast<Jetton>((winRate) / (1.5 - (winRate)) * pot);
		Jetton myRaise = static_cast<Jetton>(myBet + me->bet - shouldBet);
		ofs << "mybet, myraise " << myBet << " " << myRaise << std::endl;
		if (myRaise > shouldRaise || myRaise > me->jetton){
			nextMsg.action = raise;
			nextMsg.add_msg = myRaise;
		}
		else{
			nextMsg.action = fold;
		}
	}
	++roundCount[1];
	return nextMsg;
}
ActionMsg LosAgressStrategy::inTurn(){
	ofs << "inTurn" << std::endl;
	ofs << "winRate " << winRate << std::endl;
	ActionMsg nextMsg(fold);
	if (players.getPlayersNum() == 1){
		nextMsg.action = call;
		return nextMsg;
	}
	if (winRate < 0.4){
		nextMsg.action = fold;
	}
	else
		if (roundCount[2] == 0 || roundCount[2] == 1){
			if ((shouldBet - me->bet) / (pot + shouldBet - me->bet) < winRate / 2){
				nextMsg.action = call;
			}
			else if ((me->jetton) / (pot + me->jetton) < winRate / 2){
				nextMsg.action = call;
			}
		}
	++roundCount[2];
	return nextMsg;
}
ActionMsg LosAgressStrategy::inRiver(){
	ofs << "inFlop" << std::endl;
	ActionMsg nextMsg(fold);
	if (players.getPlayersNum() == 1){
		nextMsg.action = call;
		return nextMsg;
	}
	auto raiseNum = players.playerRaiseBefor();
	if (winRate < 0.4){
		nextMsg.action = fold;
	}
	else
		if (raiseNum == 1){
			if (static_cast<double>(shouldBet - me->bet) / (pot + shouldBet - me->bet)
				< winRate / 2){
				nextMsg.action = raise;
				nextMsg.add_msg = shouldRaise;
			}
			else if (static_cast<double>(shouldBet - me->bet) / (pot + shouldBet - me->bet)
				< winRate && me->jetton > shouldBet - me->bet){
				nextMsg.action = call;
			}
			else if (static_cast<double>(me->jetton) / (pot + me->jetton)
				< winRate / 1.5){
				nextMsg.action = call;
			}
			else{
				nextMsg.action = fold;
			}
		}
		else if (raiseNum >= 2){
			if (static_cast<double>(shouldBet - me->bet) / (pot + shouldBet - me->bet)
				< winRate / raiseNum){
				nextMsg.action = call;
			}
			else
				nextMsg.action = fold;
		}
		else{
			Jetton myBet = static_cast<Jetton>((winRate) / (1.5 - (winRate)) * pot);
			Jetton myRaise = static_cast<Jetton>(myBet + me->bet - shouldBet);
			ofs << "mybet, myraise " << myBet << " " << myRaise << std::endl;
			if (myRaise > shouldRaise || myRaise > me->jetton){
				nextMsg.action = raise;
				nextMsg.add_msg = myRaise;
			}
			else{
				nextMsg.action = fold;
			}
		}
		++roundCount[3];
		return nextMsg;
}

TighPassStrategy::TighPassStrategy(Players &players, std::array<int, 4> &roundC,
	ref_type &me, std::ofstream &of, int &totalPlay) :
	Strategy(players, roundC, me, of, totalPlay){
}

/*ActionMsg TighPassStrategy::inHold(){
ofs << "in tight pass strategy Hold" << std::endl;
auto p = players.playerInTable.find(me);
p--;
prePlayer = *p;
ActionMsg nextMsg(fold);
if (players.getPlayersNum() == 1){
nextMsg.action = call;
}
else{
PokerPair myCard;
nextMsg.action = fold;
if (me->playerCards[0] < me->playerCards[1]){
myCard.a = me->playerCards[0];
myCard.b = me->playerCards[1];
}
else{
myCard.a = me->playerCards[1];
myCard.b = me->playerCards[0];
}
int mySeat = players.mySeat();
myCardLevel = seatPoker.holdCardLevel(myCard);
if (((mySeat == at_head) &&
(myCardLevel <= 5)) ||
((mySeat == at_tail) &&
(myCardLevel <= 8))){
if (players.playerAllIn() > 0
&& players.getPlayersNum() <= 2){
if (myCardLevel == 0){
nextMsg.action = call;
}
}
else if (players.playerRaiseBefor() <= 2){
if (prePlayer->F23Bet > 0.5  && myCardLevel <= 1){
nextMsg.action = raise;
nextMsg.add_msg = (3 + dis(gen) % 3) * leastBet;
}
else if (shouldBet - me->bet < 5 * leastBet  && myCardLevel <= 2){
nextMsg.action = call;
}
}
else if (players.playerRaiseBefor() >= 3 &&
players.playerRaiseBefor() <= 5){
if (myCardLevel <= 0){
nextMsg.action = call;
}
else if (myCardLevel == 1){
if (dis(gen) % 2 == 0){
nextMsg.action = call;
}
}
}
}
}
++roundCount[0];
return nextMsg;
}*/

ActionMsg TighPassStrategy::inHold(){
	ofs << "in tigh pass Hold" << std::endl;
	ActionMsg nextMsg(fold);
	if (players.getPlayersNum() == 1){
		nextMsg.action = call;
	}
	else{
		PokerPair myCard;
		nextMsg.action = fold;
		if (me->playerCards[0] < me->playerCards[1]){
			myCard.a = me->playerCards[0];
			myCard.b = me->playerCards[1];
		}
		else{
			myCard.a = me->playerCards[1];
			myCard.b = me->playerCards[0];
		}
		int mySeat = players.mySeat();
		myCardLevel = seatPoker.holdCardLevel(myCard);
		if ((mySeat > 3)){
			if (myCardLevel <= 5 && players.playerRaiseBefor() == 0){
				nextMsg = Strategy::inHold2();
			}
			else if (myCardLevel <= 1){
				nextMsg = Strategy::inHold2();
			}
		}
		else if ((mySeat <= 3)){
			if (myCardLevel <= 6 && players.playerRaiseBefor() == 0){
				nextMsg = Strategy::inHold2();
			}
			else if (myCardLevel <=1 || (myCardLevel==5&&myCard.a.color==myCard.b.color)){
				nextMsg = Strategy::inHold2();
			}
		}
	}
	return nextMsg;
}
ActionMsg TighPassStrategy::inFlop(){
	ofs << "inFlop" << std::endl;
	ActionMsg nextMsg(fold);
	if (players.getPlayersNum() == 1){
		nextMsg.action = call;
		return nextMsg;
	}
	auto raiseNum = players.playerRaiseBefor();
	if (winRate < 0.4){
		nextMsg.action = fold;
	}
	else if (raiseNum == 1){
		if (static_cast<double>(shouldBet - me->bet) / (pot + shouldBet - me->bet)
			< winRate / 1.2){
			nextMsg.action = raise;
			nextMsg.add_msg = shouldRaise;
		}
		else if (static_cast<double>(shouldBet - me->bet) / (pot + shouldBet - me->bet)
			< winRate / 2 && me->jetton > shouldBet - me->bet){
			nextMsg.action = call;
		}
		else if (static_cast<double>(me->jetton) / (pot + me->jetton)
			< winRate / 1.5){
			nextMsg.action = call;
		}
		else{
			nextMsg.action = fold;
		}
	}
	else if (raiseNum >= 2){
		if (static_cast<double>(shouldBet - me->bet) / (pot + shouldBet - me->bet)
			< winRate / raiseNum){
			nextMsg.action = call;
		}
		else
			nextMsg.action = fold;
	}
	else{
		Jetton myBet = static_cast<Jetton>((winRate) / (1.5 - (winRate)) * pot);
		Jetton myRaise = static_cast<Jetton>(myBet + me->bet - shouldBet);
		ofs << "mybet, myraise " << myBet << " " << myRaise << std::endl;
		if (myRaise > shouldRaise || myRaise > me->jetton){
			nextMsg.action = raise;
			nextMsg.add_msg = myRaise;
		}
		else{
			nextMsg.action = fold;
		}
	}
	++roundCount[1];
	return nextMsg;
}
ActionMsg TighPassStrategy::inTurn(){
	ofs << "inTurn" << std::endl;
	ofs << "winRate " << winRate << std::endl;
	ActionMsg nextMsg(fold);
	if (players.getPlayersNum() == 1){
		nextMsg.action = call;
		return nextMsg;
	}
	if (winRate < 0.4){
		nextMsg.action = fold;
	}
	else
		if (roundCount[2] == 0 || roundCount[2] == 1){
			if ((shouldBet - me->bet) / (pot + shouldBet - me->bet) < winRate / 1.2){
				nextMsg.action = call;
			}
			else if ((me->jetton) / (pot + me->jetton) < winRate / 2){
				nextMsg.action = call;
			}
		}
	++roundCount[2];
	return nextMsg;
}
ActionMsg TighPassStrategy::inRiver(){
	ofs << "inFlop" << std::endl;
	ActionMsg nextMsg(fold);
	if (players.getPlayersNum() == 1){
		nextMsg.action = call;
		return nextMsg;
	}
	auto raiseNum = players.playerRaiseBefor();
	if (winRate < 0.4){
		nextMsg.action = fold;
	}
	else
		if (raiseNum == 1){
			if (static_cast<double>(shouldBet - me->bet) / (pot + shouldBet - me->bet)
				< winRate / 1.5){
				nextMsg.action = raise;
				nextMsg.add_msg = shouldRaise;
			}
			else if (static_cast<double>(shouldBet - me->bet) / (pot + shouldBet - me->bet)
				< winRate && me->jetton > shouldBet - me->bet){
				nextMsg.action = call;
			}
			else if (static_cast<double>(me->jetton) / (pot + me->jetton)
				< winRate / 2){
				nextMsg.action = call;
			}
			else{
				nextMsg.action = fold;
			}
		}
		else if (raiseNum >= 2){
			if (static_cast<double>(shouldBet - me->bet) / (pot + shouldBet - me->bet)
				< winRate / (2 + raiseNum)){
				nextMsg.action = call;
			}
			else
				nextMsg.action = fold;
		}
		else{
			Jetton myBet = static_cast<Jetton>((winRate) / (1.5 - (winRate)) * pot);
			Jetton myRaise = static_cast<Jetton>(myBet + me->bet - shouldBet);
			ofs << "mybet, myraise " << myBet << " " << myRaise << std::endl;
			if (myRaise > shouldRaise || myRaise > me->jetton){
				nextMsg.action = raise;
				nextMsg.add_msg = myRaise;
			}
			else{
				nextMsg.action = fold;
			}
		}
		++roundCount[3];
		return nextMsg;
}

TighAgresStrategy::TighAgresStrategy(Players &players, std::array<int, 4> &roundC,
	ref_type &me, std::ofstream &of, int &totalPlay) :
	Strategy(players, roundC, me, of, totalPlay){}

/*ActionMsg TighAgresStrategy::inHold(){
ofs << "in tight agress strategy Hold" << std::endl;
auto p = players.playerInTable.find(me);
p--;
prePlayer = *p;
ActionMsg nextMsg(fold);
if (players.getPlayersNum() == 1){
nextMsg.action = call;
}
else{
PokerPair myCard;
nextMsg.action = fold;
if (me->playerCards[0] < me->playerCards[1]){
myCard.a = me->playerCards[0];
myCard.b = me->playerCards[1];
}
else{
myCard.a = me->playerCards[1];
myCard.b = me->playerCards[0];
}
int mySeat = players.mySeat();
myCardLevel = seatPoker.holdCardLevel(myCard);
if (((mySeat == at_head) &&
(myCardLevel <= 2)) ||
((mySeat == at_tail) &&
(myCardLevel <= 3))){
if (players.playerAllIn() > 0
&& players.getPlayersNum() <= 2){
if (myCardLevel == 0){
nextMsg.action = call;
}
}
else if (players.playerRaiseBefor() <= 2){
if (prePlayer->F23Bet > 0.5  && myCardLevel <= 1){
nextMsg.action = raise;
nextMsg.add_msg = (3 + dis(gen) % 3) * leastBet;
}
else if (shouldBet - me->bet < 5 * leastBet  && myCardLevel <= 2){
nextMsg.action = call;
}
}
else if (players.playerRaiseBefor() >= 3 &&
players.playerRaiseBefor() <= 5){
if (myCardLevel == 0){
nextMsg.action = call;
}
else if (myCardLevel == 1){
if (dis(gen) % 2 == 0){
nextMsg.action = call;
}
}
}
}
}
++roundCount[0];
return nextMsg;
}*/

ActionMsg TighAgresStrategy::inHold(){
	ofs << "in tigh agree Hold" << std::endl;
	ActionMsg nextMsg(fold);
	if (players.getPlayersNum() == 1){
		nextMsg.action = call;
	}
	else{
		PokerPair myCard;
		nextMsg.action = fold;
		if (me->playerCards[0] < me->playerCards[1]){
			myCard.a = me->playerCards[0];
			myCard.b = me->playerCards[1];
		}
		else{
			myCard.a = me->playerCards[1];
			myCard.b = me->playerCards[0];
		}
		int mySeat = players.mySeat();
		myCardLevel = seatPoker.holdCardLevel(myCard);
		if ((mySeat > 3)){
			if (myCardLevel <= 5 && players.playerRaiseBefor() == 0){
				nextMsg = Strategy::inHold2();
			}
			else if (myCardLevel <= 1){
				nextMsg = Strategy::inHold2();
			}
		}
		else if ((mySeat <= 3)){
			if (myCardLevel <= 6 && players.playerRaiseBefor() == 0){
				nextMsg = Strategy::inHold2();
			}
			else if (myCardLevel <= 1 || (myCardLevel == 5 && myCard.a.color == myCard.b.color)){
				nextMsg = Strategy::inHold2();
			}
		}
	}
	return nextMsg;
}

ActionMsg TighAgresStrategy::inFlop(){
	ofs << "inFlop" << std::endl;
	ActionMsg nextMsg(fold);
	if (players.getPlayersNum() == 1){
		nextMsg.action = call;
		return nextMsg;
	}
	auto raiseNum = players.playerRaiseBefor();
	if (winRate < 0.45){
		nextMsg.action = fold;
	}
	else if (raiseNum == 1){
		if (static_cast<double>(shouldBet - me->bet) / (pot + shouldBet - me->bet)
			< winRate / 2.5){
			nextMsg.action = raise;
			nextMsg.add_msg = shouldRaise;
		}
		else if (static_cast<double>(shouldBet - me->bet) / (pot + shouldBet - me->bet)
			< winRate && me->jetton > shouldBet - me->bet){
			nextMsg.action = call;
		}
		else if (static_cast<double>(me->jetton) / (pot + me->jetton)
			< winRate / 2){
			nextMsg.action = call;
		}
		else{
			nextMsg.action = fold;
		}
	}
	else if (raiseNum >= 2){
		if (static_cast<double>(shouldBet - me->bet) / (pot + shouldBet - me->bet)
			< winRate / raiseNum){
			nextMsg.action = call;
		}
		else
			nextMsg.action = fold;
	}
	else{
		Jetton myBet = static_cast<Jetton>((winRate) / (1.5 - (winRate)) * pot);
		Jetton myRaise = static_cast<Jetton>(myBet + me->bet - shouldBet);
		ofs << "mybet, myraise " << myBet << " " << myRaise << std::endl;
		if (myRaise > shouldRaise || myRaise > me->jetton){
			nextMsg.action = raise;
			nextMsg.add_msg = myRaise;
		}
		else{
			nextMsg.action = fold;
		}
	}
	++roundCount[1];
	return nextMsg;
}
ActionMsg TighAgresStrategy::inTurn(){
	ofs << "inTurn" << std::endl;
	ofs << "winRate " << winRate << std::endl;
	ActionMsg nextMsg(fold);
	if (players.getPlayersNum() == 1){
		nextMsg.action = call;
		return nextMsg;
	}
	if (winRate < 0.4){
		nextMsg.action = fold;
	}
	else
		if (roundCount[2] == 0 || roundCount[2] == 1){
			if ((shouldBet - me->bet) / (pot + shouldBet - me->bet) < winRate / 2){
				nextMsg.action = call;
			}
			else if ((me->jetton) / (pot + me->jetton) < winRate / 3){
				nextMsg.action = call;
			}
		}
	++roundCount[2];
	return nextMsg;
}
ActionMsg TighAgresStrategy::inRiver(){
	ofs << "inFlop" << std::endl;
	ActionMsg nextMsg(fold);
	if (players.getPlayersNum() == 1){
		nextMsg.action = call;
		return nextMsg;
	}
	auto raiseNum = players.playerRaiseBefor();
	if (winRate < 0.4){
		nextMsg.action = fold;
	}
	else
		if (raiseNum == 1){
			if (static_cast<double>(shouldBet - me->bet) / (pot + shouldBet - me->bet)
				< winRate / 2){
				nextMsg.action = raise;
				nextMsg.add_msg = shouldRaise;
			}
			else if (static_cast<double>(shouldBet - me->bet) / (pot + shouldBet - me->bet)
				< winRate && me->jetton > shouldBet - me->bet){
				nextMsg.action = call;
			}
			else if (static_cast<double>(me->jetton) / (pot + me->jetton)
				< winRate / 2){
				nextMsg.action = call;
			}
			else{
				nextMsg.action = fold;
			}
		}
		else if (raiseNum >= 2){
			if (static_cast<double>(shouldBet - me->bet) / (pot + shouldBet - me->bet)
				< winRate / raiseNum){
				nextMsg.action = call;
			}
			else
				nextMsg.action = fold;
		}
		else{
			Jetton myBet = static_cast<Jetton>((winRate) / (1.5 - (winRate)) * pot);
			Jetton myRaise = static_cast<Jetton>(myBet + me->bet - shouldBet);
			ofs << "mybet, myraise " << myBet << " " << myRaise << std::endl;
			if (myRaise > shouldRaise || myRaise > me->jetton){
				nextMsg.action = raise;
				nextMsg.add_msg = myRaise;
			}
			else{
				nextMsg.action = fold;
			}
		}
		++roundCount[3];
		return nextMsg;
}

