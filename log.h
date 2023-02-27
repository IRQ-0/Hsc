#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <windows.h>
#include <fstream>
#include "clock.h"

// Priority
#undef ERROR //	 It is definied somehere else, let's just undef it
#define ERROR 1
#define WARNING 2
#define NOTE 3

// Other macors
#define START_LINE // If defined start line is turned on
#define LOG_FILE_EXTENSION ".log" // Log files extension
#define LOG_ON 1 // Log on
#define LOG_OFF 0 // Log off

class Log {
public:
	Log() {};
	
	void open(std::string, int);
	/*
	Create new log file (<path>
						 <LOG_ON / LOG_OFF>)
	*/

	~Log(void);
	/*
	Close that file (none)
	*/

	void write(int, std::string);
	/*
	Write data to log file (<mode - error, warning, note>,
							<data>)
	*/

	void close(void);
	/*
	Close log file (none)
	*/


private:
	std::fstream hndle; // Main fstream object
	int onoff;
};


#endif