#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
    char *key;
    char *value;
} Storage;

int storage_init(Storage *storage) {
    if(storage == NULL){
         return -1;
    }
    storage->key = NULL;
    storage->value = NULL;
    return 0;

}
int storage_set(Storage *storage, const char *key, const char *value) {
    if(storage == NULL || key ==NULL || value == NULL){
         return -1;
    }
storage->key = malloc(strlen(key) + 1);
if (storage->key == NULL) {
    return -1;
}

storage->value = malloc(strlen(value) + 1);
if (storage->value == NULL) {
    free(storage->key);
    storage->key = NULL;
    return -1;
}
    strcpy(storage->key, key);
    strcpy(storage->value,value);
    return 0;
}
int storage_get(Storage *storage, const char *key, char **value) {
    if (storage == NULL || key == NULL || value == NULL) {
        return -1;
    }

    if (storage->key == NULL | storage->value == NULL) {
        return -1;
    }

    if (strcmp(storage->key, key) != 0) {
        return -1;
    }

    *value = storage->value;

    return 0;
}
int storage_free(Storage *storage){
     if(storage == NULL){
         return -1;
     }
     free(storage->key);
     free(storage->value);
      
     storage->key = NULL;
     storage->value= NULL;

     return 0;
}
void storage_print(Storage *storage){
     if(storage == NULL){
         return;
     }
     if(storage->key == NULL || storage->value == NULL){
         printf("Storage is empty.\n");
     } else {
         printf("Key: %s, Value: %s\n", storage->key, storage->value);
     }
}
int main(void) {
    Storage storage;
    if(storage_init(&storage) !=0){
         fprintf(stderr, "Failed to initialize storage.\n");
        
    }
    storage_print(&storage);
    if(storage_set(&storage, "name", "Touati") != 0){
         fprintf(stderr, "Failed to set storage.\n");
        
    }
    storage_print(&storage);
    return 0;
}
