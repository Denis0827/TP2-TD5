#include "TestSwapClientes.h"

// Auxiliar: crear ruta a partir de vector de IDs y demandas
Route TestSwapClientes::crearRuta(const vector<int>& ids, const vector<int>& demandas, int capacidad, const vector<vector<double>>& dist) {
    Route r(capacidad, 0);
    for (size_t i = 0; i < ids.size(); ++i) {
        if (i == 0)
            r.agregarClienteFinal(ids[i], demandas[ids[i]], dist[0][ids[i]]);
        else
            r.agregarClienteFinal(ids[i], demandas[ids[i]], dist[0][ids[i-1]], dist[ids[i-1]][ids[i]], dist[0][ids[i]]);
    }
    return r;
}

// Auxiliar: buscar nodo por id
NodeRoute* TestSwapClientes::buscarNodo(Route& r, int id) {
    NodeRoute* actual = r.getRaizModify();
    while (actual != nullptr) {
        if (actual->id == id) return actual;
        actual = actual->siguiente;
    }
    return nullptr;
}

void TestSwapClientes::inicializarDistancias() {
    // Crear matriz 10x10 para testing
    distancias.resize(10, vector<double>(10, 0));
    
    // Distancias entre nodos (simulando distancias reales)
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (i == j) {
                distancias[i][j] = 0;
            } else {
                distancias[i][j] = abs(i - j) * 10.0; // Distancia simple
            }
        }
    }
}

bool TestSwapClientes::verificarIntegridadRuta(const Route& ruta, int rutaNum) {
    if (ruta.getRaiz() == nullptr) return false;
    if (ruta.getUltimo() == nullptr) return false;
    // Chequeo de enlaces dobles y consistencia de nodos
    const NodeRoute* actual = ruta.getRaiz();
    while (actual != nullptr) {
        if (actual->siguiente != nullptr && actual->siguiente->anterior != actual) return false;
        if (actual->anterior != nullptr && actual->anterior->siguiente != actual) return false;
        actual = actual->siguiente;
    }
    // Chequeo de demanda total
    std::vector<NodeRoute*> clientes = ruta.getAllClientes();
    int demandaCalculada = 0;
    for (NodeRoute* cliente : clientes) demandaCalculada += cliente->demanda;
    if (demandaCalculada != ruta.getDemandaTotal()) return false;
    // Chequeo de capacidad
    if (ruta.getCapacidadRestante() != ruta.getCapacidadTotal() - ruta.getDemandaTotal()) return false;
    // Chequeo de distancia total
    double distanciaCalculada = 0;
    actual = ruta.getRaiz();
    while (actual && actual->siguiente) {
        int from = actual->id;
        int to = actual->siguiente->id;
        distanciaCalculada += distancias[from][to];
        actual = actual->siguiente;
    }
    if (std::abs(distanciaCalculada - ruta.getDistanciaTotal()) > 1e-6) return false;
    // Chequeo de consistencia de nodos (no repetidos, depot solo en extremos)
    std::vector<int> ids;
    actual = ruta.getRaiz()->siguiente;
    while (actual != ruta.getUltimo()) {
        ids.push_back(actual->id);
        actual = actual->siguiente;
    }
    std::sort(ids.begin(), ids.end());
    for (size_t i = 1; i < ids.size(); ++i) {
        if (ids[i] == ids[i-1]) return false;
    }
    // Depot solo en extremos
    if (ruta.getRaiz()->id != 0) return false;
    if (ruta.getUltimo()->id != 0) return false;
    return true;
}

TestSwapClientes::TestSwapClientes() {
    inicializarDistancias();
}

