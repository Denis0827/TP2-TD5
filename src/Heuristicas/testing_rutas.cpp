#include "Route.h"
#include <vector>
#include <iostream>
using namespace std;

Route crearRuta(const vector<int>& ids, int capacidad) {
    Route r(capacidad);
    r.agregarDepot(0);
    for (int id : ids) {
        r.agregarClienteFinal(id, 1, 1.0, 1.0, 1.0);
    }
    return r;
}

NodeRoute* buscarNodo(Route& r, int id) {
    NodeRoute* actual = r.getRaiz();
    while (actual) {
        if (actual->id == id) return actual;
        actual = actual->siguiente;
    }
    return nullptr;
}

int main() {
    // Matriz de distancias para el ejemplo
    vector<vector<double>> dist = {
        {0, 1, 1, 1, 1},
        {1, 0,10, 2, 2},
        {1,10, 0, 1, 2},
        {1, 2, 1, 0, 1},
        {1, 2, 2, 1, 0}
    };

    // Ruta: 0 → 1 → 2 → 3 → 4
    Route r1 = crearRuta({1,2,3,4}, 10);
    NodeRoute* n2 = buscarNodo(r1, 2);
    NodeRoute* n3 = buscarNodo(r1, 3);

    cout << "Antes swap consecutivos: ";
    r1.imprimirRuta();
    r1.swapNodes(r1, n2, n3, dist);
    cout << "Despues swap consecutivos: ";
    r1.imprimirRuta();

    return 0;
}