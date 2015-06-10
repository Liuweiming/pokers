// Definition of the ClientSocket class
//Quote form http://tldp.org/LDP/LG/issue74/tougher.html
#ifndef ClientSocket_class
#define ClientSocket_class

#include "Socket.h"
#include <sstream>
#include <string>
#include <iostream>
class ClientSocket : private Socket
{
public:

	ClientSocket(const std::string &host, int port,
				const std::string &myAdrr, int myPort);
	virtual ~ClientSocket(){};

	const ClientSocket& operator << (const std::string&) const;
	const ClientSocket& operator >> (std::string&) const;

};


#endif