#include "Heuristicas.h"
#include <chrono>
using namespace std::chrono;

void resolverCVRP(string algoritmo, string instancia) {
    Heuristicas heuristicas(instancia);
    cout << "Solución óptima utilizando " + algoritmo << endl;
    auto start = high_resolution_clock::now();
    
    Solution solucion;

    const vector<string> algoritmos_validos = { 
        "ClarkeWright", 
        "NearestNeighbor", 
        "ClarkeWright + Swap", 
        "ClarkeWright + Relocate",
        "NearestNeighbor + Swap",
        "NearestNeighbor + Relocate",
        "GRASP ClarkeWright + Swap",
        "GRASP ClarkeWright + Relocate",
        "GRASP NeareastNeighbor + Swap",
        "GRASP NeareastNeighbor + Relocate" 
    };

    if (algoritmo == "ClarkeWright") {
        solucion = heuristicas.clarkeWright();
    } else if (algoritmo == "NearestNeighbor") {
        solucion = heuristicas.nearestNeighbor();
    } else if (algoritmo == "ClarkeWright + Swap") {
        solucion = heuristicas.clarkeWright();
        heuristicas.swap(solucion);
    }

    auto end = high_resolution_clock::now();
    auto duracion_us = duration_cast<microseconds>(end - start).count();

    solucion.imprimirSolution();

    if (duracion_us < 1000) {
        cout << "Tiempo de ejecución " + algoritmo + ": " << duracion_us << " µs" << endl;
    } else {
        cout << "Tiempo de ejecución " + algoritmo + ": " << duracion_us / 1000.0 << " ms" << endl;
    }
    cout << "==========" << endl;
}

int main() {
    resolverCVRP("ClarkeWright", "instancias/2l-cvrp-0/E016-03m.dat");
    resolverCVRP("NearestNeighbor", "instancias/2l-cvrp-0/E045-04f.dat");
    resolverCVRP("ClarkeWright + Swap", "instancias/2l-cvrp-0/E016-03m.dat");
    return 0;
}