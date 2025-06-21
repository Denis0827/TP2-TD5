#ifndef ROUTE_H
#define ROUTE_H
#include <vector>

#include "NodoCliente.h"


struct Route {
    NodoCliente* raiz;
    NodoCliente* ultimo;
    int demandaTotal;
    int capacidadRestante;
    double distanciaTotal;
};

#endif // ROUTE_H