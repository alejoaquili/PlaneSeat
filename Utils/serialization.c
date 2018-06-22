#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// supported data types
// arrays must be the ones declare in here no native array will be supported
typedef enum type {Integer, Double, String, Array, Object} Type;

typedef struct arrayNodeCDT{
    struct arrayNodeCDT * next;
    void * value;
    Type type;

} arrayNodeCDT;
typedef arrayNodeCDT * arrayNodeADT;

typedef struct arrayCDT{
    int size;
    arrayNodeADT first;
}arrayCDT;

typedef arrayCDT * arrayADT;


typedef struct objectCDT{
    char* key;
    void* value;
    Type type;
}objectCDT;

typedef objectCDT* objectADT;


//---------------Array contract------------------------------------------------

arrayADT newArray();
arrayNodeADT newArrayNode(void * value, Type type);
int getArraySize(arrayADT array);
int addNodeToArray(arrayADT array, arrayNodeADT node);
void* getValueInArray(arrayADT array ,int index);


void freeArray(arrayADT array);
static void freeArrayNode(arrayNodeADT node);
static void freeList(arrayNodeADT node);
static void* searchForValue(arrayNodeADT node, int index);

char* arrayNodeToString(arrayNodeADT node);
char * toString(void * value, Type type);

char* intToString(int number);
char* doubleToString(double number);
char* stringToString( char* string);


//----------------End of array contract----------------------------------------

// Array strucutre methods

arrayADT newArray()
{
    arrayADT new = calloc(1, sizeof(arrayCDT));
    new->size = 0 ;
    new->first = NULL;
    return new;
}


arrayNodeADT newArrayNode(void * value, Type type)
{
    arrayNodeADT new = calloc(1, sizeof(arrayNodeCDT));
    new->value = value;
    new->type = type;
    new->next = NULL;
    return new;
}

int addNodeToArray(arrayADT array, arrayNodeADT node)
{
    if(array == NULL || node == NULL)
        return -1;
    node->next = array->first;
    array->first = node;
    array->size++;
    return array->size;
}

void* getValueInArray(arrayADT array ,int index)
{
    if(array == NULL)
        return NULL;

    return searchForValue(array->first, index);
}

int getArraySize(arrayADT array)
{
    if(array == NULL)
        return -1;
    return array->size;
}

void freeArray(arrayADT array)
{
    freeList(array->first);
    free(array);
}

static void freeArrayNode(arrayNodeADT node)
{
    free(node->value);
    free(node);
}

static void freeList(arrayNodeADT node)
{
    if(node == NULL)
        return;
    freeList(node->next);
    freeArrayNode(node);
}

static void* searchForValue(arrayNodeADT node, int index)
{
    if(node == NULL)   
        return NULL;
    if(index == 0)
        return node->value;
    return searchForValue(node->next, index-1);
}



// serialization methods
char* arrayNodeToString(arrayNodeADT node)
{
    char * nodeRepresentation = toString(node->value, node->type);
    return nodeRepresentation;
}


char * toString(void * value, Type type)
{
    switch(type)
    {
        case Integer:
            return intToString(*(int*)value);
        case Double:
            return doubleToString(*(double*)value);
        case String:
            return stringToString(*(char**)value);
    }

    printf("not implemented yet...\n");
    return NULL;

}



//------------------- Primitives string methods ----------------


char* intToString(int number)
{
    // this will get you the number of chars needed for the number
    int length = snprintf(NULL, 0, "%d", number);
    //the +1 y for null terminated string
    char* numberString = calloc(length+1, sizeof(char));
    snprintf(numberString, length+1, "%d", number);
    return numberString;
}

char* doubleToString(double number)
{
    // this will get you the number of chars needed for the number
    int length = snprintf(NULL, 0, "%g", number);
    //the +1 y for null terminated string
    char* numberString = calloc(length+1, sizeof(char));
    snprintf(numberString, length+1, "%g", number);
    return numberString;
}

// ATTENTION MAAAAAYB I'LL CHANGE THIS AND MAKE A COPY OF TH STRING
char* stringToString( char* string)
{
    return string;
}

//------------------- End of primitives string methods ----------------







//----------------------------- Tests ---------------------------------
int test_intToString(void)
{
    int x = 450;
    char* number = intToString(x);
    return strcmp(number, "450") == 0;
}

int test_doubleToString(void)
{
    double x = 450;
    char* number = doubleToString(x);
    return strcmp(number, "450") == 0;
}

int testNodeArray(void)
{
    arrayADT array = newArray();
    char* str  = "prueba";
    arrayNodeADT node = newArrayNode(str, String);
    addNodeToArray(array, node);
    char* result  =  getValueInArray(array, 0);
    return strcmp(str, result) == 0;
    
}


int main(void)
{
    printf(" the result was %s\n",(test_intToString() == 0)?"False" : "True" );
    printf(" the result was %s\n",(test_doubleToString() == 0)?"False" : "True" );
    printf("DONE!\n");
}