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
    } else if (algoritmo == "ClarkeWright + Relocate" && criterio == "firstImprovement") {
        solucion = heuristicas.clarkeWright();
        heuristicas.relocate(solucion, 0, exportar);
    } else if (algoritmo == "ClarkeWright + Relocate" && criterio == "bestImprovement") {
        solucion = heuristicas.clarkeWright();
        heuristicas.relocate(solucion, 1, exportar);
    } else if (algoritmo == "NearestNeighbor + Relocate" && criterio == "firstImprovement") {
        solucion = heuristicas.nearestNeighbor();
        heuristicas.relocate(solucion, 0, exportar);
    } else if (algoritmo == "NearestNeighbor + Relocate" && criterio == "bestImprovement") {
        solucion = heuristicas.nearestNeighbor();
        heuristicas.relocate(solucion, 1, exportar);
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
    //resolverCVRP("ClarkeWright", "instancias/2l-cvrp-0/E016-03m.dat", false);
    resolverCVRP("NearestNeighbor", "instancias/2l-cvrp-0/E045-04f.dat", false);

    /*
    resolverCVRP("ClarkeWright + Swap", "instancias/2l-cvrp-0/E045-04f.dat", false, "firstImprovement");
    resolverCVRP("ClarkeWright + Swap", "instancias/2l-cvrp-0/E045-04f.dat", false, "bestImprovement");
    resolverCVRP("NearestNeighbor + Swap", "instancias/2l-cvrp-0/E045-04f.dat", false, "firstImprovement");
    resolverCVRP("NearestNeighbor + Swap", "instancias/2l-cvrp-0/E045-04f.dat", false, "bestImprovement");
    */

    //resolverCVRP("ClarkeWright + Relocate", "instancias/2l-cvrp-0/E016-03m.dat", false, "firstImprovement");
    //resolverCVRP("ClarkeWright + Relocate", "instancias/2l-cvrp-0/E016-03m.dat", false, "bestImprovement");
    resolverCVRP("NearestNeighbor + Relocate", "instancias/2l-cvrp-0/E045-04f.dat", false, "firstImprovement");
    resolverCVRP("NearestNeighbor + Relocate", "instancias/2l-cvrp-0/E045-04f.dat", false, "bestImprovement");
    return 0;
}