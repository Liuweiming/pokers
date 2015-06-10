#include "stdafx.h"
#include "MsgGenerator.h"

MsgGenerator::MsgGenerator(const std::string &host, int port,
	const std::string &myAdrr, int myPort) :
	mySocket(host, port, myAdrr, myPort){}
MsgGenerator::~MsgGenerator(){
	for (auto p = msgs.begin(); p != msgs.end(); ++p){
		delete *p;
	}
}
std::vector<Message *> &MsgGenerator::getMessage(){
	for (auto p = msgs.begin(); p != msgs.end(); ++p){
		delete *p;
	}
	msgs.clear();
	std::string str;
	mySocket >> str;

	if (str == "seat/ ")
		getSeatMsg();
	else if (str == "game-over ")
		getGameOverMsg();
	else if (str == "blind/ ")
		getblindMsg();
	else if (str == "hold/ ")
		getHoldMsg();
	else if (str == "inquire/ ")
		getInquireMsg();
	else if (str == "notify/ ")
		getNotifyMsg();
	else if (str == "flop/ ")
		getFlopMsg();
	else if (str == "turn/ ")
		getTurnMsg();
	else if (str == "river/ ")
		getRiverMsg();
	else if (str == "showdown/ ")
		getShowDownMsg();
	else if (str == "pot-win/ ")
		getPotWinMsg();
	else{
		for (auto p = msgs.begin(); p != msgs.end(); ++p){
			delete *p;
		}
		msgs.clear();
		msgs.push_back(new ErrorMsg("socket getting error head"));
	}
	return msgs;
}
void MsgGenerator::sendMessage(Message &msg){
	std::ostringstream sendMsg;
	if (msg.msg_type == reg_msg){
		RegMsg &regMsg = static_cast<RegMsg &>(msg);
		sendMsg << "reg: " << regMsg.playerID << " " << regMsg.playerName
			<< " need_notify \n";
		mySocket << sendMsg.str();
	}
	else if (msg.msg_type == action_msg){
		ActionMsg &actionMsg = static_cast<ActionMsg &>(msg);
		switch (actionMsg.action)
		{
		case blind:
			break;
		case check: sendMsg << "check \n";
			break;
		case call: sendMsg << "call \n";
			break;
		case raise: sendMsg << "raise " << actionMsg.add_msg << " \n";
			break;
		case all_in: sendMsg << "all_in \n";
			break;
		case fold: sendMsg << "fold \n";
			break;
		case lose:
			break;
		default:
			break;
		}
		mySocket << sendMsg.str();
	}
}

