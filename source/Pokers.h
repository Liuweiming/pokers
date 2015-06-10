#pragma once
#include <array>
#include <algorithm>
#include <iostream>
enum CardType{
	straight_flush, four_of_a_kind, full_house, flush, straight,
	three_of_a_kind, two_pair, one_pair, high_card
};
enum PokerColor{ spades, hearts, clubs, diamonds, noncolor };
enum PokerPoint{ _A, _K, _Q, _J, _10, _9, _8, _7, _6, _5, _4, _3, _2, _1, _0 };

class Poker{
public:
	typedef int pokerOrder;
	friend bool operator == (const Poker &lhs, const Poker &rhs);
	friend bool operator != (const Poker &lhs, const Poker &rhs);
	friend bool operator < (const Poker &lhs, const Poker &rhs);
	friend bool operator > (const Poker &lhs, const Poker &rhs);
	friend Poker operator + (Poker lhs, pokerOrder addPoints);
	friend Poker operator - (Poker lhs, pokerOrder addPoints);
	friend std::ostream &operator << (std::ostream &os, const Poker &ohs);
	PokerColor color;
	PokerPoint point;
public:
	Poker(){}
	Poker(pokerOrder col, pokerOrder poi) : color(static_cast<PokerColor>(col)),
		point(static_cast<PokerPoint>(poi)){}
	Poker(pokerOrder order) : color(static_cast<PokerColor>(order % 4)),
		point(static_cast<PokerPoint>(order / 4)){}
	Poker(PokerColor col, PokerPoint poi) : color(col), point(poi){}
	Poker(PokerPoint poi) : color(noncolor), point(poi){}
	Poker(PokerColor col) : color(col), point(_0){}
	Poker(const Poker &old) : color(old.color), point(old.point){}
	~Poker(){}
	Poker &operator = (const Poker &old);
	Poker &operator += (pokerOrder addPoints);
	Poker &operator -= (pokerOrder addPoints);
};

class Probability;
class Pokers
{
public:
	friend bool operator == (const Pokers &lhs, const Pokers &rhs);
	friend bool operator != (const Pokers &lhs, const Pokers &rhs);
	friend bool operator < (const Pokers &lhs, const Pokers &rhs);
	friend bool operator > (const Pokers &lhs, const Pokers &rhs);
	friend std::ostream &operator << (std::ostream &os, const Pokers &ohs);
	friend class Probability;
private:
	std::array<Poker, 7> pokers;
	std::array<Poker, 5> finished_pokers;
	CardType cardType;
	bool NOT_FLUSH;
	void cardtypes();
	void fullCardTypes();
	bool isStraight(const std::array<Poker, 7> &pokerc);
	bool isStraightFlush();
	bool isFlush();
	bool isSameKind();
	inline void getResult(const std::array<Poker, 7> &cPokers);
	int sstraight(const std::array<Poker, 7> &cPokers,
		std::array<Poker, 7> &ret);
	int fflush(const std::array<Poker, 7> &cPokers,
		std::array<Poker, 7> &ret);
	int sameKind(const std::array<Poker, 7> &cPokers,
		std::array<Poker, 7> &ret);
public:
	Pokers(std::array<Poker, 7> &pok, bool not_F) : pokers(pok),
		cardType(high_card), NOT_FLUSH(not_F) {
		cardtypes();
	}
	Pokers(std::array<Poker, 7> &pok) : pokers(pok),
		cardType(high_card), NOT_FLUSH(true){
		fullCardTypes();
	}
	template <typename T>
	Pokers(T beg, T end, bool not_F) : NOT_FLUSH(not_F), cardType(high_card) {
		if (beg + 7 <= end)
			for (int i = 0; i != 7; ++i){
				pokers[i] = *(beg + i);
			}
		cardtypes();
	}
	Pokers(const Pokers &old) : pokers(old.pokers),
		finished_pokers(old.finished_pokers),
		cardType(old.cardType), NOT_FLUSH(old.NOT_FLUSH){}
	Pokers &operator=(const Pokers &old){
		pokers = old.pokers;
		finished_pokers = old.finished_pokers;
		cardType = old.cardType;
		NOT_FLUSH = old.NOT_FLUSH;
		return *this;
	}
	bool HoldCardIn(){
		auto p = std::find(finished_pokers.begin(), finished_pokers.end(), pokers[0]);
		auto q = std::find(finished_pokers.begin(), finished_pokers.end(), pokers[1]);
		if (p != finished_pokers.end() && q != finished_pokers.end()){
			return true;
		}
		return false;
	}
	CardType getCardType(){
		return cardType;
	}
	~Pokers(){}
};



