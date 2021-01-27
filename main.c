#include <stdio.h>
#include "object.h"

int main(void){

    // This works like a class but without the ID
    size_t elementsSizeArray[] = {12, 5, 9, 13, 17, 21};
    size_t elementsTypeSizeArray[] = {SHORT_S, CHAR_S, DOUBLE_S,
                                      SHORT_S, INT_S, DOUBLE_S};
    size_t elementCount = 6;

    //alloc object

    Object* obj = obj_Alloc(elementsSizeArray, elementsTypeSizeArray, elementCount);

    /*  Static allocation: 

    Object obj = (Object){{0}, {0}, 0, 0, NULL};

    size_t* auxS[elementCount] = {0};
    size_t* auxT[elementCount] = {0};
    obj->size_vector = auxS;
    obj->type_size_vector = auxT;

    obj_MapMembers(&obj, elementsSizeArray, elementsTypeSizeArray, elementCount);
    */

    printf("Number of elements in the object: %u\n", obj->element_count);
    printf("Size in bytes of the object: %u\n", obj->byte_count);
    printf("Size of the elements in the object: ");

    for(int i = 0; i < elementCount; i++){
        printf("%u", obj->size_vector[i]);

        if(i < elementCount - 1){
            printf(", ");
        }else{
            printf("\n");
        }
    }

    printf("Size of the types used in the object: ");

    for(int i = 0; i < elementCount; i++){
        printf("%u", obj->type_size_vector[i]);

        if(i < elementCount - 1){
            printf(", ");
        }else{
            printf("\n");
        }
    }

    printf("First element:\n");

    short sharr[] = { 13, -13, 0, 0, 4, 90, 350, -234, 7, 23, 19, 1};

    obj_SetAtIndex(obj, sharr, 0);

    for(int i = 0; i < 12; i++){
        printf("%i", *(short*)(obj->data + 2*i));

        if(i < 11){
            printf(", ");
        }else{
            printf("\n");
        }
    }

    char* str = "duck";

    obj_SetAtIndex(obj, str, 1); // writes to object "data" buffer at index 1

    char* rst = (char*) obj_GetFromIndex(obj, 1);

    printf("Second element:\n");
    printf("%s\n", rst);

    if(obj->is_dynamic){
        printf("Object is dynamic.\n");

        obj_Free(obj);
    }

    return 0;
}