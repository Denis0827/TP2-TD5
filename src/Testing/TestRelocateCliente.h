#ifndef TESTRELOCATECLIENTE_H
#define TESTRELOCATECLIENTE_H

#include "../Modelado_Clase/Route.h"
#include "../Modelado_Clase/Solution.h"

#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
using namespace std;

class TestRelocateCliente {
    private:
        vector<vector<double>> distancias;  // Matriz de distancias para testing
        
        Route crearRuta(const vector<int>& ids, const vector<int>& demandas, int capacidad, const vector<vector<double>>& dist);
        NodeRoute* buscarNodo(Route& r, int id);

        // Funciones auxiliares para cálculos y verificaciones
        void inicializarDistancias();
        bool verificarIntegridadRuta(const Route& ruta, int rutaNum);

    public:
        TestRelocateCliente();

        // Tests principales
        bool testRelocateEntreRutas();           // CASO 1: Relocate entre rutas diferentes
        bool testRelocateMismaRuta();            // CASO 2: Relocate en la misma ruta
        bool testRelocateConsecutivos();         // CASO 3: Relocate en posiciones consecutivas
        
        // Tests de casos bordes
        bool testRelocateAlInicio();             // CASO 4: Relocate al inicio de la ruta
        bool testRelocateAlFinal();              // CASO 5: Relocate al final de la ruta
        bool testRelocateRutaUnCliente();        // CASO 6: Ruta con un solo cliente
        
        // Tests de casos extremos
        bool testRelocateExtremosMismaRuta();    // CASO 7: Relocate desde extremo a extremo
        bool testRelocateExtremosEntreRutas();   // CASO 8: Relocate extremos entre rutas
        bool testRelocatePosicionesInversas();   // CASO 9: Relocate en posiciones inversas

        void ejecutarTodosLosTests();
    };

#endif // TESTRELOCATECLIENTE_H
