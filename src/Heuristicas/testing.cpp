#include "Heuristicas.h"

// Imprime todas las rutas de la solución
void imprimirSolucion(const vector<Route>& solucion) {
    if (solucion.empty()) {
        cout << "No existe ninguna solución óptima utilizando este algoritmo" << std::endl;
        return;
    }
    int id = 1;
    for (const auto& ruta: solucion) {
        cout << "Ruta " << id++ << ": ";
        NodoCliente* actual = ruta.raiz;
        while (actual != nullptr) {
            cout << actual->id << " ";
            actual = actual->siguiente;
        }
        cout << "| Demanda total: " << ruta.demandaTotal
             << " | Capacidad restante: " << ruta.capacidadRestante
             << " | Distancia total: " << ruta.distanciaTotal << endl;
    }
}


void imprimirSavings(const std::vector<Saving>& savings) {
    for (const auto& s : savings) {
        std::cout << "i: " << s.i << ", j: " << s.j << ", saving: " << s.saving << std::endl;
    }
}

void imprimirNodos(const std::vector<Node>& nodos) {
    for (const auto& nodo : nodos) {
        std::cout << "ID: " << nodo.id << ", x: " << nodo.x << ", y: " << nodo.y << std::endl;
    }
}

// Imprime la secuencia de nodos de una ruta enlazada
void imprimirRuta(const Route& ruta) {
    std::cout << "Ruta: ";
    NodoCliente* actual = ruta.raiz;
    while (actual != nullptr) {
        std::cout << actual->id << " ";
        actual = actual->siguiente;
    }
    std::cout << "| Demanda total: " << ruta.demandaTotal << std::endl;
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


int main() {
    Heuristicas heuristicas("instancias/2l-cvrp-0/E016-03m.dat");
    int depotId = heuristicas.getInstancia().getDepotId();
    vector<vector<double>> distancias = heuristicas.getInstancia().getDistanceMatrix();
    vector<Node> clientes = heuristicas.getInstancia().getNodes();
    vector<int> demandas = heuristicas.getInstancia().getDemands();
    int n = heuristicas.getInstancia().getDimension();

    vector<Route> solucion = heuristicas.clarkeWright();
    vector<Route> solucion2 = heuristicas.vecinoMasCercano();

    //vector<vector<int>> matriz = ordenarPorDistancias(distancias);

    //imprimirMatriz(matriz);
    imprimirSolucion(solucion); // Imprime la solución
    cout << "==========" << endl;
    imprimirSolucion(solucion2);

    return 0;
}