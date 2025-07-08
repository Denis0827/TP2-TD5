#include "Heuristicas.h"

double Heuristicas::chequearMejoraRelocate(Route& rutaA, Route& rutaB, NodeRoute* cliente, NodeRoute* destinoPrev, const vector<int>& demandas, const vector<vector<double>>& distancias) {
    int cliente_i = cliente->id;
    int cliente_p = cliente->anterior->id;
    int cliente_q = cliente->siguiente->id;
    int cliente_j = destinoPrev->id;
    NodeRoute* destinoNext = destinoPrev->siguiente;
    int cliente_k = destinoNext->id;

    double mejora = 0.0;
    bool excede_capacidad = false;

    // Si se trata de rutas distintas, chequea capacidad antes de mover
    if (&rutaA != &rutaB) {
        if (rutaB.getDemandaTotal() + demandas[cliente_i] > rutaB.getCapacidadTotal()) {
            excede_capacidad = true;
        }
    }

    // Si no se viola capacidad, calcular la mejora
    if (!excede_capacidad) {
        // Se evalúa la diferencia de costo entre quitar i de su ruta y colocarlo entre j y k
        double costo_actual = distancias[cliente_p][cliente_i] + distancias[cliente_i][cliente_q] + distancias[cliente_j][cliente_k];
        double costo_nuevo = distancias[cliente_p][cliente_q] + distancias[cliente_j][cliente_i] + distancias[cliente_i][cliente_k];

        if (costo_nuevo < costo_actual) {
            mejora = costo_actual - costo_nuevo;
        } 
    }

    return mejora;
}
void Heuristicas::relocate(Solution& solucion, int criterio, bool exportar) {
    const vector<vector<double>>& distancias = this->_instancia.getDistanceMatrix(); // O(1)
    const vector<int>& demandas = this->_instancia.getDemands(); // O(1)
    const vector<Route*>& rutas = solucion.getRutas(); // O(K)
    const vector<tuple<NodeRoute*, Route*>>& clientes_a_visitar = solucion.getAllClientesSol(); // O(N)

    string algoritmo_goloso = solucion.getAlgoritmo();
    if (criterio == 0) {
        solucion.setAlgoritmo(algoritmo_goloso + " + Relocate (FirstImprovement)"); // O(1)
    } else {
        solucion.setAlgoritmo(algoritmo_goloso + " + Relocate (BestImprovement)"); // O(1)
    }

    int numero_iteracion = 1;
    if (exportar) {
        solucion.exportarSolutionParcial(this->_instancia.getNodes(), numero_iteracion++); // O(N)
    }

    // Por cada cliente de la solución actual
    for (int i = 0; i < static_cast<int>(clientes_a_visitar.size()); i++) { // O(N)
        NodeRoute* clienteA = get<0>(clientes_a_visitar[i]);
        Route* rutaA = get<1>(clientes_a_visitar[i]);

        bool relocate_valido = false;
        Route* best_rutaB = nullptr;
        NodeRoute* best_destinoPrev = nullptr;
        double best_mejora = 0.0;

        // Intentamos mover clienteA a todas las posiciones válidas en todas las rutas
        for (int j = 0; j < static_cast<int>(rutas.size()); j++) { // O(K)
            Route* rutaB = rutas[j];
            NodeRoute* destinoPrev = rutaB->getRaizModify(); // O(1)

            while (destinoPrev != rutaB->getUltimo()->anterior) { // O(M) por ruta
                // Validaciones para evitar posiciones inválidas o redundantes
                if (clienteA == destinoPrev ||
                    clienteA == destinoPrev->siguiente  ||
                    clienteA->siguiente == destinoPrev || destinoPrev->siguiente == clienteA)
                    break;

                double mejora = chequearMejoraRelocate(*rutaA, *rutaB, clienteA, destinoPrev, demandas, distancias); // O(1)

                if (criterio == 0) { // First Improvement
                    if (mejora > 0.0) {
                        rutaA->relocateCliente(*rutaB, clienteA, destinoPrev, demandas, distancias); // O(1)
                        if (exportar) {
                            solucion.exportarSolutionParcial(this->_instancia.getNodes(), numero_iteracion++); // O(N)
                        }
                        relocate_valido = true;
                    } else {
                        destinoPrev = destinoPrev->siguiente; // O(1)
                    }
                } else { // Best Improvement
                    if (mejora > best_mejora) {
                        best_mejora = mejora;
                        best_rutaB = rutaB;
                        best_destinoPrev = destinoPrev;
                    }
                    destinoPrev = destinoPrev->siguiente;
                }
            }

            // En First Improvement salimos al encontrar la primera mejora
            if (relocate_valido) break; // O(1)
        }

        // En Best Improvement aplicamos la mejor reubicación encontrada
        if (criterio == 1 && best_mejora > 0.0) {
            rutaA->relocateCliente(*best_rutaB, clienteA, best_destinoPrev, demandas, distancias); // O(1)
            if (exportar) {
                solucion.exportarSolutionParcial(this->_instancia.getNodes(), numero_iteracion++); // O(N)
            }
        }
    }

    solucion.eliminarRutasVacias(); // O(K)
}

// Complejidad total del operador Relocate: O(N * K * M) ≈ O(N^2)
// Justificación: para cada uno de los N clientes se evalúan posibles reubicaciones en K rutas,
// con hasta M posiciones de inserción por ruta (M acotado por N). Como el chequeo de mejora y
// aplicación son O(1), el recorrido del vecindario domina el costo total. El resultado es O(N^2),
// en línea con la implementación observada.