#pragma once
#include "Message.h"
#include "Pokers.h"
#include "Players.h"
#include "MsgGenerator.h"
#include "Me.h"
#include <fstream>
class MainController
{
public:
private:
	Players players;
	MsgGenerator msgGen;
	Me me;
	PID playerID;
	std::ofstream ofs;
public:
	MainController(const std::string &severAdrr, int severPort,
		const std::string &myAdrr, int myPort, PID playerID) : players(ofs, playerID), 
		msgGen(severAdrr, severPort, myAdrr, myPort),
		me(msgGen, players, playerID, ofs), playerID(playerID),
		ofs(std::to_string(playerID) + ".txt", std::fstream::out){}
	~MainController(){}
	void regist(){
		me.rigist();
	}
	void runner(){
		while (true){
			auto msgs = msgGen.getMessage();
			if (!msgs.empty()){
				if (msgs.front()->msg_type == game_over_msg || msgs.front()->msg_type == error_msg){
					break;
				}
				players.getPlayersMsg(msgs);
				if (players.players[playerID].lastAction != fold)
					me.MeGetMsg(*(msgs.front()));
			}
		}
		for (auto p = players.players.begin(); p != players.players.end(); ++p){
			ofs << p->second << std::endl;
		}
	}
};

