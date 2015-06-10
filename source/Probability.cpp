#include "stdafx.h"
#include "Probability.h"



Probability::Probability(){}
Probability::Probability(std::array<Poker, 7> &myC, std::array<Poker, 7> &othC) :
myCards(myC), othCards(othC), myWin(0), oWin(0){
	for (int i = 2; i != 5; ++i){
		othCards[i] = myCards[i];
	}
}
Probability::Probability(std::array<Poker, 7> &myC) :
myCards(myC), myWin(0), oWin(0){
	for (int i = 2; i != 5; ++i){
		othCards[i] = myCards[i];
	}
}
void Probability::setMyCard(std::array<Poker, 7> &myC){
	myCards = myC;
	for (int i = 2; i != 5; ++i){
		othCards[i] = myCards[i];
	}
}
void Probability::setTurn(Poker &turn){
	myCards[5] = turn;
}
void Probability::setRiver(Poker &river){
	myCards[6] = river;
}

double Probability::calFlop_3(){
	ini(5);
	distOtherCard(0, 0, 5);
	distOtherColor(0, 0, 5);
	return cal_3();
}
double Probability::calFlop_more(){
	ini(5);
	distOtherCard(0, 0, 5);
	distOtherColor(0, 0, 5);
	MoreWinFactor = 0.6;
	return cal_more();
}
double Probability::calFlop_2(){
	ini(5);
	distOtherCard(0, 0, 5);
	distOtherColor(0, 0, 5);
	return static_cast<double>(myWin) / (myWin + oWin);
}
double Probability::calFlop_2_know(){
	ini(5);
	for (int i = 0; i != 2; ++i){
		--pointc[othCards[i].point];
		--colorc[othCards[i].color];
	}
	distCommonCard(0, 5);
	distCommonColor(0, 5);
	return static_cast<double>(myWin) / (myWin + oWin);
}
double Probability::calTurn_2(){
	ini(6);
	distOtherCard(0, 0, 6);
	distOtherColor(0, 0, 6);
	return static_cast<double>(myWin) / (myWin + oWin);
}
double Probability::calTurn_3(){
	ini(6);
	distOtherCard(0, 0, 6);
	distOtherColor(0, 0, 6);
	return cal_3();
}
double Probability::calTurn_more(){
	ini(6);
	distOtherCard(0, 0, 6);
	distOtherColor(0, 0, 6);
	MoreWinFactor = 0.8;
	return cal_more_3();
}
double Probability::calRiver(int playerNum){
	ini(7);
	distOther(0, 0, 0);
	return static_cast<double>(myWin) / (myWin + oWin);
}
double Probability::calFlop(int playerNum){
	if (playerNum == 2){
		return calFlop_2();
	}
	else if (playerNum == 3){
		return calFlop_3();
	}
	else{
		return calFlop_more();
	}
}
double Probability::calTurn(int playerNum){
	if (playerNum == 2){
		return calTurn_2();
	}
	else if (playerNum == 3){
		return calTurn_3();
	}
	else{
		return calTurn_more();
	}
}




Probability::~Probability()
{
}

void Probability::distOther(int pbeg, int cbeg, int i){
	for (int k = 0; k != MinPointNum * ColorNum; ++k){
		if (pokerc[k / 4][k % 4] == 0){
			continue;
		}
		Poker tmp(k);
		othCards[0] = tmp;
		for (int m = k + 1; m != MinPointNum * ColorNum; ++m){
			if (pokerc[m / 4][m % 4] == 0){
				continue;
			}
			Poker tmp2(m);
			othCards[1] = tmp2;
			calPokers();
		}
	}
}

void Probability::distOtherCard(int beg, int i, const int &next){
	for (int k = beg; k != MinPointNum; ++k){
		if (pointc[k] == 0)
			continue;
		Poker tmp(static_cast<PokerPoint>(k));
		--pointc[k];
		othCards[i] = tmp;
		if (i == 1){
			distCommonCard(0, next);
		}
		else
			distOtherCard(k, i + 1, next);
		++pointc[k];
	}
}

