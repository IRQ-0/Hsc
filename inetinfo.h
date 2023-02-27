#ifndef INETINFO_H
#define INETINFO_H

#include <winsock2.h>
#include <iphlpapi.h>
#include <ws2ipdef.h>
#include <iostream>
#include <string>
#include <windows.h>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include <sstream>

#define ENABLED 1
#define DISABLED 0

#define NO_DOTS

#define IPCONFIG_COMMAND "ipconfig /all"

struct INET_INFO {
	std::string host;
	std::string domain;
	std::string dnsServersList;
	std::string node;
	std::string dhcp;
	int routing;
	int arp;
	int dns;
};

INET_INFO getInetInfo(void);
std::string getIpconfig(void);
std::string getRaw(void);

#endif