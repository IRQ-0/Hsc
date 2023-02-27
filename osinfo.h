#ifndef OSINFO_H
#define OSINFO_H

#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <pdh.h>
#include <sstream>
#include <string.h>
#include <math.h>
#include "cpuload.h"

#define INT_TO_STRING(a) std::ostringstream ss; ss << a; return ss.str();
#define INT_TO_HEX_STRING(a) std::ostringstream ss; ss << std::hex << a; return std::string("0x") + ss.str();

class OsInfo {
	public:
		// Motherboard
		std::string getMotherBoardName(void);
		std::string getMotherBoardVersion(void);	
		std::string getMotherBoardManufacturer(void);

		// BIOS
		std::string getBiosReleaseDate(void);
		std::string getBiosVendor(void);
		std::string getBiosVersion(void);

		// CPU
		std::string getCpuMhz(void);
		std::string getCpuName(void);
		std::string getCpuType(void);
		std::string getCpuThreads(void);
		std::string getCpuUsage(void);

		// Memory
		std::string getPageSize(void);
		std::string getMinimumAllocationAdrress(void);
		std::string getMaximumAllocationAdrress(void);
		std::string getTotalVirtualMemory(void);
		std::string getTotalPhysicalMemory(void);
		std::string getUsedVirtualMemory(void);
		std::string getUsedPhysicalMemory(void);
		std::string getUsedVirtualMemoryInProc(void);
		std::string getUsedPhysicalMemoryInProc(void);

	private:
		// Don't use this by yourself unless you know how to

		// Funcions
		std::string regReadString(std::string, std::string);
		DWORD regReadDword(std::string, std::string);
		SYSTEM_INFO getSysBasicInfo(void);

		// Constans
		const int BUFF_SIZE = 4096;
};


#endif
