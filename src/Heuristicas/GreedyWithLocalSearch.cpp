#include "Heuristicas.h"

Solution Heuristicas::greedyWithLocalSearch(const string& algoritmo_greedy, const vector<string>& operadores_local, int criterio, bool exportar) {
    Solution solucion = Solution();

    if (algoritmo_greedy == "ClarkeWright") {
        solucion = clarkeWright(false);
    } else if (algoritmo_greedy == "NearestNeighbor") {
        solucion = nearestNeighbor(false);
    }

    string algoritmo_goloso = solucion.getAlgoritmo();

    for (int i = 0; i < static_cast<int>(operadores_local.size()); i++) {
        if (operadores_local[i] == "Swap") {
            swap(solucion, criterio, exportar);
        } else if (operadores_local[i] == "Relocate") {
            relocate(solucion, criterio, exportar);
        } 
    }

    if (static_cast<int>(operadores_local.size()) == 2) {
        if (operadores_local[0] == "Swap") {
            if (criterio == 0) {
                solucion.setAlgoritmo(algoritmo_goloso + " + Swap + Relocate (FirstImprovement)"); // O(1)
            } else {
                solucion.setAlgoritmo(algoritmo_goloso + " + Swap + Relocate (BestImprovement)"); // O(1)
            }
        } else {
            if (criterio == 0) {
                solucion.setAlgoritmo(algoritmo_goloso + " + Relocate + Swap (FirstImprovement)"); // O(1)
            } else {
                solucion.setAlgoritmo(algoritmo_goloso + " + Relocate + Swap (BestImprovement)"); // O(1)
            }
        }
    }

    return solucion;
    
}