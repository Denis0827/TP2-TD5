#ifndef SOLUTION_H
#define SOLUTION_H

#include "VRPLIBReader.h"
#include "Route.h"

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
        string _instanciaCVRP;

    public:
        Solution();
        Solution(int cantidad_camiones, string algoritmo, string instancia);

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

        // Constructor de copia
        Solution(const Solution& other);
    
        // Operador de asignación
        Solution& operator=(const Solution& other);
    
        // Destructor
        ~Solution();
    
        // Método para obtener una ruta por índice
        Route* getRutaByIndex(int index);
    
        // Método para obtener el costo total de la solución
        double getCostoTotal() const;
};

#endif // SOLUTION_H