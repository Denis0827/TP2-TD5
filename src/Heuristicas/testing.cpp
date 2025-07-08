#include "Heuristicas.h"
#include <chrono>
using namespace std::chrono;

void resolverCVRP(string algoritmo, string instancia, bool exportar = false, string criterio = "", int rcl_size = 3, 
    string operador_local="Relocate", int numero_iteraciones = 1000) {

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
    } else if (algoritmo == "NearestNeighborRandomized") {
        solucion = heuristicas.nearestNeighborRandomized(0, exportar, rcl_size);
    } else if (algoritmo == "GRASP" && criterio == "firstImprovement") {
        solucion = heuristicas.GRASP(numero_iteraciones, operador_local, 0, rcl_size);
    } else if (algoritmo == "GRASP" && criterio == "bestImprovement") {
        solucion = heuristicas.GRASP(numero_iteraciones, operador_local, 1, rcl_size);
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
    resolverCVRP("ClarkeWright", "instancias/2l-cvrp-0/E045-04f.dat", false);
    resolverCVRP("NearestNeighborRandomized", "instancias/2l-cvrp-0/E045-04f.dat", false);

    /*
    resolverCVRP("ClarkeWright + Swap", "instancias/2l-cvrp-0/E045-04f.dat", false, "firstImprovement");
    resolverCVRP("ClarkeWright + Swap", "instancias/2l-cvrp-0/E045-04f.dat", false, "bestImprovement");
    resolverCVRP("NearestNeighbor + Swap", "instancias/2l-cvrp-0/E045-04f.dat", false, "firstImprovement");
    resolverCVRP("NearestNeighbor + Swap", "instancias/2l-cvrp-0/E045-04f.dat", false, "bestImprovement");
    */

    /*
    resolverCVRP("ClarkeWright + Relocate", "instancias/2l-cvrp-0/E101D11r.dat", true, "firstImprovement");
    resolverCVRP("ClarkeWright + Relocate", "instancias/2l-cvrp-0/E101D11r.dat", true, "bestImprovement");
    resolverCVRP("NearestNeighbor + Relocate", "instancias/2l-cvrp-0/E101D11r.dat", true, "firstImprovement");
    resolverCVRP("NearestNeighbor + Relocate", "instancias/2l-cvrp-0/E101D11r.dat", true, "bestImprovement");
    */

    resolverCVRP("GRASP", "instancias/2l-cvrp-0/E045-04f.dat", false, "bestImprovement", 3, "Relocate", 1000);
    return 0;
}