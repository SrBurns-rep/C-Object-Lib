#include <stdio.h>
#include "object.h"

/**
 * Create a object defined using args
 * 
*/

#define ELEMENT_MAX_SIZE 0x010000UL // 65kB
#define NAME_BUFFER_SIZE 64
#define NAME_BUFFER_MAX 16
#define ARG_BUFFER_SIZE 64
#define COMMON_TYPES_MAX 7

const char* COMMON_TYPES[] = {"char", "short", "int", "long", "longlong", "float", "double"}; // no unsigned because I'm lazy
const size_t COMMON_TYPES_SIZES[] = {CHAR_S, SHORT_S, INT_S, LONG_S, LONGLONG_S, FLOAT_S, DOUBLE_S}; // must be synchronized

size_t typeSizeFromStr(char* str){
    size_t size = 0;

    if(str == NULL) return 0;

    for(int i = 0; i < COMMON_TYPES_MAX; i++){
        if(strcmp(str, COMMON_TYPES[i]) == 0){
            size = COMMON_TYPES_SIZES[i];
            break;
        }
    }

    return size;
}

/** argv[0]        argv[1]        argv[2]           ...
 *  type,name,size type,name,size type,name,size
*/
int main(int argc, char const *argv[]){

    if(!argc){
        printf("Exited with no arguments.\n");
        exit(EXIT_FAILURE);
    }

    if(argc > NAME_BUFFER_MAX){
        printf("Buffer overflow, too much arguments.\n");
        exit(EXIT_FAILURE);
    }

    size_t objSizesBuffer[argc];
    size_t objTypesBuffer[argc];
    char nameBuffer[NAME_BUFFER_MAX][NAME_BUFFER_SIZE] = {0}; // Constant buffer size
    char* argBuffer = calloc(ARG_BUFFER_SIZE, CHAR_S);

    for(int i = 0; i < argc - 1; i++){
        char* token;
        int len = strlen(argv[i + 1]);

        if(len > ARG_BUFFER_SIZE){

            printf("Buffer overflow.\n");
            exit(EXIT_FAILURE);
        }

        int aux = 0;

        strcpy(argBuffer, argv[i + 1]);

        token = strtok(argBuffer, ",");

        if(objTypesBuffer[i] = typeSizeFromStr(token)){         // type

            token = strtok(NULL, ",");
            
            if(token != NULL){                                  // name

                strcpy(nameBuffer[i], token);

                token = strtok(NULL, ",");

                if(token != NULL){                              // size

                    if(sscanf(token, "%i", &aux) == EOF){
                        printf("sscanf found an error.\n");
                    }

                    objSizesBuffer[i] = (size_t) aux;
                }else{

                    printf("No size specified.\n");
                    exit(EXIT_FAILURE);
                }

            }else{

                printf("No element name in args.\n");
                exit(EXIT_FAILURE);
            }

        }else{

            printf("Type syntax error: %s\n", token);
        }

    }

    if(argc > 1){ // Create Object

        for(int i = 0; i < argc - 1; i++){      // Check for values too high

            size_t elementByteSize = objSizesBuffer[i] * objTypesBuffer[i];

            if(objSizesBuffer[i] != 0 && elementByteSize / objSizesBuffer[i] != objTypesBuffer[i]){ 

                printf("Object Buffer overflow.\n");
                exit(EXIT_FAILURE);

            }else if(elementByteSize > ELEMENT_MAX_SIZE){

                printf("Object Buffer overflow.\n");
                exit(EXIT_FAILURE);
            }
        }

        //Allocate object

        Object* obj = obj_Alloc(objSizesBuffer, objTypesBuffer, argc - 1);

        if(obj != NULL){
            printf("Object allocated!\n");
        }

        unsigned char id[32] = {0};
        obj_GetID(obj, id);

            //TO-DO File I/O for object

        // printing object:
        printf("Object ID: ");

        for(int i = 0; i < 32; i++){
            unsigned char c = id[i];
            printf("%X ", c);
        }
        printf("\n");

        for(int i = 0; i < obj->element_count; i++){
            printf("-----------------------------------\n");
            printf("Element no %i name: %s\n", i + 1, nameBuffer[i]);
            printf("Size: %u\n", obj->table[i].size);
            printf("Type: %u\n", obj->table[i].type);

            //printf("Content:"); No FILE I/O for that

        }
        printf("-----------------------------------\n");

        obj_Free(obj);

        printf("Object destroyed\n");

    }else{

        printf("Not enough arguments.\n");
    }

    printf("Arg count: %i\nLast arg: %s\n", argc, argv[argc - 1]);

    return 0;
}