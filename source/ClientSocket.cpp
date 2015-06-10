// Implementation of the ClientSocket class
//Quote form http://tldp.org/LDP/LG/issue74/tougher.html
#include "ClientSocket.h"
#include "SocketException.h"


ClientSocket::ClientSocket(const std::string &host, int sevPort, 
					const std::string &myAdrr, int myPort)
{
	try{
	if (!Socket::create())
	{
		throw SocketException("Could not create client socket.");
	}
	if(!Socket::bind(myAdrr, myPort))
	{
		throw SocketException("Could not bind client socket.");
	}
	if (!Socket::connect(host, sevPort))
	{
		throw SocketException("Could not bind to port.");
	}
	}catch(SocketException e){
		std::cerr << e.description();
	}

}


const ClientSocket& ClientSocket::operator << (const std::string& s) const
{
	try{
	if (!Socket::send(s))
	{
		throw SocketException("Could not write to socket.");
	}
	}catch(SocketException e){
		std::cerr << e.description();
	}
	return *this;

}


const ClientSocket& ClientSocket::operator >> (std::string& s) const
{	
//my new code
	try{
	static std::stringstream ist;
	static std::string str;
	std::getline(ist, s);
	while(ist.fail()){
		ist.clear();
		ist.str("");
		if (!Socket::recv(str))
		{
			throw SocketException("Could not read from socket.");
		}
		ist << str;
		std::getline(ist, s);
	}
	}catch(SocketException e){
		std::cerr << e.description();
	}
	return *this;
}