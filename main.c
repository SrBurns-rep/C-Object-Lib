#include <stdio.h>
#include "object.h"

int main(int argc, char const *argv[])
{

    size_t size[] = {45, 3};
    size_t type[] = {CHAR_S, INT_S};

    char* str = "Hello, I'm an object!";
    int nums[] = {-15, 320, 40};

    Object* testobj = obj_Alloc(size, type, 2);

    obj_Set(testobj, str, 0);
    obj_Set(testobj, nums, 1);
    char* id = testobj->id;

    printf("ID:");
    for(int i = 0; i < 3*sizeof(size_t); i += sizeof(size_t)){
        printf(" %X", *(size_t*)(id + i));
    }
    printf("\n");

    printf("Size in bytes: %lu\n", testobj->byte_count);
    printf("String: %s\n", (char*)testobj->table[0].data);
    printf("Int: %i\n", *(int*)(testobj->table[1].data + 2*sizeof(int)));

    char* str2 = "I like to collect elements OwO";
    int nums2[] = {300, -300, 150};
    int buffer[3] = {0};

    obj_Set(testobj, str2, 0);
    obj_Set(testobj, nums2, 1);

    printf("String: %s\n", (char*)testobj->table[0].data);

    obj_Get(testobj, buffer, 1);

    printf("Int: %i\n", buffer[2]);

    int nums3[3] = {500, -45, 200};

    obj_ObjAppend(testobj, nums3, 3, INT_S);

    printf("Object has a new ID:");
    for(int i = 0; i < 3*sizeof(size_t); i += sizeof(size_t)){
        printf(" %X", *(size_t*)(id + i));
    }
    printf("\n");

    printf("Int: %i\n", *(int*)(testobj->table[2].data + 2*sizeof(int)));
    printf("String: %s\n", (char*)testobj->table[0].data);

    return 0;
}