#ifndef ROUTE_H
#define ROUTE_H

#include "NodeRoute.h"
#include <iostream>
#include <vector>
using namespace std;

class Route {
    private:
        NodeRoute* _raiz;
        NodeRoute* _ultimo;
        int _demandaTotal;
        int _capacidad;
        double _distanciaTotal;

    public: 
        Route(); // O(1)
        Route(int capacidad, int depot); // O(1)

        int getClientePadreId() const; // O(1)
        int getClienteFinalId() const; // O(1)
        int getDemandaTotal() const; // O(1)
        int getCapacidadTotal() const; // O(1)
        int getCapacidadRestante() const; // O(1)
        double getDistanciaTotal() const; // O(1)
        vector<NodeRoute*> getAllClientes() const; // O(N)

        const NodeRoute* getRaiz() const; // O(1)
        const NodeRoute* getUltimo() const; // O(1)
        NodeRoute* getRaizModify(); // O(1)
        NodeRoute* getUltimoModify(); // O(1)
        // OJO con los usos inapropiados de getModify porque afectan el encapsulamiento y pueden dañar la estructura
        // interna de Route si no se usa de manera adecuada

        // Por default, iniciaizamos dist_ij y dist_depj en 0.0 porque es el caso borde donde agregamos 
        // cliente a una ruta donde no hay todavía ningún cliente (solo tenemos a los 2 depot)
        void agregarClienteInicio(int id, int demanda, double dist_depi, double dist_ij = 0.0, double dist_depj = 0.0); // O(1)
        void agregarClienteFinal(int id, int demanda, double dist_depi, double dist_ij = 0.0, double dist_depj = 0.0); // O(1)
        // j es el nuevo cliente, i es el anterior primero/ultimo

        void unirRutas(Route& otraRuta, double dist_ij, double dist_depi, double dis_depj); // O(1)
        // chequear dist_ij
        void swapClientes(Route& otraRuta, NodeRoute* clienteA, NodeRoute* clienteB, int demandaA, int demandaB, 
            double costo_anterior_A, double costo_anterior_B, double costo_nuevo_A, double costo_nuevo_B); // O(1)
        // Pre: i debe ser cliente de la ruta actual, j debe ser cliente de otraRuta
        //void relocateCliente();

        void imprimirRuta() const; // O(N)

};

#endif // ROUTE_H