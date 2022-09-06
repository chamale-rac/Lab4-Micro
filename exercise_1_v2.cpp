/*
*------------------------------------------
* exercise_1_v2.cpp
* -----------------------------------------
* UNIVERSIDAD DEL VALLE DE GUATEMALA
* FACULTAD DE INGENIERÍA
* DEPARTAMENTO DE CIENCIA DE LA COMPUTACION
* CC3086 - Programacion de Microprocesadores
*
* by Samuel Chamalé
* jueves, 5 de septiembre del 2022
*------------------------------------------
* Description: In this version I aboard the first problem of the worksheet using a library that alows me to work with .json files. 
* Again, I just want to remark that using structs is not the bestway to do achieve the resolution, however I was decided to explore 
* the use of this structures.
*------------------------------------------
*/

#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>
#include <pthread.h>
#include <string>

using namespace std;
using json = nlohmann::json;

#define monthsNumber 2
#define productsNumber 8

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
    std::ifstream retrieved("data_1_remastered.json");
    json data = json::parse(retrieved);
    pthread_t threads[monthsNumber*productsNumber];
    months = new monthData[monthsNumber];
    
    for(int i = 0; i < monthsNumber; i++) {
        months[i] = {.name = data["months"][i]["name"], .products = new productData[productsNumber], .costs = data["months"][i]["costs"]};
        for(int j = 0; j < productsNumber; j++) { 
            months[i].products[j] = {.name = data["months"][i]["products"][j]["name"], .price = data["months"][i]["products"][j]["price"], .fixedCost = data["months"][i]["products"][j]["fixedCost"], .soldUnits = data["months"][i]["products"][j]["soldUnits"]};
            pthread_create(&threads[(i*productsNumber)+(j)], nullptr, operationsProduct, &months[i].products[j]);
        }
    }
    
    for(int i = 0; i < monthsNumber; i++) {
        std::cout << "Reporte del mes: " << months[i].name << "\n" << "Ventas por producto: " << std::endl;
        for(int j = 0; j < productsNumber; j++) { 
            pthread_join(threads[(i*productsNumber)+(j)], nullptr);            
            std::cout << "\t" + months[i].products[j].name + ": Q" << months[i].products[j].sold << std::endl;
            months[i].sold += months[i].products[j].sold;

        }
        months[i].utilities = months[i].sold - months[i].costs;
        std::cout << "Total ventas: Q" << months[i].sold << std::endl; 
        std::cout << "Costos variables: Q" << months[i].costs << std::endl; 
        std::cout << "Utilidad del mes: Q" << months[i].utilities << "\n" << std::endl;
    }
    return 0;
}

// REFERENCES
// https://es.stackoverflow.com/questions/46909/cual-es-la-diferencia-entre-int-e-int
// https://www.geeksforgeeks.org/array-of-strings-in-cpp-5-different-ways-to-create/
// https://www.w3schools.com/cpp/cpp_structs.asp#:~:text=Structures%20(also%20called%20structs)%20are,%2C%20bool%2C%20etc.).
// https://cplusplus.com/doc/tutorial/arrays/
// https://linux-packages.com/ubuntu-focal-fossa/package/nlohmann-json3-dev
// https://github.com/nlohmann/json#examples
