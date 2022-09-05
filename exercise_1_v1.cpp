/*
*------------------------------------------
* exercise_1_v1.cpp
* -----------------------------------------
* UNIVERSIDAD DEL VALLE DE GUATEMALA
* FACULTAD DE INGENIERÍA
* DEPARTAMENTO DE CIENCIA DE LA COMPUTACION
* CC3086 - Programacion de Microprocesadores
*
* by Samuel Chamalé
* jueves, 1 de septiembre del 2022
*------------------------------------------
* Description: In this version I aboard the first problem of the worksheet using many arrays with the needed information. 
* I just want to remark that using structs is not the bestway to do achieve the resolution, however I was decided to explore 
* the use of this structures.
*------------------------------------------
*/

#include <iostream>
#include <pthread.h>
#include <string>

using namespace std;
#define monthsNumber 2
#define productsNumber 8

std::string monthNames[monthsNumber] = {"Junio", "Agosto"};
float monthCosts[monthsNumber] = {45640, 40590};
std::string productNames[productsNumber] = {"Porcion Pastel Chocolate","White Mocha","Cafe americano","Latte","Toffee Coffee","Cappucino","S'mores Latte","Cafe tostado molido"};
float unitPrices[productsNumber] = {60, 32, 22, 24, 28, 24, 32, 60};
float staticCosts[productsNumber] = {20, 19.2, 13.2, 17.2, 20.1, 17.2, 23, 20};
float unitsSold[monthsNumber][productsNumber] = {{300, 400, 1590, 200, 390, 1455, 800, 60}, {250, 380, 800, 250, 600, 1200, 1540, 15}};

struct productData {
    string name;
    float price;
    float fixedCost;    
    float soldUnits;
    float sold;
    float utility;
};

struct monthData {
    string name;
    productData *products;
    float sold;
    float costs;
    float utilities;
};

monthData *months;

void *operationsProduct(void *arg) {
    productData *p = (productData*) arg;
    *p = {.sold = (p->price * p->soldUnits), .utility = p->soldUnits * (p->price  - p->fixedCost)};
    return nullptr;
}

int main(int argc, char *argv[]) {
    pthread_t threads[monthsNumber*productsNumber];
    months = new monthData[monthsNumber];
    
    for(int i = 0; i < monthsNumber; i++) {
        months[i] = {.name = monthNames[i], .products = new productData[productsNumber], .costs = monthCosts[i]};
        for(int j = 0; j < productsNumber; j++) { 
            months[i].products[j] = {.name = productNames[j], .price = unitPrices[j], .fixedCost = staticCosts[j], .soldUnits = unitsSold[i][j]};
            pthread_create(&threads[(i*productsNumber)+(j)], nullptr, operationsProduct, &months[i].products[j]);
        }
    }
    
    for(int i = 0; i < monthsNumber; i++) {
        for(int j = 0; j < productsNumber; j++) { 
            pthread_join(threads[(i*productsNumber)+(j)], nullptr);
            months[i].sold += months[i].products[j].sold;
            months[i].utilities += months[i].products[j].utility;
        }
        months[i].utilities = months[i].utilities - months[i].costs;
        std::cout << months[i].sold << std::endl; 
        std::cout << months[i].costs << std::endl; 
        std::cout << months[i].utilities << std::endl;
    }
    return 0;
}

// REFERENCES
// https://es.stackoverflow.com/questions/46909/cual-es-la-diferencia-entre-int-e-int
// https://www.geeksforgeeks.org/array-of-strings-in-cpp-5-different-ways-to-create/
// https://www.w3schools.com/cpp/cpp_structs.asp#:~:text=Structures%20(also%20called%20structs)%20are,%2C%20bool%2C%20etc.).
// https://cplusplus.com/doc/tutorial/arrays/
