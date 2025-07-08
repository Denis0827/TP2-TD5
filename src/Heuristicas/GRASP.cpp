#include "Heuristicas.h"

Solution Heuristicas::GRASP(int numero_iteraciones, string operador_local, int criterio_local, int rcl_size) {
    int k = this->_instancia.getNumVehicles(); // O(1)
    // Se obtiene la cantidad de vehículos para inicializar la solución.

    double best_dist = numeric_limits<double>::max(); // O(1)
    // Inicializa la mejor distancia como infinita.

    Solution best_sol = Solution(k, "GRASP", this->_nombreInstancia); // O(1)
    // Se inicializa la mejor solución vacía con etiqueta "GRASP".

    for (int i = 0; i < numero_iteraciones; i++) { // O(T)
        // Repite el proceso T veces, cada una genera una solución distinta.

        Solution solucion_golosa = nearestNeighborRandomized(i, false, rcl_size); // O(N^2)
        // Construcción aleatorizada de una solución factible. Ya se justificó que tiene complejidad O(N^2).

        if (operador_local == "Swap") {
            swap(solucion_golosa, criterio_local, false); // O(N^2)
            // Aplica el operador de mejora local Swap si fue seleccionado.
        } else if (operador_local == "Relocate") {
            relocate(solucion_golosa, criterio_local, false); // O(N^2)
            // Aplica el operador de mejora local Relocate si fue seleccionado.
        }
        // Ambos operadores exploran el vecindario completo y tienen complejidad O(N^2).

        double distancia_total = solucion_golosa.getDistanciaTotal(); // O(1)
        // Calcula el costo total de la solución generada.

        if (distancia_total < best_dist) { // O(1)
            best_dist = distancia_total; // O(1)
            best_sol = solucion_golosa; // O(1)
            // Actualiza la mejor solución si se obtiene una de menor costo.
        }
    }

    return best_sol; // O(1)
    // Devuelve la mejor solución encontrada tras T iteraciones.
}

// Complejidad total del algoritmo: O(T * N^2)
// Justificación: el ciclo principal se repite T veces. En cada iteración se construye una solución con
// nearestNeighborRandomized (O(N^2)) y se mejora con un operador local (también O(N^2)).
// Como estas dos fases dominan el costo, la complejidad total es O(T * N^2).