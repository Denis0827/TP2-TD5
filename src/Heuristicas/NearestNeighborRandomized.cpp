#include "Heuristicas.h"

// Función auxiliar para obtener candidatos aleatorios de la RCL
int Heuristicas::clienteAleatorioRCL(const vector<double>& distanciasCliente, int id, const vector<int>& visitados, 
    const vector<int>& demandas, int capacidadRestante, int rcl_size, mt19937& gen) {

    vector<pair<double, int>> candidatos; // O(1) 
    
    // obtener todos los clientes válidos con sus distancias
    for (int i = 1; i < static_cast<int>(distanciasCliente.size()); i++) { // O(N)
        if (!visitados[i] && demandas[i] <= capacidadRestante) { // O(1) verificamos si es posible agregar el cliente a la ruta
            if (i != id) { // O(1)
                candidatos.push_back({distanciasCliente[i], i}); // O(1)
            }
        }
    }
    
    // si no hay candidatos, devolver vector vacío
    if (candidatos.empty()) {
        return -1;
    }
    
    // usar nth_element para encontrar los rcl_size mínimos sin ordenar todo
    int elementos_a_tomar = min(rcl_size, static_cast<int>(candidatos.size())); // O(1)
    
    // usar nth_element para que candidatos tenga los primeros rcl_size elementos menores a rcl_size (sin ordenar)
    nth_element(candidatos.begin(), candidatos.begin() + elementos_a_tomar, candidatos.end()); // O(N)
    // nth_element usa QuickSelect, una variante de QuickSort, que en promedio es O(N) utilizando optimizaciones
    // eficientes, aunque en peor caso pueda llegar a O(N^2)

    vector<int> resultado;
    for (int i = 0; i < elementos_a_tomar; i++) { // O(rcl_size)
        resultado.push_back(candidatos[i].second); // O(1)
    }

    uniform_int_distribution<int> dist_inicial(0, elementos_a_tomar - 1);
    int indice_elegido = dist_inicial(gen);
    int cliente_a_tomar = candidatos[indice_elegido].second;

    return cliente_a_tomar;
}
// Complejidad total: O(N) en promedio

Solution Heuristicas::nearestNeighborRandomized(int i_random, bool exportar, int rcl_size) {
    const vector<vector<double>>& distancias = this->_instancia.getDistanceMatrix(); // O(1)
    const vector<int>& demandas = this->_instancia.getDemands(); // O(1)
    int depot = this->_instancia.getDepotId(); // O(1)
    int capacidad = this->_instancia.getCapacity(); // O(1)
    int n = this->_instancia.getDimension(); // O(1)
    int k = this->_instancia.getNumVehicles(); // O(1)

    vector<int> clientes_depot_ordenados = ordenarPorDistancias(distancias[depot]); // O(N*logN)
    // Ordena los clientes según su distancia al depósito para formar la RCL inicial.

    mt19937 gen;
    gen.seed(time(nullptr) + i_random); // O(1)
    // Inicializa el generador de números aleatorios con una semilla distinta por iteración.

    int clientes_no_visitados = n - 1; // O(1)
    vector<int> visitados (n + 1, 0); // O(N)
    visitados[depot] = 1; // O(1)
    // Inicializa los vectores auxiliares.

    int numero_iteracion = 1;
    Solution solucion = Solution(k, "NearestNeighborRandomized", this->_nombreInstancia); // O(1)

    if (exportar) {
        solucion.exportarSolutionParcial(this->_instancia.getNodes(), numero_iteracion++); // O(N)
    }

    while (clientes_no_visitados > 0) { // Máximo N veces
        vector<int> rcl_inicial;

        for (int i = 1; i <= static_cast<int>(n); i++) { // O(N)
            if (visitados[clientes_depot_ordenados[i]] == 0) {
                rcl_inicial.push_back(clientes_depot_ordenados[i]); // O(1)
                if (static_cast<int>(rcl_inicial.size()) == rcl_size) break; // O(1)
            }
        }

        uniform_int_distribution<int> dist_inicial(0, rcl_inicial.size() - 1); // O(1)
        int indice_elegido = dist_inicial(gen); // O(1)
        int primer_cliente_sin_ruta = rcl_inicial[indice_elegido]; // O(1)
        visitados[primer_cliente_sin_ruta] = 1; // O(1)
        clientes_no_visitados--; // O(1)

        Route* rutaActual = new Route(capacidad, depot); // O(1)
        rutaActual->agregarClienteInicio(primer_cliente_sin_ruta, demandas[primer_cliente_sin_ruta],
            distancias[depot][primer_cliente_sin_ruta]); // O(1)

        int actual = primer_cliente_sin_ruta; // O(1)
        bool puedeAgregar = true;

        while (puedeAgregar != false) {
            // Selección aleatoria del siguiente cliente a partir de la RCL
            int candidato = clienteAleatorioRCL(distancias[actual], actual, visitados, demandas, 
                rutaActual->getCapacidadRestante(), rcl_size, gen); // O(N) promedio

            if (candidato != -1) {
                rutaActual->agregarClienteFinal(candidato, demandas[candidato],
                    distancias[actual][depot], distancias[actual][candidato], distancias[candidato][depot]); // O(1)

                visitados[candidato] = 1; // O(1)
                actual = candidato; // O(1)
                clientes_no_visitados--; // O(1)

                if (exportar) {
                    Solution parcial = solucion;
                    parcial.agregarRuta(rutaActual); // O(1)
                    parcial.exportarSolutionParcial(this->_instancia.getNodes(), numero_iteracion++); // O(N)
                }

            } else {
                puedeAgregar = false; // No hay más clientes factibles para esta ruta
            }
        }

        solucion.agregarRuta(rutaActual); // O(1)

        if (exportar) {
            solucion.exportarSolutionParcial(this->_instancia.getNodes(), numero_iteracion++); // O(N)
        }
    }

    return solucion;
}

// Complejidad total del algoritmo: O(N^2)
// Justificación: Cada cliente es agregado una sola vez, y en cada inserción se evalúan O(N) candidatos.
// Además, la construcción de rutas implica N selecciones con un costo promedio de O(N) cada una.