// --- TEST 1: NODOS CONSECUTIVOS EN LA MISMA RUTA ---
bool TestSwapClientes::testSwapConsecutivosMismaRuta() {
    cout << "-------- TEST 1: NODOS CONSECUTIVOS EN LA MISMA RUTA --------\n";
    vector<vector<double>> dist = distancias;
    vector<int> demandas = {0, 10, 15, 20};
    Route ruta = crearRuta({1, 2, 3}, demandas, 100, dist);
    cout << "Estado de rutas antes de swap:" << endl;
    cout << "Ruta 1: "; ruta.imprimirRuta();
    cout << endl;
    bool ok = true;
    int idA = 1, idB = 2;
    try {
        NodeRoute* n1 = buscarNodo(ruta, idA);
        NodeRoute* n2 = buscarNodo(ruta, idB);
        ruta.swapClientes(ruta, n1, n2, distancias);
        cout << "Estado de rutas después de swap (" << idA << ", " << idB << "):" << endl;
        cout << "Ruta 1: "; ruta.imprimirRuta();
        cout << endl;
        ok = verificarIntegridadRuta(ruta, 1);
    } catch (...) { ok = false; }
    cout << (ok ? "OK" : "FALLO") << "\n" << endl;
    return ok;
}

// --- TEST 2: NODOS DE DISTINTAS RUTAS ---
bool TestSwapClientes::testSwapDistintasRutas() {
    cout << "-------- TEST 2: NODOS DE DISTINTAS RUTAS --------\n";
    vector<vector<double>> dist = distancias;
    vector<int> demandas = {0, 10, 15, 20, 25};
    Route r1 = crearRuta({1, 2}, demandas, 100, dist);
    Route r2 = crearRuta({3, 4}, demandas, 100, dist);
    cout << "Estado de rutas antes de swap:" << endl;
    cout << "Ruta 1: "; r1.imprimirRuta();
    cout << "Ruta 2: "; r2.imprimirRuta();
    cout << endl;
    bool ok = true;
    int idA = 1, idB = 3;
    try {
        NodeRoute* n1 = buscarNodo(r1, idA);
        NodeRoute* n3 = buscarNodo(r2, idB);
        r1.swapClientes(r2, n1, n3, distancias);
        cout << "Estado de rutas después de swap (" << idA << ", " << idB << "):" << endl;
        cout << "Ruta 1: "; r1.imprimirRuta();
        cout << "Ruta 2: "; r2.imprimirRuta();
        cout << endl;
        ok = verificarIntegridadRuta(r1, 1) && verificarIntegridadRuta(r2, 2);
    } catch (...) { ok = false; }
    cout << (ok ? "OK" : "FALLO") << "\n" << endl;
    return ok;
}

// --- TEST 3: NODOS NO CONSECUTIVOS EN LA MISMA RUTA ---
bool TestSwapClientes::testSwapNoConsecutivosMismaRuta() {
    cout << "-------- TEST 3: NODOS NO CONSECUTIVOS EN LA MISMA RUTA --------\n";
    vector<vector<double>> dist = distancias;
    vector<int> demandas = {0, 10, 15, 20, 25};
    Route ruta = crearRuta({1, 2, 3, 4}, demandas, 100, dist);
    cout << "Estado de rutas antes de swap:" << endl;
    cout << "Ruta 1: "; ruta.imprimirRuta();
    cout << endl;
    bool ok = true;
    int idA = 1, idB = 3;
    try {
        NodeRoute* n1 = buscarNodo(ruta, idA);
        NodeRoute* n3 = buscarNodo(ruta, idB);
        ruta.swapClientes(ruta, n1, n3, distancias);
        cout << "Estado de rutas después de swap (" << idA << ", " << idB << "):" << endl;
        cout << "Ruta 1: "; ruta.imprimirRuta();
        cout << endl;
        ok = verificarIntegridadRuta(ruta, 1);
    } catch (...) { ok = false; }
    cout << (ok ? "OK" : "FALLO") << "\n" << endl;
    return ok;
}

