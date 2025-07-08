#include "Heuristicas.h"

Solution Heuristicas::GRASP(int numero_iteraciones, string operador_local, int criterio_local, int rcl_size) {
    int k = this->_instancia.getNumVehicles(); // O(1)

    double best_dist = numeric_limits<double>::max();
    Solution best_sol = Solution(k, "GRASP", this->_nombreInstancia);

    for (int i = 0; i < numero_iteraciones; i++) {
        Solution solucion_golosa = nearestNeighborRandomized(i, false, rcl_size);
        
        if (operador_local == "Swap") {
            swap(solucion_golosa, criterio_local, false);
        } else if (operador_local == "Relocate") {
            relocate(solucion_golosa, criterio_local, false);
        }

        double distancia_total = solucion_golosa.getDistanciaTotal();

        if (distancia_total < best_dist) {
            best_dist = distancia_total;
            best_sol = solucion_golosa;
        }
    }

    return best_sol;
}