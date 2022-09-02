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
    productData *product = (productData*) arg;
    (*product).sold = (*product).price * (*product).soldUnits;  
    (*product).utility = (*product).sold - ((*product).soldUnits * (*product).fixedCost); 
    return nullptr;
}

int main(int argc, char *argv[]) {
    pthread_t threads[monthsNumber*productsNumber];
    months = new monthData[monthsNumber];
    
    for(int i = 0; i < monthsNumber; i++) {
        monthData *month = &months[i];
        (*month).products = new productData[productsNumber];
        (*month).name = monthNames[i];
        (*month).costs = monthCosts[i];
        for(int j = 0; j < productsNumber; j++) { 
            productData *product = &(*month).products[j];
            (*product).name = productNames[j];
            (*product).price = unitPrices[j];
            (*product).fixedCost = staticCosts[j];
            (*product).soldUnits  = unitsSold[i][j];
            pthread_create(&threads[(i*productsNumber)+(j)], nullptr, operationsProduct, product);
        }
    }
    
    for(int i = 0; i < monthsNumber; i++) {
        monthData *month = &months[i];
        for(int j = 0; j < productsNumber; j++) { 
            pthread_join(threads[(i*productsNumber)+(j)], nullptr);
            productData *product = &(*month).products[j];
            (*month).sold += (*product).sold;
            (*month).utilities += (*product).utility;
        }
        (*month).utilities = (*month).utilities - (*month).costs;
        std::cout << (*month).sold << std::endl; 
        std::cout << (*month).costs << std::endl; 
        std::cout << (*month).utilities << std::endl;
    }
    return 0;
}
