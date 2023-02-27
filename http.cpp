#include "http.h"

HttpBridge::HttpBridge(std::string raw) {
	this->raw = raw;
}

Request HttpBridge::getRequestObject(void) {
	Request req(this->raw);
	return req;
}

std::string HttpBridge::getRaw(void) {
	return this->raw;
}

int HttpBridge::getOutputStream(std::fstream* out) {
	out->open(HTTP_SERVER_TEMP_FILE_LOCATION, std::ios::out);
	if (out->bad()) {
		return OUTPUT_STREAM_ERROR;
	}

	return 0;
}

void HttpBridge::closeOutputStream(std::fstream* out) {
	out->flush();
	out->close();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Httpserver::Httpserver(std::string ip, int port, int log, std::string logpath, std::string homedir) {
	this->ip = ip;
	this->port = port;
	this->iflog = log;
	this->homedir = homedir;
	this->lg.open(logpath, log);
}

void Httpserver::addHsc(Hsc a) {
	this->hscList.push_back(a);
}

Httpserver::~Httpserver() {
	this->lg.close();
}

void Httpserver::start(void) {
	int error = 0;
	Telnet t;
	socketinfo sockinfo;

	// Creating main socket
	SOCKET sock = t.newSocket(this->ip, this->port, AS_SERVER, &error);
	// Catching an error

	switch (error) {
	case 0: // No error
		break;

	case WSA_STARTUP_ERROR:
		LOG_ERROR("WSA startup error");
		this->lg.close();
		exit(1);
		break;

	case SOCKET_CREATE_ERROR:
		LOG_ERROR("Socket create error");
		this->lg.close();
		exit(1);
		break;
	
	case CLIENT_CONNECT_ERRROR:
		LOG_ERROR("Client connect error");
		this->lg.close();
		exit(1);
		break;

	case BIND_ERROR:
		LOG_ERROR("Bind error");
		this->lg.close();
		exit(1);
		break;

	case LISTEN_ERROR:
		LOG_ERROR("Listen error");
		this->lg.close();
		exit(1);
		break;

		default:
			__asm__("nop");
	}
	
	// Server starting here
	this->lg.write(NOTE, "Starting server...");

	while (1) {
		SOCKET cli = t.acceptClient(sock, &sockinfo, &error);
		if (cli == 0) {
			this->lg.write(WARNING, "Accecpt client error");
		}

		std::cout << "Client connected from: " << sockinfo.clientsrcadress << " port: " << std::to_string(sockinfo.clientsrcport) << std::endl;

		std::string buffer;
		buffer.clear();

		while (1) {
			// Reading data
			buffer = t.recvData(cli);
			if (buffer.length() <= 0) {
				break;
			}

			BOOL hscFound = FALSE;
			
			Request req(buffer);
			std::string filename = req.parseRequest();

			int i = 0;
			for (i = 0; i < this->hscList.size(); i++) {
				if (this->hscList.at(i).name.compare(filename) == 0) {
					hscFound = TRUE;
					break;
				}
			}

			if (hscFound) {
				HttpBridge brdg(buffer);
				this->brg = brdg;

				std::thread thr(this->hscList.at(i).funcPtr);
				thr.join();

				std::string doc = readFile(HTTP_SERVER_TEMP_FILE_LOCATION);
				std::string all = createHeader(doc.length(), TEXT_FILE) + doc;

				std::cout << all << std::endl;

				t.sendData(cli, all);
				continue;
			}

			// If name of requested resource is "\" or "" "inedex.html" is returned
			if (strcmp(filename.c_str(), "/") == 0 || strcmp(filename.c_str(), "") == 0) {
				filename = std::string("index.html");
			}
			std::string doc = readFile(this->homedir + std::string("\\") + filename);
			
			// Check if file is BIN or TEXT
			int filetype = req.checkFileType(filename);
			
			if (filetype == BIN_FILE) {
				int fsize = getbinsize(filename);
				std::string header = createHeader(fsize, filetype);
				int length = header.length();
				send(cli, header.c_str(), length, 0);
				send(cli, readbin(filename), fsize, 0);
			} else {
				std::string all = createHeader(doc.length(), filetype) + doc;
				t.sendData(cli, all);
			}
		}

		// Client disconnected, close everything
		t.close(cli);
		this->lg.write(NOTE, "Client disconnected");

	}
}

HttpBridge Httpserver::getBridge(void) {
	return this->brg;
}

size_t Httpserver::getbinsize(std::string filename) {
	FILE *f;
	std::string buffer;
	//buffer = this->homedir + std::string("\\") + filename.substr(1);	
	f = fopen(buffer.c_str(), "rb");
	if (!f) {
		return 0;
	}
	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);
	fclose(f);
	
	return size;
}

char* Httpserver::readbin(std::string filename) {
	FILE *f;
	f = fopen(filename.c_str(), "rb");

	if (!f)
		return nullptr;
	
	int size = (size_t)getbinsize(filename);
	char* filetab = (char*)malloc(size * sizeof(char*));

	if (filetab == nullptr)
		return nullptr;

	fread(filetab, 1, size, f);
	fclose(f);
	
	return filetab;
	
}

std::string Httpserver::error404(void) {
	std::string err = std::string("<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\r\n") + 
	std::string("<html>\r\n") +
	std::string("<head>\r\n") +
	std::string("<title>404 Not Found</title>\r\n") +
	std::string("</head>\r\n") +
	std::string("<body>\r\n") +
	std::string("<h1>Not Found</h1>\r\n") +
	std::string("<p>The requested URL was not found on this server.</p>\r\n") +
	std::string("<hr>\r\n") +
	std::string("<address> " + std::string(SERVER_NAME) + " </address>\r\n") +
	std::string("</body>\r\n") +
	std::string("</html>\r\n");
	
	return err;
}



std::string Httpserver::readFile(std::string filename) {
	std::fstream doc;
	std::string buffer;
	doc.open(filename, std::ios::in);
	if (doc.is_open() != TRUE)
		return error404();
	std::stringstream stream;
    stream << doc.rdbuf();
    doc.close();
    
    return stream.str();
}

std::string Httpserver::createHeader(int size, int doctype) {
	std::string header = std::string("HTTP/1.1 200 \r\n");
	
    header += std::string("Date: ") + getDate() + std::string(" ") + getTime() + std::string("\r\n");
    
    header += std::string("Server: ") + std::string(SERVER_NAME) + ("\r\n");
    
	header += std::string("Accept-Ranges: bytes\r\n");
	
	header += std::string("Content-Type: ");
	if (doctype == BIN_FILE) {
		header += std::string("image/jpeg\r\n");
	} else {
		header += std::string("text/html\r\n");
	}
	
	std::ostringstream ss;
    ss << size;
	header += std::string("Content-Length: ") + ss.str() + std::string("\r\n");
	
	header += std::string("Keep-Alive: timeout=5, max=100\r\n");

	header += std::string("Access-Control-Allow-Origin: *\r\n");
	
	header += std::string("Connection: Keep-Alive\r\n");
	
	header += std::string("\r\n");
						 
	return header;
}