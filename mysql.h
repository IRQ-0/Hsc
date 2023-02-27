#ifndef _MYSQL_H
#define _MYSQL_H

#include <iostream>
#include <string>
#include <mysql.h>

// Query error macro
#define QUERY_OK 0
#define QUERY_ERROR 1

// connection structore definition
struct connection {
  std::string user;
  std::string passwd;
  std::string host;
  std::string db;
  int port;
};

MYSQL* dbConnect(connection);
/* Connect to MySQL database (<connection structure>);

Returns: NULL if error or connection handle on success
*/

int runQuery(MYSQL*, std::string);
/* Run query (<connection handle
              <query>)

Returns: QUERY_OK on success or QUERY_ERROR on error
*/

void dbDisconnect(MYSQL*);
/* Diconnect with MySQL database (<connection handle>)

Returns: nothing
*/

MYSQL_RES* getResult(MYSQL*);
/* Return result handle after calling runQuery() (<connection handle>)

Returns: Result handle
*/

int getFields(MYSQL_RES*);
/* Get number of fields in result (<result handle>)

Returns: Number of fields
*/

#endif
