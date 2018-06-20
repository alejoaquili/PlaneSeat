#include "dataBaseADT.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h> 

typedef struct dataBaseCDT
{
   sqlite3 * db;
   char * errorMessage;
   int returnCode;
}dataBaseCDT;

static int callBackShow(void * notUsed, int argc, char ** argv, char ** colName) 
{
   for(int i = 0; i < argc; i++) 
      printf("%s = %s\n", colName[i], argv[i] ? argv[i] : "NULL");
   printf("\n");
   return 0;
}

static int callBackNotShow(void * notUsed, int argc, char ** argv, char ** colName) 
{
   return 0;
}

dataBaseADT createDataBase(char * dBName, boolean foreignKey) 
{
   dataBaseADT dataBase = malloc(sizeof(dataBaseCDT));
   
   if(dBName == NULL || strcmp(dBName, ":memory:") == 0)
      printf("Creating an in-memory database in RAM that lasts only for the duration of the session.\n");

   dataBase->errorMessage = NULL;
   dataBase->returnCode = sqlite3_open(dBName, &dataBase->db);

   if(dataBase->returnCode) 
   {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(dataBase->db));
      return NULL;
   } 
   if(foreignKey)
      sqlite3_db_config(dataBase->db, SQLITE_DBCONFIG_ENABLE_FKEY, 1, NULL);
   return dataBase;
}

dataBaseADT openDataBase(char * dBName)
{
   if(dBName == NULL || strcmp(dBName, ":memory:") == 0)
      return NULL;
   dataBaseADT dataBase = malloc(sizeof(dataBaseCDT));

   if(dataBase->returnCode) 
   {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(dataBase->db));
      return NULL;
   } 
   return dataBase;
}

int deleteDataBase(dataBaseADT dataBase, boolean waitStatements)
{
   if(dataBase == NULL)
      return -1;
   sqlite3_stmt * preparedStatement;
   int result = sqlite3_close(dataBase->db);
   if(result == SQLITE_BUSY)
   {
      if(!waitStatements)
         return result;
      while(result == SQLITE_BUSY)
      {
         preparedStatement = sqlite3_next_stmt(dataBase->db, NULL);
         if(preparedStatement)
            sqlite3_finalize(preparedStatement);
         result = sqlite3_close(dataBase->db); 
      }
   }
   free(dataBase);
   return result;
}

int closeDataBase(dataBaseADT dataBase)
{
   if(dataBase == NULL)
      return -1;
   return sqlite3_close(dataBase->db);
}

int executeQueryDataBase(dataBaseADT dataBase, const char * query, boolean showQuery)
{
   if(dataBase == NULL)
      return -1;
   if(showQuery)
      dataBase->returnCode = sqlite3_exec(dataBase->db, query, callBackShow, 0, &(dataBase->errorMessage));
   dataBase->returnCode = sqlite3_exec(dataBase->db, query, callBackNotShow, 0, &(dataBase->errorMessage));
   if(dataBase->returnCode != SQLITE_OK)
   {
      fprintf(stderr, "SQL error: %s\n", dataBase->errorMessage);
      sqlite3_free(dataBase->errorMessage);
      return 1;
   }
   return 0;
}

