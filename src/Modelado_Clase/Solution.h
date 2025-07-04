#ifndef SOLUTION_H
#define SOLUTION_H

#include "VRPLIBReader.h"
#include "Route.h"
#include <string>

#include <fstream>
#include <iomanip>
#include <sstream>

class Solution {
    private: 
        vector<tuple<int, Route*>> _rutas; // guarda punteros a rutas
        int _cantidad_rutas;
        int _cantidad_camiones;
        int _ultimo_id;
        string _algoritmo;

    public:
        Solution();
        Solution(int cantidad_camiones, string algoritmo);

        int getCantidadRutas() const;
        int getCantidadCamiones() const;
        string getAlgoritmo() const;
        vector<tuple<int, Route*>> getRutas() const;
        vector<tuple<NodeRoute*, Route*>> getAllClientesSol() const;

        bool esFactible() const;
        void setAlgoritmo(string algoritmo);

        void agregarRuta(Route* ruta);

        void imprimirSolution();
        void exportarSolutionParcial(const vector<Node>& nodos, int numero_iteracion);
};

#endif // SOLUTION_H