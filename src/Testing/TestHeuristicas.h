#ifndef TESTHEURISTICAS_H
#define TESTHEURISTICAS_H

#include "../Modelado_Clase/Solution.h"
#include "../Modelado_Clase/Route.h"
#include "../Modelado_Clase/VRPLIBReader.h"
#include "../Heuristicas/Heuristicas.h"

#include <iostream>
#include <cassert>
#include <set>
#include <algorithm>
#include <vector>
#include <string>
using namespace std;

class TestHeuristicas {
private:
    vector<vector<double>> distancias;
    vector<int> demandas;
    int capacidad;
    int depot;
    VRPLIBReader instancia;
    string nombreInstancia;

    void inicializarDatos(const string& path, const string& nombre);
    bool verificarIntegridadSolucion(const Solution& sol, const string& nombreAlgoritmo);
    bool verificarIntegridadRuta(const Route& ruta, int rutaNum);
    vector<int> obtenerClientesInstancia() const;

public:
    TestHeuristicas(const string& path, const string& nombre);
    bool testNearestNeighbor();
    bool testClarkeWright();
    bool testNearestNeighborRandomized();
    bool testGRASP();
    bool testCWFeasibility();
    bool testNNFeasibility();
    bool testGRASPFeasibility();
    void ejecutarTodosLosTests();
};

#endif // TESTHEURISTICAS_H
