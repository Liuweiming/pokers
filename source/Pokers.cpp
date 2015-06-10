#include "stdafx.h"
#include "Pokers.h"

Poker &Poker::operator = (const Poker &old){
	color = old.color;
	point = old.point;
	return *this;
}
Poker &Poker::operator += (pokerOrder addPoints){
	if (point - addPoints >= _A)
		point = static_cast<PokerPoint>(point - addPoints);
	return *this;
}
Poker &Poker::operator -= (pokerOrder addPoints){
	if (point + addPoints <= _2)
		point = static_cast<PokerPoint>(point + addPoints);
	return *this;
}
bool operator == (const Poker &lhs, const Poker &rhs){
	if (lhs.point == rhs.point)
		return true;
	return false;
}
bool operator != (const Poker &lhs, const Poker &rhs){
	return !(lhs == rhs);
}
bool operator > (const Poker &lhs, const Poker &rhs){
	if (lhs.point * 4 + lhs.color > rhs.point * 4 + rhs.color)
		return true;
	return false;
}
bool operator < (const Poker &lhs, const Poker &rhs){
	if (lhs.point * 4 + lhs.color < rhs.point * 4 + rhs.color)
		return true;
	return false;
}
Poker operator + (Poker lhs, Poker::pokerOrder addPoints){
	lhs += addPoints;
	return lhs;
}
Poker operator - (Poker lhs, Poker::pokerOrder addPoints){
	lhs -= addPoints;
	return lhs;
}
std::ostream &operator << (std::ostream &os, const Poker &ohs){
	os << ohs.point;
	return os;
}

void Pokers::getResult(const std::array<Poker, 7> &cPokers){
	for (int i = 0; i != 5; ++i){
		finished_pokers[i] = cPokers[i];
	}
}
void Pokers::fullCardTypes(){
	if (isStraightFlush()){
		return;
	}
	else if (isStraight(pokers)){
		return;
	}
	isSameKind();
	if (cardType == high_card){
		std::array<Poker, 7> ret = pokers;
		std::sort(ret.begin(), ret.end());
		getResult(ret);
	}
}
void Pokers::cardtypes(){
	if (NOT_FLUSH){
		if (isStraight(pokers)){
			return;
		}
		isSameKind();
	}
	else{
		if (isFlush())
			return;
	}
	if (cardType == high_card){
		std::array<Poker, 7> ret = pokers;
		std::sort(ret.begin(), ret.end());
		getResult(ret);
	}
}
bool Pokers::isStraight(const std::array<Poker, 7> &pokerc){
	std::array<Poker, 7> ret;
	int size = sstraight(pokerc, ret);
	if (size >= 5){
		cardType = straight;
		getResult(ret);
		return true;
	}
	return false;
}

