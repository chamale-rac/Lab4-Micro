
/*
*------------------------------------------
* exercise_1.cpp
* -----------------------------------------
* UNIVERSIDAD DEL VALLE DE GUATEMALA
* FACULTAD DE INGENIERÍA
* DEPARTAMENTO DE CIENCIA DE LA COMPUTACION
* CC3086 - Programacion de Microprocesadores
*
* by Samuel Chamalé
* jueves, 1 de septiembre del 2022
*------------------------------------------
* Description: ... 
*------------------------------------------
*/

#include <iostream>
#include <pthread.h>
#include <bits/stdc++.h>
#include <string>
using namespace std;
#define monthsNumber 2;
#define productsNumber 8;

std::string productNames[productsNumber] = {"Porcion Pastel Chocolate","White Mocha","Cafe americano","Latte","Toffee Coffee","Cappucino","S'mores Latte","Cafe tostado molido"};
int unitPrices[productsNumber] = {60, 32, 22, 24, 28, 24, 32, 60};
float staticCosts[productsNumber] = {20, 19.2, 13.2, 17.2, 20.1, 17.2, 23, 20};
int unitsSold[monthsNumber][productsNumber] = {{300, 400, 1590, 200, 390, 1455, 800, 60}, {250, 380, 800, 250, 600, 1200, 1540, 15}};
int index;

void dataFiller();
void operationsMonth();

struct productData {
    //Know
    string name;
    float price;
    float fixedCost;    
    float soldUnits;
    //Unknow
    float sold;
    float utility;
}

struct monthData {
    //Know
    string name;
    productData *products;
    //Unknow
    float sold;
    float costs;
    float utilities;
}
monthData *months;

struct threadData {
    int month;
    int product;
} 
threadData thrData;

void *operationsProduct(void *arg) {
    intervalData *ptr = (thrData*) arg;
    productData *product = &months[ptr -> month].products[ptr -> product];

    product.sold = product.price * product.soldUnits;  
    product.utility = product.sold - (product.soldUnits * product.fixedCost); 
    return nullptr;
}

int main(int argc, char *argv[]) {
    dataFiller();
  
    pthread_t threads[monthsNumber*productsNumber];

    index = 0;
    for(int i = 0; i < monthsNumber; i++){
        for(int j = 0; j < productsNumber; j++){ 
            thrData.month = i;
            thrData.product = j;
            pthread_create(&threads[index], nullptr, operationsProduct, &thrData);
            index++;
        }
    }

    index = 0;
    for(int i = 0; i < monthsNumber; i++){
        for(int j = 0; j < productsNumber; j++){ 
            pthread_join(threads[index], nullptr);
            index++;
        }
    }
    
    operationsMonth();
    //std::cout << "Total amount: " + std::to_string(totalSum) << std::endl;  
    return 0;
}

void dataFiller() {
    months = new monthData[monthsNumber];
    
    months[0].name = "June";
    months[0].products = new productData[productsNumber];
    months[0].costs = 45640;  

    months[1].name = "August";
    months[1].products = new productData[productsNumber];
    months[0].costs = 40590;

    for(int i = 0; i < monthsNumber; i++){
        for(int j = 0; j < productsNumber; j++){
            months[i].products[j].name = productNames[j];
            months[i].products[j].price = unitPrices[j];
            months[i].products[j].fixedCost = staticCosts[j];
            months[i].products[j].soldUnits  = unitsSold[i][j];
        }
    }        
}

void operationsMonth() {
    for(int i = 0; i < monthsNumber; i++){
        for(int j = 0; j < productsNumber; j++){
            months[i].sold += months[i].products[j].sold;
            months[i].utilities += months[i].products[j].utility;
        }
        months[i].utilities = months[i].utilities - months[i].costs;
    }  
}

// REFERENCES
// https://es.stackoverflow.com/questions/46909/cual-es-la-diferencia-entre-int-e-int
// https://www.geeksforgeeks.org/array-of-strings-in-cpp-5-different-ways-to-create/
// https://www.w3schools.com/cpp/cpp_structs.asp#:~:text=Structures%20(also%20called%20structs)%20are,%2C%20bool%2C%20etc.).
// https://cplusplus.com/doc/tutorial/arrays/
