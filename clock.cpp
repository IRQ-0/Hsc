#include "clock.h"

std::string getDate(void) {
	std::time_t time = std::time(NULL);
	std::tm* now = std::localtime(&time);
	char datebuffer[1000];
	sprintf(datebuffer, "%d-%d-%d", (now->tm_year + 1900), (now->tm_mon + 1), now->tm_mday);
	std::string date = std::string(datebuffer);
	
	return date;
}

std::string getTime(void) {
	std::time_t time = std::time(NULL);
	std::tm* now = std::localtime(&time);
	char datebuffer[1000];
	sprintf(datebuffer, "%d:%d:%d", now->tm_hour, now->tm_min, now->tm_sec);
	std::string date = std::string(datebuffer);
	
	return date;
}

std::string getTimeStamp(void) {
	std::string ret = getDate() + std::string(" ") + getTime();
	
	return ret;
}
