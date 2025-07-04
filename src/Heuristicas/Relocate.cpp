/*#include "Heuristicas.h"

vector<Route> Heuristicas::relocate(const vector<Route>& solucion_inicial) {
    vector<Route> solucion = solucion_inicial; 
    int cantidad_vehiculos = solucion_inicial.size();
    const vector<vector<double>>& distancias = instancia.getDistanceMatrix();
    const vector<int>& demandas = instancia.getDemands();
    int capacidad = instancia.getCapacity();
    int n = instancia.getDimension();

    int clientesNoVisitados = n - 1;
    vector<int> visitados (n + 1, 0);

    for (int i = 0; i < cantidad_vehiculos; i++){
        int cliente_i = solucion_inicial[i].nodes[1];
        int cliente_p = solucion_inicial[i].nodes[0];
        int cliente_q = solucion_inicial[i].nodes[2];

        int costo_pi = distancias[cliente_p][cliente_i];
        int costo_iq = distancias[cliente_i][cliente_q];
        int costo_pq = distancias[cliente_p][cliente_q];

        int cliente_j = 0;
        int cliente_k = 0;

        int costo_jk = 0;
        int costo_ji = 0;
        int costo_ik = 0;

        for (int s = 0; s < cantidad_vehiculos; s++) {
            int cant_clientes_vehiculo_s = solucion_inicial[s].nodes.size() - 2; // elimino los depot del inicio y final
            for (int j = 0; j <= cant_clientes_vehiculo_s; j++) {
                // verifico que cliente_j no sea igual a cliente_i ni cliente_p
                if (solucion_inicial[s].nodes[j] != cliente_i && solucion_inicial[s].nodes[j] != cliente_p) { 
                    cliente_j = solucion_inicial[s].nodes[j];
                    cliente_k = solucion_inicial[s].nodes[j + 1];
                    costo_jk = distancias[cliente_j][cliente_k];
                    costo_ji = distancias[cliente_j][cliente_i];
                    costo_ik = distancias[cliente_i][cliente_k];
                    
                    // verificacion de mejora
                }
            }
        }



        insert

    }
}

*/


#include "Heuristicas.h"

void Heuristicas::relocate(Solution& solucion, int criterio, bool exportar) {
    const vector<vector<double>>& distancias = this->_instancia.getDistanceMatrix();
    const vector<int>& demandas = this->_instancia.getDemandas();
    const vector<tuple<int, Route*>>& rutas = solucion.getRutas();
    const vector<tuple<NodeRoute*, Route*>>& clientes_a_visitar = solucion.getAllClientesSol();

    int numero_iteracion = 0;
    if (exportar) {
        solucion.exportarSolutionParcial(this->_instancia.getNodes(), numero_iteracion++);
    }

    for (int i = 0; i < static_cast<int>(clientes_a_visitar.size()); i++) {
        NodeRoute* clienteA = get<0>(clientes_a_visitar[i]);
        Route* rutaA = get<1>(clientes_a_visitar[i]);

        bool relocate_valido = false;
        Route* best_rutaB = nullptr;
        NodeRoute* best_destinoPrev = nullptr;
        double best_mejora = 0.0;

        for (int j = 0; j < static_cast<int>(rutas.size()); j++) {
            Route* rutaB = get<1>(rutas[j]);

            for (NodeRoute* destinoPrev = rutaB->getRaizModify(); destinoPrev != rutaB->getUltimo(); destinoPrev = destinoPrev->siguiente) {
                if (clienteA == destinoPrev || clienteA->siguiente == destinoPrev || destinoPrev->siguiente == clienteA)
                    continue;

                int id_i = clienteA->id;
                int id_p = clienteA->anterior->id;
                int id_q = clienteA->siguiente->id;
                int id_j = destinoPrev->id;
                int id_k = destinoPrev->siguiente->id;

                if (rutaA != rutaB && rutaB->getDemandaTotal() + demandas[id_i] > rutaB->getCapacidadTotal())
                    continue;

                double costo_actual = distancias[id_p][id_i] + distancias[id_i][id_q] + distancias[id_j][id_k];
                double costo_nuevo = distancias[id_p][id_q] + distancias[id_j][id_i] + distancias[id_i][id_k];
                double mejora = costo_actual - costo_nuevo;

                if (mejora > 0.0) {
                    if (criterio == 0) {
                        if (rutaA->relocateCliente(*rutaB, clienteA, destinoPrev, demandas, distancias)) {
                            if (exportar)
                                solucion.exportarSolutionParcial(this->_instancia.getNodes(), numero_iteracion++);
                            relocate_valido = true;
                            break;
                        }
                    } else if (mejora > best_mejora) {
                        best_mejora = mejora;
                        best_rutaB = rutaB;
                        best_destinoPrev = destinoPrev;
                    }
                }
            }

            if (criterio == 0 && relocate_valido) break;
        }

        if (criterio == 1 && best_mejora > 0.0) {
            rutaA->relocateCliente(*best_rutaB, clienteA, best_destinoPrev, demandas, distancias);
            if (exportar)
                solucion.exportarSolutionParcial(this->_instancia.getNodes(), numero_iteracion++);
        }
    }

    solucion.setAlgoritmo(solucion.getAlgoritmo() + " + Relocate");
}
