#include "Request.h"

Request::Request(std::string raw) {
	this->raw = raw;
}

std::string Request::getParam(std::string paramName) {
	std::string::size_type nstart, nend;
	std::string buffer, line;
	std::istringstream input;
	input.str(this->raw);

	std::getline(input, line);

	if (line.find('?') == std::string::npos)
		return std::string("");

	nstart = line.find('?');
	nend = line.find(' ', nstart);

	if (nstart == std::string::npos || nend == std::string::npos)
		return std::string("");	

	std::string paramsWhole = line.substr(nstart + 1, ((line.length() - nstart) - (line.length() - nend)) - 1);

	// Check if there is only one parameter (is there no '&' character)
	if (line.find('&') == std::string::npos) {
		nend = paramsWhole.find('=');
		std::string oneParamName = paramsWhole.substr(0, nend);

		if (oneParamName.compare(paramName) == 0) {
			std::string out = paramsWhole.substr(nend + 1, paramsWhole.length() - nend);
			return out;
		}
	} else {
		nstart = 0;
		for ( ; ; ) {
			nend = paramsWhole.find('&', nstart);
			std::string paramTry = paramsWhole.substr(nstart, paramsWhole.length() - (paramsWhole.length() - nend));

			std::string param = paramTry.substr(0, paramTry.find('='));
			if (paramName.compare(param) == 0) {
				nstart = paramTry.find('=');
				std::string outStr = paramTry.substr(nstart + 1, paramTry.length() - (paramTry.length() - nend));

				return outStr;
			}

			if (nend == std::string::npos) { break; }
			nstart = nend + 1;

			if (nstart == std::string::npos || nend == std::string::npos) { break; }


		}
	}
	
	return std::string("");
}

std::string Request::parseRequest(void) {
	std::string line;
	this->vect = parseLines();
	for (int i = 0; i < this->vect.size(); i++) {
		line.clear();
		line = this->vect.at(i);
		if (line.find("GET") != std::string::npos) {
			return getDocumentName();
		}
	}
}

std::string Request::getElement(std::string elementname) {
	std::string line;
	for (int i = 0; i < this->vect.size(); i++) {
		line.clear();
		line = this->vect.at(i);
		std::string::size_type n;
		n = line.find(':');
		if (n == std::string::npos) {
			continue;
		}
		if (strcmp(line.substr(0, n).c_str(), elementname.c_str()) == 0) {
			return line.substr(n + 2, std::string::npos);
		}
	}
	
	return "";
}

std::vector<std::string> Request::parseLines(void) {
	std::vector<std::string> vect;
	std::string buffer;
	std::istringstream f(this->raw);
	while (std::getline(f, buffer)) {
        vect.push_back(buffer);
    }
    
    return vect;
}

int Request::checkFileType(std::string filename) {
	std::string::size_type n;
	std::string buffer = "";
	while (1) {
		n = filename.find('.', n + 1);
		if (n == std::string::npos) {
			break;
		}
		buffer = filename.substr(n);
	}
	
	if (buffer == "") {
		return TEXT_FILE;
	}
	std::string temp;
	temp.clear();
	for (int i = 0; i < buffer.length(); i++) {
		temp.push_back(toupper(buffer.at(i)));
	}
	
	filename = temp;
	for (int i = 0; i < IMAGE_EXTENTIONS_NUMBER; i++) {
		if (strcmp(temp.c_str(), imgaeextentions[i]) == 0) {
			return BIN_FILE;
		}
	}
	
	return TEXT_FILE;
}

std::string Request::getRaw(void) {
	return this->raw;
}

std::string Request::getDocumentName(void) {
	std::string::size_type nstart, nend;
	std::string line;
	std::istringstream input;
	input.str(this->raw);

	std::getline(input, line);

	nstart = line.find(' ') + 1;
	nend = line.find('?', nstart + 1);

	if (nend == std::string::npos) {
		nend = line.find(' ', nstart + 1);
	}
	
	return line.substr(nstart + 1, ((line.length() - nstart) - (line.length() - nend)) - 1);
}