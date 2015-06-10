#pragma once
#include "Pokers.h"
#include <array>
#define PointNum 13
#define MinPointNum 13
#define ColorNum 4

class Probability
{
private:
	std::array<Poker, 7> myCards, othCards;
	std::array<std::size_t, 9> handCardType, myHandCardType;
	double MoreWinFactor;
	int total;
	int n;
	int pointc[PointNum];
	int colorc[ColorNum];
	int pokerc[PointNum][ColorNum];
	int myWin, oWin;
	bool shouldUpdate;
	void distOther(int pbeg, int cbeg, int i);
	void distOtherCard(int beg, int i, const int &next);
	void distOtherColor(int beg, int i, const int &next);
	void distCommonCard(int beg, int i);
	void distCommonColor(int beg, int i);
	void calPros(int turn, int river);
	void calPokers();
	void calColor(int turn, int river);
	void calDry(){
		Pokers op(othCards);
		handCardType[op.cardType] += 1;
		++total;
	}
	void ini(int round);
	double cal_3();
	double cal_more();
	double cal_more_3();
public:
	Probability();
	Probability(std::array<Poker, 7> &myC, std::array<Poker, 7> &othC);
	Probability(std::array<Poker, 7> &myC);
	void setMyCard(std::array<Poker, 7> &myC);
	void setTurn(Poker &turn);
	void setRiver(Poker &river);
	~Probability();
	double calFlop_3();
	double calFlop_more();
	double calFlop_2();
	double calFlop_2_know();
	double calTurn_2();
	double calTurn_3();
	double calTurn_more();
	double calRiver(int playerNum);
	double calFlop(int playerNum);
	double calTurn(int playerNum);
	bool isDry(){
		ini(5);
		othCards[5] = Poker(_0);
		othCards[6] = Poker(_1);
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
				calDry();
			}
		}
		double dry = 0;
		for (int i = 0; i != 6; ++i){
			dry += handCardType[i];
		}
		if (dry / total < 0.2)
			return true;
		return false;
	}
};

