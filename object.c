#include "object.h"

void obj_IdMake(Object* obj){

    size_t idmem = (size_t) &obj;

    memcpy(obj->id, &obj->byte_count, sizeof(size_t));
    memcpy(obj->id + sizeof(size_t), &obj->element_count, sizeof(size_t));
    memcpy(obj->id + 2 * sizeof(size_t), &idmem, sizeof(size_t));
}

Object* obj_Alloc(size_t* sizearr, size_t* typearr, int element_count){

    if(element_count == 0) return NULL;

    size_t byteSize = 0;
    size_t shift = 0;

    Element* tableaux = malloc(element_count * sizeof(Element));

    for(int i = 0; i < element_count; i++){
        tableaux[i].size = sizearr[i];
        tableaux[i].type = typearr[i];
        byteSize += sizearr[i] * typearr[i];
    }

    Object* obj = malloc(sizeof(Object));
    void* dataux = calloc(byteSize, 1);

    for(int i = 0; i < element_count; i++){
        if(i == 0){
            tableaux[i].data = dataux;
        }else{
            shift += sizearr[i-1]*typearr[i-1];

            tableaux[i].data = (void*)(dataux + shift);
        }
    }

    obj->table = tableaux;
    obj->byte_count = byteSize;
    obj->element_count = element_count;

    obj_IdMake(obj);
    
    return obj;
}

void* obj_Access(Object* obj, int index){
    
    if(index < 0 || index >= obj->element_count) return NULL;

    return obj->table[index].data;
}

int obj_Set(Object* obj, void* src, int index){
    if(index < 0 || index >= obj->element_count) return -1;

    size_t size = obj->table[index].size;
    size_t type = obj->table[index].type;
    
    if(size == 0 || type == 0) return -1;

    memcpy(obj->table[index].data, src, size*type);

    return 0;
}

void* obj_Get(Object* obj, void* buffer, int index){
    if(index < 0 || index >= obj->element_count) return NULL;

    size_t size = obj->table[index].size;
    size_t type = obj->table[index].type;
    
    if(size == 0 || type == 0) return NULL;

    memcpy(buffer, obj->table[index].data, size*type);

    return buffer;
}

int obj_ObjAppend(Object* obj, void* src, size_t srcsize, size_t srctype){ // this function is in WIP!!
    size_t shift = 0;
    int newSize = srcsize * srctype;

    int newIndex = obj->element_count;

    ++obj->element_count;                   // new element count
    obj->byte_count += newSize;             // new byte count

    obj->table = realloc(obj->table, obj->element_count * sizeof(Element));

    void* dataux = realloc(obj->table[0].data, obj->byte_count);

    obj->table[newIndex].size = srcsize;
    obj->table[newIndex].type = srctype;

    for(int i = 0; i < newIndex; i++){
        shift += obj->table[i].size * obj->table[i].type; 
    }
    obj->table[0].data = dataux;
    obj->table[newIndex].data = (void*)(dataux + shift);

    memcpy(obj->table[newIndex].data, src, newSize);

    return 0;
}

int obj_ElementAppend(Object* obj, void* src, size_t srcsize, int index){ // worse than object append lol

    if(index < 0 || index >= obj->element_count) return -1;

    int oldElementSize = obj->table[index].size * obj->table[index].type;
    obj->table[index].size += srcsize;

    int newSize = obj->byte_count + srcsize * obj->table[index].type;

    void* newData = realloc(obj->table[index].data, newSize);

    obj->table[index].data = newData;

    memcpy(obj->table[index].data + oldElementSize, src, srcsize * obj->table[index].type);

    return 1000;
}

void obj_SetID(Object* obj, char* id){
    memcpy(obj->id, id, 32 * sizeof(unsigned char));
}

char* obj_GetID(Object* obj, char* idbuffer){
    memcpy(idbuffer, obj->id, 32 * sizeof(unsigned char));
    return idbuffer;
}

bool obj_IdCompare(Object* obj1, Object* obj2){
    for(int i = 0; i < 2 * sizeof(size_t); i++){
        if(obj1->id[i] != obj2->id[i]) return false;
    }
    return true;
}