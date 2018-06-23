#include <stdio.h>
#include "dataBase.h"
#include "include/CuTest.h"
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "include/CuTestSO.h"
#include "string.h"

void testCreateDb(CuTest* tc);
void testInserInDb(CuTest* tc);

void givenADbName();
void givenADb();
void givenATuppleToInsert();

int whenDbIsCreated(char * dnName);
int whenTuppleIsInserted(dataBaseADT db, char* tuppleNameForInsertion);


/*-----------------------------------
 variables for db testing
 ------------------------------------
*/
char * dbNameForTest = "dbTest";
char * dbName;
dataBaseADT db;

//------------------------------------




/*-----------------------------------
 variables for query testing
 ------------------------------------
*/
char * tableName = "COMPANY";
//------------------------------------



/*
--------------------------------------
variables for insert testing
--------------------------------------
*/

char * exampleTuppleForInsertion = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
         "VALUES (17, 'Paul', 33, 'California', 20000.00 ); " \
		;

char* tuppleNameForInsertion;
//------------------------------------


/*-----------------------------------
 variables for insert testing
 ------------------------------------
*/


//------------------------------------





void testCreateDb(CuTest* tc)
{
	givenADbName();
	CuAssert(tc,"Fail to create a db", whenDbIsCreated(dbName));
}

void testInserInDb(CuTest* tc)
{
	givenADbName();
	givenADb();
	givenATuppleToInsert();
    
	CuAssert(tc,"Fail to insert a tupple", whenTuppleIsInserted(db,tuppleNameForInsertion));
}




void givenADbName()
{
	dbName = dbNameForTest;
}

void givenADb()
{
	db = openDataBase(dbName);
}

void givenATuppleToInsert()
{
	tuppleNameForInsertion = exampleTuppleForInsertion;
}





int whenDbIsCreated(char * dnName)
{
	db = createDataBase(dbName, false);
	return !checkIfNull(db, "db creation fail");
}

int whenTuppleIsInserted(dataBaseADT db, char* tuppleNameForInsertion)
{
	return executeQueryDataBase(db,tuppleNameForInsertion,
	true);

}


CuSuite* CuQueueGetSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, testCreateDb);
	
	return suite;
}



// funciones para llevar a otro luagar

int checkIfNull(void * pointer, char* errorMessage)
{
	if(pointer == NULL)
	{
		printf("Null pointer error: %s", errorMessage);
		return true;
	}

	return false;
}
