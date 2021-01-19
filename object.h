#ifndef _OBJECT_H
#define _OBJECT_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define PWORD sizeof(void*)

/**
 * Object struct.
*/
typedef struct ObjectType{
    size_t* size_vector;
    size_t* type_size_vector;
    size_t element_count; 
    size_t byte_count;
    bool is_dynamic;
    void* data;

}Object;

/**
 * This function creates a map for the elements in the object,
 * it takes an already allocated object pointer, an array of element sizes
 * an array of the sizes of each element and the number of elements.
*/
int obj_MapMembers(Object* obj, size_t* sizes, size_t* type_sizes, size_t element_count);

/**
 * This function has a map integrated and allocates the object automatically.
*/
Object* obj_Alloc(size_t* sizes, size_t* type_sizes, size_t element_count);

/**
 * Destroy an object if allocated by obj_Alloc.
*/
int obj_Free(Object* obj);

/**
 * Set an element in src at index in data buffer. 
*/
int obj_SetAtIndex(Object* obj, void* src, size_t index);

/**
 * Get an element at index from the object.
*/
void* obj_GetFromIndex(Object* obj, size_t index);

#endif