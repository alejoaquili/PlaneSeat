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

// this is object structures

typedef struct objectCDT{
    char* key;
    void* value;
    Type type;
}objectCDT;

typedef objectCDT* objectADT;

typedef struct jsonCDT{
    arrayADT nodes;
    int size;
}jsonCDT;
typedef jsonCDT* jsonADT;

// end on object structures

// buffer structures

typedef struct bufferCDT{
    void* buffer;
    int size;
}bufferCDT;

typedef bufferCDT* bufferADT;


// end of buffer structures

//---------------Array contract------------------------------------------------

arrayADT newArray();
arrayNodeADT newArrayNode(void * value, Type type);
int getArraySize(arrayADT array);
int addNodeToArray(arrayADT array, arrayNodeADT node);
void* getValueInArray(arrayADT array ,int index);
Type getTypeFromIndex(arrayADT array, int index);

void freeArray(arrayADT array);
static void freeArrayNode(arrayNodeADT node);
static void freeList(arrayNodeADT node);
static void* searchForValue(arrayNodeADT node, int index);
static arrayNodeADT getNodeAt(arrayNodeADT node, int index);

char* arrayNodeToString(arrayNodeADT node);
char * toString(void * value, Type type);

// primitives string methods
char* intToString(int number);
char* doubleToString(double number);
char* stringToString( char* string);
char* arrayToString(arrayADT array);

//----------------End of array contract----------------------------------------

//------------------- beginnig of buffer contract -----------------------------
bufferADT newBuffer();
int addToBuffer(bufferADT buffer,void* value, int size);
int getBufferSize(bufferADT buffer);
void getLastFromBuffer(bufferADT buffer, void* destination, int size);
void freeBuffer(bufferADT buffer);
static int addSpaceToBuffer(bufferADT buffer, int size);
//------------------------------- end -----------------------------------------

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

Type getTypeFromIndex(arrayADT array, int index)
{
    if(array == NULL || index < 0)
        return NULL;
    
    arrayNodeADT node = getNodeAt(array->first, index);
    return node->type;
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

static arrayNodeADT getNodeAt(arrayNodeADT node, int index)
{
    if(node == NULL)
        return NULL;
    if(index == 0)
        return node;
    return getNodeAt(node->next, index-1);
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
            return stringToString((char*)value);
        case Array:
            return arrayToString((arrayADT) value);
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

char* stringToString( char* string)
{
    char* newString = calloc(strlen(string)+1, sizeof(char));
    memcpy(newString, string, strlen(string));
    return string;
}


char* arrayToString(arrayADT array)
{
    char* opening = "[";
    char* closing = "]";
    char* comma = ",";
    char* arrayToString;
    bufferADT buffer = newBuffer();
    addToBuffer(buffer, opening,1);
    for(int i = getArraySize(array)-1 ; i >= 0; i--)
    {
        char* indexString = toString(getValueInArray(array,i), getTypeFromIndex(array,i));
        int size = strlen(indexString);
        addToBuffer(buffer, indexString, size);
        if(i != 0)// to prevent last item from putting a comma
            addToBuffer(buffer, comma, 1);
    }
    addToBuffer(buffer, closing, 1);

    char* composedArrayString = calloc(getBufferSize(buffer), sizeof(char));
    getLastFromBuffer(buffer,composedArrayString, getBufferSize(buffer));
    freeBuffer(buffer);
    return composedArrayString;    
}

//------------------- End of primitives string methods ------------------------






//--------------------- beginning of buffer methods----------------------------
// this buffer is a stack

bufferADT newBuffer()
{
    bufferADT new = calloc(1, sizeof(bufferCDT));
    new->size = 0;
    new->buffer = NULL;
    return new;
}

static int addSpaceToBuffer(bufferADT buffer, int size)
{
    void * newMemmory = calloc(size + buffer->size, sizeof(char));
    if(memcpy(newMemmory, buffer->buffer, buffer->size) == NULL)
        return -1;
    free(buffer->buffer);
    buffer->buffer = newMemmory;
    buffer->size += size;
    return 1;
}


int addToBuffer(bufferADT buffer,void* value, int size)
{
    if(buffer == NULL || size < 0)
        return -1;

    addSpaceToBuffer(buffer, size);
    return memcpy(buffer->buffer+buffer->size - size,value, size ) != NULL;   
}



void getLastFromBuffer(bufferADT buffer, void* destination, int size)
{
    memcpy(destination, buffer->buffer+buffer->size - size, size);
    buffer->size -= size;    
}

int getBufferSize(bufferADT buffer)
{
    return buffer->size;
}

void freeBuffer(bufferADT buffer)
{
    free(buffer->buffer);
    free(buffer);
}


// --------------------------- end of buffer ----------------------------------




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


int testArrayToString(void)
{
    arrayADT array = newArray();
    char* subject1 =  "poo";
    char* subject2 =  "pi";
    char* subject3 =  "paw";

    arrayNodeADT node1 = newArrayNode(subject1, String);
    arrayNodeADT node2 = newArrayNode(subject2, String);
    arrayNodeADT node3 = newArrayNode(subject3, String);
    addNodeToArray(array, node1);
    addNodeToArray(array, node2);
    addNodeToArray(array, node3);
    char* string = arrayToString(array);
    char* expectedResult = "[poo,pi,paw]";
    return strcmp(string,expectedResult ) == 0;
}


int testArrayOfArraytoString(void)
{
    arrayADT array = newArray();
    arrayADT subjects = newArray();

    char* subject1 =  "poo";
    char* subject2 =  "pi";
    char* subject3 =  "paw";

    int age = 21;
    char* name =  "santi";

    arrayNodeADT node1 = newArrayNode(subject1, String);
    arrayNodeADT node2 = newArrayNode(subject2, String);
    arrayNodeADT node3 = newArrayNode(subject3, String);
    addNodeToArray(subjects, node1);
    addNodeToArray(subjects, node2);
    addNodeToArray(subjects, node3);
    arrayNodeADT nameNode = newArrayNode(name, String);
    arrayNodeADT ageNode = newArrayNode(&age, Integer);
    addNodeToArray(array, nameNode);
    addNodeToArray(array, ageNode);
    arrayNodeADT subjectsNode = newArrayNode(subjects, Array);

    addNodeToArray(array, subjectsNode);
    char* string = arrayToString(array);
    char* expectedResult = "[poo,pi,paw]";
    printf("the result was ... %s\n", string );
    return strcmp(string,expectedResult ) == 0;
}


int testBuffer()
{
    bufferADT buffer = newBuffer();
    char* testString = "santi";
    int size = strlen(testString) + 1;
    addToBuffer(buffer, testString, size);
    char* str = malloc(size * sizeof(char));
    getLastFromBuffer(buffer, str, size);
    freeBuffer(buffer);
    return strcmp(str, testString) == 0;
}


int main(void)
{
    printf("going to run tests...\n");
    printf(" the result was %s\n",(test_intToString() == 0)?"False" : "True" );
    printf(" the result was %s\n",(test_doubleToString() == 0)?"False" : "True" );
    printf(" the result was %s\n",(testBuffer() == 0)?"False" : "True" );
    printf(" the result was %s\n",(testArrayToString() == 0)?"False" : "True" );
    printf(" the result was %s\n",(testArrayOfArraytoString() == 0)?"False" : "True" );
    printf("DONE!\n");
}