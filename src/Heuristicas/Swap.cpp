#include "Swap.h"
#include <algorithm>
#include <unordered_map>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool swap_clientes(vector<Route>& rutas, const vector<vector<double>>& distancias, const vector<int>& demandas, int capacidad) {
    int r = rutas.size();

    for (int i = 0; i < r; ++i) {
        for (int j = i; j < r; ++j) {  // permitimos i == j para intra-ruta
            Route& rutaA = rutas[i];
            Route& rutaB = rutas[j];

            for (size_t posA = 1; posA < rutaA.nodes.size() - 1; ++posA) {
                size_t inicioB;
                if (i == j) {
                    inicioB = posA + 1; // para intra-ruta
                } else {
                    inicioB = 1; // para inter-ruta
                }

                for (size_t posB = inicioB; posB < rutaB.nodes.size() - 1; ++posB) {
                    int clienteA = rutaA.nodes[posA];
                    int clienteB = rutaB.nodes[posB];

                    bool swapValido = true;

                    // En inter-ruta, controlar capacidad
                    if (i != j) {
                        int demandaA = demandas[clienteA];
                        int demandaB = demandas[clienteB];

                        int nuevaDemandaA = rutaA.totalDemand - demandaA + demandaB;
                        int nuevaDemandaB = rutaB.totalDemand - demandaB + demandaA;

                        if (nuevaDemandaA > capacidad || nuevaDemandaB > capacidad) {
                            swapValido = false;
                        }
                    }

                    if (swapValido) {
                        // Calcular costo antes y después
                        double costoAntes = 0.0;
                        double costoDespues = 0.0;

                        if (posA > 0 && posA + 1 < rutaA.nodes.size()) { // para no incluir al deposito
                            costoAntes += distancias[rutaA.nodes[posA - 1]][clienteA] +
                                          distancias[clienteA][rutaA.nodes[posA + 1]];
                            costoDespues += distancias[rutaA.nodes[posA - 1]][clienteB] +
                                            distancias[clienteB][rutaA.nodes[posA + 1]];
                        }

                        if (posB > 0 && posB + 1 < rutaB.nodes.size()) {
                            costoAntes += distancias[rutaB.nodes[posB - 1]][clienteB] +
                                          distancias[clienteB][rutaB.nodes[posB + 1]];
                            costoDespues += distancias[rutaB.nodes[posB - 1]][clienteA] +
                                            distancias[clienteA][rutaB.nodes[posB + 1]];
                        }

                        if (costoDespues < costoAntes) {
                            swap(rutaA.nodes[posA], rutaB.nodes[posB]);

                            if (i != j) {
                                int demandaA = demandas[clienteA];
                                int demandaB = demandas[clienteB];
                                rutaA.totalDemand = rutaA.totalDemand - demandaA + demandaB;
                                rutaB.totalDemand = rutaB.totalDemand - demandaB + demandaA;
                            }

                            return true;  // mejora encontrada
                        }
                    }
                }
            }
        }
    }

    return false;
}
