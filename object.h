#ifndef _OBJECT_H
#define _OBJECT_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// QOL macros:
#define CHAR_S      sizeof(char)  // char size
#define SHORT_S     sizeof(short) // short...
#define INT_S       sizeof(int)
#define FLOAT_S     sizeof(float)
#define DOUBLE_S    sizeof(double)
#define LONG_S      sizeof(long)
#define LONGLONG_S  sizeof(long long)

/**
 * Element struct.
*/
typedef struct ElementType{
    size_t size;
    size_t type;
    void* data;
}Element;
/**
 * Object struct.
*/
typedef struct ObjectType{
    unsigned char id[32];
    Element* table;
    size_t element_count;
    size_t byte_count;

}Object;
/**
 * Makes an id with object size in bytes, element count and it's own
 * address on memory, object size in bytes and the element count
 * can work as a "class" to define object format, and the object
 * address on memory can be used as an individual identifier.
*/
void obj_IdMake(Object* obj);
/**
 * Allocates de object, map the members using indexes from typearr and
 * sizearr
*/
Object* obj_Alloc(size_t* sizearr, size_t* typearr, int element_count);

/**
 * Access an element from the object using an index
*/
void* obj_Access(Object* obj, int index);

/**
 * Set an element in the object using an index
*/
int obj_Set(Object* obj, void* src, int index);

/**
 * Get an element in the object using an index
 * this function uses a buffer to return the
 * element
*/
void* obj_Get(Object* obj, void* buffer, int index);

/**
 * [WIP] Adds a new element in object
*/
int obj_ObjAppend(Object* obj, void* src, size_t srcsize, size_t srctype);

/**
 * [WIP] Adds array in src to a element in object
*/
int obj_ElementAppend(Object* obj, void* src, size_t srcsize, int index);

/**
 * Sets a user defined ID
*/
void obj_SetID(Object* obj, char* id);

/**
 * Gets object ID
*/
char* obj_GetID(Object* obj, char* idbuffer);

/**
 * Compares the first two segments of two objects IDs, return true
 * if equal, false otherwise
*/
bool obj_IdCompare(Object* obj1, Object* obj2);

#endif