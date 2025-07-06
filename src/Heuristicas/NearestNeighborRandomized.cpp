#include "Heuristicas.h"
#include <vector>
#include <random>
#include <algorithm>
#include <limits>
#include <iostream>
using namespace std;

// Definición de la función auxiliar ordenarPorDistancias
vector<int> ordenarPorDistancias(const vector<double>& distancias) {
    int n = distancias.size();
    vector<pair<double, int>> dist_con_id;
    vector<int> clientes_ordenados(n + 1, 0);

    for (int i = 1; i < n; ++i) {
        dist_con_id.push_back({distancias[i], i});
    }

    sort(dist_con_id.begin(), dist_con_id.end());

    int id = 1;
    for (auto& p : dist_con_id) {
        clientes_ordenados[id++] = p.second;
    }

    return clientes_ordenados;
}

// Definición de las funciones privadas de Heuristicas

vector<int> Heuristicas::obtenerCandidatosRCL(const vector<double>& distanciasCliente, int id, 
                                    const vector<int>& visitados, const vector<int>& demandas, 
                                    int capacidadRestante, double alpha) {
    vector<pair<double, int>> candidatos;

    for (int i = 1; i < (int)distanciasCliente.size(); i++) {
        if (!visitados[i] && demandas[i] <= capacidadRestante && i != id) {
            candidatos.push_back({distanciasCliente[i], i});
        }
    }

    if (candidatos.empty()) {
        return vector<int>();
    }

    sort(candidatos.begin(), candidatos.end());

    double minDist = candidatos[0].first;
    double maxDist = candidatos.back().first;
    double threshold = minDist + alpha * (maxDist - minDist);

    vector<int> rcl;
    for (const auto& candidato : candidatos) {
        if (candidato.first <= threshold) {
            rcl.push_back(candidato.second);
        }
    }

    return rcl;
}

int Heuristicas::seleccionarPrimerClienteRCL(const vector<int>& clientes_depot_ordenados, 
                                   const vector<int>& visitados, int n, double alpha) {
    vector<int> candidatos_disponibles;

    for (int i = 1; i < n; i++) {
        if (visitados[clientes_depot_ordenados[i]] == 0) {
            candidatos_disponibles.push_back(clientes_depot_ordenados[i]);
        }
    }

    if (candidatos_disponibles.empty()) {
        return -1;
    }

    int rcl_size = max(1, static_cast<int>(candidatos_disponibles.size() * alpha));

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, rcl_size - 1);

    return candidatos_disponibles[dis(gen)];
}

Solution Heuristicas::nearestNeighborGRASP(double alpha, int maxIteraciones) {
    const vector<vector<double>>& distancias = this->_instancia.getDistanceMatrix();
    const vector<int>& demandas = this->_instancia.getDemands();
    int depot = this->_instancia.getDepotId();
    int capacidad = this->_instancia.getCapacity();
    int n = this->_instancia.getDimension();
    int k = this->_instancia.getNumVehicles();

    Solution mejorSolucion(k, "GRASP-NearestNeighbor");
    double mejorCosto = numeric_limits<double>::max();

    for (int iter = 0; iter < maxIteraciones; iter++) {
        vector<int> clientes_depot_ordenados = ordenarPorDistancias(distancias[depot]);

        int clientes_no_visitados = n - 1;
        vector<int> visitados(n + 1, 0);
        visitados[depot] = 1;

        Solution solucionActual(k, "GRASP-NearestNeighbor");

        // Aquí solo creamos una ruta con un primer cliente randomizado para test
        int primer_cliente = seleccionarPrimerClienteRCL(clientes_depot_ordenados, visitados, n, alpha);

        if (primer_cliente == -1) break;

        visitados[primer_cliente] = 1;
        clientes_no_visitados--;

        Route* rutaActual = new Route(capacidad, depot);
        rutaActual->agregarClienteInicio(primer_cliente, demandas[primer_cliente], distancias[depot][primer_cliente]);

        solucionActual.agregarRuta(rutaActual);

        double costoActual = solucionActual.getCostoTotal();
        if (costoActual < mejorCosto) {
            mejorCosto = costoActual;
            mejorSolucion = solucionActual;
        }
    }

    return mejorSolucion;
}
