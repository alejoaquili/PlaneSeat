#ifndef DATABASE_ADT_H
#define DATABASE_ADT_H

#include "utils.h"

typedef struct dataBaseCDT * dataBaseADT; 

dataBaseADT createDataBase(char * dBName, boolean foreignKey);

dataBaseADT openDataBase(char * dBName);

int deleteDataBase(dataBaseADT dataBase, boolean waitStatements);

int closeDataBase(dataBaseADT dataBase);

int executeQueryDataBase(dataBaseADT dataBase, const char * query, boolean showQuery);


#endif

