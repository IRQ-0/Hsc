#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <vector>
#include <sstream>

// File types
#define TEXT_FILE 1
#define BIN_FILE 2

// Defined image extentions
#define IMAGE_EXTENTIONS_NUMBER 5

class Request {
	public:
		Request() {};
		/* Empty constructor */
		
		Request(std::string);
		/* Main constructor (<raw request>)

		Returns: Nothing
		*/


		std::string parseRequest(void);
		/* Look for GET signature in raw request (<none>)

		Returns: Line with GET signature
		*/

		std::string getRaw(void);
		/* Get raw request given earlier (<none>)

		Returns: Raw request
		*/

		std::string getElement(std::string);
		/* Get specified in request element value (<element name>)

		Returns: Element value
		*/

		int checkFileType(std::string);
		/* Check if requested file is text or binary (<filename>)

		Returns: TEXT_FILE or BIN_FILE
		*/

		std::string getParam(std::string);
		/* Get parameter given by request (<parameter name>)

		Returns: Parameter value
		*/

	private:
		// Functions
		std::vector<std::string> parseLines(void); // Parse request into seperete lines
		std::string getDocumentName(void); // Get requested file name

		// Varribles
		std::vector<std::string> vect; // Request lines
		std::string raw; // Raw request
		
		// Constants
		const char* imgaeextentions[IMAGE_EXTENTIONS_NUMBER] = {".JPG", ".JPE", ".BMP", ".GIF", ".PNG"}; // Defined possible image extentions
};

#endif
