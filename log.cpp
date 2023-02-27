#include "log.h"


void Log::open(std::string logPath, int onoff) {
	this->onoff = onoff;

	if (logPath.back() != '\\') {
		logPath.push_back('\\');
	}

	this->hndle.open(logPath + getDate() + std::string(LOG_FILE_EXTENSION), std::ios::app);
	if (this->hndle.bad()) {
		fprintf(stderr, "Cannot open log file");
		exit(1);
	}

	#ifdef START_LINE

	// Add one line on start to make it look better
	this->hndle << std::endl;

	#endif

	this->hndle.flush();
}

Log::~Log(void) {
	this->hndle.close();
}

void Log::write(int mode, std::string data) {
	if (this->onoff == LOG_OFF)
		return;

	std::string buffer = std::string("<");

	switch (mode) {
	case ERROR:
		buffer += std::string("ERROR>   ");
	break;

	case WARNING:
		buffer += std::string("WARNING> ");
	break;

	case NOTE:
		buffer += std::string("NOTE>    ");
	break;

	default:
		return;
	}

	buffer += std::string("[") + getTimeStamp() + std::string("] -> ") + data;

	this->hndle << buffer << std::endl;
	this->hndle.flush();
}

void Log::close(void) {
	this->hndle.close();
}