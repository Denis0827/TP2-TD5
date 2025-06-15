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

    std::unordered_map<int, Route> rutaCliente;
    for (int i = 0; i <= n; i++) {
        if (i != depot) {
            rutaCliente[i] = Route{{depot, i, depot}, demandas[i]}; // seteamos las rutas iniciales
        }
    } // ESTA PARTE NO VA

    // construimos una matriz de clientes ordenados por distancia, para cada cliente 
    vector<vector<int>> clientesOrdenadosPorDistancia = ordenarPorDistancias(distancias);
    vector<int> clientesNoVisitados = {};
    for (int i = 1; i <= n; i++) {
        if (i != depot) {
            clientesNoVisitados.push_back(i); 
            // inicializo los clientes no visitados como todos los clientes menos el depot
        }
    } 

    while (clientesNoVisitados.size() > 0) {
        Route rutaActual;
        rutaActual.nodes.push_back(depot);
        rutaActual.totalDemand = 0;
        int actual = depot;
        int capacidadRestante = capacidad;

        while ()
    }
    
}
    
    // empiezo del depot, veo el nodo con min distancia y agregarlo a la ruta
    
    // ahora me paro en el ultimo que agregue. busco el nodo con min distancia a este nodo, 
    // si veo que al agregarlo no me paso de la capacidad, lo agrego. sino paso al prox min

    nodo_ordenados [i][0]
    vector<int> clientes_visitados;
    for (int r = 0; r <= n; r++) {
        
    }



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