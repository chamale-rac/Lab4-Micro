/*
*------------------------------------------
* exercise_2.cpp
* -----------------------------------------
* UNIVERSIDAD DEL VALLE DE GUATEMALA
* FACULTAD DE INGENIERÍA
* DEPARTAMENTO DE CIENCIA DE LA COMPUTACION
* CC3086 - Programacion de Microprocesadores
*
* by Samuel Chamalé
* jueves, 5 de septiembre del 2022
*------------------------------------------
* Description: This basic program generates a simulation of the production of coffee.
*------------------------------------------
*/

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <iostream>
#include <pthread.h>
#include <string>

using namespace std;
#define toasterNumber 2
#define threadsNumber 3
#define neededPacks 

pthread_mutex_t mutex;

struct machine {
  int coffeeDimension;
};
machine silo, vault, produced;

void *toasterOperation(void *arg) {         
    long i = (long) arg;
    while(produced.coffeeDimension < neededPacks) {   
        pthread_mutex_lock(&mutex);
        silo.coffeeDimension++;
        pthread_mutex_unlock(&mutex); 
        pthread_mutex_lock(&mutex);        
        produced.coffeeDimension++;  
        pthread_mutex_unlock(&mutex);  
        std::cout << "The toaster " + std::to_string(i)  + " produced: 1 lb of toasted coffee;"<< std::endl;
        sleep(1);   
    }
    return NULL;
}

void *packerOperation(void *arg) {
    while(produced.coffeeDimension < neededPacks)  {
        if(silo.coffeeDimension >= 5) {
            while(vault.coffeeDimension < neededPacks) { 
                pthread_mutex_lock(&mutex);
                std::cout << "Coffee lbs in silo: " + std::to_string(silo.coffeeDimension) << std::endl;
                silo.coffeeDimension--;
                pthread_mutex_unlock(&mutex);                 
                vault.coffeeDimension++;      
                std::cout << "Coffee lbs in vault: " + std::to_string(vault.coffeeDimension) << std::endl;    
                std::cout << "The packer produced: 1 pack of 1 lb of toasted coffee;" << std::endl;
                sleep(1); 
            }
        }
    }    
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t threads[threadsNumber];
    pthread_mutex_init(&mutex, NULL);
    pthread_create(&threads[0], NULL, toasterOperation, (void*)(1));
    pthread_create(&threads[1], NULL, toasterOperation, (void*)(2));    
    pthread_create(&threads[2], NULL, packerOperation, NULL);
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);    
    pthread_join(threads[2], NULL);
    std::cout << "Coffee lbs in silo: " + std::to_string(silo.coffeeDimension) << std::endl;
    std::cout << "Coffee lbs in vault: " + std::to_string(vault.coffeeDimension) << std::endl;
    pthread_mutex_destroy(&mutex);    
    return 0;
}

// REFERENCES
// Examples in Canvas and my own exercise in the second laboratory.