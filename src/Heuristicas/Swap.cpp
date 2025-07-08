#include "Heuristicas.h"

double Heuristicas::chequearMejoraSwap(Route& ruta_A, Route& ruta_B, NodeRoute* clienteA, NodeRoute* clienteB, const vector<vector<double>>& distancias) {
    // Inicialización de variables
    int demandaA = clienteA->demanda; // O(1)
    int demandaB = clienteB->demanda; // O(1)

    bool misma_ruta = ruta_A.getRaiz()->siguiente == ruta_B.getRaiz()->siguiente; // O(1) verifica que son la misma ruta
    bool excede_capacidad = false; // O(1)

    // Si los nodos están en la misma ruta, no hay problema de capacidad.
    // Si están en distintas rutas, verifico que el swap no exceda capacidad.
    if (!misma_ruta) { // O(1)
        int nuevaDemandaA = ruta_A.getDemandaTotal() - demandaA + demandaB; // O(1)
        int nuevaDemandaB = ruta_B.getDemandaTotal() - demandaB + demandaA; // O(1)

        if (nuevaDemandaA > ruta_A.getCapacidadTotal() || nuevaDemandaB > ruta_B.getCapacidadTotal()) { // O(1)
            excede_capacidad = true; // O(1)
        }
    }
    
    double costo_anterior_A, costo_anterior_B, costo_nuevo_A, costo_nuevo_B;
    if (!excede_capacidad) { // O(1)
        // Identifico vecinos de cada cliente para calcular el costo del cambio
        int cliente_anterior_A = clienteA->anterior->id; // O(1)
        int cliente_siguiente_A = clienteA->siguiente->id; // O(1)
        int cliente_anterior_B = clienteB->anterior->id; // O(1)
        int cliente_siguiente_B = clienteB->siguiente->id; // O(1)

        // Ejemplo ilustrativo A - misma ruta no consecutivos
        // Ruta antes: 0 1 2 3 4 5 0 (swap 2,4)
        // Ruta despues: 0 1 4 3 2 5 0
        // Costo anterior: 1->2 + 2->3 + 3->4 + 4->5
        // Costo nuevo: 1->4 + 4->3 + 3->2 + 2->5

        // Ejemplo ilustrativo B - distintas rutas
        // Rutas antes: (0 1 2 3 0) (0 4 5 6 0) (swap 2,5)
        // Ruta despues: (0 1 5 3 0) (0 4 2 6 0)
        // Costo anterior: 1->2 + 2->3 + 4->5 + 5->6
        // Costo nuevo: 1->5 + 5->3 + 4->2 + 2->6

        // Podemos ver que en los casos de ejemplos A y B buscamos hacer la misma verificacion y asignacion

        // Ejemplo ilustrativo C - misma ruta consecutivos
        // Ruta antes: 0 1 2 3 4 0 (swap 2,3)
        // Ruta despues: 0 1 3 2 4 0
        // Costo anterior: 1->2 + 2->3 + 2->3 + 3->4
        // Costo nuevo: 1->3 + 3-> 2 + 3->2 + 2->4 (notar que 2->3 es igual a 3->2)

        // Costo antes del swap para A y B
        costo_anterior_A = distancias[cliente_anterior_A][clienteA->id] + distancias[clienteA->id][cliente_siguiente_A]; // O(1)
        costo_anterior_B = distancias[cliente_anterior_B][clienteB->id] + distancias[clienteB->id][cliente_siguiente_B]; // O(1)

        if (clienteA->siguiente != clienteB && clienteB->siguiente != clienteA) { 
            // Costo después del swap para A y B (no consecutivos)
            costo_nuevo_A = distancias[cliente_anterior_B][clienteA->id] + distancias[clienteA->id][cliente_siguiente_B]; // O(1)
            costo_nuevo_B = distancias[cliente_anterior_A][clienteB->id] + distancias[clienteB->id][cliente_siguiente_A]; // O(1)
        } else { // Nodos consecutivos
            if (clienteB == clienteA->siguiente) { // Caso A->B consecutivos
                costo_nuevo_A = distancias[clienteA->id][clienteB->id] + distancias[clienteA->id][cliente_siguiente_B]; // O(1)
                costo_nuevo_B = distancias[cliente_anterior_A][clienteB->id] + distancias[clienteB->id][clienteA->id]; // O(1)
            } else { // Caso B->A consecutivos
                costo_nuevo_A = distancias[clienteA->id][clienteB->id] + distancias[clienteA->id][cliente_anterior_B]; // O(1)
                costo_nuevo_B = distancias[cliente_siguiente_A][clienteB->id] + distancias[clienteB->id][clienteA->id]; // O(1)
            }
        }
    }

    // Comparo mejora de costo
    double mejora = costo_anterior_A + costo_anterior_B - costo_nuevo_A - costo_nuevo_B;
    // Si no se excede la capacidad y mejora > 0, retorno mejora, si no, retorno 0
    if (!excede_capacidad && mejora > 0.0) { // O(1)
        return mejora; // O(1)
    } else {
        return 0.0; // O(1)
    }
}

