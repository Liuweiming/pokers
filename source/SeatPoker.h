#pragma once
#include <array>
#include "Pokers.h"
struct PokerPair;
bool operator==(const PokerPair &lhs, const PokerPair &rhs);
struct PokerPair{
	Poker a;
	Poker b;
	friend bool operator==(const PokerPair &lhs, const PokerPair &rhs);
};

class SeatPoker{
public:
	SeatPoker(){}
	~SeatPoker(){}
	int holdCardLevel(const PokerPair &pokers){
		if ((pokers.a == _A && pokers.b == _A) ||
			(pokers.a == _K && pokers.b == _K) ||
			(pokers.a == _A && pokers.b == _K) ||
			(pokers.a == _Q && pokers.b == _Q))
			return 0;
		else if ((pokers.a == _J && pokers.b == _J) ||
			(pokers.a == _10 && pokers.b == _10) ||
			(pokers.a == _A && pokers.b == _Q))
			return 1;
		else if ((pokers.a == _9 && pokers.b == _9) ||
			(pokers.a == _8 && pokers.b == _8) ||
			(pokers.a == _A && pokers.b == _J) ||
			((pokers.a == _A && pokers.b == _10) &&
			(pokers.a.color == pokers.b.color)))
			return 2;
		else if ((pokers.a == _7 && pokers.b == _7) ||
			(pokers.a == _A && pokers.b == _10) ||
			((pokers.a == _J && pokers.b == _10) &&
			(pokers.a.color == pokers.b.color)) ||
			((pokers.a == _10 && pokers.b == _9) &&
			(pokers.a.color == pokers.b.color)))
			return 3;
		else if ((pokers.a == _6 && pokers.b == _6) ||
			(pokers.a == _A && pokers.b == _9) ||
			((pokers.a == _A && pokers.b == _8) &&
			(pokers.a.color == pokers.b.color)) ||
			(pokers.a == _K && pokers.b == _Q))
			return 4;
		else if (((pokers.a < _5 && pokers.b == pokers.a + 1)
			&& (pokers.a.color == pokers.b.color)) ||
			(pokers.a == _A && pokers.b == _8) ||
			(pokers.a == _K && pokers.b == _J) ||
			(pokers.a == _Q && pokers.b == _J) ||
			(pokers.a<_9&&pokers.b<_9&&pokers.a.color == pokers.b.color))
			return 5;
		else if ((pokers.a == _5 && pokers.b == _5) ||
			(pokers.a == _4 && pokers.b == _4) ||
			(pokers.a == _3 && pokers.b == _3) ||
			(pokers.a == _2 && pokers.b == _2))
			return 6;
		else if ((pokers.a == _A))
			return 7;
		else if ((pokers.a < _4 && pokers.b == pokers.a + 1)
			&& (pokers.a.color == pokers.b.color))
			return 8;
		else if ((pokers.b < _4) &&
			(pokers.b < pokers.a + 4) &&
			(pokers.a.color == pokers.b.color))
			return 9;
		else if ((pokers.a < _7) &&
			(pokers.b < pokers.a + 5))
			return 10;
		return 11;
	}
};