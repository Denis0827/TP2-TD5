#include "VecinoMasCercanoSolver.h"
#include <algorithm>
#include <unordered_map>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<vector<int>> ordenarPorDistancias(const vector<vector<double>>& distancias){
    int n = distancias.size();
    vector<vector<int>> nodos_ordenados(n, vector<int>(n, 0)); // inicializa todo en 0

    for (int i = 1; i < n; i++) {
        vector<std::pair<double, int>> distanciasDesdeI;
        for (int j = 1; j < n; j++) {
            if (i != j) {
                distanciasDesdeI.push_back({distancias[i][j], j});
            }
        }
        
        // ordenar la fila por distancia creciente
        sort(distanciasDesdeI.begin(), distanciasDesdeI.end()); // primero ordena por distancia. Si empata, ordena por numero de cliente

        int id = 1;
        // guardar solo los índices de los clientes
        for (auto it = distanciasDesdeI.begin(); it != distanciasDesdeI.end(); ++it) {
            nodos_ordenados[i][id] = it->second;
            id++;
        }
    }
    return nodos_ordenados;
}

vector<Route> vecinoMasCercano(const VRPLIBReader& instancia) {
    const vector<vector<double>>& distancias = instancia.getDistanceMatrix();
    const vector<int>& demandas = instancia.getDemands();
    int depot = instancia.getDepotId(); 
    int capacidad = instancia.getCapacity();
    int n = instancia.getDimension();

    // solucion con las rutas
    vector<Route> rutas;

    // construimos una matriz de clientes ordenados por distancia, para cada cliente 
    vector<vector<int>> clientesOrdenadosPorDistancia = ordenarPorDistancias(distancias);

    // para saber cuando todos los clientes tienen una ruta
    int clientesNoVisitados = n - 1; // sin contar el deposito
    vector<int> visitados (n + 1, 0);
    visitados[depot] = 1; // no queremos considerar el depot, solo clientes

    // empiezo del depot, veo el nodo con min distancia y agregarlo a la ruta
    while (clientesNoVisitados > 0) {
        Route rutaActual;
        rutaActual.nodes.push_back(depot);
        rutaActual.totalDemand = 0;
        int actual = depot;
        int capacidadRestante = capacidad;

        // me dice si encuentra clientes que se pueden agregar a la ruta
        bool puedeAgregar = true;

        while (puedeAgregar != false) {
            bool puedeAgregar = false; // si se encuentra un cliente se puede seguir buscando mas, sino la ruta esta hecha
            for (int i = 1; i < n; ++i) {  // empiezo desde 1 (salteando columna 0 que no usamos)
                int candidato = clientesOrdenadosPorDistancia[actual][i];
                if (!visitados[candidato] && demandas[candidato] <= capacidadRestante) { // si es posible agrego el cliente a la ruta
                    rutaActual.nodes.push_back(candidato);
                    rutaActual.totalDemand += demandas[candidato];
                    capacidadRestante -= demandas[candidato];
                    visitados[candidato] = 1;
                    actual = candidato;
                    clientesNoVisitados--;
                    puedeAgregar = true;
                    break;  // solo agregamos un cliente por iteración
                }
            }
        }
        rutaActual.nodes.push_back(depot); // cerrar ruta
        rutas.push_back(rutaActual); // agrego ruta a la solucion
    }
    return rutas;
}