void Heuristicas::swap(Solution& solucion, int criterio, bool exportar) {
    const vector<vector<double>>& distancias = this->_instancia.getDistanceMatrix(); // O(1)
    const vector<Route*>& rutas = solucion.getRutas(); // O(1)
    const vector<tuple<NodeRoute*, Route*>>& clientes_a_visitar = solucion.getAllClientesSol(); // O(1)
    
    // Agrega el nombre del operador de mejora aplicado al nombre del algoritmo original
    string algoritmo_goloso = solucion.getAlgoritmo();
    if (criterio == 0) {
        solucion.setAlgoritmo(algoritmo_goloso + " + Swap (FirstImprovement)"); // O(1)
    } else {
        solucion.setAlgoritmo(algoritmo_goloso + " + Swap (BestImprovement)"); // O(1)
    }
    
    int numero_iteracion = 1; // O(1)
    if (exportar) {
        // Exporta la solución inicial antes de aplicar el operador
        solucion.exportarSolutionParcial(this->_instancia.getNodes(), numero_iteracion++);
    }

    // Recorremos todos los clientes de la solución
    for (int i = 0; i < static_cast<int>(clientes_a_visitar.size()); i++) { // O(N)
        Route* rutaA = get<1>(clientes_a_visitar[i]); // O(1)
        NodeRoute* actualA = get<0>(clientes_a_visitar[i]); // O(1) 

        // Variables auxiliares para aplicar swap
        bool swap_valido = false; // O(1)
        double best_mejora = 0.0; // O(1)
        NodeRoute* best_cliente = nullptr; // O(1)
        Route* best_rutaB = nullptr; // O(1)

        // Recorremos todas las rutas para comparar clientes entre rutas distintas
        for (int j = 0; j < static_cast<int>(rutas.size()); j++) { // O(K)
            Route* rutaB = rutas[j]; // O(1)
            NodeRoute* actualB = rutaB->getRaizModify()->siguiente; // O(1)

            // Mientras no hayamos hecho un swap válido (solo para FirstImprovement) y no lleguemos al final de la ruta
            while (!swap_valido && actualB != rutaB->getUltimo()) { // O(M) (cantidad de nodos en rutaB)

                if (actualA == actualB) {
                    actualB = actualB->siguiente; // O(1)
                } else {
                    if (criterio == 0) { // First Improvement
                        if (chequearMejoraSwap(*rutaA, *rutaB, actualA, actualB, distancias) > 0.0) { // O(1)
                            // Si el swap mejora, lo aplicamos directamente (first improvement)
                            rutaA->swapClientes(*rutaB, actualA, actualB, distancias); // O(1)
                            swap_valido = true; // O(1)

                            if (exportar) {
                                solucion.exportarSolutionParcial(this->_instancia.getNodes(), numero_iteracion++); // O(N)
                            }

                        } else {
                            actualB = actualB->siguiente; // O(1)
                        }
                    } else { // Best Improvement
                        double mejora_auxiliar = chequearMejoraSwap(*rutaA, *rutaB, actualA, actualB, distancias); // O(1)
                        if (mejora_auxiliar > best_mejora) { // O(1)
                            // Guardamos el mejor swap encontrado hasta ahora
                            best_mejora = mejora_auxiliar; // O(1)
                            best_cliente = actualB; // O(1)
                            best_rutaB = rutaB; // O(1)
                        }
                        actualB = actualB->siguiente; // O(1)
                    }
                    
                }
            }
            // Salgo si ya hice un swap en first improvement
            if (swap_valido) break; // O(1)
        }

        // Si estamos en Best Improvement, aplicamos el mejor swap encontrado si hubo mejora
        if (criterio == 1 && best_mejora > 0.0) { // O(1)
            rutaA->swapClientes(*best_rutaB, actualA, best_cliente, distancias); // O(1)
            if (exportar) {
                solucion.exportarSolutionParcial(this->_instancia.getNodes(), numero_iteracion++); // O(N)
            }
        }
    }
} // Complejidad total del operador Relocate: O(N * K * M) ≈ O(N^2)
// Justificación: para cada uno de los N clientes se evalúan posibles reubicaciones en K rutas,
// con hasta M posiciones de inserción por ruta (M acotado por N).
// En cada evaluación se realiza una operación en tiempo constante (chequearMejoraRelocate → O(1)).
// Por lo tanto, el tiempo total está dominado por el recorrido del vecindario completo, resultando en O(N^2),
// lo cual es coherente con la implementación observada.