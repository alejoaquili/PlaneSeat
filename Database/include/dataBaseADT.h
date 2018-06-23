#ifndef DATABASE_ADT_H
#define DATABASE_ADT_H


#include "utils.h"


typedef struct dataBaseCDT * dataBaseADT; 

dataBaseADT createDataBase(char * dBName, boolean foreignKey);

dataBaseADT openDataBase(char * dBName);

int deleteDataBase(dataBaseADT dataBase, boolean waitStatements);

int closeDataBase(dataBaseADT dataBase);

int executeQueryDataBase(dataBaseADT dataBase, const char * query, boolean showQuery);

int prepareStatement(dataBaseADT dataBase, char *  query);

int finalizeStatement(dataBaseADT dataBase);

int stepStatement(dataBaseADT dataBase);

int getColumnsCountOfAStatement(dataBaseADT dataBase);

int getIntFromColumn(dataBaseADT dataBase, int col, int * ret);

int getCharFromColumn(dataBaseADT dataBase, int col);

char * getTextFromColumn(dataBaseADT dataBase, int col);

void freeDBText(char * text);


#endif

