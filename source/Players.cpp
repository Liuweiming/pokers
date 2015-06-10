#include "stdafx.h"
#include "Players.h"

Players::Players(std::ofstream &of, PID meID) : ofs(of), meID(meID){}
Players::~Players(){}
int Players::mySeat(){
	int i = 0;
	myiter = playerInTable.find(*myiter);
	auto p = myiter;
	p++;
	for (; p != myiter; p++){
		++i;
	}
	ofs << "my Seat " << i << std::endl;
	return i;
	
}
void Players::getPlayersMsg(std::vector<Message *> &msgs){
	if (msgs.front()->msg_type == seat_msg){
		if (players.empty()){
			iniPlayers(msgs);
		}
		playerInTable.clear();
		for (auto &p : msgs){
			SeatMsg *seatMsg = static_cast<SeatMsg *>(p);
			PokerPlayer &player = players[seatMsg->playerID];
			player.getPlayerMsg(*p);
			playerInTable.cirLstInsert(&player);
		}
		playerInTable.sort([](const ref_type &lhs, const ref_type &rhs){
			return lhs->seat < rhs->seat;
		});
		me = &players[meID];
		isJion = false;
		isUpdate = true;
		canJion = false;
		shouldDe = false;
		alwaysDe = false;
		notAlways = true;
		hasNotify = false;
		playersRaiseC = 0;
		playersAllC = 0;
		roundC = 0;
		raisePlayer = nullptr;
		lastRaisePlayer = nullptr;
		isNotify = false;
		totalSeatIn = playerInTable.size();
	}
	else if (msgs.front()->msg_type == inquire_msg ||
		msgs.front()->msg_type == notify_msg){
		int i = 0;
		Message *potmsg = nullptr;
		//获取奖池消息
		potmsg = msgs.back();
		msgs.pop_back();
		lastRaisePlayer = raisePlayer;
		raisePlayer = nullptr;
		if (msgs.front()->msg_type == notify_msg &&
			(circle != pre_flop && circle != pre_turn && circle != pre_river)){
			auto q = msgs.begin();
			Jetton bet = 0;
			for (; q != msgs.end(); ++q){
				auto notRaise = static_cast<NotifyMsg *>(*q);
				if (notRaise->action != fold && notRaise->action != all_in){
					if (bet == 0){
						bet = notRaise->bet;
					}
					else if (notRaise->bet != bet){
						break;
					}
				}
			}
			if (q == msgs.end()){
				isNotify = true;
				ofs << "is Notify" << std::endl;
			}
			else{
				isNotify = false;
			}
		}
		else{
			isNotify = false;
		}
		++roundC;
		for (auto p = msgs.rbegin(); p != msgs.rend(); ++p){
			ref_type player = &players[(*p)->playerID];
			auto inMsg = static_cast<InquireMsg *>((*p));
			if (isNotify){
				hasNotify = true;
				if (lastRaisePlayer != nullptr){
					if (lastRaisePlayer->seat <= me->seat){
						if (player->seat >= lastRaisePlayer->seat &&
							player->seat < me->seat){
							ofs << player->playerID << " was ignored" << std::endl;
							continue;
						}
					}
					else{
						if (player->seat >= lastRaisePlayer->seat ||
							player->seat < me->seat){
							ofs << player->playerID << " was ignored" << std::endl;
							continue;
						}
					}
				}
				else{
					if (player->seat != small_blind &&
						player->seat != big_blind &&
						player->seat < me->seat){
						ofs << player->playerID << " was ignored" << std::endl;
						continue;
					}
				}
			}
			else{
				if (hasNotify == true){
					if (*p == msgs.front()){
						hasNotify = false;
					}
					if (player->seat >= me->seat){
						ofs << player->playerID << " was ignored" << std::endl;
						continue;
					}
				}
				else if (lastRaisePlayer != nullptr &&
					lastRaisePlayer != playerInTable.front() &&
					(circle == pre_flop || circle == pre_turn || circle == pre_river)){
					if (player->seat >= lastRaisePlayer->seat){
						ofs << player->playerID << " was ignored" << std::endl;
						continue;
					}
				}
			}
			if (player->seat == small_blind){
				if (notAlways == true){
					alwaysDe = false;
				}
				if (notAlways == false){
					if (meID == player->playerID){
						alwaysDe = true;
					}
					else{
						alwaysDe = false;
					}
					notAlways = true;
				}
				if (circle == pre_turn){
					if (alwaysDe == false){
						circle = turn_card;
						isUpdate = false;
					}
				}
				else if (circle == pre_river){
					if (alwaysDe == false){
						circle = river_card;
						isUpdate = false;
					}
				}
				if (circle == pre_flop){
					if (alwaysDe == false){
						circle = flop_card;
						isUpdate = false;
					}
				}
				if (isJion == false && totalSeatIn == 2){
					if (canJion == false){
						canJion = true;
					}
					else{
						auto jionPlayer = playerInTable.begin();
						if (inMsg->action == fold || inMsg->action == call)
							++jionPlayer;
						for (; jionPlayer != playerInTable.end(); ++jionPlayer){
							++(*jionPlayer)->joinC;
						}
						isJion = true;
						//ofs << "roundC " << roundC << std::endl;
					}
				}
				if (shouldDe == true && circle != hold_card){
					playersAllC = 0;
					playersRaiseC = 0;
					shouldDe = false;
				}

			}
			if (player->seat == big_blind){
				if (isJion == false){
					if (canJion == false){
						canJion = true;
					}
					else{
						auto jionPlayer = playerInTable.begin();
						if ((*jionPlayer)->seat == small_blind){
							++(*jionPlayer)->joinC;
							++jionPlayer;
						}
						if (inMsg->action == fold || inMsg->action == call ||
							inMsg->action == check)
							++jionPlayer;
						for (; jionPlayer != playerInTable.end(); ++jionPlayer){
							++(*jionPlayer)->joinC;
						}
						//ofs << "roundC " << roundC << std::endl;
						isJion = true;
					}
				}

			}

			if (circle == hold_card && isUpdate == false){
				for (int i = 0; i != 8; ++i){
					for (int j = 0; j != 8; ++j){
						playerAction[i][j] = 0;
					}
				}
				isUpdate = true;
			}
			else if (circle == flop_card && isUpdate == false){
				for (int i = 0; i != 8; ++i){
					if (playerAction[i][raise] != 0){
						playerLastRaise[i] = true;
					}
					else
						playerLastRaise[i] = false;
				}
				for (int i = 0; i != 8; ++i){
					for (int j = 0; j != 8; ++j){
						playerAction[i][j] = 0;
					}
				}
				isUpdate = true;
			}
			else if (circle == turn_card && isUpdate == false){
				for (int i = 0; i != 8; ++i){
					if (playerAction[i][raise] != 0){
						playerLastRaise[i] = true;
					}
					else
						playerLastRaise[i] = false;
				}
				for (int i = 0; i != 8; ++i){
					for (int j = 0; j != 8; ++j){
						playerAction[i][j] = 0;
					}
				}
				isUpdate = true;
			}
			else if (circle == river_card && isUpdate == false){
				for (int i = 0; i != 8; ++i){
					if (playerAction[i][raise] != 0){
						playerLastRaise[i] = true;
					}
					else
						playerLastRaise[i] = false;
				}
				for (int i = 0; i != 8; ++i){
					for (int j = 0; j != 8; ++j){
						playerAction[i][j] = 0;
					}
				}
				isUpdate = true;
			}
			if (player->lastAction != fold && player->lastAction != all_in){
				player->getPlayerMsg(*(*p));
				switch (player->lastAction)
				{
				case attend:
					break;
				case blind:
					break;
				case check:
					if (playerLastRaise[player->seat] == true){
						++player->NCBetC;
						playerLastRaise[player->seat] = false;
					}
					break;
				case call:
					if (playerLastRaise[player->seat] == true){
						++player->NCBetC;
						playerLastRaise[player->seat] = false;
					}
					if (playerRaiseBefor(player->seat) == 1){
						++player->N3BetC;
						++player->NFBetC;
					}
					if (playerRaiseBefor(player->seat) >= 2){
						++player->N3BetC;
						++player->NF23BetC;
					}
					break;
				case raise:
					if ((circle == hold_card || circle == pre_flop)
						&& player->isRaise == false){
						++player->PFRC;
						player->isRaise = true;
					}
					if (playerLastRaise[player->seat] == true){
						++player->CBetC;
						playerLastRaise[player->seat] = false;
					}
					if (playerRaiseBefor(player->seat) == 1){
						++player->_3BetC;
						++player->NFBetC;
					}
					if (playerRaiseBefor(player->seat) >= 2){
						++player->_3BetC;
						++player->NF23BetC;
					}
					++playersRaiseC;
					raisePlayer = player;
					break;
				case all_in:
					if ((circle == hold_card || circle == pre_flop)
						&& player->isRaise == false){
						++player->PFRC;
						player->isRaise = true;
					}
					if (playerLastRaise[player->seat] == true){
						++player->CBetC;
						playerLastRaise[player->seat] = false;
					}
					if (playerRaiseBefor(player->seat) == 1){
						++player->_3BetC;
						++player->NFBetC;
					}
					if (playerRaiseBefor(player->seat) >= 2){
						++player->_3BetC;
						++player->NF23BetC;
					}
					++playersAllC;
					++playersRaiseC;
					raisePlayer = player;
					break;
				case fold:
					if (playerLastRaise[player->seat] == true){
						++player->NCBetC;
						playerLastRaise[player->seat] = false;
					}
					if (playerRaiseBefor(player->seat) == 1){
						++player->N3BetC;
						++player->FBetC;
					}
					if (playerRaiseBefor(player->seat) >= 2){
						++player->N3BetC;
						++player->F23BetC;
					}
					break;
				case lose:
					break;
				default:
					break;
				}
				if ((*p)->playerID == meID && shouldDe == false){
					playersRaiseC = 0;
					playersAllC = 0;
				}
				if (alwaysDe == true && (circle == pre_flop ||
					circle == pre_turn || circle == pre_river)){
					playersRaiseC = 0;
					playersAllC = 0;
				}
			}
			++playerAction[player->seat][player->lastAction];
			playersBet[i] = inMsg->bet;
			++i;
			if (inMsg->action == fold){
				auto player = playerInTable.find_if(
					playerInTable.begin(), playerInTable.end(),
					[p](const ref_type &hs){return hs->playerID == (*p)->playerID; });
				if (player != playerInTable.end())
					playerInTable.removeNode(player);
			}
		}
		for (int j = i; j != 8; ++j){
			playersBet[j] = 0;
		}
		if (potmsg != nullptr){
			playersBet[8] = static_cast<TotalPotMsg *>(potmsg)->totalPot;
		}

	}
	else if (msgs.front()->msg_type == show_down_msg ||
		msgs.front()->msg_type == pot_win_msg){
		for (auto &p : msgs){
			ref_type player = &players[p->playerID];
			player->getPlayerMsg(*p);
		}
	}
	else{
		for (auto &p : msgs){
			for (auto &player : playerInTable)
				player->getPlayerMsg(*p);
		}
	}
	if (msgs.front()->msg_type == blind_msg){
		BlindMsg *bmsg = static_cast<BlindMsg *>(msgs.front());
		leastBet = leastRaise = bmsg->bigBlind;
	}
	else if (msgs.front()->msg_type == hold_cards_msg){
		circle = hold_card;
		for (int i = 0; i != 8; ++i){
			for (int j = 0; j != 8; ++j){
				playerAction[i][j] = 0;
			}
		}
	}
	else if (msgs.front()->msg_type == flop_msg){
		circle = pre_flop;
		shouldDe = true;
		notAlways = false;
	}
	else if (msgs.front()->msg_type == turn_msg){
		circle = pre_turn;
		shouldDe = true;
		notAlways = false;
	}
	else if (msgs.front()->msg_type == river_msg){
		circle = pre_river;
		shouldDe = true;
		notAlways = false;
	}
}
Players::ref_type Players::getMe(PID playerID){
	auto pid = playerID;
	//ofs << "playerID " << playerID << std::endl;
	myiter = playerInTable.find_if(playerInTable.begin(), playerInTable.end(),
		[pid](const ref_type &hs){return hs->playerID == pid; });
	if (myiter == playerInTable.end()){
		std::cerr << "error when getMe, pid : " << playerID << std::endl;
		if (playerInTable.size() != 0){
			for (auto p = playerInTable.begin(); p != playerInTable.end(); ++p){
				std::cerr << (*p)->playerID << std::endl;
			}
		}
		return nullptr;
	}
	ofs << (*myiter)->playerID << std::endl;
	return *myiter;
}
std::size_t Players::getPlayersNum(){
	ofs << "playerNum " << playerInTable.size() << std::endl;
	return playerInTable.size();
}
void Players::getBetAndRaise(){
	Jetton betMost = 0, betSec = 0;
	int mosti = 0;
	pot = playersBet[8];
	for (int i = 0; i != 8; ++i){
		if (playersBet[i] > betMost){
			betMost = playersBet[i];
			mosti = i;
		}
	}
	for (int i = 0; i != 8; ++i){
		if (playersBet[i] > betSec && i != mosti)
			betSec = playersBet[i];
	}
	if (betSec == 0)
		betSec = betMost;
	shouldBet = betMost;
	if (shouldBet < leastBet){
		shouldBet = leastBet;
	}
	shouldRaise = betMost - betSec;
	if (shouldRaise < leastRaise){
		shouldRaise = leastRaise;
	}
}
bool Players::mySituation(){
	int rank = 0;
	myiter = playerInTable.find(*myiter);
	for (auto p = myiter; p != playerInTable.end(); ++p){
		if ((*p)->type == loose_aggre){
			rank -= 4;
		}
		if ((*p)->type == tight_aggre){
			rank -= 0;
		}
		if ((*p)->type == loose_pass){
			rank += 2;
		}
		if ((*p)->type == tight_pass){
			rank += 1;
		}
	}
	if (rank >= 0)
		return true;
	return false;
}

