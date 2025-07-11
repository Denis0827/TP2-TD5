#include "Heuristicas.h"

vector<int> Heuristicas::ordenarPorDistancias(const vector<double>& distancias) {
    int n = distancias.size(); // O(1)
    vector<pair<double, int>> dist_con_id; // O(1) también guardo los ids para después invocarlo
    vector<int> clientes_ordenados(n + 1, 0); // O(N)

    // Inicializamos dist_con_id con su respectivo id
    for (int i = 1; i < static_cast<int>(n); ++i) { // O(N)
        dist_con_id.push_back({distancias[i], i}); // O(1)
    }

    sort(dist_con_id.begin(), dist_con_id.end()); // O(N*logN) primero ordena por distancia. Si empata, ordena por numero de cliente

    // guardar solo los índices de los clientes
    int id = 1; // O(1)
    for (auto it = dist_con_id.begin(); it != dist_con_id.end(); ++it) { // O(N)
        clientes_ordenados[id] = it->second; // O(1) me fijo en qué posición está del ordenamiento
        id++; // O(1)
    }

    return clientes_ordenados; // O(1)
}
// Complejidad total: O(N*logN)

int Heuristicas::clienteMinimoDistancia(const vector<double>& distanciasCliente, int id, const vector<int>& visitados, const vector<int>& demandas, int capacidadRestante) {
    int minCliente = -1; // O(1)
    double minDist = numeric_limits<double>::max(); // O(1) empiezo como inf

    for (int i = 1; i < static_cast<int>(distanciasCliente.size()); i++) { // O(N)
        if (!visitados[i] && demandas[i] <= capacidadRestante) { // O(1) verificamos si es posible agregar el cliente a la ruta
            if (i != id) { // O(1)
                if (distanciasCliente[i] < minDist) { // O(1) actualizamos el minimo
                    minDist = distanciasCliente[i]; // O(1)
                    minCliente = i; // O(1)
                }
            }
        }
    }

    return minCliente; // O(1)
}
// Complejidad total: O(N)

Solution Heuristicas::nearestNeighbor(bool exportar) {
    const vector<vector<double>>& distancias = this->_instancia.getDistanceMatrix(); // O(1)
    const vector<int>& demandas = this->_instancia.getDemands(); // O(1)
    int depot = this->_instancia.getDepotId(); // O(1)
    int capacidad = this->_instancia.getCapacity(); // O(1)
    int n = this->_instancia.getDimension(); // O(1)
    int k = this->_instancia.getNumVehicles(); // O(1)

    vector<int> clientes_depot_ordenados = ordenarPorDistancias(distancias[depot]); // O(N*logN)

    // para saber cuándo todos los clientes tienen una ruta
    int clientes_no_visitados = n - 1; // O(1) sin contar el deposito
    vector<int> visitados (n + 1, 0); // O(N) cada i del vector es el cliente i
    visitados[depot] = 1; // O(1) no queremos considerar el depot, solo clientes

    int numero_iteracion = 1;
    Solution solucion = Solution(k, "NearestNeighbor", this->_nombreInstancia);

    if (exportar) {
        solucion.exportarSolutionParcial(this->_instancia.getNodes(), numero_iteracion++);
    }

    // empiezo del depot, veo el nodo con min distancia y agregarlo a la ruta
    while (clientes_no_visitados > 0) { 
        // busco en el vector de distancias del depot cuál es el primero que no fue visitado todavía
        int primer_cliente_sin_ruta = 0; // O(1)
        
        for (int i = 1; i <= static_cast<int>(n); i++) { // O(N)
            if (visitados[clientes_depot_ordenados[i]] == 0) { // O(1)
                primer_cliente_sin_ruta = clientes_depot_ordenados[i]; // O(1)
                visitados[clientes_depot_ordenados[i]] = 1; // O(1)
                clientes_no_visitados--; // O(1)
                break; // si encontré el cliente salgo del ciclo, me quedo con el primero
            }
        }
        
        Route* rutaActual = new Route(capacidad, depot); // O(1) creamos la ruta nueva
        rutaActual->agregarClienteInicio(primer_cliente_sin_ruta, demandas[primer_cliente_sin_ruta],
            distancias[depot][primer_cliente_sin_ruta]); // O(1) agregamos el cliente encontrado

        int actual = primer_cliente_sin_ruta; // O(1)
        bool puedeAgregar = true; // O(1) me dice si encuentra clientes que se pueden agregar a la ruta

        while (puedeAgregar != false) { // O(1)
            int candidato = clienteMinimoDistancia(distancias[actual], actual, visitados, demandas, 
                rutaActual->getCapacidadRestante()); // O(N) 
            
            if (candidato != -1) { // O(1) solo entro si existe candidato, es decir si existe minimo que cumple la factibilidad
                // agregamos el candidato a la ruta
                rutaActual->agregarClienteFinal(candidato, demandas[candidato], distancias[actual][depot], distancias[actual][candidato], 
                    distancias[candidato][depot]); // O(1)

                visitados[candidato] = 1; // O(1) ya visité entonces el candidato
                actual = candidato; // O(1) ahora busco el minimo cliente desde el candidato (candidato del candidato)
                clientes_no_visitados--; // O(1)
                
                if (exportar) {
                    Solution parcial = solucion;
                    parcial.agregarRuta(rutaActual);
                    parcial.exportarSolutionParcial(this->_instancia.getNodes(), numero_iteracion++);
                }

            } else puedeAgregar = false; // O(1) si no encontré ningun candidato, la ruta esta hecha
            
        }
        solucion.agregarRuta(rutaActual); // O(1) agrego ruta a la solucion

        if (exportar) {
            solucion.exportarSolutionParcial(this->_instancia.getNodes(), numero_iteracion++);
        }
    }
    return solucion;
    // Complejidad total del ciclo: O(N^2)
    // ¿Por qué? Cada cliente puede estar únicamente en una ruta, entonces visitaremos solo única vez a cada cliente
    // Para cada cliente, puede estar o al principio de la ruta (primer_cliente_sin_ruta) o en el medio (candidato)
    // Cada búsqueda respectiva se realiza en O(N), por lo que con N clientes tendríamos O(N^2)
}
// Complejidad total NearestNeighbor: O(N^2)
// Complejidad total NearestNeighbor: O(N^2)