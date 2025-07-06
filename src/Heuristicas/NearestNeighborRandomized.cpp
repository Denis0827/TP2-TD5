#include "Heuristicas.h"

// Función auxiliar para obtener candidatos aleatorios de la RCL
int clienteAleatorioRCL (const vector<double>& distanciasCliente, int id, const vector<int>& visitados, const vector<int>& demandas, int capacidadRestante, int rcl_size, mt19937& gen) {
    vector<pair<double, int>> candidatos;

    // Buscar todos los candidatos factibles
    for (int i = 1; i < static_cast<int>(distanciasCliente.size()); i++) {
        if (!visitados[i] && demandas[i] <= capacidadRestante && i != id)  {
            candidatos.push_back({distanciasCliente[i], i});
        }
    }

    if (candidatos.empty()) {
        return -1; // No hay candidatos factibles
    }

    // Ordenar candidatos por distancia (ascendente)
    sort(candidatos.begin(), candidatos.end());

    // Crear RCL con los mejores candidatos (hasta rcl_size)
    int rcl_actual_size = min(rcl_size, static_cast<int>(candidatos.size()));

    // Seleccionar aleatoriamente uno de los candidatos en la RCL
    uniform_int_distribution<int> dist(0, rcl_actual_size - 1);
    int indice_elegido = dist(gen);

    return candidatos[indice_elegido].second;
}

Solution Heuristicas::graspNearestNeighbor(int rcl_size, int seed) {
    const vector<vector<double>>& distancias = this->_instancia.getDistanceMatrix(); // O(1)
    const vector<int>& demandas = this->_instancia.getDemands(); // O(1)
    int depot = this->_instancia.getDepotId(); // O(1)
    int capacidad = this->_instancia.getCapacity(); // O(1)
    int n = this->_instancia.getDimension(); // O(1)
    int k = this->_instancia.getNumVehicles(); // O(1)

    // INicializar generador de números aleatorios
    mt19937 gen;
    if (seed == -1) {
        gen.seed(time(nullptr)); // Usar tiempo actual si no se especifica semilla
    } else {
        gen.seed(seed); // Usás la misma seed → mismo resultado
    }

    // Crear RCL para selección del primer cliente de cada ruta
    vector<pair<double, int>> candidatos_iniciales;
    for (int i = 1; i < n; i++) {
        candidatos_iniciales.push_back({distancias[depot][i], i});
    }
    sort(candidatos_iniciales.begin(), candidatos_iniciales.end());

    // para saber cuando todos los clientes tienen una ruta
    int clientes_no_visitados = n - 1; // O(1) sin contar el deposito
    vector<int> visitados (n + 1, 0); // O(N) cada i del vector es el cliente i
    visitados[depot] = 1; // O(1) no queremos considerar el depot, solo clientes

    Solution solucion = Solution(k, "GRASP-NearestNeighbor", this->_nombreInstancia);

    while (clientes_no_visitados > 0) {
        // Seleccionar primer cliente de la ruta usando RCL
        int primer_cliente_sin_ruta = -1;

        // Crear RCL con clientes no visitados más cercanos al depot
        vector<pair<double, int>> rcl_inicial;
        for (const auto& candidato : candidatos_iniciales) {
            if (!visitados[candidato.second]) {
                rcl_inicial.push_back(candidato);
                if (static_cast<int>(rcl_inicial.size()) >= rcl_size) {
                    break; // Solo tomamos los primeros rcl_size candidatos
                }
            }
        }

        if (!rcl_inicial.empty()) {
            // Seleccionar aleatoriamente del RCL inicial
            uniform_int_distribution<int> dist_inicial(0, rcl_inicial.size() - 1);
            int indice_elegido = dist_inicial(gen);
            primer_cliente_sin_ruta = rcl_inicial[indice_elegido].second;

            visitados[primer_cliente_sin_ruta] = 1;
            clientes_no_visitados--;
        }
        Route* rutaActual = new Route(capacidad, depot); // O(1) creamos la ruta nueva
        rutaActual->agregarClienteInicio(primer_cliente_sin_ruta, demandas[primer_cliente_sin_ruta],
            distancias[depot][primer_cliente_sin_ruta]); // O(1) agregamos el cliente encontrado

        int actual = primer_cliente_sin_ruta; // O(1)
        bool puedeAgregar = true; // O(1) me dice si encuentra clientes que se pueden agregar a la ruta

        while (puedeAgregar != false) { // O(1)
            int candidato = clienteAleatorioRCL(distancias[actual], actual, visitados, demandas, 
                rutaActual->getCapacidadRestante(), rcl_size, gen);

            if (candidato != -1) { // O(1)
                // Agregar el candidato a la ruta
                rutaActual->agregarClienteFinal(candidato, demandas[candidato], distancias[actual][depot], distancias[actual][candidato], 
                    distancias[candidato][depot]); // O(1)
                
                visitados[candidato] = 1; // O(1) ya visité entonces el candidato
                actual = candidato; // O(1) ahora busco el minimo cliente desde el candidato (candidato del candidato)
                clientes_no_visitados--; // O(1)
            } else {
                puedeAgregar = false; // O(1) si no encontré ningun candidato, la ruta esta hecha
            }
            
        }
        solucion.agregarRuta(rutaActual); // O(1) agrego ruta a la solucion
    }
    return solucion;
}