void Players::iniPlayers(std::vector<Message *> &msgs){
	for (auto &p : msgs){
		SeatMsg *seatMsg = static_cast<SeatMsg *>(p);
		PokerPlayer player(seatMsg->playerID);
		players[seatMsg->playerID] = player;
	}
}
bool Players::playerInSeat(ref_type player){
	auto p = playerInTable.find_if(playerInTable.begin(), playerInTable.end(),
		[player](const ref_type &hs){return hs == player; });
	if (p != playerInTable.end()){
		return true;
	}
	return false;
}
Jetton Players::maxJetton(){
	Jetton max = 0;
	for (auto p = playerInTable.begin(); p != playerInTable.end(); ++p){
		if ((*p)->jetton > max){
			max = (*p)->jetton;
			maxiter = p;
		}
	}
	return max;
}
Jetton Players::minJetton(){
	Jetton min = 0;
	for (auto p = playerInTable.begin(); p != playerInTable.end(); ++p){
		if ((*p)->jetton < min){
			min = (*p)->jetton;
			miniter = p;
		}
	}
	return min;
}

PlayersType Players::getPlayersType(iter_type &ret){
	myiter = playerInTable.find(*myiter);
	ret = myiter;
	if (playerInTable.size() > 4){
		return normal;
	}
	else if (playerInTable.size() == 2){
		auto p = myiter;
		p--;
		return (*p)->type;
	}
	else{
		int types[5];
		iter_type iter[5];
		for (int i = 0; i != 5; ++i){
			types[i] = 0;
		}
		auto p = myiter;
		p--;
		for (; p != myiter; p--){
			++types[(*p)->type];
			iter[(*p)->type] = p;
		}
		types[3] = 0;
		int i = 5;
		if (types[4] + types[3] != 0){
			if (types[0] + types[1] == 0){
				if (types[4] == 0){
					ret = iter[3];
					return tight_pass;
				}
				else{
					ret = iter[4];
					return tight_aggre;
				}
			}
			else{
				return normal;
			}
		}
		else{
			if (types[1] == 0){
				if (types[0] != 0){
					ret = iter[0];
					return loose_pass;
				}
				else{
					return normal;
				}
			}
			else{
				ret = iter[1];
				return loose_aggre;
			}
		}
		if (i == 5){
			return normal;
		}
	}
	return normal;
}
int Players::playerRaiseBefor(int myseat){
	int ret = 0;
	for (int i = 0; i != 8; ++i){
		if (i == myseat){
			continue;
		}
		ret += playerAction[i][raise];
		ret += playerAction[i][all_in];
	}
	return ret;
}

int Players::playerRaiseBefor(){
	ofs << "playerRaiseBefor " << playersRaiseC << std::endl;
	return playersRaiseC;
}
int Players::playerAllIn(int myseat){
	int ret = 0;
	for (int i = 0; i != 8; ++i){
		if (i == myseat){
			continue;
		}
		ret += playerAction[i][all_in];
	}
	return ret;
}

int Players::playerAllIn(){
	ofs << "playerAllIn" << playersAllC << std::endl;
	return playersAllC;
}

Players::ref_type Players::getRaisePlayer(){
	return raisePlayer;
}