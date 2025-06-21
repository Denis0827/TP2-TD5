#include "Heuristicas.h"

vector<vector<int>> ordenarPorDistancias(const vector<vector<double>>& distancias){
    int n = distancias.size();
    vector<vector<int>> clientes_ordenados(n, vector<int>(n - 1, 0)); // inicializa todo en 0
    // cada fila i es el vector de mínimas distancias del cliente i

    for (int i = 1; i < n; i++) {
        vector<std::pair<double, int>> distanciasDesdeI;
        for (int j = 1; j < n; j++) {
            if (i != j) {
                distanciasDesdeI.push_back({distancias[i][j], j});
            }
        }
        
        // ordenar la fila por distancia creciente
        sort(distanciasDesdeI.begin(), distanciasDesdeI.end()); // primero ordena por distancia. Si empata, ordena por numero de cliente

        // guardar solo los índices de los clientes
        int id = 1;
        for (auto it = distanciasDesdeI.begin(); it != distanciasDesdeI.end(); ++it) {
            clientes_ordenados[i][id] = it->second;
            id++;
        }
    }
    return clientes_ordenados;
}

vector<Route> Heuristicas::vecinoMasCercano() {
    const vector<vector<double>>& distancias = this->_instancia.getDistanceMatrix();
    const vector<int>& demandas = this->_instancia.getDemands();
    int depot = this->_instancia.getDepotId(); 
    int capacidad = this->_instancia.getCapacity();
    int n = this->_instancia.getDimension();
    int k = this->_instancia.getNumVehicles();

    vector<Route> solucion;
    // construimos una matriz de clientes ordenados por distancia, para cada cliente 
    vector<vector<int>> matrizClientesOrdenados = ordenarPorDistancias(distancias);

    // para saber cuando todos los clientes tienen una ruta
    int clientesNoVisitados = n - 1; // sin contar el deposito
    vector<int> visitados (n + 1, 0); // cada i del vector es el cliente i
    visitados[depot] = 1; // no queremos considerar el depot, solo clientes

    // empiezo del depot, veo el nodo con min distancia y agregarlo a la ruta
    while (clientesNoVisitados > 0) {
        // busco en el vector de distancias del depot cuál es el primero que no fue visitado todavía
        int primer_cliente_sin_ruta = 0;
        for (int i = 1; i < static_cast<int>(n); i++) {
            if (visitados[matrizClientesOrdenados[depot][i]] == 0) {
                primer_cliente_sin_ruta = matrizClientesOrdenados[depot][i];
                visitados[matrizClientesOrdenados[depot][i]] = 1;
                clientesNoVisitados--;
                break;
            }
        }

        Route rutaActual = Route(capacidad);
        rutaActual.agregarDepot(depot);
        rutaActual.agregarClienteInicio(primer_cliente_sin_ruta, demandas[primer_cliente_sin_ruta], 
            distancias[depot][primer_cliente_sin_ruta]);

        int actual = primer_cliente_sin_ruta;
        bool puedeAgregar = true; // me dice si encuentra clientes que se pueden agregar a la ruta

        while (puedeAgregar != false) {
            puedeAgregar = false; // si se encuentra un cliente se puede seguir buscando mas, sino la ruta esta hecha
            for (int i = 1; i < static_cast<int>(n); i++) {
                int candidato = matrizClientesOrdenados[actual][i];
                // verificamos si es posible agregar el cliente a la ruta
                if (!visitados[candidato] && demandas[candidato] <= rutaActual.getCapacidadRestante()) { 
                    // agregamos el candidato a la ruta
                    rutaActual.agregarClienteFinal(candidato, demandas[candidato], distancias[actual][candidato], 
                        distancias[candidato][depot], distancias[actual][depot]);

                    visitados[candidato] = 1;
                    actual = candidato;
                    clientesNoVisitados--;
                    i = 1; // empezamos a recorrer el vector de distancias del nuevo candidato desde cero
                }
            }
        }
        solucion.push_back(rutaActual); // agrego ruta a la solucion
    }
    
    // Si k = 0, significa que no hay restricción de vehículos
    if (k != 0 && static_cast<int>(solucion.size()) > k) {
        return {}; // si la cantidad de vehículos en la solución sobrepasa el límite de vehículos, no existe sol óptima
    } else {
        return solucion;
    }
}