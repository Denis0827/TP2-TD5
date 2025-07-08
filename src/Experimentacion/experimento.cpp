#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <vector>

#include "../Heuristicas/Heuristicas.h"
using namespace std;
using namespace std::chrono;

void ejecutar_algoritmos(const string& instancia, const vector<string>& algoritmos, const double& distancia_optima) {
    size_t slash_pos = instancia.find_last_of('/');
    size_t dot_pos = instancia.find_last_of('.');
    string nombre = instancia.substr(slash_pos + 1, dot_pos - slash_pos - 1);

    // Nombre del archivo CSV
    string csv_path = "src/Experimentacion/csv_exportados/" + nombre + ".csv";
    ofstream csv(csv_path);
    csv << "Algoritmo,Tiempo_us,Distancia,Factibilidad\n";
    csv << "Optimo" << "," << 0 << "," << distancia_optima << "," << 1 << "\n";

    cout << "Instancia: " << nombre << endl;
    cout << "Costo óptimo dado: " << distancia_optima << endl;
    cout << "--------------" << endl;

    for (const string& algoritmo : algoritmos) {
        Heuristicas heuristicas(instancia, nombre);
        auto start = high_resolution_clock::now();

        Solution solucion;

        if (algoritmo == "CW") {
            solucion = heuristicas.clarkeWright(false);
        } else if (algoritmo == "NN") {
            solucion = heuristicas.nearestNeighbor(false);
        } else if (algoritmo == "CW+SW_Best") {
            solucion = heuristicas.clarkeWright();
            heuristicas.swap(solucion, 1, false);
        } else if (algoritmo == "NN+SW_First") {
            solucion = heuristicas.nearestNeighbor();
            solucion = heuristicas.greedyWithLocalSearch("NearestNeighbor", {}, int criterio, bool exportar)
            heuristicas.swap(solucion, 0, false);
        } else if (algoritmo == "NN+SW_Best") {
            solucion = heuristicas.nearestNeighbor();
            heuristicas.swap(solucion, 1, false);
        } else if (algoritmo == "CW+RL_Best") {
            solucion = heuristicas.clarkeWright();
            heuristicas.relocate(solucion, 1, false);
        } else if (algoritmo == "NN+RL_First") {
            solucion = heuristicas.nearestNeighbor();
            heuristicas.relocate(solucion, 0, false);
        } else if (algoritmo == "NN+RL_Best") {
            solucion = heuristicas.nearestNeighbor();
            heuristicas.relocate(solucion, 1, false);
        } else if (algoritmo == "GRASP+SW_Best") {
            solucion = heuristicas.GRASP(1000, "Swap", 1, 3);
        } else if (algoritmo == "GRASP+RL_Best") {
            solucion = heuristicas.GRASP(1000, "Relocate", 1, 3);
        }

        auto end = high_resolution_clock::now();
        auto duracion_us = duration_cast<microseconds>(end - start).count();
        double distancia = solucion.getDistanciaTotal();
        int factible = solucion.esFactible() ? 1 : 0;
        cout << "Algoritmo: " << algoritmo << endl;
        cout << "Tiempo de ejecución: " << duracion_us << endl;
        cout << "Distancia total: " << distancia << endl;
        cout << "Factibilidad: " << factible << endl;
        cout << "--------------" << endl;
        csv << algoritmo << "," << duracion_us << "," << distancia << "," << factible << "\n";
    }
    csv.close();
    cout << "Resultados exportados a: " << csv_path << endl;
}

int main() {
    vector<string> instancias = {
        "instancias/2l-cvrp-0/E045-04f.dat",
        "instancias/2l-cvrp-0/E051-05e.dat",
        "instancias/2l-cvrp-0/E072-04f.dat",
        "instancias/2l-cvrp-0/E076-08u.dat",
        "instancias/2l-cvrp-0/E101-08e.dat",
        "instancias/2l-cvrp-0/E121-07c.dat",
        "instancias/2l-cvrp-0/E135-07f.dat",
        "instancias/2l-cvrp-0/E151-12c.dat",
        "instancias/2l-cvrp-0/E200-17C.dat",
    };

    // resultados sacados de instancias/soluciones
    vector<double> distancias_optimas_dadas = {723.541, 524.611, 241.973, 742.582, 826.136, 1042.115, 1163.599, 1031.073, 1311.348};

    vector<string> algoritmos = {"CW", "NN", "CW+SW_Best", "NN+SW_First", "NN+SW_Best", "CW+RL_Best", 
        "NN+RL_First", "NN+RL_Best", "GRASP+SW_Best", "GRASP+RL_Best"};

    for (int i = 0; i < static_cast<int>(instancias.size()); i++) {
        ejecutar_algoritmos(instancias[i], algoritmos, distancias_optimas_dadas[i]);
    }
    return 0;
}
