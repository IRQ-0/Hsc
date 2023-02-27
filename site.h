#ifndef SITE_H
#define SITE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include "inetinfo.h"

#define STATUS_HTML_FILENAME "status.html"

struct siteParams {
	std::string motherboardName;
	std::string motherboardVersion;

	std::string BIOSDate;
	std::string BIOSVendor;
	std::string BIOSVersion;

	std::string CPUClock;
	std::string CPUName;
	std::string CPUType;
	std::string CPUThreads;

	std::string memoryMinAllocAddr;
	std::string memoryMaxAllocAddr;
	std::string memoryVirtualTotal;
	std::string memoryPhysicalTotal;

	INET_INFO inetInfo;
};

std::string createSite(siteParams);



#endif