#ifndef ROUTE_H
#define ROUTE_H
#include <vector>

#include "NodoCliente.h"

struct Route {
    NodoCliente* raiz;
    NodoCliente* ultimo;
    int totalDemand;
    int capacidad;
};

#endif // ROUTE_H