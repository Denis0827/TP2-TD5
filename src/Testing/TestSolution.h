#ifndef TESTSOLUTION_H
#define TESTSOLUTION_H

#include "../Modelado_Clase/Solution.h"
#include "../Modelado_Clase/Route.h"
#include <vector>
#include <string>
#include <iostream>
#include <cassert>
using namespace std;

class TestSolution {
    private:
        vector<vector<double>> distancias;
        void inicializarDistancias();
        Route* crearRuta(const vector<int>& ids, const vector<int>& demandas, int capacidad, const vector<vector<double>>& dist);
    
    public:
        TestSolution();
        // Tests principales
        bool testAgregarRutaYGetters();
        bool testDistanciaTotal();
        bool testFactibilidad();
        bool testGetAllClientesSol();
        bool testSetAlgoritmo();
        // Casos borde
        bool testSolucionVacia();
        bool testUnicaRutaUnCliente();
        bool testRutasConClientesRepetidos();
        bool testRutasSinClientes();
        bool testEliminarRutasVacias();
        void ejecutarTodosLosTests();
};

#endif // TESTSOLUTION_H
