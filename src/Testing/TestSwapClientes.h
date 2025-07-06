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
        void verificarIntegridadRuta(const Route& ruta, int rutaNum);

    public:
        TestSwapClientes();

        // Tests principales
        void testSwapConsecutivosMismaRuta();       // CASO 1: Nodos consecutivos en la misma ruta
        void testSwapDistintasRutas();              // CASO 2: Nodos de distintas rutas
        void testSwapNoConsecutivosMismaRuta();     // CASO 3: Nodos de la misma ruta pero no consecutivos
        
        // Tests de casos bordes
        void testSwapRutaUnSoloCliente();           // CASO 4: Ruta con un solo cliente

        void testSwapExtremosMismaRuta();           // CASO 5: Swap entre los extremos de una misma ruta
        void testSwapExtremosEntreRutas();          // CASO 6: Swap entre los extremos de dos rutas distintas
        
        void testSwapInversoMismaRuta();            // CASO 7: Swap en el orden invertido dentro de una misma ruta
        void testSwapInversoEntreRutas();           // CASO 8: Swap en el orden invertido dentro de dos rutas distintas

        void ejecutarTodosLosTests();
    };

#endif // TESTSWAPCLIENTES_H