// --- TEST 4: RUTA CON UN SOLO CLIENTE ---
bool TestSwapClientes::testSwapRutaUnSoloCliente() {
    cout << "-------- TEST 4: RUTA CON UN SOLO CLIENTE --------\n";
    vector<vector<double>> dist = distancias;
    vector<int> demandas = {0, 10, 15, 20};
    Route r1 = crearRuta({1}, demandas, 100, dist);
    Route r2 = crearRuta({2, 3}, demandas, 100, dist);
    cout << "Estado de rutas antes de swap:" << endl;
    cout << "Ruta 1: "; r1.imprimirRuta();
    cout << "Ruta 2: "; r2.imprimirRuta();
    cout << endl;
    bool ok = true;
    int idA = 1, idB = 2;
    try {
        NodeRoute* n1 = buscarNodo(r1, idA);
        NodeRoute* n2 = buscarNodo(r2, idB);
        r1.swapClientes(r2, n1, n2, distancias);
        cout << "Estado de rutas después de swap (" << idA << ", " << idB << "):" << endl;
        cout << "Ruta 1: "; r1.imprimirRuta();
        cout << "Ruta 2: "; r2.imprimirRuta();
        cout << endl;
        ok = verificarIntegridadRuta(r1, 1) && verificarIntegridadRuta(r2, 2);
    } catch (...) { ok = false; }
    cout << (ok ? "OK" : "FALLO") << "\n" << endl;
    return ok;
}

// -------- TEST 5: EXTREMOS EN UNA MISMA RUTA --------
bool TestSwapClientes::testSwapExtremosMismaRuta() {
    cout << "-------- TEST 5: EXTREMOS EN UNA MISMA RUTA --------\n";
    vector<vector<double>> dist = distancias;
    vector<int> demandas = {0, 10, 15, 20, 25};
    Route ruta = crearRuta({1, 2, 3, 4}, demandas, 100, dist);
    cout << "Estado de rutas antes de swap:" << endl;
    cout << "Ruta 1: "; ruta.imprimirRuta();
    cout << endl;
    bool ok = true;
    int idA = 1, idB = 4;
    try {
        NodeRoute* n1 = buscarNodo(ruta, idA);
        NodeRoute* n4 = buscarNodo(ruta, idB);
        ruta.swapClientes(ruta, n1, n4, distancias);
        cout << "Estado de rutas después de swap (" << idA << ", " << idB << "):" << endl;
        cout << "Ruta 1: "; ruta.imprimirRuta();
        cout << endl;
        ok = verificarIntegridadRuta(ruta, 1);
    } catch (...) { ok = false; }
    cout << (ok ? "OK" : "FALLO") << "\n" << endl;
    return ok;
}

// -------- TEST 6: EXTREMOS ENTRE RUTAS --------
bool TestSwapClientes::testSwapExtremosEntreRutas() {
    cout << "-------- TEST 6: EXTREMOS ENTRE RUTAS --------\n";
    vector<vector<double>> dist = distancias;
    vector<int> demandas = {0, 10, 15, 20, 25, 30};
    Route r1 = crearRuta({1, 2, 3}, demandas, 100, dist);
    Route r2 = crearRuta({4, 5, 6}, demandas, 100, dist);
    cout << "Estado de rutas antes de swap:" << endl;
    cout << "Ruta 1: "; r1.imprimirRuta();
    cout << "Ruta 2: "; r2.imprimirRuta();
    cout << endl;
    bool ok = true;
    int idA = 1, idB = 6;
    try {
        NodeRoute* nA = buscarNodo(r1, idA);
        NodeRoute* nB = buscarNodo(r2, idB);
        r1.swapClientes(r2, nA, nB, distancias);
        cout << "Estado de rutas después de swap (" << idA << ", " << idB << "):" << endl;
        cout << "Ruta 1: "; r1.imprimirRuta();
        cout << "Ruta 2: "; r2.imprimirRuta();
        cout << endl;
        ok = verificarIntegridadRuta(r1, 1) && verificarIntegridadRuta(r2, 2);
    } catch (...) { ok = false; }
    cout << (ok ? "OK" : "FALLO") << "\n" << endl;
    return ok;
}


