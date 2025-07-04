#include "Heuristicas.h"

void Heuristicas::swap(Solution& solucion) {
    const vector<vector<double>>& distancias = this->_instancia.getDistanceMatrix(); // O(1)
    vector<tuple<int, Route*>> rutas = solucion.getRutas();

    for (int i = 0; i < static_cast<int>(rutas.size()); i++) {
        Route rutaA = *get<1>(rutas[i]);
        NodeRoute* actualA = rutaA.getRaizModify()->siguiente;

        while (actualA != rutaA.getUltimo()) {

            for (int j = 0; j < static_cast<int>(rutas.size()); j++) {
                Route rutaB = *get<1>(rutas[j]);
                NodeRoute* actualB = rutaB.getRaizModify()->siguiente;
                bool swap_valido = false;

                while (!swap_valido && actualB != rutaB.getUltimo()) {
                    if (actualA == actualB) {
                        actualB = actualB->siguiente;
                    } else {
                        // first improvement
                        if (rutaA.swapClientes(rutaB, actualA, actualB, distancias)) {
                            cout << "Swap valido entre " << actualA->id << " y " << actualB->id << endl;
                            swap_valido = true;
                        } else {
                            actualB = actualB->siguiente;
                        }
                    }
                }

                if (swap_valido) break;
            }

            actualA = actualA->siguiente;
        }
    }

    string algoritmo_goloso = solucion.getAlgoritmo();
    solucion.setAlgoritmo(algoritmo_goloso + " + Swap");
}