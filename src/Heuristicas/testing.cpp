#include "Heuristicas.h"
#include <chrono>
using namespace std::chrono;

// Imprime todas las rutas de la solución
void imprimirSolucion(const vector<Route>& solucion) {
    if (solucion.empty()) {
        cout << "No existe ninguna solución óptima utilizando este algoritmo" << std::endl;
        return;
    }
    int id = 1;
    double distanciaTotalGlobal = 0.0;
    for (const auto& ruta: solucion) {
        cout << "Ruta " << id++ << ": ";
        const NodoCliente* actual = ruta.getRaizSinMod();
        while (actual != nullptr) {
            cout << actual->id << " ";
            actual = actual->siguiente;
        }
        cout << "| Demanda total: " << ruta.getDemandaTotal()
             << " | Capacidad restante: " << ruta.getCapacidadRestante()
             << " | Distancia total: " << ruta.getDistanciaTotal() << endl;
        distanciaTotalGlobal += ruta.getDistanciaTotal();
    }
    cout << "Distancia total sumada de todas las rutas: " << distanciaTotalGlobal << endl;
}

void imprimirNodos(const std::vector<Node>& nodos) {
    for (const auto& nodo : nodos) {
        std::cout << "ID: " << nodo.id << ", x: " << nodo.x << ", y: " << nodo.y << std::endl;
    }
}

void imprimirMatriz(const vector<vector<int>>& matriz) {
    for (size_t i = 0; i < matriz.size(); ++i) {
        std::cout << "Fila " << i << ": ";
        for (size_t j = 0; j < matriz[i].size(); ++j) {
            std::cout << matriz[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

double recalcularDistanciaTotal(const vector<Route>& solucion, const vector<vector<double>>& distancias) {
    double total = 0.0;
    for (const auto& ruta : solucion) {
        const NodoCliente* actual = ruta.getRaizSinMod();
        while (actual && actual->siguiente) {
            total += distancias[actual->id][actual->siguiente->id];
            actual = actual->siguiente;
        }
    }
    return total;
}

int main() {
    Heuristicas heuristicas("instancias/2l-cvrp-0/E200-17c.dat");
    //int depotId = heuristicas.getInstancia().getDepotId();
    vector<vector<double>> distancias = heuristicas.getInstancia().getDistanceMatrix();
    vector<Node> clientes = heuristicas.getInstancia().getNodes();
    vector<int> demandas = heuristicas.getInstancia().getDemands();
    //int n = heuristicas.getInstancia().getDimension();

    cout << "Solución óptima utilizando ClarkeWright" << endl;
    auto start1 = high_resolution_clock::now();
    vector<Route> solucion = heuristicas.clarkeWright();
    auto end1 = high_resolution_clock::now();
    imprimirSolucion(solucion);
    auto duracion1_us = duration_cast<microseconds>(end1 - start1).count();
    if (duracion1_us < 1000) {
        cout << "Tiempo de ejecución ClarkeWright: " << duracion1_us << " µs" << endl;
    } else {
        cout << "Tiempo de ejecución ClarkeWright: " << duracion1_us / 1000.0 << " ms" << endl;
    }
    cout << "==========" << endl;

    cout << "Solución óptima utilizando VecinosMasCercanos" << endl;
    auto start2 = high_resolution_clock::now();
    vector<Route> solucion2 = heuristicas.vecinoMasCercano();
    auto end2 = high_resolution_clock::now();
    imprimirSolucion(solucion2);
    auto duracion2_us = duration_cast<microseconds>(end2 - start2).count();
    if (duracion2_us < 1000) {
        cout << "Tiempo de ejecución VecinosMasCercanos: " << duracion2_us << " µs" << endl;
    } else {
        cout << "Tiempo de ejecución VecinosMasCercanos: " << duracion2_us / 1000.0 << " ms" << endl;
    }
    cout << "==========" << endl;

    return 0;
}