#include "Heuristicas.h"

double Heuristicas::chequearMejoraRelocate(Route& rutaA, Route& rutaB, NodeRoute* cliente, NodeRoute* destinoPrev, const vector<int>& demandas, const vector<vector<double>>& distancias) {
    int cliente_i = cliente->id;
    int cliente_p = cliente->anterior->id;
    int cliente_q = cliente->siguiente->id;
    int cliente_j = destinoPrev->id;
    NodeRoute* destinoNext = destinoPrev->siguiente;
    int cliente_k = destinoNext->id;

    double mejora = 0.0;
    bool misma_ruta = rutaA.getRaiz()->siguiente == rutaB.getRaiz()->siguiente;
    bool excede_capacidad = false;

    if (!misma_ruta) {
        if (rutaB.getDemandaTotal() + demandas[cliente_i] > rutaB.getCapacidadTotal()) {
            excede_capacidad = true;
        }
    }

    if (!excede_capacidad) {
        double costo_actual = distancias[cliente_p][cliente_i] + distancias[cliente_i][cliente_q] + distancias[cliente_j][cliente_k];
        double costo_nuevo = distancias[cliente_p][cliente_q] + distancias[cliente_j][cliente_i] + distancias[cliente_i][cliente_k];

        if (costo_nuevo < costo_actual) {
            mejora = costo_actual - costo_nuevo;
        } 
    }

    return mejora;
}

void Heuristicas::relocate(Solution& solucion, int criterio, bool exportar) {
    const vector<vector<double>>& distancias = this->_instancia.getDistanceMatrix();
    const vector<int>& demandas = this->_instancia.getDemands();
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
            NodeRoute* destinoPrev = rutaB->getRaizModify(); // cliente_j

            // el cliente_j debe tener tener al menos un siguiente que luego lo elegimos como cliente_k
            while (destinoPrev != rutaB->getUltimo()->anterior) { 

                if (clienteA == destinoPrev || // no tiene sentido que i = j porque estariamos poniendo a i en la arista i->k que ya no existe
                    clienteA == destinoPrev->siguiente  || // no tiene sentido que i = k porque pondriamos a i en la arista j->i que ya no existe
                    clienteA->siguiente == destinoPrev || destinoPrev->siguiente == clienteA) // ya estarian en esas posiciones
                    break;

                if (rutaA != rutaB && rutaB->getDemandaTotal() + demandas[clienteA->id] > rutaB->getCapacidadTotal())
                    continue;

                double mejora = chequearMejoraRelocate(*rutaA, *rutaB, clienteA, destinoPrev, demandas, distancias);

                if (criterio == 0) {
                    if (mejora > 0.0) {
                        rutaA->relocateCliente(*rutaB, clienteA, destinoPrev, demandas, distancias);
                        if (exportar) {
                            solucion.exportarSolutionParcial(this->_instancia.getNodes(), numero_iteracion++);
                        }
                        relocate_valido = true;
                    } else {
                        destinoPrev = destinoPrev->siguiente;
                    }
                } else {
                    if (mejora > best_mejora) {
                        best_mejora = mejora;
                        best_rutaB = rutaB;
                        best_destinoPrev = destinoPrev;
                    }
                    destinoPrev = destinoPrev->siguiente;
                }

            }

            // salgo si ya hice relocate en first improvement 
            if (relocate_valido) break; // O(1)
        }

        if (criterio == 1 && best_mejora > 0.0) {
            rutaA->relocateCliente(*best_rutaB, clienteA, best_destinoPrev, demandas, distancias);
            if (exportar) {
                solucion.exportarSolutionParcial(this->_instancia.getNodes(), numero_iteracion++);
            }
        }
    }

    solucion.setAlgoritmo(solucion.getAlgoritmo() + " + Relocate");
}