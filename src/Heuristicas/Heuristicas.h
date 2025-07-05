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
        string _nombreInstancia;

        // Funciones auxiliares para GRASP
        vector<int> obtenerCandidatosRCL(const vector<double>& distanciasCliente, int id, 
                                    const vector<int>& visitados, const vector<int>& demandas, 
                                    int capacidadRestante, double alpha);
    
        int seleccionarPrimerClienteRCL(const vector<int>& clientes_depot_ordenados, 
                                   const vector<int>& visitados, int n, double alpha);
    
        Solution busquedaLocalRelocate(Solution& solucion, const vector<vector<double>>& distancias, 
                                  const vector<int>& demandas);

    public:
        // inicializa _instancia usando el constructor adecuado
        Heuristicas(const string& instanciaPath, const string& nombre)
            : _instancia(instanciaPath), _nombreInstancia(nombre) {} // O(1)
        VRPLIBReader getInstancia() { return this->_instancia; } // O(1)

        Solution clarkeWright(bool exportar = false); // O(N^2*logN)
        Solution nearestNeighbor(bool exportar = false); // O(N^2)
        void relocate(Solution& solucion, int criterio, bool exportar);
        void swap(Solution& solucion, int criterio, bool exportar = false); 
        // criterio: 0 -> First Improvement, 1 -> Best Improvement

        // Método GRASP para Nearest Neighbor
        // alpha: parámetro de randomización (0.0 = completamente greedy, 1.0 = completamente random)
        // maxIteraciones: número máximo de iteraciones GRASP
        Solution nearestNeighborGRASP(double alpha = 0.3, int maxIteraciones = 100);

};

#endif // HEURISTICAS_H