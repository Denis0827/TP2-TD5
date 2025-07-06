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

Solution Heuristicas::nearestNeighborRandomized(bool exportar, int rcl_size) {
    const vector<vector<double>>& distancias = this->_instancia.getDistanceMatrix(); // O(1)
    const vector<int>& demandas = this->_instancia.getDemands(); // O(1)
    int depot = this->_instancia.getDepotId(); // O(1)
    int capacidad = this->_instancia.getCapacity(); // O(1)
    int n = this->_instancia.getDimension(); // O(1)
    int k = this->_instancia.getNumVehicles(); // O(1)

    vector<int> clientes_depot_ordenados = ordenarPorDistancias(distancias[depot]); // O(N*logN)

    // INicializar generador de números aleatorios
    mt19937 gen;
    gen.seed(time(nullptr)); // Usar tiempo actual

    // para saber cuándo todos los clientes tienen una ruta
    int clientes_no_visitados = n - 1; // O(1) sin contar el deposito
    vector<int> visitados (n + 1, 0); // O(N) cada i del vector es el cliente i
    visitados[depot] = 1; // O(1) no queremos considerar el depot, solo clientes

    int numero_iteracion = 1;
    Solution solucion = Solution(k, "NearestNeighborRandomized", this->_nombreInstancia);

    if (exportar) {
        solucion.exportarSolutionParcial(this->_instancia.getNodes(), numero_iteracion++);
    }

    while (clientes_no_visitados > 0) {
        // Seleccionar primer cliente de la ruta usando RCL
        vector<int> rcl_inicial;

        for (int i = 1; i <= static_cast<int>(n); i++) { // O(N)
            int cliente_sin_ruta;

            if (visitados[clientes_depot_ordenados[i]] == 0) { // O(1)
                cliente_sin_ruta = clientes_depot_ordenados[i]; // O(1)
                rcl_inicial.push_back(cliente_sin_ruta);

                if (static_cast<int>(rcl_inicial.size()) == rcl_size) {
                    break; // si encontré los primeros rcl_size candidatos salgo del ciclo
                }
            }
        }

        // Recordar que puedo tener menos que rcl_size candidatos, pero ese valor es la cota maxima

        // Seleccionar aleatoriamente del RCL inicial
        uniform_int_distribution<int> dist_inicial(0, rcl_inicial.size() - 1);
        int indice_elegido = dist_inicial(gen);
        int primer_cliente_sin_ruta = rcl_inicial[indice_elegido];
        visitados[primer_cliente_sin_ruta] = 1;
        clientes_no_visitados--;
        
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
}