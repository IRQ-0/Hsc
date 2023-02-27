#include <iostream>
#include "http.h"
#include "log.h"
#include "mysql.h"
#include "clock.h"
#include "osinfo.h"
#include "site.h"
#include "inetinfo.h"

// Server configuration
#define IP "127.0.0.1"
#define PORT 8000
#define LOG_FOLDER "logs"
#define HTML_FOLDER "httpHome"

// Mysql DB params
#define DB_USER "mysql"
#define DB_PASSWORD "database only"
#define DB_HOST "127.0.0.1"
#define DB_DATABASE "db"
#define DB_PORT 3306

// JSON params
#define CPU_USAGE_NAME "procusge"
#define PHYS_MEM_USAGE_NAME "physusage"
#define VIRT_MEM_USAGE_NAME "virtusage"

// Inrernal server error definition
#define INTERNAL_SERVER_ERROR \
	out << "<html>" << std::endl; \
	out << "<head>" << std::endl; \
	out << "<title> Login </title>" << std::endl; \
	out << "</head>" << std::endl; \
	out << "<body>" << std::endl; \
	out << "<h1> Internal server error </h1>" << std::endl; \
  	out << "<a href=\"index.html\"> Get back to login site </a>" << std::endl; \
  	out << "</body>" << std::endl; \
  	out << "</html>" << std::endl; \
	bridge.closeOutputStream(&out); \
  	return;

void __cdecl login(void);
void __cdecl panel(void);
void __cdecl stats(void);
void __cdecl ipconfig(void);

Httpserver httpserver(IP, PORT, 1, LOG_FOLDER, HTML_FOLDER);
MYSQL* conn;

int main(void) {
	connection cn;
    cn.user = DB_USER;
    cn.passwd = DB_PASSWORD;
    cn.host = DB_HOST;
    cn.db = DB_DATABASE;
    cn.port = DB_PORT;

    conn = dbConnect(cn);

  	if (conn == NULL) {
    	std::cout << "Cannot connect to db" << std::endl;
    	return 1;
  	} else {
   		std::cout << "Connected" << std::endl;
  	}

  	Hsc loginHsc;
	loginHsc.name = std::string("login.hsc");
	loginHsc.funcPtr = login;

	Hsc statsHSc;
	statsHSc.name = std::string("stats.hsc");
	statsHSc.funcPtr = stats;

	Hsc ipconfigHsc;
	ipconfigHsc.name = std::string("ipconfig.hsc");
	ipconfigHsc.funcPtr = ipconfig;

	Hsc panelHsc;
	panelHsc.name = std::string("panel.hsc");
	panelHsc.funcPtr = panel;

	httpserver.addHsc(loginHsc);
	httpserver.addHsc(statsHSc);
	httpserver.addHsc(ipconfigHsc);
	httpserver.addHsc(panelHsc);
	httpserver.start();

  	dbDisconnect(conn);

	return 0;
}

void __cdecl panel(void) {
	HttpBridge bridge = httpserver.getBridge();
	Request req = bridge.getRequestObject();
	std::fstream out;
	bridge.getOutputStream(&out); 

	// Actual code

	siteParams prm;
  	OsInfo info;
  	INET_INFO inf;
  	
  	inf = getInetInfo();
  	prm.inetInfo = inf;
	prm.motherboardName = info.getMotherBoardName();
 	prm.motherboardVersion = info.getMotherBoardVersion();
  	prm.BIOSDate = info.getBiosReleaseDate();
 	prm.BIOSVendor = info.getBiosVendor();
 	prm.BIOSVersion = info.getBiosVersion();
	prm.CPUClock = info.getCpuMhz();
	prm.CPUName = info.getCpuName();
	prm.CPUType = info.getCpuType();
 	prm.CPUThreads = info.getCpuThreads();

	prm.memoryMinAllocAddr = info.getMinimumAllocationAdrress();
  	prm.memoryMaxAllocAddr = info.getMaximumAllocationAdrress();
  	prm.memoryVirtualTotal = info.getTotalVirtualMemory();
  	prm.memoryPhysicalTotal = info.getTotalPhysicalMemory();
  
  	out << createSite(prm);

	bridge.closeOutputStream(&out);
}

void __cdecl ipconfig(void) {
	HttpBridge bridge = httpserver.getBridge();
	Request req = bridge.getRequestObject();
	std::fstream out;
	bridge.getOutputStream(&out); 

	// Actual code

	out << getIpconfig();
	
	bridge.closeOutputStream(&out); 	
}

void __cdecl stats(void) {
	HttpBridge bridge = httpserver.getBridge();
	Request req = bridge.getRequestObject();
	std::fstream out;
	bridge.getOutputStream(&out); 

	// Actual code

	OsInfo info;
	std::string json = "{\"" + std::string(CPU_USAGE_NAME) + "\" : \"";

	json += info.getCpuUsage();

	json += "\", \"" + std::string(PHYS_MEM_USAGE_NAME) + "\" : \"";

	json += info.getUsedPhysicalMemoryInProc();

	json += "\", \"" + std::string(VIRT_MEM_USAGE_NAME) + "\" : \"";

	json += info.getUsedVirtualMemoryInProc();

	json += "\"}";

	out << json << std::endl;

	bridge.closeOutputStream(&out);
}

void __cdecl login(void) {
	HttpBridge bridge = httpserver.getBridge();
	Request req = bridge.getRequestObject();
	std::fstream out;
	bridge.getOutputStream(&out);

	// Actual code

	std::string login, password;
	login = req.getParam("login");
	password = req.getParam("password");

	std::string query = "SELECT password FROM users WHERE username = '" + login + "';";

	runQuery(conn, query.c_str());

  	MYSQL_RES* res = getResult(conn);
  	int fields = getFields(res);

  	MYSQL_ROW row = mysql_fetch_row(res);

  	if (row == NULL || row == (MYSQL_ROW) 0) {
  		out << "<html>" << std::endl;
		out << "<head>" << std::endl;
		out << "<title> Login </title>" << std::endl;
		out << "</head>" << std::endl;
		out << "<body>" << std::endl;

  		out << "<h1> Invalid login or password </h1>" << std::endl;
  		out << "<a href=\"index.html\"> Get back to login site </a>" << std::endl;

  		out << "</body>" << std::endl;
		out << "</html>" << std::endl;
		bridge.closeOutputStream(&out);
  		return;
  	}

  	if (password.compare(std::string(row[0])) == 0) {
  		out << "<html>" << std::endl;
		out << "<head>" << std::endl;
		out << "<title> Login </title>" << std::endl;
		out << "</head>" << std::endl;
		out << "<body>" << std::endl;

  		out << "<script> window.location.href = \"http://127.0.0.1:8000/panel.hsc\" </script>" << std::endl;

  		out << "</body>" << std::endl;
		out << "</html>" << std::endl;
		bridge.closeOutputStream(&out);

		query = "UPDATE users SET lastlogin = '" + getTimeStamp() + "' WHERE username = '" + login + "';";

		if (runQuery(conn, query) != QUERY_OK) {
			INTERNAL_SERVER_ERROR;
			exit(1);
		}

  	} else {
  		out << "<h1> Invalid login or password </h1>" << std::endl;
  		out << "<a href=\"index.html\"> Get back to login site </a>" << std::endl;

  		out << "</body>" << std::endl;
		out << "</html>" << std::endl;
  	}

	bridge.closeOutputStream(&out);
}