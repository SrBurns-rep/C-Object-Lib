#include "object.h"

int obj_MapMembers(Object* obj, size_t* sizes, size_t* type_sizes, size_t element_count){
    int byteSizeAux = 0;

    obj->element_count = element_count;

    // calc object size in bytes
    for(int i = 0; i < element_count; i++){

        byteSizeAux += sizes[i] * type_sizes[i];

        obj->size_vector[i] = sizes[i];
        obj->type_size_vector[i] = type_sizes[i];
    }

    obj->byte_count = byteSizeAux;
    obj->is_dynamic = false;

    return byteSizeAux;

}

Object* obj_Alloc(size_t* sizes, size_t* type_sizes, size_t element_count){

    size_t sizeInBytes = 0;

    Object* obj = malloc(sizeof(Object)); // Allocates object pointer

    size_t* auxS = malloc(element_count * sizeof(size_t)); // Allocates element size array
    size_t* auxT = malloc(element_count * sizeof(size_t)); // Allocates element type size array

    obj->size_vector = auxS;
    obj->type_size_vector = auxT;

    sizeInBytes = obj_MapMembers(obj, sizes, type_sizes, element_count); // Maps the object

    void* dataAux = calloc(1, sizeInBytes); // Allocates data buffer and fill with 0s

    obj->data = dataAux;
    obj->is_dynamic = true;

    return obj;
}

int obj_Free(Object* obj){
    if(obj->is_dynamic){ // Checks if it can be destroyed

        // Passing to variables to ease the process

        size_t* fauxS = obj->size_vector;
        size_t* fauxT = obj->type_size_vector;

        Object* fauxObj = obj;

        free(fauxS);
        free(fauxT);
        free(fauxObj); 

        return EXIT_SUCCESS;
    }else{

        return EXIT_FAILURE;
    }
}

int obj_SetAtIndex(Object* obj, void* src, size_t index){

    if(index >= obj->element_count) return EXIT_FAILURE; // Checks if index is higher than the number of elements

    // Calculating data shift:

    int shift = 0;

    if(! index){ // If index is '0' there's no shift 

        size_t size = obj->size_vector[0];
        size_t type = obj->type_size_vector[0];

        memcpy(obj->data, src, size * type);

        return EXIT_SUCCESS;
    }

    for(int i = 0; i < index; i++){ // For higher indexes the total shift must be a summation

        size_t size = obj->size_vector[i];
        size_t type = obj->type_size_vector[i];

        shift += size*type;
    }

    // Actual size to be copied
    size_t size = obj->size_vector[index];
    size_t type = obj->type_size_vector[index];

    void* cpy = (void*)(obj->data + shift); // cpy refers to data buffer pointer plus the calculated shift

    memcpy(cpy, src, size * type);

    return EXIT_SUCCESS;
}

void* obj_GetFromIndex(Object* obj, size_t index){

    if(index >= obj->element_count) return NULL; // Returns null if index is higher than allowed in the object size array

    int shift = 0;

    // Same process

    if(! index){ 

        return obj->data;
    }

    for(int i = 0; i < index; i++){
        size_t size = obj->size_vector[i];
        size_t type = obj->type_size_vector[i];

        shift += size * type;
    }

    void* aux = (void*)(obj->data + shift); // aux refers to data buffer plus the calculated shift

    return aux; // returns the reference with shift in memory

}
