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
        void verificarIntegridadRuta(const Route& ruta, int rutaNum);

    public:
        TestRelocateCliente();

        // Tests principales
        void testRelocateEntreRutas();           // CASO 1: Relocate entre rutas diferentes
        void testRelocateMismaRuta();            // CASO 2: Relocate en la misma ruta
        void testRelocateConsecutivos();         // CASO 3: Relocate en posiciones consecutivas
        
        // Tests de casos bordes
        void testRelocateAlInicio();             // CASO 4: Relocate al inicio de la ruta
        void testRelocateAlFinal();              // CASO 5: Relocate al final de la ruta
        void testRelocateRutaUnCliente();        // CASO 6: Ruta con un solo cliente
        
        // Tests de casos extremos
        void testRelocateExtremosMismaRuta();    // CASO 7: Relocate desde extremo a extremo
        void testRelocateExtremosEntreRutas();   // CASO 8: Relocate extremos entre rutas
        void testRelocatePosicionesInversas();   // CASO 9: Relocate en posiciones inversas

        void ejecutarTodosLosTests();
    };

#endif // TESTRELOCATECLIENTE_H
