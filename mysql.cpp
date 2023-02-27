#include "mysql.h"

MYSQL* dbConnect(connection cn) {
  MYSQL* conn = mysql_init(NULL);
  if (mysql_real_connect(conn, cn.host.c_str(), cn.user.c_str(), cn.passwd.c_str(), cn.db.c_str(), cn.port, NULL, 0) == NULL) {
    fprintf(stderr, "%s\n", mysql_error(conn));
    return NULL;
  }
  
  return conn;
}

int runQuery(MYSQL* conn, std::string query) {
  if (mysql_query(conn, query.c_str()))
    return QUERY_ERROR;
  else
    return QUERY_OK;
}

void dbDisconnect(MYSQL* conn) {
  mysql_close(conn);
}

MYSQL_RES* getResult(MYSQL* conn) {
  return mysql_store_result(conn);
}

int getFields(MYSQL_RES* res) {
  return mysql_num_fields(res);
}