std::vector<Message *> &MsgGenerator::getSeatMsg(){
	std::string buffer;
	mySocket >> buffer;
	std::istringstream strs(buffer);
	Seat seat;
	PID pid;
	Jetton jetton;
	Money money;
	while (buffer != "/seat " && (buffer[0] == 'b' || buffer[0] == 's')){
		strs >> buffer;
		if (buffer == "button:")
			seat = button;
		else if (buffer == "small"){
			strs >> buffer;
			seat = small_blind;
		}
		else{
			strs >> buffer;
			seat = big_blind;
		}
		strs >> pid;
		strs >> jetton;
		strs >> money;
		msgs.push_back(new SeatMsg(pid, seat, jetton, money));
		mySocket >> buffer;
		strs.str(buffer);
	}
	int i = 2;
	while (buffer != "/seat " && i != 8){
		seat = static_cast<Seat>(i);
		strs >> pid;
		strs >> jetton;
		strs >> money;
		msgs.push_back(new SeatMsg(pid, seat, jetton, money));
		mySocket >> buffer;
		strs.str(buffer);
		++i;
	}
	return msgs;
}
std::vector<Message *> &MsgGenerator::getGameOverMsg(){
	msgs.push_back(new GameOverMsg());
	return msgs;
}
std::vector<Message *> &MsgGenerator::getblindMsg(){
	std::string buffer;
	mySocket >> buffer;
	std::istringstream strs(buffer);
	Jetton sbBlind[2];
	int i = 0;
	while (buffer != "/blind " && i != 2){
		strs >> buffer;
		strs >> sbBlind[i];
		mySocket >> buffer;
		strs.str(buffer);
		++i;
	}
	if (i == 1){
		msgs.push_back(new BlindMsg(sbBlind[0], sbBlind[0]));
	}
	else if (i == 2){
		msgs.push_back(new BlindMsg(sbBlind[0], sbBlind[1]));
	}
	return msgs;
}
Poker MsgGenerator::getPoker(std::istringstream &strs){
	std::string buffer;
	PokerColor color;
	PokerPoint point;
	Poker card;
	strs >> buffer;
	if (buffer == "SPADES"){
		color = spades;
	}
	else if (buffer == "HEARTS")
		color = hearts;
	else if (buffer == "CLUBS")
		color = clubs;
	else if (buffer == "DIAMONDS")
		color = diamonds;
	else{
		//msgs.push_back(new ErrorMsg("socket getting error head"));
		color = noncolor;
	}
	strs >> buffer;
	if ('2' <= buffer[0] && buffer[0] <= '9'){
		point = static_cast<PokerPoint>(14 - buffer[0] + 48);
	}
	else if (buffer == "10")
		point = _10;
	else if (buffer == "J")
		point = _J;
	else if (buffer == "Q")
		point = _Q;
	else if (buffer == "K")
		point = _K;
	else if (buffer == "A")
		point = _A;
	else{
		//msgs.push_back(new ErrorMsg("socket getting error head"));
		point = _0;
	}
	card.color = color;
	card.point = point;
	return card;
}
std::vector<Message *> &MsgGenerator::getHoldMsg(){
	Poker card[2];
	std::string buffer;
	mySocket >> buffer;
	std::istringstream strs(buffer);
	int i = 0;
	while (buffer != "/hold " && i != 2){
		card[i] = getPoker(strs);
		++i;
		mySocket >> buffer;
		strs.str(buffer);
	}
	msgs.push_back(new HoldCardMsg(card[0], card[1]));
	return msgs;
}
Action MsgGenerator::getAction(std::istringstream &strs){
	std::string buffer;
	Action action;
	strs >> buffer;
	if (buffer == "blind")
		action = blind;
	else if (buffer == "check")
		action = check;
	else if (buffer == "call")
		action = call;
	else if (buffer == "raise")
		action = raise;
	else if (buffer == "all_in")
		action = all_in;
	else if (buffer == "fold")
		action = fold;
	else{
		//msgs.push_back(new ErrorMsg("socket getting error head"));
		action = lose;
	}
	return action;
}
std::vector<Message *> &MsgGenerator::getInquireMsg(){
	std::string buffer;
	mySocket >> buffer;
	std::istringstream strs(buffer);
	PID pid;
	Jetton jetton;
	Money money;
	Jetton bet;
	Jetton totalPot;
	Action lastAction;
	int i = 0;
	while (buffer[0] != 't' && i != 8){
		strs >> pid;
		strs >> jetton;
		strs >> money;
		strs >> bet;
		lastAction = getAction(strs);
		msgs.push_back(new InquireMsg(pid, jetton, bet, money, lastAction));
		mySocket >> buffer;
		strs.str(buffer);
		++i;
	}
	if (buffer != "/inquire "){
		strs >> buffer;
		strs >> buffer;
		strs >> totalPot;
		mySocket >> buffer;
	}
	msgs.push_back(new TotalPotMsg(totalPot));
	return msgs;
}
std::vector<Message *> &MsgGenerator::getNotifyMsg(){
	std::string buffer;
	mySocket >> buffer;
	std::istringstream strs(buffer);
	PID pid;
	Jetton jetton;
	Money money;
	Jetton bet;
	Jetton totalPot;
	Action lastAction;
	int i = 0;
	while (buffer[0] != 't' && i != 8){
		strs >> pid;
		strs >> jetton;
		strs >> money;
		strs >> bet;
		lastAction = getAction(strs);
		msgs.push_back(new NotifyMsg(pid, jetton, bet, money, lastAction));
		mySocket >> buffer;
		strs.str(buffer);
		++i;
	}
	if (buffer != "/notify "){
		strs >> buffer;
		strs >> buffer;
		strs >> totalPot;
		mySocket >> buffer;
	}
	msgs.push_back(new TotalPotMsg(totalPot));
	return msgs;
}
std::vector<Message *> &MsgGenerator::getFlopMsg(){
	std::string buffer;
	mySocket >> buffer;
	std::istringstream strs(buffer);
	Poker card[3];
	int i = 0;
	while (buffer != "/flop " && i != 3){
		card[i] = getPoker(strs);
		++i;
		mySocket >> buffer;
		strs.str(buffer);
	}
	msgs.push_back(new FlopMsg(card[0], card[1], card[2]));
	return msgs;
}
std::vector<Message *> &MsgGenerator::getTurnMsg(){
	std::string buffer;
	mySocket >> buffer;
	std::istringstream strs(buffer);
	Poker card;
	int i = 0;
	while (buffer != "/turn " && i != 1){
		card = getPoker(strs);
		++i;
		mySocket >> buffer;
		strs.str(buffer);
	}
	msgs.push_back(new TurnMsg(card));
	return msgs;
}
std::vector<Message *> &MsgGenerator::getRiverMsg(){
	std::string buffer;
	mySocket >> buffer;
	std::istringstream strs(buffer);
	Poker card;
	int i = 0;
	while (buffer != "/river " && i != 1){
		card = getPoker(strs);
		++i;
		mySocket >> buffer;
		strs.str(buffer);
	}
	msgs.push_back(new RiverMsg(card));
	return msgs;
}
CardType MsgGenerator::getCardType(std::istringstream &strs){
	std::string buffer;
	CardType cardType;
	strs >> buffer;
	if (buffer == "HIGH_CARD")
		cardType = high_card;
	else if (buffer == "ONE_PAIR")
		cardType = one_pair;
	else if (buffer == "TWO_PAIR")
		cardType = two_pair;
	else if (buffer == "THREE_OF_A_KIND")
		cardType = three_of_a_kind;
	else if (buffer == "STRAIGHT")
		cardType = straight;
	else if (buffer == "FLUSH")
		cardType = flush;
	else if (buffer == "FULL_HOUSE")
		cardType = full_house;
	else if (buffer == "FOUR_OF_A_KIND")
		cardType = four_of_a_kind;
	else if (buffer == "STRAIGHT_FLUSH")
		cardType = straight_flush;
	else{
		//msgs.push_back(new ErrorMsg("socket getting error head"));
		cardType = high_card;
	}
	return cardType;
}
std::vector<Message *> &MsgGenerator::getShowDownMsg(){
	std::string buffer;
	mySocket >> buffer;
	std::istringstream strs(buffer);
	PID pid;
	Poker card[2];
	CardType cardType;
	while (buffer != "/common "){
		mySocket >> buffer;
	}
	mySocket >> buffer;
	strs.str(buffer);
	int i = 0;
	while (buffer != "/showdown " && i != 8){
		strs >> buffer;
		strs >> pid;
		card[0] = getPoker(strs);
		card[1] = getPoker(strs);
		cardType = getCardType(strs);
		msgs.push_back(new ShowDownMsg(pid, card[0], card[1], cardType));
		mySocket >> buffer;
		strs.str(buffer);
		++i;
	}
	return msgs;
}
std::vector<Message *> &MsgGenerator::getPotWinMsg(){
	std::string buffer;
	mySocket >> buffer;
	std::istringstream strs(buffer);
	PID pid;
	Jetton win;
	int i = 0;
	while (buffer != "/pot-win " && i != 8){
		strs >> pid;
		strs >> buffer;
		strs >> win;
		msgs.push_back(new PotWinMsg(pid, win));
		mySocket >> buffer;
		strs.str(buffer);
		++i;
	}
	return msgs;
}