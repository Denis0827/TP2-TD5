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
        void exportarSolutionParcial(int numero_iteracion);
};

/*

    // Si k = 0, significa que no hay restricción de vehículos
    if (k != 0 && static_cast<int>(solucion.size()) > k) {
        return solucion; // si la cantidad de vehículos en la solución sobrepasa el límite de vehículos, no existe sol óptima
    } else {
        return solucion;
    }
*/

#endif // SOLUTION_H