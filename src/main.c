#include "db/sql.h"
#include "server/server.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void print_usage()
{
  printf("Usage:\n");
  printf("  Database commands:\n");
  printf("    -s                   Select all records from Friends table\n");
  printf("    -i <TABLE> <COLUMN> <VALUE>    Insert data into specified table\n");
  printf("  Server commands:\n");
  printf("    --start             Start the web server\n");
  printf("    --stop              Stop the web server\n");
  printf("    --help              Show this help message\n");
}

bool validate_insert_args(int argc, char *argv[])
{
  if(argc < 5)
    {
      printf("Insufficient arguments for insert operation\n");
      printf("Correct format is: \"-i <TABLE> <COLUMN> <VALUE>\"\n");
      return false;
    }

  for(int i = 2; i < 5; i++)
    {
      if(strlen(argv[i]) < 1 || strlen(argv[i]) > 256)
        {
          printf("Invalid argument length\n");
          printf("Arguments must be between 1 and 256 characters.\n");
          printf("Correct format is: \"-i <TABLE> <COLUMN> <VALUE>\"\n");
          return false;
        }
    }
  return true;
}

int main(int argc, char *argv[])
{
  if(argc < 2)
    {
      print_usage();
      return 1;
    }

  // Handle server commands
  if(strcmp(argv[1], "--start") == 0)
    {
      printf("Starting web server...\n");
      return start_server();
    }

  if(strcmp(argv[1], "--stop") == 0)
    {
      printf("Stopping web server not implemented yet\n");
      // TODO: Implement server shutdown mechanism
      return 0;
    }

  if(strcmp(argv[1], "--help") == 0)
    {
      print_usage();
      return 0;
    }

  // Handle database commands
  sqlite3 *db;
  char *err_msg = 0;

  int rc = sqlite3_open("test_db", &db);
  if(rc != SQLITE_OK)
    {
      fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
      sqlite3_close(db);
      return 1;
    }

  // Create table if it doesn't exist
  char *sql = "CREATE TABLE IF NOT EXISTS Friends(Id INTEGER PRIMARY KEY, Name TEXT);";
  rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
  if(rc != SQLITE_OK)
    {
      fprintf(stderr, "SQL error: %s\n", err_msg);
      sqlite3_free(err_msg);
      sqlite3_close(db);
      return 1;
    }

  // Handle select command
  if(strcmp(argv[1], "-s") == 0)
    {
      rc = select_table(db);
    }
  // Handle insert command
  else if(strcmp(argv[1], "-i") == 0)
    {
      if(!validate_insert_args(argc, argv))
        {
          sqlite3_close(db);
          return 1;
        }
      rc = insert_data(db, argv[2], argv[3], argv[4]);
    }
  else
    {
      printf("Unknown command: %s\n", argv[1]);
      print_usage();
      rc = 1;
    }

  sqlite3_close(db);
  return rc;
}