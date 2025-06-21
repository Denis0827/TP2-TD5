#ifndef ROUTE_H
#define ROUTE_H

#include <vector>
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
        void agregarClienteInicio(int id, int demanda, double distancia); // O(1)
        void agregarClienteFinal(int id, int demanda, double dist_ij, double dist_depj, double dist_depi); // O(1)
        void unirRutas(Route& otraRuta, double dist_ij, double dist_depi, double dis_depj); // O(1)
        void imprimirRuta() const; // O(N)
};

#endif // ROUTE_H