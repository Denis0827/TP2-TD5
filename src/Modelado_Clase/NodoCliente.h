#ifndef NODOCLIENTE_H
#define NODOCLIENTE_H
#include <vector>

struct NodoCliente {
    int id;
    NodoCliente* anterior;
    NodoCliente* siguiente;
};

#endif // NODOCLIENTE_H