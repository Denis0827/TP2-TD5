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
        Route();
        Route(int capacidad);

        int getClientePadreId() const;
        int getClienteFinalId() const;
        int getDemandaTotal() const;
        int getCapacidadRestante() const;
        double getDistanciaTotal() const;

        NodoCliente* getRaiz();
        NodoCliente* getUltimo();
        NodoCliente* getRaizSinMod() const;
        NodoCliente* getUltimoSinMod() const;

        void agregarDepot(int depot);
        void agregarClienteInicio(int id, int demanda, double distancia);
        void agregarClienteFinal(int id, int demanda, double dist_ij, double dist_depj, double dist_depi);
        void unirRutas(Route& otraRuta, double dist_ij, double dist_depi, double dis_depj);
        void imprimirRuta() const;
};

#endif // ROUTE_H