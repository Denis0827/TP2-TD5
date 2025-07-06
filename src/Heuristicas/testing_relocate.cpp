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

int main() {
    vector<vector<double>> dist = {
        {0, 1, 1, 1, 1},
        {1, 0,10, 2, 2},
        {1,10, 0, 1, 2},
        {1, 2, 1, 0, 5},
        {1, 2, 2, 5, 0}
    };

    vector<int> demandas = {0, 1, 1, 1, 1};

    // Test 1: Entre rutas
    Route r1 = crearRuta({1, 2}, 10, dist);
    Route r2 = crearRuta({3, 4}, 10, dist);
    cout << "--- TEST 1: ENTRE RUTAS ---\n"; 
    r1.imprimirRuta(); 
    r2.imprimirRuta();
    auto n1 = buscarNodo(r1, 1); 
    auto d1 = buscarNodo(r2, 3);
    if (n1 && d1) {
        cout << (r1.relocateCliente(r2, n1, d1, demandas, dist) ? "OK" : "FALLO") << "\n"; 
        r1.imprimirRuta(); 
        r2.imprimirRuta();
    }

    // Test 2: Misma ruta
    Route r3 = crearRuta({1, 2, 3, 4}, 10, dist);
    cout << "\n--- TEST 2: MISMA RUTA ---\n"; 
    r3.imprimirRuta();
    auto n2 = buscarNodo(r3, 2); 
    auto d2 = buscarNodo(r3, 3);
    if (n2 && d2) {
        cout << (r3.relocateCliente(r3, n2, d2, demandas, dist) ? "OK" : "FALLO") << "\n"; 
        r3.imprimirRuta();
    }

    // Test 3: j == p
    Route r4 = crearRuta({1, 2, 3, 4}, 10, dist);
    cout << "\n--- TEST 3: j == p ---\n"; 
    r4.imprimirRuta();
    auto n3 = buscarNodo(r4, 2); 
    auto d3 = buscarNodo(r4, 1);
    if (n3 && d3) {
        cout << (r4.relocateCliente(r4, n3, d3, demandas, dist) ? "ERROR" : "OK") << "\n"; 
        r4.imprimirRuta();
    }

    // Test 4: k == q
    Route r5 = crearRuta({1, 2, 3, 4}, 10, dist);
    cout << "\n--- TEST 4: k == q ---\n"; 
    r5.imprimirRuta();
    auto n4 = buscarNodo(r5, 2); 
    auto d4 = buscarNodo(r5, 3);
    if (n4 && d4) {
        cout << (r5.relocateCliente(r5, n4, d4, demandas, dist) ? "OK" : "FALLO") << "\n"; 
        r5.imprimirRuta();
    }

    // Test 5: j == q
    Route r6 = crearRuta({1, 2, 3, 4}, 10, dist);
    cout << "\n--- TEST 5: j == q ---\n"; 
    r6.imprimirRuta();
    auto n5 = buscarNodo(r6, 2); 
    auto d5 = buscarNodo(r6, 3); // j == q
    if (n5 && d5) {
        cout << (r6.relocateCliente(r6, n5, d5, demandas, dist) ? "OK" : "FALLO") << "\n"; 
        r6.imprimirRuta();
    }

    // Test 6: No mejora costo
    Route r7 = crearRuta({1, 2, 3, 4}, 10, dist);
    cout << "\n--- TEST 6: NO MEJORA COSTO ---\n"; 
    r7.imprimirRuta();
    auto n6 = buscarNodo(r7, 2); 
    auto d6 = buscarNodo(r7, 1);
    if (n6 && d6) {
        cout << (r7.relocateCliente(r7, n6, d6, demandas, dist) ? "ERROR" : "OK") << "\n"; 
        r7.imprimirRuta();
    }

    // Test 7: Rompe capacidad
    Route r8a = crearRuta({1, 2}, 2, dist);
    Route r8b = crearRuta({3, 4}, 2, dist);
    cout << "\n--- TEST 7: ROMPE CAPACIDAD ---\n"; 
    r8a.imprimirRuta(); 
    r8b.imprimirRuta();
    auto n7 = buscarNodo(r8a, 1); 
    auto d7 = buscarNodo(r8b, 3);
    if (n7 && d7) {
        cout << (r8a.relocateCliente(r8b, n7, d7, demandas, dist) ? "ERROR" : "OK") << "\n"; 
        r8a.imprimirRuta(); 
        r8b.imprimirRuta();
    }

    // Test 8: Insertar antes del depósito (último cliente)
    Route r9 = crearRuta({1, 2, 3, 4}, 10, dist);
    cout << "\n--- TEST 8: INSERTAR ANTES DEL DEPOT FINAL ---\n"; 
    r9.imprimirRuta();
    auto n8 = buscarNodo(r9, 2); 
    auto d8 = r9.getUltimoModify()->anterior;
    if (n8 && d8) {
        cout << (r9.relocateCliente(r9, n8, d8, demandas, dist) ? "OK" : "FALLO") << "\n"; 
        r9.imprimirRuta();
    }

    // Test 9: Relocate a sí mismo
    Route r10 = crearRuta({1, 2, 3, 4}, 10, dist);
    cout << "\n--- TEST 9: RELOCATE A SÍ MISMO ---\n"; 
    r10.imprimirRuta();
    auto n9 = buscarNodo(r10, 2); 
    if (n9) {
        cout << (r10.relocateCliente(r10, n9, n9, demandas, dist) ? "ERROR" : "OK") << "\n"; 
        r10.imprimirRuta();
    }

    return 0;
}




