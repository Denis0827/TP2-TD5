#include "Heuristicas.h"
#include <chrono>
using namespace std::chrono;

void resolverCVRP(string algoritmo, string instancia, bool exportar = false, string criterio = "") {
    size_t slash_pos = instancia.find_last_of('/');
    size_t dot_pos = instancia.find_last_of('.');
    string nombre = instancia.substr(slash_pos + 1, dot_pos - slash_pos - 1);

    Heuristicas heuristicas(instancia, nombre);
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
        solucion = heuristicas.clarkeWright(exportar);
    } else if (algoritmo == "NearestNeighbor") {
        solucion = heuristicas.nearestNeighbor(exportar);
    } else if (algoritmo == "ClarkeWright + Swap" && criterio == "firstImprovement") {
        solucion = heuristicas.clarkeWright();
        heuristicas.swap(solucion, 0, exportar);
    } else if (algoritmo == "ClarkeWright + Swap" && criterio == "bestImprovement") {
        solucion = heuristicas.clarkeWright();
        heuristicas.swap(solucion, 1, exportar);
    } else if (algoritmo == "NearestNeighbor + Swap" && criterio == "firstImprovement") {
        solucion = heuristicas.nearestNeighbor();
        heuristicas.swap(solucion, 0, exportar);
    } else if (algoritmo == "NearestNeighbor + Swap" && criterio == "bestImprovement") {
        solucion = heuristicas.nearestNeighbor();
        heuristicas.swap(solucion, 1, exportar);
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
    resolverCVRP("ClarkeWright", "instancias/2l-cvrp-0/E200-17c.dat", true);
    resolverCVRP("NearestNeighbor", "instancias/2l-cvrp-0/E200-17c.dat", true);
    resolverCVRP("ClarkeWright + Swap", "instancias/2l-cvrp-0/E200-17c.dat", true, "firstImprovement");
    resolverCVRP("ClarkeWright + Swap", "instancias/2l-cvrp-0/E200-17c.dat", true, "bestImprovement");
    resolverCVRP("NearestNeighbor + Swap", "instancias/2l-cvrp-0/E200-17c.dat", true, "firstImprovement");
    resolverCVRP("NearestNeighbor + Swap", "instancias/2l-cvrp-0/E200-17c.dat", true, "bestImprovement");
    return 0;
}