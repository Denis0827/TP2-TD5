#include "Heuristicas.h"
#include <chrono>
using namespace std::chrono;

void resolverCVRP(string algoritmo, int criterio, string instancia) {
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
    } else if (algoritmo == "ClarkeWright + Swap" && criterio == 0) {
        solucion = heuristicas.clarkeWright();
        heuristicas.swap(solucion, 0);
    } else if (algoritmo == "ClarkeWright + Swap" && criterio == 1) {
        solucion = heuristicas.clarkeWright();
        heuristicas.swap(solucion, 1);
    } else if (algoritmo == "NearestNeighbor + Swap" && criterio == 0) {
        solucion = heuristicas.nearestNeighbor();
        heuristicas.swap(solucion, 0);
    } else if (algoritmo == "NearestNeighbor + Swap" && criterio == 1) {
        solucion = heuristicas.nearestNeighbor();
        heuristicas.swap(solucion, 1);
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
    //resolverCVRP("ClarkeWright", -1, "instancias/2l-cvrp-0/E200-17c.dat");
    resolverCVRP("NearestNeighbor", -1, "instancias/2l-cvrp-0/E045-04f.dat");
    //resolverCVRP("ClarkeWright + Swap", 1, "instancias/2l-cvrp-0/E200-17c.dat");
    resolverCVRP("NearestNeighbor + Swap", 0, "instancias/2l-cvrp-0/E045-04f.dat");
    resolverCVRP("NearestNeighbor + Swap", 1, "instancias/2l-cvrp-0/E045-04f.dat");
    return 0;
}