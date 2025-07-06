#ifndef HEURISTICAS_H
#define HEURISTICAS_H

#include "../Modelado_Clase/VRPLIBReader.h"
#include "../Modelado_Clase/Saving.h"
#include "../Modelado_Clase/Route.h"
#include "../Modelado_Clase/Solution.h"

#include <string>
#include <algorithm>
#include <unordered_map>
#include <random>
#include <ctime>

class Heuristicas {
    private: 
        VRPLIBReader _instancia;
        string _nombreInstancia;

        // Funciones auxiliares para ClarkeWright
        vector<Saving> calcularSavings(const vector<vector<double>>& distancias, int depotId);
        int findPadre(vector<int>& padres_ruta, int cliente);
        tuple<int, int> unionFind(vector<int>& padres_ruta, vector<int>& rango_cliente, int i, int j);
        bool chequeoSolapamiento(const Route& ruta_i, const Route& ruta_j, int i, int j);

        // Funciones auxiliares para NearestNeighbor
        vector<int> ordenarPorDistancias(const vector<double>& distancias);
        int clienteMinimoDistancia(const vector<double>& distanciasCliente, int id, 
            const vector<int>& visitados, const vector<int>& demandas, int capacidadRestante);

        // Funciones auxiliares para Swap
        double chequearMejoraSwap(Route& ruta_A, Route& ruta_B, NodeRoute* clienteA, NodeRoute* clienteB, 
            const vector<vector<double>>& distancias);

        // Funciones auxiliares para Relocate
        double chequearMejoraRelocate(Route& rutaA, Route& rutaB, NodeRoute* cliente, NodeRoute* destinoPrev, 
            const vector<int>& demandas, const vector<vector<double>>& distancias);

        // Funciones auxiliares para GRASP
        int clienteAleatorioRCL (const vector<double>& distanciasCliente, int id, const vector<int>& visitados, 
            const vector<int>& demandas, int capacidadRestante, int rcl_size, mt19937& gen);

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
        Solution nearestNeighborRandomized(bool exportar = false, int rcl_size = 3); // O(N^2)
        Solution GRASP(bool exportar = false);
};

#endif // HEURISTICAS_H