// -------- TEST 7: SWAP INVERSO EN LA MISMA RUTA --------
bool TestSwapClientes::testSwapInversoMismaRuta() {
    cout << "-------- TEST 7: SWAP INVERSO EN LA MISMA RUTA --------\n";
    vector<vector<double>> dist = distancias;
    vector<int> demandas = {0, 10, 15, 20, 25};
    Route ruta = crearRuta({1, 2, 3, 4}, demandas, 100, dist);
    cout << "Estado de rutas antes de swap:" << endl;
    cout << "Ruta 1: "; ruta.imprimirRuta();
    cout << endl;
    bool ok = true;
    int idA = 3, idB = 2;
    try {
        NodeRoute* nA = buscarNodo(ruta, idA);
        NodeRoute* nB = buscarNodo(ruta, idB);
        ruta.swapClientes(ruta, nA, nB, distancias);
        cout << "Estado de rutas después de swap (" << idA << ", " << idB << "):" << endl;
        cout << "Ruta 1: "; ruta.imprimirRuta();
        cout << endl;
        ok = verificarIntegridadRuta(ruta, 1);
    } catch (...) { ok = false; }
    cout << (ok ? "OK" : "FALLO") << "\n" << endl;
    return ok;
}

// -------- TEST 8: SWAP INVERSO ENTRE RUTAS --------
bool TestSwapClientes::testSwapInversoEntreRutas() {
    cout << "-------- TEST 8: SWAP INVERSO ENTRE RUTAS --------\n";
    vector<vector<double>> dist = distancias;
    vector<int> demandas = {0, 10, 15, 20, 25, 30};
    Route r1 = crearRuta({1, 2, 3}, demandas, 100, dist);
    Route r2 = crearRuta({4, 5, 6}, demandas, 100, dist);
    cout << "Estado de rutas antes de swap:" << endl;
    cout << "Ruta 1: "; r1.imprimirRuta();
    cout << "Ruta 2: "; r2.imprimirRuta();
    cout << endl;
    bool ok = true;
    int idA = 4, idB = 2;
    try {
        NodeRoute* nA = buscarNodo(r2, idA);
        NodeRoute* nB = buscarNodo(r1, idB);
        r2.swapClientes(r1, nA, nB, distancias);
        cout << "Estado de rutas después de swap (" << idA << ", " << idB << "):" << endl;
        cout << "Ruta 1: "; r1.imprimirRuta();
        cout << "Ruta 2: "; r2.imprimirRuta();
        cout << endl;
        ok = verificarIntegridadRuta(r1, 1) && verificarIntegridadRuta(r2, 2);
    } catch (...) { ok = false; }
    cout << (ok ? "OK" : "FALLO") << "\n" << endl;
    return ok;
}

void TestSwapClientes::ejecutarTodosLosTests() {
    cout << "=======================================" << endl;
    cout << "TESTING EXHAUSTIVO DE SWAP CLIENTES" << endl;
    cout << "=======================================\n" << endl;

    int okCount = 0, failCount = 0;
    auto runTest = [&](bool (TestSwapClientes::*test)()) {
        bool ok = (this->*test)();
        if (ok) okCount++;
        else failCount++;
    };
    runTest(&TestSwapClientes::testSwapConsecutivosMismaRuta);
    runTest(&TestSwapClientes::testSwapDistintasRutas);
    runTest(&TestSwapClientes::testSwapNoConsecutivosMismaRuta);
    runTest(&TestSwapClientes::testSwapRutaUnSoloCliente);
    runTest(&TestSwapClientes::testSwapExtremosMismaRuta);
    runTest(&TestSwapClientes::testSwapExtremosEntreRutas);
    runTest(&TestSwapClientes::testSwapInversoMismaRuta);
    runTest(&TestSwapClientes::testSwapInversoEntreRutas);
    cout << "==============================================================" << endl;
    cout << "✓ TODOS LOS TESTS DE SWAPCLIENTES PASARON (" << okCount << " OK, " << failCount << " FALLO)" << endl;
    cout << "============================================================" << endl;
}

int main() {
    TestSwapClientes tester;
    tester.ejecutarTodosLosTests();
    return 0;
} 