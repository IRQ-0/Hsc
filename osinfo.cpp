#include "osinfo.h"

template <typename Type> std::string floatToStr(const Type & t) {
  	std::ostringstream os;
  	os << t;
  	return os.str();
}

std::string OsInfo::regReadString(std::string path, std::string value) {
	HKEY key;
    DWORD type = REG_SZ;
    char buffer[this->BUFF_SIZE];
    unsigned int len = sizeof(buffer);
    
    if (RegOpenKey(HKEY_LOCAL_MACHINE, path.c_str(), &key)) 
    	return std::string("");

    if (RegQueryValueEx(key, value.c_str(), NULL, &type, (LPBYTE)&buffer, (LPDWORD)&len))
    	return std::string("");

    return std::string(buffer);
}

DWORD OsInfo::regReadDword(std::string path, std::string value) {
	HKEY key;
    DWORD type = REG_DWORD;
    DWORD ret;
    unsigned int len = sizeof(ret);
    
    if (RegOpenKey(HKEY_LOCAL_MACHINE, path.c_str(), &key))
    	return 0;

    if (RegQueryValueEx(key, value.c_str(), NULL, &type, (LPBYTE)&ret, (LPDWORD)&len))
    	return 0;
	
	return ret;
}


std::string OsInfo::getUsedVirtualMemoryInProc(void) {
	MEMORYSTATUSEX meminfo;
	meminfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&meminfo);
	float used = (float)(meminfo.ullTotalPageFile - meminfo.ullAvailPageFile) / (1024*1024*1024);
	float total = (float)meminfo.ullTotalPageFile / (1024*1024*1024);

	//return floatToStr((used / total) * 100) + std::string("%");
	int out = (int) ((used / total) * 100);
	INT_TO_STRING(out);
}

std::string OsInfo::getUsedPhysicalMemoryInProc(void) {
	MEMORYSTATUSEX meminfo;
	meminfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&meminfo);
	float used = (float)(meminfo.ullTotalPhys - meminfo.ullAvailPhys) / (1024*1024*1024);
	float total = (float)meminfo.ullTotalPhys / (1024*1024*1024);

	//return floatToStr((used / total) * 100) + std::string("%");
	int out = (int) ((used / total) * 100);
	INT_TO_STRING(out);
}

std::string OsInfo::getCpuUsage(void) {
	INT_TO_STRING(getCpuLoad());
}


std::string OsInfo::getCpuMhz(void) {
    INT_TO_STRING(regReadDword("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", "~Mhz"));
}

SYSTEM_INFO OsInfo::getSysBasicInfo(void) {
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	
	return sysinfo;
}

std::string OsInfo::getCpuType(void) {
	SYSTEM_INFO sysinfo = getSysBasicInfo();
	switch (sysinfo.dwProcessorType) {
		case PROCESSOR_INTEL_386:
			return std::string("Intel 386");
			
		case PROCESSOR_INTEL_486:
			return std::string("Intel 486");
			break;
			
		case PROCESSOR_INTEL_PENTIUM:
			return std::string("Intel Pentium");
			break;
			
		case PROCESSOR_INTEL_IA64:
			return std::string("IA-64");
			break;
			
		case PROCESSOR_AMD_X8664:
			return std::string("x86-64");
			break;
			
		default:
			return std::string("Unknown");
	}
}

std::string OsInfo::getTotalVirtualMemory(void) {
	MEMORYSTATUSEX meminfo;
	meminfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&meminfo);

	return floatToStr((float)meminfo.ullTotalPageFile / (1024*1024*1024)) + std::string(" Gb");
}

std::string OsInfo::getTotalPhysicalMemory(void) {
	MEMORYSTATUSEX meminfo;
	meminfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&meminfo);

	return floatToStr((float)meminfo.ullTotalPhys / (1024*1024*1024)) + std::string(" Gb");
}

std::string OsInfo::getUsedVirtualMemory(void) {
	MEMORYSTATUSEX meminfo;
	meminfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&meminfo);

	return floatToStr((float)(meminfo.ullTotalPageFile - meminfo.ullAvailPageFile) / (1024*1024*1024)) + std::string(" Gb");
}

std::string OsInfo::getUsedPhysicalMemory(void) {
	MEMORYSTATUSEX meminfo;
	meminfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&meminfo);

	return floatToStr((float)(meminfo.ullTotalPhys - meminfo.ullAvailPhys) / (1024*1024*1024)) + std::string(" Gb");
}

std::string OsInfo::getPageSize(void) {
	SYSTEM_INFO sysinfo = getSysBasicInfo();
	INT_TO_STRING(sysinfo.dwPageSize);
}

std::string OsInfo::getMinimumAllocationAdrress(void) {
	SYSTEM_INFO sysinfo = getSysBasicInfo();
	INT_TO_HEX_STRING(sysinfo.lpMinimumApplicationAddress);
}

std::string OsInfo::getMaximumAllocationAdrress(void) {
	SYSTEM_INFO sysinfo = getSysBasicInfo();
	INT_TO_HEX_STRING(sysinfo.lpMaximumApplicationAddress);
}

std::string OsInfo::getCpuThreads(void) {
	SYSTEM_INFO sysinfo = getSysBasicInfo();
	INT_TO_STRING(sysinfo.dwNumberOfProcessors);
}

std::string OsInfo::getCpuName(void) {
	return regReadString("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", "ProcessorNameString");
}

std::string OsInfo::getMotherBoardManufacturer(void) {
	return regReadString("HARDWARE\\DESCRIPTION\\System\\BIOS", "BaseBoardManufacturer");
}

std::string OsInfo::getMotherBoardName(void) {
	return regReadString("HARDWARE\\DESCRIPTION\\System\\BIOS", "BaseBoardProduct");
}

std::string OsInfo::getMotherBoardVersion(void) {
	return regReadString("HARDWARE\\DESCRIPTION\\System\\BIOS", "BaseBoardVersion");
}

std::string OsInfo::getBiosReleaseDate(void) {
	return regReadString("HARDWARE\\DESCRIPTION\\System\\BIOS", "BIOSReleaseDate");
}

std::string OsInfo::getBiosVendor(void) {
	return regReadString("HARDWARE\\DESCRIPTION\\System\\BIOS", "BIOSVendor");
}

std::string OsInfo::getBiosVersion(void) {
	return regReadString("HARDWARE\\DESCRIPTION\\System\\BIOS", "BIOSVersion");
}