#include "Heuristicas.h"

int demandaA = 0;
int demandaB = 0;
double costo_anterior_A, costo_anterior_B, costo_nuevo_A, costo_nuevo_B;

double chequearMejora(Route& ruta_A, Route& ruta_B, NodeRoute* clienteA, NodeRoute* clienteB, const vector<vector<double>>& distancias) {
    // Inicialización de variables
    demandaA = clienteA->demanda;
    demandaB = clienteB->demanda;

    bool misma_ruta = ruta_A.getRaiz()->siguiente->id == ruta_B.getRaiz()->siguiente->id; // si las rutas no tienen el mismo primer cliente, entonces son distintas rutas
    bool excede_capacidad = false;

    // si los nodos estan en la misma ruta, no habria conflicto de capacidad al swapear
    // pero si estan en distintas rutas, debo chequear si el swap es valido, es decir, 
    // si cambiar el cliente de ruta no me rompe la capacidad
    if (!misma_ruta) {
        int nuevaDemandaA = ruta_A.getDemandaTotal() - demandaA + demandaB;
        int nuevaDemandaB = ruta_B.getDemandaTotal() - demandaB + demandaA;

        if (nuevaDemandaA > ruta_A.getCapacidadTotal() || nuevaDemandaB > ruta_B.getCapacidadTotal()) {
            excede_capacidad = true;
        }
    }
    
    if (!excede_capacidad) { // ahora me asegure que no me rompe la capacidad en ningun caso
        int cliente_anterior_A = clienteA->anterior->id;
        int cliente_siguiente_A = clienteA->siguiente->id;
        int cliente_anterior_B = clienteB->anterior->id;
        int cliente_siguiente_B = clienteB->siguiente->id;

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

        costo_anterior_A = distancias[cliente_anterior_A][clienteA->id] + distancias[clienteA->id][cliente_siguiente_A];
        costo_anterior_B = distancias[cliente_anterior_B][clienteB->id] + distancias[clienteB->id][cliente_siguiente_B];

        if (clienteA->siguiente != clienteB && clienteB->siguiente != clienteA) { // nodos no consecutivos
            costo_nuevo_A = distancias[cliente_anterior_B][clienteA->id] + distancias[clienteA->id][cliente_siguiente_B];
            costo_nuevo_B = distancias[cliente_anterior_A][clienteB->id] + distancias[clienteB->id][cliente_siguiente_A];
        } else { // nodos consecutivos
            if (clienteB == clienteA->siguiente) {
                costo_nuevo_A = distancias[clienteA->id][clienteB->id] + distancias[clienteA->id][cliente_siguiente_B];
                costo_nuevo_B = distancias[cliente_anterior_A][clienteB->id] + distancias[clienteB->id][clienteA->id];
            } else {
                costo_nuevo_A = distancias[clienteA->id][clienteB->id] + distancias[clienteA->id][cliente_anterior_B];
                costo_nuevo_B = distancias[cliente_siguiente_A][clienteB->id] + distancias[clienteB->id][clienteA->id];
            }
        }
    }

    double mejora = costo_anterior_A + costo_anterior_B - costo_nuevo_A - costo_nuevo_B;
    // Chequeo si hay alguna mejora de costo y realizo el swap
    if (!excede_capacidad && mejora > 0.0) {
        return mejora;
    } else {
        return 0.0;
    }
}

void Heuristicas::swap(Solution& solucion) {
    const vector<vector<double>>& distancias = this->_instancia.getDistanceMatrix();
    const vector<tuple<int, Route*>>& rutas = solucion.getRutas();

    for (int i = 0; i < static_cast<int>(rutas.size()); i++) {
        Route* rutaA = get<1>(rutas[i]);
        NodeRoute* actualA = rutaA->getRaizModify()->siguiente;

        while (actualA != rutaA->getUltimo()) {
            bool swap_valido = false;

            for (int j = 0; j < static_cast<int>(rutas.size()); j++) {
                Route* rutaB = get<1>(rutas[j]);
                NodeRoute* actualB = rutaB->getRaizModify()->siguiente;

                while (!swap_valido && actualB != rutaB->getUltimo()) {
                    if (actualA == actualB) {
                        actualB = actualB->siguiente;
                    } else {
                        // first improvement
                        if (chequearMejora(*rutaA, *rutaB, actualA, actualB, distancias) > 0.0) {
                            //cout << "Swap valido entre " << actualA->id << " y " << actualB->id << endl;
                            rutaA->swapClientes(*rutaB, actualA, actualB, demandaA, demandaB, costo_anterior_A,
                                costo_anterior_B, costo_nuevo_A, costo_nuevo_B);
                            actualA = actualB->siguiente;
                            //solucion.imprimirSolution();
                            //cout << "==" << endl;
                            swap_valido = true;
                        } else {
                            actualB = actualB->siguiente;
                        }
                    }
                }

                if (swap_valido) break;
            }

            if (!swap_valido) actualA = actualA->siguiente;
        }
    }

    string algoritmo_goloso = solucion.getAlgoritmo();
    solucion.setAlgoritmo(algoritmo_goloso + " + Swap");
}