void Probability::distOtherColor(int beg, int i, const int &next){
	for (int k = beg; k != ColorNum; ++k){
		if (colorc[k] == 0)
			continue;
		Poker tmp(static_cast<PokerColor>(k));
		--colorc[k];
		othCards[i] = tmp;
		if (i == 1){
			distCommonColor(0, next);
		}
		else
			distOtherColor(k, i + 1, next);
		++colorc[k];
	}
}

void Probability::distCommonCard(int beg, int i){
	for (int k = beg; k != PointNum; ++k){
		if (pointc[k] == 0)
			continue;
		Poker tmp(static_cast<PokerPoint>(k));
		--pointc[k];
		othCards[i] = tmp;
		myCards[i] = tmp;
		if (i == 6){
			calPros(beg, k);
		}
		else{
			distCommonCard(k, i + 1);
		}
		++pointc[k];
	}
}

void Probability::distCommonColor(int beg, int i){
	for (int k = beg; k != ColorNum; ++k){
		if (colorc[k] == 0)
			continue;
		Poker tmp(static_cast<PokerColor>(k));
		--colorc[k];
		othCards[i] = tmp;
		myCards[i] = tmp;
		if (i == 6){
			calColor(beg, k);
		}
		else{
			distCommonColor(k, i + 1);
		}
		++colorc[k];
	}
}

void Probability::calPros(int turn, int river){
	Pokers mp(myCards, true);
	Pokers op(othCards, true);
	int tmp1, tmp2;
	if (turn == river)
		tmp1 = ((pointc[turn] + 2) * (pointc[river] + 1)) / 2;
	else
		tmp1 = (pointc[turn] + 1) * (pointc[river] + 1);
	int point1 = pointc[op.pokers[0].point];
	int point2 = pointc[op.pokers[1].point];
	if (op.pokers[0] == op.pokers[1]){
		tmp2 = (point1 + 2) * (point2 + 1) / 2;
	}
	else
		tmp2 = (point1 + 1) * (point2 + 1);
	int all = tmp1 * tmp2;
	if (mp > op){
		myHandCardType[mp.cardType] += all;
		myWin += all;
	}
	else{
		handCardType[op.cardType] += all;
		oWin += all;
	}
	total += all;
}

