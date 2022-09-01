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
#define monthsNumber 2
#define productsNumber 8

std::string productNames[productsNumber] = {"Porcion Pastel Chocolate","White Mocha","Cafe americano","Latte","Toffee Coffee","Cappucino","S'mores Latte","Cafe tostado molido"};
float unitPrices[productsNumber] = {60, 32, 22, 24, 28, 24, 32, 60};
float staticCosts[productsNumber] = {20, 19.2, 13.2, 17.2, 20.1, 17.2, 23, 20};
float unitsSold[monthsNumber][productsNumber] = {{300, 400, 1590, 200, 390, 1455, 800, 60}, {250, 380, 800, 250, 600, 1200, 1540, 15}};

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
};

struct monthData {
    //Know
    string name;
    productData *products;
    //Unknow
    float sold;
    float costs;
    float utilities;
};

struct threadData {
    int month;
    int product;
};

monthData *months;
threadData *thrData;

void *operationsProduct(void *arg) {
    threadData *ptr = (threadData*) arg;
    productData *product = &months[(*ptr).month].products[(*ptr).product];
    (*product).sold = (*product).price * (*product).soldUnits;  
    (*product).utility = (*product).sold - ((*product).soldUnits * (*product).fixedCost); 
    return nullptr;
}

int main(int argc, char *argv[]) {
    dataFiller();
    pthread_t threads[monthsNumber*productsNumber];
    thrData = new threadData[monthsNumber*productsNumber];
    
    int index = 0;
    for(int i = 0; i < monthsNumber; i++){
        for(int j = 0; j < productsNumber; j++){ 
            thrData[index].month = i;
            thrData[index].product = j;
            pthread_create(&threads[index], nullptr, operationsProduct, &thrData[index]);
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
    
    return 0;
}

void dataFiller() {
    months = new monthData[monthsNumber];
    
    months[0].name = "June";
    months[0].products = new productData[productsNumber];
    months[0].costs = 45640;  

    months[1].name = "August";
    months[1].products = new productData[productsNumber];
    months[1].costs = 40590;

    for(int i = 0; i < monthsNumber; i++){
        for(int j = 0; j < productsNumber; j++){
            productData *product = &months[i].products[j];
            (*product).name = productNames[j];
            (*product).price = unitPrices[j];
            (*product).fixedCost = staticCosts[j];
            (*product).soldUnits  = unitsSold[i][j];
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
        std::cout << months[i].sold << std::endl; 
        std::cout << months[i].costs << std::endl; 
        std::cout << months[i].utilities << std::endl;  
        std::cout << "\n" << std::endl; 
    }  
}
