#ifndef HEURISTICAS_H
#define HEURISTICAS_H

#include "../Modelado_Clase/VRPLIBReader.h"
#include "../Modelado_Clase/Saving.h"
#include "../Modelado_Clase/Route.h"
#include "../Modelado_Clase/Solution.h"

#include <string>
#include <algorithm>
#include <unordered_map>

class Heuristicas {
    private: 
        VRPLIBReader _instancia;

    public:
        // inicializa _instancia usando el constructor adecuado
        Heuristicas(const string& instanciaPath): _instancia(instanciaPath) {}
        VRPLIBReader getInstancia() { return this->_instancia; } // O(1)

        Solution clarkeWright(); // O(N^2*logN)
        Solution nearestNeighbor(); // O(N^2)
        void relocate(Solution& solucion);
        void swap(Solution& solucion, int criterio); 
        // criterio: 0 -> First Improvement, 1 -> Best Improvement
};

#endif // HEURISTICAS_H