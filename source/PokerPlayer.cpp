#include "stdafx.h"
#include "PokerPlayer.h"

PokerPlayer::PokerPlayer() : type(normal){
	for (int i = 0; i != 9; ++i){
		handCardType[i] = 0;
	}
	playerID = 0;
	foldC = 0; raiseC = 0; all_inC = 0; checkC = 0; callC = 0;
	joinC = 0; winC = 0; showC = 0; should_loseC = 0; true_winC = 0;
	totalPlay = 0;
	winRate = 0;
	bet = 0;
	F23BetC = 0; NF23BetC = 0; _3BetC = 0; N3BetC = 0; FBetC = 0; NFBetC = 0;
	PFRC = 0; CBetC = 0; NCBetC = 0; WTSDC = 0;
	VPIP = 0; PFR = 0; AF = 0; _3Bet = 0; F23Bet = 0; Cbet = 0; FBet = 0; WTSD = 0;
	lastAction = attend;
}

PokerPlayer::PokerPlayer(PID playerID) : playerID(playerID), type(normal){
	for (int i = 0; i != 9; ++i){
		handCardType[i] = 0;
	}
	foldC = 0; raiseC = 0; all_inC = 0; checkC = 0; callC = 0;
	joinC = 0; winC = 0; showC = 0; should_loseC = 0; true_winC = 0;
	totalPlay = 0;
	winRate = 0;
	bet = 0;
	F23BetC = 0; NF23BetC = 0; _3BetC = 0; N3BetC = 0; FBetC = 0; NFBetC = 0;
	PFRC = 0; CBetC = 0; NCBetC = 0; WTSDC = 0;
	VPIP = 0; PFR = 0; AF = 0; _3Bet = 0; F23Bet = 0; Cbet = 0; FBet = 0;  WTSD = 0;
	lastAction = attend;
}

PokerPlayer::~PokerPlayer(){}

void PokerPlayer::getPlayerMsg(Message &msg){
	msg.getPlayerMsg(*this);
	if (totalPlay > 10){
		VPIP = static_cast<double>(joinC) / totalPlay;
		PFR = static_cast<double>(PFRC) / totalPlay;
		if (callC > 0)
			AF = static_cast<double>(raiseC) / callC;
		else if (foldC != 0)
			AF = 0;
		else
			AF = 100;
		if (_3BetC + N3BetC > 0)
			_3Bet = static_cast<double>(_3BetC) / (_3BetC + N3BetC);
		else
			_3Bet = 0;
		if (F23BetC + NF23BetC > 0){
			F23Bet = static_cast<double>(F23BetC) / (F23BetC + NF23BetC);
		}
		else
			F23Bet = 0;
		if (CBetC + NCBetC > 0){
			Cbet = static_cast<double>(CBetC) / (CBetC + NCBetC);
		}
		else
			Cbet = 0.0001;
		if (showC > 0){
			WTSD = static_cast<double>(WTSDC) / showC;
		}
		else
			WTSD = 0;
		if (FBetC + NFBetC > 0){
			FBet = static_cast<double>(FBetC) / (FBetC + NFBetC);
		}
		else
			FBet = 0;
		if (VPIP > 0.23){
			if (PFR < 0.1){
				type = loose_pass;
			}
			else
				type = loose_aggre;
		}
		else if (VPIP < 0.14){
			if (PFR > 0.06){
				type = tight_aggre;
			}
			else
				type = tight_pass;
		}
		else
			type = normal;
	}
	else type = normal;
}

CardType PokerPlayer::getMyCardType(Circle cir){
	auto cards = playerCards;
	if (cir == flop_card){
		cards[5] = _0;
		cards[6] = _1;
		Pokers pokers(cards);
		return pokers.getCardType();
	}
	else if (cir == turn_card){
		cards[6] = _0;
		Pokers pokers(cards);
		return pokers.getCardType();
	}
	else{
		Pokers pokers(cards);
		return pokers.getCardType();
	}
}

bool operator < (const PokerPlayer &lhs, const PokerPlayer &rhs){
	if (lhs.playerID < rhs.playerID)
		return true;
	return false;
}
bool operator == (const PokerPlayer &lhs, const PokerPlayer &rhs){
	if (lhs.playerID == rhs.playerID)
		return true;
	return false;
}

std::ostream &operator<<(std::ostream &os, const PokerPlayer &hs){
	os << "playerID " << hs.playerID << "\ttype: " << hs.type << std::endl;
	os << "VPIP\tPFR\tAF\t3Bet\tF2Bet\tCBet\tFBet\tWTSD\n";
	os << std::setprecision(2) << hs.VPIP << "\t" << hs.PFR << "\t" << hs.AF << "\t" << hs._3Bet
		<< "\t" << hs.F23Bet << "\t" << hs.Cbet << "\t" << hs.FBet << "\t" << hs.WTSD << "\n";
	os << "tPlay\tJoinC\tshowC\twinC\tPFRC\tWTSDC\n";
	os << std::setprecision(2) << hs.totalPlay << "\t" << hs.joinC << "\t" << hs.showC << "\t"
		<< hs.winC << "\t" << hs.PFRC << "\t" << hs.WTSDC;
	return os;
}