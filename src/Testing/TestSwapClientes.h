#ifndef TESTSWAPCLIENTES_H
#define TESTSWAPCLIENTES_H

#include "../Modelado_Clase/Route.h"
#include "../Modelado_Clase/Solution.h"

#include <iostream>
#include <cassert>
#include <vector>
using namespace std;

class TestSwapClientes {
    private:
        vector<vector<double>> distancias;  // Matriz de distancias para testing
        
        // Funciones auxiliares para cálculos y verificaciones
        void inicializarDistancias();
        double calcularCostoAnterior(NodeRoute* cliente);
        double calcularCostoNuevo(NodeRoute* cliente, NodeRoute* nuevoAnterior, NodeRoute* nuevoSiguiente);
        void verificarIntegridadRuta(const Route& ruta, const string& nombre);
        void imprimirRutaBonita(const Route& ruta, const string& nombre);

    public:
        TestSwapClientes();
        
        // Tests principales
        void testSwapConsecutivosMismaRuta(); // CASO 1: Nodos consecutivos en la misma ruta
        void testSwapDistintasRutas(); // CASO 2: Nodos de distintas rutas
        void testSwapNoConsecutivosMismaRuta(); // CASO 3: Nodos de la misma ruta pero no consecutivos
        
        // Tests de casos bordes
        void testSwapExtremosRutas(); // CASO 4: Caso borde - Swap con nodos en extremos de rutas
        void testSwapRutaUnSoloCliente(); // CASO 5: Caso borde - Ruta con un solo cliente
        void testSwapCapacidadesDiferentes(); // CASO 6: Caso borde - Swap entre rutas con capacidades diferentes
        
        void ejecutarTodosLosTests();
    };

#endif // TESTSWAPCLIENTES_H