#ifndef TESTSWAPCLIENTES_H
#define TESTSWAPCLIENTES_H

#include "../Modelado_Clase/Route.h"
#include "../Modelado_Clase/Solution.h"

#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
using namespace std;

class TestSwapClientes {
    private:
        vector<vector<double>> distancias;  // Matriz de distancias para testing
        
        Route crearRuta(const vector<int>& ids, const vector<int>& demandas, int capacidad, const vector<vector<double>>& dist);
        NodeRoute* buscarNodo(Route& r, int id);

        // Funciones auxiliares para cálculos y verificaciones
        void inicializarDistancias();
        bool verificarIntegridadRuta(const Route& ruta, int rutaNum);

    public:
        TestSwapClientes();

        // Tests principales
        bool testSwapConsecutivosMismaRuta();       // CASO 1: Nodos consecutivos en la misma ruta
        bool testSwapDistintasRutas();              // CASO 2: Nodos de distintas rutas
        bool testSwapNoConsecutivosMismaRuta();     // CASO 3: Nodos de la misma ruta pero no consecutivos
        
        // Tests de casos bordes
        bool testSwapRutaUnSoloCliente();           // CASO 4: Ruta con un solo cliente

        bool testSwapExtremosMismaRuta();           // CASO 5: Swap entre los extremos de una misma ruta
        bool testSwapExtremosEntreRutas();          // CASO 6: Swap entre los extremos de dos rutas distintas
        
        bool testSwapInversoMismaRuta();            // CASO 7: Swap en el orden invertido dentro de una misma ruta
        bool testSwapInversoEntreRutas();           // CASO 8: Swap en el orden invertido dentro de dos rutas distintas

        void ejecutarTodosLosTests();
    };

#endif // TESTSWAPCLIENTES_H