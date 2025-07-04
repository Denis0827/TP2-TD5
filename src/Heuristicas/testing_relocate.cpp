#include "Route.h"
#include <vector>
#include <iostream>
using namespace std;

// ---------- Auxiliares ----------
NodeRoute* buscarNodo(Route& r, int id) {
    NodeRoute* actual = r.getRaizModify();
    while (actual != nullptr) {
        if (actual->id == id) return actual;
        actual = actual->siguiente;
    }
    return nullptr;
}

Route crearRuta(const vector<int>& ids, int capacidad, const vector<vector<double>>& dist) {
    Route r(capacidad, 0);
    for (int i = 0; i < (int)ids.size(); i++) {
        if (i == 0)
            r.agregarClienteFinal(ids[i], 1, dist[0][ids[i]]);
        else
            r.agregarClienteFinal(ids[i], 1, dist[0][ids[i - 1]], dist[ids[i - 1]][ids[i]], dist[0][ids[i]]);
    }
    return r;
}

// ---------- Test Relocate ----------
int main() {
    vector<vector<double>> dist = {
        {0, 1, 1, 1, 1},
        {1, 0,10, 2, 2},
        {1,10, 0, 1, 2},
        {1, 2, 1, 0, 5},
        {1, 2, 2, 5, 0}
    };

    vector<int> demandas = {0, 1, 1, 1, 1}; // Para ids 0..4

    Route r1 = crearRuta({1, 2}, 10, dist);
    Route r2 = crearRuta({3, 4}, 10, dist);

    cout << "--- ANTES DE RELOCATE ---" << endl;
    cout << "Ruta 1: "; r1.imprimirRuta();
    cout << "Ruta 2: "; r2.imprimirRuta();

    NodeRoute* clienteA = buscarNodo(r1, 1);         // nodo a mover
    NodeRoute* destinoPrev = buscarNodo(r2, 3);      // insertar después de este

    if (clienteA && destinoPrev) {
        bool exito = r1.relocateCliente(r2, clienteA, destinoPrev, demandas, dist);
        cout << "\nResultado relocate: " << (exito ? "éxito" : "falló") << endl;
    } else {
        cout << "Error: No se encontraron los nodos." << endl;
    }

    cout << "\n--- DESPUÉS DE RELOCATE ---" << endl;
    cout << "Ruta 1: "; r1.imprimirRuta();
    cout << "Ruta 2: "; r2.imprimirRuta();

    return 0;
}

int main() {
    testRelocateCliente();
    return 0;
}
