#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <thread>
#include "telnet.h"
#include "request.h"
#include "log.h"

// Server params
#define SERVER_NAME "UHTTP"
#define HTTP_SERVER_TEMP_FILE_LOCATION ".\\temp\\ret0.html"

// Typedef for giving Hsc function pointer
typedef void (*serve)(void);

// Error macros
#define LOG_ERROR(a) this->lg.write(ERROR, a);
#define OUTPUT_STREAM_ERROR 1

// Hsc struct definition
struct Hsc {
	serve funcPtr;
	std::string name;
};

class HttpBridge {
	public:
		HttpBridge() {};
		/* Empty constructor */

		HttpBridge(std::string);
		/* Main constructor (<raw request>)

		Returns: nothing
		*/

		int getOutputStream(std::fstream*);
		/* Get pointer to output file stream (<fstream pointer>)

		Returns: OUTPUT_STREAM_ERROR on error or 0 on success
		*/

		void closeOutputStream(std::fstream*);
		/* Close output stream (<fstream pointer>)

		Returns: nothing
		*/

		Request getRequestObject(void);
		/* Get request object to for ex. parse request and get params (<none>)

		Returns: Request object
		*/

		std::string getRaw(void);
		/* Get raw data that was put while creating object (<none>)

		Returns: raw data
		*/


	private:
		std::string raw; // Varrible for holding raw input data
};

class Httpserver {
	public:
		Httpserver() { };
		/* Empty constructor */

		~Httpserver();
		/* Destuctor */

		Httpserver(std::string, int, int, std::string, std::string);
		/* Main constructor (<ip address whitch you want server to listen on>,
							 <port whitch you want server to listen on>,
							 <enable or disable logging>,
							 <path for logs (not needed if previous parameter is disabled)>,
							 <path for HTML docs>)
		Returns: nothing
		*/
		
		void start(void);
		/* Start server with previously given parameters (<none>)

		Returns: Nothing
		*/

		HttpBridge getBridge(void);
		/* Get HttpBrodge object (<none>)

		Returns: HttpBridge object
		*/

		void addHsc(Hsc);
		/* Add Hsc (HTTP Server Code) (<Hsc structure>)

		Returns: Nothing
		*/
	private:

		// Functions
		void log(std::string); // Log function
		std::string createHeader(int, int); // Creating header
		std::string readFile(std::string); // Reading text file
		std::string error404(void); // Error 404 creating
		char* readbin(std::string); // Read binary file
		size_t getbinsize(std::string); // Get size of binary file
		
		// Varribles
		std::string ip; // Server listening ip
		std::string homedir; // HTML docs folder
		int port; // Server listening port
		int iflog; // Log or not
		Log lg; // Logger object
		HANDLE hThread; // Thread for current Hsc thread
		HttpBridge brg; // HttpBridge object
		std::vector<Hsc> hscList; // List of Hsc objects
};

#endif
