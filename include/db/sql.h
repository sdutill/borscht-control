#ifndef SQL_H
#define SQL_H

#include "sqlite3.h"

int handle_sql_commands(int argc, char *argv[]);
int insert_data(sqlite3 *db, char *table, char *column, char *value);
int select_table(sqlite3 *db);

#endif