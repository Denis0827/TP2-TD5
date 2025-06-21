#ifndef ROUTE_H
#define ROUTE_H

#include <iostream>
#include "NodoCliente.h"
using namespace std;

class Route {
    private:
        NodoCliente* _raiz;
        NodoCliente* _ultimo;
        int _demandaTotal;
        int _capacidad;
        double _distanciaTotal;

    public: 
        Route(); // O(1)
        Route(int capacidad); // O(1)

        int getClientePadreId() const; // O(1)
        int getClienteFinalId() const; // O(1)
        int getDemandaTotal() const; // O(1)
        int getCapacidadRestante() const; // O(1)
        double getDistanciaTotal() const; // O(1)

        NodoCliente* getRaiz(); // O(1)
        NodoCliente* getUltimo(); // O(1)
        const NodoCliente* getRaizSinMod() const; // O(1)
        const NodoCliente* getUltimoSinMod() const; // O(1)

        void agregarDepot(int depot); // O(1)
        // Por default, iniciaizamos dist_ij y dist_depj en 0.0 porque es el caso borde donde agregamos cliente
        // a una ruta donde no hay todavía ningún cliente (solo tenemos a los 2 depot)
        void agregarClienteInicio(int id, int demanda, double dist_depi, double dist_ij = 0.0, double dist_depj = 0.0); // O(1)
        void agregarClienteFinal(int id, int demanda, double dist_depi, double dist_ij = 0.0, double dist_depj = 0.0); // O(1)
        void unirRutas(Route& otraRuta, double dist_ij, double dist_depi, double dis_depj); // O(1)
        void imprimirRuta() const; // O(N)
};

#endif // ROUTE_H