void Probability::calPokers(){
	Pokers mp(myCards);
	Pokers op(othCards);
	if (mp > op){
		myHandCardType[mp.cardType] += 1;
		myWin += 1;
	}
	else{
		handCardType[op.cardType] += 1;
		oWin += 1;
	}
	total += 1;
}
void Probability::calColor(int turn, int river){
	Pokers mp(myCards, false);
	Pokers op(othCards, false);
	if (mp.getCardType() == CardType::flush || op.getCardType() == CardType::flush){
		int tmp1, tmp2;
		if (turn == river)
			tmp1 = ((colorc[turn] + 2) * (colorc[river] + 1)) / 2;
		else
			tmp1 = (colorc[turn] + 1) * (colorc[river] + 1);
		int color1 = colorc[op.pokers[0].color];
		int color2 = colorc[op.pokers[1].color];
		if (op.pokers[0] == op.pokers[1]){
			tmp2 = (color1 + 2) * (color2 + 1) / 2;
		}
		else
			tmp2 = (color1 + 1) * (color2 + 1);
		int all = tmp1 * tmp2;
		if (mp > op){
			myHandCardType[mp.cardType] += all;
			myWin += all;
		}
		else{
			handCardType[op.cardType] += all;
			oWin += all;
		}
		total += all;
	}
}
void Probability::ini(int round){
	myWin = oWin = 0;
	total = 0;
	for (int i = 0; i != 9; ++i){
		handCardType[i] = 0;
		myHandCardType[i] = 0;
	}
	for (int i = 0; i != PointNum; ++i){
		pointc[i] = ColorNum;
	}
	for (int i = 0; i != ColorNum; ++i){
		colorc[i] = PointNum;
	}
	for (int i = 0; i != PointNum; ++i){
		for (int j = 0; j != ColorNum; ++j)
			pokerc[i][j] = 1;
	}
	for (int i = 5; i != round; ++i){
		othCards[i] = myCards[i];
	}
	for (int i = 0; i != round; ++i){
		--pointc[myCards[i].point];
		--colorc[myCards[i].color];
		--pokerc[myCards[i].point][myCards[i].color];
	}
}
double Probability::cal_3(){
	Pokers mp(myCards, true);
	double winRate = static_cast<double>(myWin) / (myWin + oWin);
	if (mp.cardType == three_of_a_kind){
		winRate *= 0.9;
	}
	else if (mp.cardType == two_pair){
		winRate *= 0.7;
	}
	else if (mp.cardType == one_pair || mp.cardType == high_card){
		winRate *= 0.5;
	}
	return winRate;
}
double Probability::cal_more(){
	unsigned firMax = 0, firI = 0;
	unsigned secMax = 0, secI = 0;
	unsigned thirMax = 0, thirI = 0;
	double firWin = 0, secWin = 0, thirWin = 0;
	for (unsigned i = 0; i != 9; ++i){
		if (static_cast<double>(handCardType[i]) /
			(handCardType[i] + myHandCardType[i]) > firWin){
			firWin = static_cast<double>(handCardType[i]) /
				(handCardType[i] + myHandCardType[i]);
			firMax = handCardType[i];
			firI = i;
		}
	}
	for (unsigned i = 0; i != 9; ++i){
		if (static_cast<double>(handCardType[i]) /
			(handCardType[i] + myHandCardType[i]) > secWin && i != firI){
			secWin = static_cast<double>(handCardType[i]) /
				(handCardType[i] + myHandCardType[i]);
			secMax = handCardType[i];
			secI = i;
		}
	}
	for (unsigned i = 0; i != 9; ++i){
		if (static_cast<double>(handCardType[i]) /
			(handCardType[i] + myHandCardType[i]) > thirWin &&
			i != firI && i != secI){
			thirWin = static_cast<double>(handCardType[i]) /
				(handCardType[i] + myHandCardType[i]);
			thirMax = handCardType[i];
			thirI = i;
		}
	}
	unsigned shouldWin = firMax + secMax + thirMax;
	unsigned shouldLose = oWin - shouldWin;
	double mySWin = myHandCardType[firI] + myHandCardType[secI] + myHandCardType[thirI];
	double mySLose = myWin - mySWin;
	double winRate = mySLose / (shouldLose + mySLose) * (1 - MoreWinFactor) +
		mySWin / (shouldWin + mySWin) * MoreWinFactor;
	return winRate;
}
double Probability::cal_more_3(){
	unsigned firMax = 0, firI = 0;
	unsigned secMax = 0, secI = 0;
	double firWin = 0, secWin = 0;
	for (unsigned i = 0; i != 9; ++i){
		if (static_cast<double>(handCardType[i]) /
			(handCardType[i] + myHandCardType[i]) > firWin){
			firWin = static_cast<double>(handCardType[i]) /
				(handCardType[i] + myHandCardType[i]);
			firMax = handCardType[i];
			firI = i;
		}
	}
	for (unsigned i = 0; i != 9; ++i){
		if (static_cast<double>(handCardType[i]) /
			(handCardType[i] + myHandCardType[i]) > secWin && i != firI){
			secWin = static_cast<double>(handCardType[i]) /
				(handCardType[i] + myHandCardType[i]);
			secMax = handCardType[i];
			secI = i;
		}
	}
	unsigned shouldWin = firMax + secMax;
	unsigned shouldLose = oWin - shouldWin;
	double mySWin = myHandCardType[firI] + myHandCardType[secI];
	double mySLose = myWin - mySWin;
	double winRate = mySLose / (shouldLose + mySLose) * (1 - MoreWinFactor) +
		mySWin / (shouldWin + mySWin) * MoreWinFactor;
	return winRate;
}