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

    Object* obj = malloc(sizeof(Object));

    size_t* auxS = malloc(element_count * sizeof(size_t));
    size_t* auxT = malloc(element_count * sizeof(size_t));

    obj->size_vector = auxS;
    obj->type_size_vector = auxT;

    sizeInBytes = obj_MapMembers(obj, sizes, type_sizes, element_count);

    void* dataAux = calloc(1, sizeInBytes);

    obj->data = dataAux;
    obj->is_dynamic = true;

    return obj;
}

int obj_Free(Object* obj){
    if(obj->is_dynamic){

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

    if(index >= obj->element_count) return EXIT_FAILURE;

    int shift = 0;

    if(! index){
        memcpy(obj->data, src, obj->size_vector[0] * obj->type_size_vector[0]);

        return EXIT_SUCCESS;
    }

    for(int i = 0; i < index; i++){

        size_t size = obj->size_vector[i];
        size_t type = obj->type_size_vector[i];

        shift += size*type;
    }

    size_t size = obj->size_vector[index];
    size_t type = obj->type_size_vector[index];

    void* cpy = (void*)(obj->data + shift);

    memcpy(cpy, src, size * type);

    return EXIT_SUCCESS;
}

void* obj_GetFromIndex(Object* obj, size_t index){

    if(index >= obj->element_count) return NULL;

    int shift = 0;

    if(! index){

        return obj->data;
    }

    for(int i = 0; i < index; i++){
        size_t size = obj->size_vector[i];
        size_t type = obj->type_size_vector[i];

        shift += size * type;
    }

    void* aux = (void*)(obj->data + shift);

    return aux;

}
