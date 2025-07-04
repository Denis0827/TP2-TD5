#include "Route.h"
#include <vector>
#include <iostream>
using namespace std;

// Busca el puntero al nodo con id dado en la ruta
NodeRoute* buscarNodo(Route& r, int id) {
    NodeRoute* actual = r.getRaizModify();
    while (actual != nullptr) {
        if (actual->id == id) return actual;
        actual = actual->siguiente;
    }
    return nullptr;
}

// Crea una ruta con los ids dados, capacidad y matriz de distancias
Route crearRuta(const vector<int>& ids, int capacidad, const vector<vector<double>>& dist) {
    Route r(capacidad, 0);
    for (int i = 0; i < ids.size(); i++) {
        if (i == 0)
            r.agregarClienteFinal(ids[i], 1, dist[0][ids[i]]);
        else
            r.agregarClienteFinal(ids[i], 1, dist[0][ids[i-1]], dist[ids[i-1]][ids[i]], dist[0][ids[i]]);
    }
    return r;
}

int main() {
    // Matriz de distancias para el ejemplo
    vector<vector<double>> dist = {
        {0, 1, 1, 1, 1},
        {1, 0,10, 2, 2},
        {1,10, 0, 1, 2},
        {1, 2, 1, 0, 5},
        {1, 2, 2, 5, 0}
    };

    // --- Test 1: Swap entre rutas ---
    Route r1 = crearRuta({1,2}, 10, dist);
    Route r2 = crearRuta({3,4}, 10, dist);

    cout << "Antes swap entre rutas:" << endl;
    cout << "Ruta 1: "; r1.imprimirRuta();
    cout << "Ruta 2: "; r2.imprimirRuta();

    NodeRoute* n2 = buscarNodo(r1, 2);
    NodeRoute* n3 = buscarNodo(r2, 3);

    if (n2 && n3) {
        r1.swapClientes(r2, n2, n3, dist);
    } else {
        cout << "Error: No se encontraron los nodos para swap entre rutas." << endl;
    }

    cout << "Despues swap entre rutas:" << endl;
    cout << "Ruta 1: "; r1.imprimirRuta();
    cout << "Ruta 2: "; r2.imprimirRuta();

    // --- Test 2: Swap consecutivos en la misma ruta ---
    Route r3 = crearRuta({1,2,3,4}, 10, dist);
    NodeRoute* n1 = buscarNodo(r3, 1);
    NodeRoute* n2b = buscarNodo(r3, 2);

    cout << "\nAntes swap consecutivos en la misma ruta: ";
    r3.imprimirRuta();
    if (n1 && n2b) {
        r3.swapClientes(r3, n1, n2b, dist);
    } else {
        cout << "Error: No se encontraron los nodos para swap consecutivo." << endl;
    }
    cout << "Despues swap consecutivos en la misma ruta: ";
    r3.imprimirRuta();

    return 0;
}