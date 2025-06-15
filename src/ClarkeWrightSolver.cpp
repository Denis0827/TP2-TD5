#include "ClarkeWrightSolver.h"
#include <algorithm>
#include <unordered_map>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool Saving::operator<(const Saving& other) const {
    return saving > other.saving; // orden descendente
}

// Función auxiliar para obtener un vector de Saving dada un vector de distancias inicial,
// ordenado de mayor a menor
vector<Saving> calcularSavings(const vector<vector<double>>& distancias, int depotId) {
    std::vector<Saving> savings;
    int n = distancias.size();

    for (int i = 0; i < n; ++i) {
        if (i != depotId) {
            for (int j = i + 1; j < n; ++j) {
                if (j != depotId) {
                    double sij = distancias[depotId][i] + distancias[depotId][j] - distancias[i][j]; // calcula el ahorro
                    savings.push_back({i, j, sij});
                }
            }
        }
    }

    sort(savings.begin(), savings.end()); // ordena de mayor a menor

    return savings;
}

vector<Route> clarkeWright(const VRPLIBReader& instancia) {
    const vector<vector<double>>& distancias = instancia.getDistanceMatrix();
    const vector<int>& demandas = instancia.getDemands();
    int depot = instancia.getDepotId(); 
    int capacidad = instancia.getCapacity();
    int n = instancia.getDimension();

    std::unordered_map<int, Route> rutaCliente;
    for (int i = 0; i <= n; i++) {
        if (i != depot) {
            rutaCliente[i] = Route{{depot, i, depot}, demandas[i]}; // seteamos las rutas iniciales
        }
    }

    vector<Saving> savings = calcularSavings(distancias, depot);

    for (int s = 0; s < savings.size(); s++) { // recorre los savings desde el mayor al menor
        int i = savings[s].i;
        int j = savings[j].j;

        if (rutaCliente.find(i) != rutaCliente.end() 
            && rutaCliente.find(j) != rutaCliente.end()) { 
            // si no se encuentran en el mapa, ya están mapeados en otra ruta distinta
            Route ruta_i = rutaCliente[i];
            Route ruta_j = rutaCliente[j];

            // chequeo de solapamiento
            if (ruta_i.nodes[ruta_i.nodes.size() - 2] == i // si el cliente i es el último a visitar en la ruta_i le puedo seguir agregando rutas
                && ruta_j.nodes[1] == 1) { // si j es el primer cliente que se visita en esta ruta se puede agregar la ruta_j a la ruta_i
                int demandaAgregada = ruta_i.totalDemand + ruta_j.totalDemand;
                if (demandaAgregada <= capacidad) { // si la nueva demanda no supera la capacidad agregamos ruta_j a ruta_i
                    ruta_i.totalDemand = demandaAgregada;
                    ruta_i.nodes.pop_back();
                    ruta_i.nodes.insert(ruta_i.nodes.end(), ruta_j.nodes.begin() + 1, ruta_j.nodes.end());
                    rutaCliente.erase(j); // eliminamos ruta_j del mapa cuando ya es parte de ruta_i
                }
            }
        }
    }
    
    vector<Route> solucion = {};
    for (auto it = rutaCliente.begin(); it != rutaCliente.end(); ++it) {
        solucion.push_back(it->second);
    }

    return solucion;
}