#include "dataBaseADT.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h> 

#define READY_ROW SQLITE_ROW

#define DONE SQLITE_DONE


typedef struct dataBaseCDT
{
    sqlite3 * db;
    char * errorMessage;
    int returnCode;
    sqlite3_stmt * statement;
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
    dataBase->statement = NULL;
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
    else
    {
        dataBase->returnCode = sqlite3_exec(dataBase->db, query, callBackNotShow, 0, &(dataBase->errorMessage));
    }
    if(dataBase->returnCode != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", dataBase->errorMessage);
        sqlite3_free(dataBase->errorMessage);
        return 1;
    }
    return 0;
}

int prepareStatement(dataBaseADT dataBase, char *  query)
{
    if(dataBase == NULL || query == NULL)
    {
        fprintf(stderr, "prepareStatement():Invalid Arguments");
        return -1;
    }
    sqlite3_prepare_v2(dataBase->db, query, -1, &dataBase->statement, NULL);
    return 0;
}

int finalizeStatement(dataBaseADT dataBase)
{
    if(dataBase == NULL || dataBase->statement == NULL)
    {
        fprintf(stderr, "finalizeStatement():Invalid Arguments");
        return -1;
    }
    return sqlite3_finalize(dataBase->statement);
}

int stepStatement(dataBaseADT dataBase)
{
    if(dataBase == NULL || dataBase->statement == NULL)
    {
        fprintf(stderr, "stepStatement():Invalid Arguments");
        return -1;
    }
    return sqlite3_step(dataBase->statement);
}

int getColumnsCountOfAStatement(dataBaseADT dataBase)
{
    if(dataBase == NULL || dataBase->statement == NULL)
    {
        fprintf(stderr, "stepStatement():Invalid Arguments");
        return -1;
    }
    return sqlite3_column_count(dataBase->statement);
}

int getIntFromColumn(dataBaseADT dataBase, int col, int * ret)
{
    if(dataBase == NULL)
    {
        fprintf(stderr, "getIntFromColumn():Invalid Arguments");
        return -1;
    }
    int maxCol = getColumnsCountOfAStatement(dataBase);
    if(col >= maxCol || sqlite3_column_type(dataBase->statement, col) != SQLITE_INTEGER)
    {
        fprintf(stderr, "getIntFromColumn():Invalid Arguments");
        return -1;
    }
    *ret = sqlite3_column_int(dataBase->statement, col);
    return 0;
}

int getCharFromColumn(dataBaseADT dataBase, int col)
{
    if(dataBase == NULL)
    {
        fprintf(stderr, "getIntFromColumn():Invalid Arguments");
        return -1;
    }
    int maxCol = getColumnsCountOfAStatement(dataBase);
    if(col >= maxCol || sqlite3_column_type(dataBase->statement, col) != SQLITE_TEXT)
    {
        fprintf(stderr, "getIntFromColumn():Invalid Arguments");
        return -1;
    }
    return *((char *)sqlite3_column_text(dataBase->statement, col));
}

char * getTextFromColumn(dataBaseADT dataBase, int col)
{
    if(dataBase == NULL)
    {
        fprintf(stderr, "getIntFromColumn():Invalid Arguments");
        return NULL;
    }
    int maxCol = getColumnsCountOfAStatement(dataBase);
    if(col >= maxCol || sqlite3_column_type(dataBase->statement, col) != SQLITE_TEXT)
    {
        fprintf(stderr, "getIntFromColumn():Invalid Arguments");
        return NULL;
    }
    return (char *)sqlite3_column_text(dataBase->statement, col);
}

