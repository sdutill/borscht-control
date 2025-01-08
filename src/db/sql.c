#include "sqlite3.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "db/sql.h"

int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
  for(int i = 0; i < argc; i++)
    {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
  printf("\n");
  return 0;
}

int insert_data(sqlite3 *db, char *table, char *column, char *value)
{
  char *err_msg = 0;
  char sql[800];
  sprintf_s(sql, sizeof(sql), "INSERT INTO \"%s\" (\"%s\") VALUES ('%s')", table, column, value);
  printf("Executing SQL: %s\n", sql);

  int rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
  if(rc != SQLITE_OK)
    {
      fprintf(stderr, "SQL error: %s\n", err_msg);
      sqlite3_free(err_msg);
      return 1;
    }
  return 0;
}
int select_table(sqlite3 *db)
{
  char *err_msg = 0;
  char *sql = "SELECT * FROM Friends";

  int rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
  if(rc != SQLITE_OK)
    {
      fprintf(stderr, "SQL error: %s\n", err_msg);
      sqlite3_free(err_msg);
      return 1;
    }
  return 0;
}