bool Pokers::isStraightFlush(){
	std::array<Poker, 7> ret;
	int size = fflush(pokers, ret);
	if (size >= 5){
		if (isStraight(ret)){
			cardType = straight_flush;
		}
		else{
			cardType = CardType::flush;
			getResult(ret);
		}
		return true;
	}
	return false;
}
bool Pokers::isFlush(){
	std::array<Poker, 7> ret;
	int size = fflush(pokers, ret);
	if (size >= 5){
		cardType = CardType::flush;
		getResult(ret);
		return true;
	}
	return false;
}
bool Pokers::isSameKind(){
	std::array<Poker, 7> ret;
	auto size = sameKind(pokers, ret);
	if (size == 5){
		if (ret[0] == ret[3]){
			if (cardType > four_of_a_kind){
				cardType = four_of_a_kind;
				for (int i = 0; i != 4; ++i){
					finished_pokers[i + 1] = ret[i];
				}
				finished_pokers[0] = ret[4];
			}
		}
		if (ret[0] == ret[2]){
			if (ret[4] == ret[3]){
				if (cardType > full_house){
					cardType = full_house;
					getResult(ret);
				}
			}
			else
				if (cardType > three_of_a_kind){
					cardType = three_of_a_kind;
					getResult(ret);
				}
		}
		if (ret[0] == ret[1]){
			if (ret[4] == ret[2]){
				if (cardType > full_house){
					cardType = full_house;
					for (int i = 0; i != 2; ++i){
						finished_pokers[i + 3] = ret[i];
					}
					for (int i = 2; i != 5; ++i){
						finished_pokers[i - 2] = ret[i];
					}
				}
			}
			else if (ret[2] == ret[3]){
				if (cardType > two_pair){
					cardType = two_pair;
					getResult(ret);
				}
			}
			else{
				if (cardType > one_pair){
					cardType = one_pair;
					getResult(ret);
				}
			}
		}
		return true;
	}
	return false;
}
int Pokers::sstraight(const std::array<Poker, 7> &cPokers,
	std::array<Poker, 7> &ret){
	std::array<Poker, 7> tmp = cPokers;
	std::sort(tmp.begin(), tmp.end());
	auto end = std::unique(tmp.begin(), tmp.end());
	auto size = end - tmp.begin();
	if (size < 5){
		return 0;
	}
	std::sort(tmp.begin(), end);
	if (tmp[0] == _A){
		if (tmp[size - 1] == _2 &&
			tmp[size - 2] == _3 &&
			tmp[size - 3] == _4 &&
			tmp[size - 4] == _5
			){
			ret[0] = tmp[0];
			for (int i = 1; i != 5; ++i){
				ret[i] = tmp[size - 5 + i];
			}
			return 5;
		}
	}
	int j = 1;
	ret[0] = tmp[0];
	for (int i = 0; i != size - 1; ++i){
		if ((tmp[i] == tmp[i + 1] + 1)
			&& tmp[i + 1] != _1){
			ret[j] = tmp[i + 1];
			++j;
		}
		else if (i < size - 5){
			j = 1;
			ret[0] = tmp[i + 1];
		}
		else{
			if (j >= 5){
				break;
			}
			j = 0;
			break;
		}
	}
	return j;
}
int Pokers::fflush(const std::array<Poker, 7> &cPokers,
	std::array<Poker, 7> &ret){
	std::array<Poker, 7> tmp = cPokers;
	std::sort(tmp.begin(), tmp.end(),
		[](const Poker &lhs, const Poker &rhs)->bool{return lhs.color < rhs.color; });
	int j = 1;
	ret[0] = tmp[0];
	for (int i = 0; i != 6; ++i){
		if (tmp[i].color == tmp[i + 1].color){
			ret[j] = tmp[i + 1];
			++j;
		}
		else if (i < 2){
			j = 1;
			ret[0] = tmp[i + 1];
		}
		else{
			if (j >= 5){
				break;
			}
			j = 0;
			break;
		}
	}
	std::sort(ret.begin(), ret.begin() + j);
	for (int i = j; i != 7; ++i){
		ret[i] = Poker(noncolor);
	}
	return j;
}
int Pokers::sameKind(const std::array<Poker, 7> &cPokers,
	std::array<Poker, 7> &ret){
	std::array<Poker, 7> tmp = cPokers;
	std::sort(tmp.begin(), tmp.end());
	auto p = tmp.begin();
	int j = 0;
	while (p != tmp.end()){
		p = std::adjacent_find(p, tmp.end());
		if (p != tmp.end()){
			if (j == 0 || (j != 0 && *p != ret[j - 1])){
				ret[j] = *p;
				++j;
			}
			ret[j] = *(p + 1);
			++j;
			++p;
		}
	}
	if (j > 5){
		//超过5张牌的三对，三条两对，四条一对， 四条三条, 三条三条
		if (ret[j - 1] != ret[j - 3]){
			j -= 2;
		}
		else if (ret[j - 1] == ret[j - 4]){
			//四条一对, 四条三条
			if (j == 6){
				for (int i = 0; i != j - 2; ++i){
					ret[i] = ret[i + 2];
				}
			}
			else
				for (int i = 1; i != j - 2; ++i){
					ret[i] = ret[i + 2];
				}
			j -= 2;
		}
		else if (ret[j - 1] == ret[j - 3]){
			//三条两对，三条三条，三条四条
			if (ret[0] == ret[3]){
				j -= 3;
			}
			else if (j == 6){
				j -= 1;
			}
			else{
				for (int i = 2; i != j - 2; ++i){
					ret[i] = ret[i + 2];
				}
				j -= 2;
			}
		}

	}
	if (j != 0 && j < 5){
		//不足5张牌的一对，两对， 三条或四条
		for (auto p = tmp.begin(); p != tmp.end(); ++p){
			if ((*p) != ret[0] && (*p) != ret[j - 1]){
				ret[j] = *p;
				++j;
			}
			if (j == 5)
				break;
		}
	}
	return j;
}

bool operator == (const Pokers &lhs, const Pokers &rhs){
	if (lhs.cardType != rhs.cardType)
		return false;
	else{
		for (auto p = lhs.finished_pokers.begin(), q = rhs.finished_pokers.begin();
			p != lhs.finished_pokers.end(); ++p, ++q){
			if (*p != *q)
				return false;
		}
		return true;
	}
}
bool operator != (const Pokers &lhs, const Pokers &rhs){
	return !(lhs == rhs);
}
bool operator > (const Pokers &lhs, const Pokers &rhs){
	if (lhs.cardType < rhs.cardType)
		return true;
	else if (lhs.cardType > rhs.cardType)
		return false;
	else if (lhs.cardType == straight){
		if (lhs.finished_pokers[4] == _2 && lhs.finished_pokers[0] == _A){
			return false;
		}
		return true;
	}
	else{
		for (auto p = lhs.finished_pokers.begin(), q = rhs.finished_pokers.begin();
			p != lhs.finished_pokers.end(); ++p, ++q){
			if ((*p).point < (*q).point)
				return true;
			else if ((*p).point >(*q).point)
				return false;
		}
		return false;
	}
}
bool operator < (const Pokers &lhs, const Pokers &rhs){
	if (lhs.cardType > rhs.cardType)
		return true;
	else if (lhs.cardType < rhs.cardType)
		return false;
	else if (lhs.cardType == straight){
		if (rhs.finished_pokers[4] == _2 && rhs.finished_pokers[0] == _A){
			return false;
		}
		return true;
	}
	else{
		for (auto p = lhs.finished_pokers.begin(), q = rhs.finished_pokers.begin();
			p != lhs.finished_pokers.end(); ++p, ++q){
			if ((*p).point >(*q).point)
				return true;
			else if ((*p).point < (*q).point)
				return false;
		}
		return false;
	}
}

std::ostream &operator << (std::ostream &os, const Pokers &ohs){
	for (int i = 0; i != 5; ++i){
		os << ohs.finished_pokers[i] << " ";
	}
	os << ohs.cardType;
	return os;
}