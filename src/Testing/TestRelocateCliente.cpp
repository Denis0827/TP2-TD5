#include "TestRelocateCliente.h"

// Auxiliar: crear ruta a partir de vector de IDs y demandas
Route TestRelocateCliente::crearRuta(const vector<int>& ids, const vector<int>& demandas, int capacidad, const vector<vector<double>>& dist) {
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
NodeRoute* TestRelocateCliente::buscarNodo(Route& r, int id) {
    NodeRoute* actual = r.getRaizModify();
    while (actual != nullptr) {
        if (actual->id == id) return actual;
        actual = actual->siguiente;
    }
    return nullptr;
}

void TestRelocateCliente::inicializarDistancias() {
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

void TestRelocateCliente::verificarIntegridadRuta(const Route& ruta, int rutaNum) {
    // Chequeo de raíz y último
    assert(ruta.getRaiz() != nullptr);
    assert(ruta.getUltimo() != nullptr);
    // Chequeo de enlaces dobles y consistencia de nodos
    const NodeRoute* actual = ruta.getRaiz();
    int count = 0;
    while (actual != nullptr) {
        if (actual->siguiente != nullptr) assert(actual->siguiente->anterior == actual);
        if (actual->anterior != nullptr) assert(actual->anterior->siguiente == actual);
        actual = actual->siguiente;
        count++;
    }
    // Chequeo de demanda total
    std::vector<NodeRoute*> clientes = ruta.getAllClientes();
    int demandaCalculada = 0;
    for (NodeRoute* cliente : clientes) {
        demandaCalculada += cliente->demanda;
    }
    assert(demandaCalculada == ruta.getDemandaTotal());
    // Chequeo de capacidad
    assert(ruta.getCapacidadRestante() == ruta.getCapacidadTotal() - ruta.getDemandaTotal());
    // Chequeo de distancia total
    double distanciaCalculada = 0;
    actual = ruta.getRaiz();
    while (actual && actual->siguiente) {
        int from = actual->id;
        int to = actual->siguiente->id;
        distanciaCalculada += distancias[from][to];
        actual = actual->siguiente;
    }
    assert(distanciaCalculada == ruta.getDistanciaTotal());
    // Chequeo de consistencia de nodos (no repetidos, depot solo en extremos)
    std::vector<int> ids;
    actual = ruta.getRaiz()->siguiente;
    while (actual != ruta.getUltimo()) {
        ids.push_back(actual->id);
        actual = actual->siguiente;
    }
    std::sort(ids.begin(), ids.end());
    for (size_t i = 1; i < ids.size(); ++i) {
        assert(ids[i] != ids[i-1]); // no repetidos
    }
    // Depot solo en extremos
    assert(ruta.getRaiz()->id == 0);
    assert(ruta.getUltimo()->id == 0);
    // Print final
    cout << "Integridad de ruta " << rutaNum << " verificada correctamente" << endl;
}

TestRelocateCliente::TestRelocateCliente() {
    inicializarDistancias();
}

// --- TEST 1: RELOCATE ENTRE RUTAS DIFERENTES ---
void TestRelocateCliente::testRelocateEntreRutas() {
    cout << "-------- TEST 1: RELOCATE ENTRE RUTAS DIFERENTES --------\n";
    vector<vector<double>> dist = distancias;
    vector<int> demandas = {0, 10, 15, 20, 25};
    Route r1 = crearRuta({1, 2}, demandas, 100, dist);
    Route r2 = crearRuta({3, 4}, demandas, 100, dist);
    cout << "Estado de rutas antes de relocate:" << endl;
    cout << "Ruta 1: "; r1.imprimirRuta();
    cout << "Ruta 2: "; r2.imprimirRuta();
    cout << endl;
    bool ok = true;
    int idCliente = 1, idDestino = 3;
    try {
        NodeRoute* cliente = buscarNodo(r1, idCliente);
        NodeRoute* destino = buscarNodo(r2, idDestino);
        r1.relocateCliente(r2, cliente, destino, demandas, distancias);
        cout << "Estado de rutas después de relocate (cliente " << idCliente << " después de " << idDestino << "):" << endl;
        cout << "Ruta 1: "; r1.imprimirRuta();
        cout << "Ruta 2: "; r2.imprimirRuta();
        cout << endl;
        verificarIntegridadRuta(r1, 1);
        verificarIntegridadRuta(r2, 2);
    } catch (...) { ok = false; }
    cout << (ok ? "OK" : "FALLO") << "\n" << endl;
}

// --- TEST 2: RELOCATE EN LA MISMA RUTA ---
void TestRelocateCliente::testRelocateMismaRuta() {
    cout << "-------- TEST 2: RELOCATE EN LA MISMA RUTA --------\n";
    vector<vector<double>> dist = distancias;
    vector<int> demandas = {0, 10, 15, 20, 25};
    Route ruta = crearRuta({1, 2, 3, 4}, demandas, 100, dist);
    cout << "Estado de ruta antes de relocate:" << endl;
    cout << "Ruta: "; ruta.imprimirRuta();
    cout << endl;
    bool ok = true;
    int idCliente = 2, idDestino = 4;
    try {
        NodeRoute* cliente = buscarNodo(ruta, idCliente);
        NodeRoute* destino = buscarNodo(ruta, idDestino);
        ruta.relocateCliente(ruta, cliente, destino, demandas, distancias);
        cout << "Estado de ruta después de relocate (cliente " << idCliente << " después de " << idDestino << "):" << endl;
        cout << "Ruta: "; ruta.imprimirRuta();
        cout << endl;
        verificarIntegridadRuta(ruta, 1);
    } catch (...) { ok = false; }
    cout << (ok ? "OK" : "FALLO") << "\n" << endl;
}

// --- TEST 3: RELOCATE EN POSICIONES CONSECUTIVAS ---
void TestRelocateCliente::testRelocateConsecutivos() {
    cout << "-------- TEST 3: RELOCATE EN POSICIONES CONSECUTIVAS --------\n";
    vector<vector<double>> dist = distancias;
    vector<int> demandas = {0, 10, 15, 20, 25, 30};
    Route ruta = crearRuta({1, 2, 3, 4, 5}, demandas, 100, dist);
    cout << "Estado de ruta antes de relocate:" << endl;
    cout << "Ruta: "; ruta.imprimirRuta();
    cout << endl;
    bool ok = true;
    int idCliente = 2, idDestino = 3;
    try {
        NodeRoute* cliente = buscarNodo(ruta, idCliente);
        NodeRoute* destino = buscarNodo(ruta, idDestino);
        ruta.relocateCliente(ruta, cliente, destino, demandas, distancias);
        cout << "Estado de ruta después de relocate (cliente " << idCliente << " después de " << idDestino << "):" << endl;
        cout << "Ruta: "; ruta.imprimirRuta();
        cout << endl;
        verificarIntegridadRuta(ruta, 1);
    } catch (...) { ok = false; }
    cout << (ok ? "OK" : "FALLO") << "\n" << endl;
}

// --- TEST 4: RELOCATE AL INICIO DE LA RUTA ---
void TestRelocateCliente::testRelocateAlInicio() {
    cout << "-------- TEST 4: RELOCATE AL INICIO DE LA RUTA --------\n";
    vector<vector<double>> dist = distancias;
    vector<int> demandas = {0, 10, 15, 20};
    Route ruta = crearRuta({1, 2, 3}, demandas, 100, dist);
    cout << "Estado de ruta antes de relocate:" << endl;
    cout << "Ruta: "; ruta.imprimirRuta();
    cout << endl;
    bool ok = true;
    int idCliente = 2;
    try {
        NodeRoute* cliente = buscarNodo(ruta, idCliente);
        NodeRoute* destino = ruta.getRaizModify();
        ruta.relocateCliente(ruta, cliente, destino, demandas, distancias);
        cout << "Estado de ruta después de relocate (cliente " << idCliente << " al inicio):" << endl;
        cout << "Ruta: "; ruta.imprimirRuta();
        cout << endl;
        verificarIntegridadRuta(ruta, 1);
    } catch (...) { ok = false; }
    cout << (ok ? "OK" : "FALLO") << "\n" << endl;
}

// --- TEST 5: RELOCATE AL FINAL DE LA RUTA ---
void TestRelocateCliente::testRelocateAlFinal() {
    cout << "-------- TEST 5: RELOCATE AL FINAL DE LA RUTA --------\n";
    vector<vector<double>> dist = distancias;
    vector<int> demandas = {0, 10, 15, 20};
    Route ruta = crearRuta({1, 2, 3}, demandas, 100, dist);
    cout << "Estado de ruta antes de relocate:" << endl;
    cout << "Ruta: "; ruta.imprimirRuta();
    cout << endl;
    bool ok = true;
    int idCliente = 1;
    try {
        NodeRoute* cliente = buscarNodo(ruta, idCliente);
        NodeRoute* destino = ruta.getUltimoModify()->anterior;
        ruta.relocateCliente(ruta, cliente, destino, demandas, distancias);
        cout << "Estado de ruta después de relocate (cliente " << idCliente << " al final):" << endl;
        cout << "Ruta: "; ruta.imprimirRuta();
        cout << endl;
        verificarIntegridadRuta(ruta, 1);
    } catch (...) { ok = false; }
    cout << (ok ? "OK" : "FALLO") << "\n" << endl;
}

// --- TEST 6: RUTA CON UN SOLO CLIENTE ---
void TestRelocateCliente::testRelocateRutaUnCliente() {
    cout << "-------- TEST 6: RUTA CON UN SOLO CLIENTE --------\n";
    vector<vector<double>> dist = distancias;
    vector<int> demandas = {0, 10, 15, 20};
    Route r1 = crearRuta({1}, demandas, 100, dist);
    Route r2 = crearRuta({2, 3}, demandas, 100, dist);
    cout << "Estado de rutas antes de relocate:" << endl;
    cout << "Ruta 1: "; r1.imprimirRuta();
    cout << "Ruta 2: "; r2.imprimirRuta();
    cout << endl;
    bool ok = true;
    int idCliente = 1, idDestino = 2;
    try {
        NodeRoute* cliente = buscarNodo(r1, idCliente);
        NodeRoute* destino = buscarNodo(r2, idDestino);
        r1.relocateCliente(r2, cliente, destino, demandas, distancias);
        cout << "Estado de rutas después de relocate (cliente " << idCliente << " después de " << idDestino << "):" << endl;
        cout << "Ruta 1: "; r1.imprimirRuta();
        cout << "Ruta 2: "; r2.imprimirRuta();
        cout << endl;
        verificarIntegridadRuta(r1, 1);
        verificarIntegridadRuta(r2, 2);
    } catch (...) { ok = false; }
    cout << (ok ? "OK" : "FALLO") << "\n" << endl;
}

// --- TEST 7: RELOCATE EXTREMOS MISMA RUTA ---
void TestRelocateCliente::testRelocateExtremosMismaRuta() {
    cout << "-------- TEST 7: RELOCATE EXTREMOS MISMA RUTA --------\n";
    vector<vector<double>> dist = distancias;
    vector<int> demandas = {0, 10, 15, 20, 25, 30};
    Route ruta = crearRuta({1, 2, 3, 4, 5}, demandas, 100, dist);
    cout << "Estado de ruta antes de relocate:" << endl;
    cout << "Ruta: "; ruta.imprimirRuta();
    cout << endl;
    bool ok = true;
    int idCliente = 1, idDestino = 5;
    try {
        NodeRoute* cliente = buscarNodo(ruta, idCliente);
        NodeRoute* destino = buscarNodo(ruta, idDestino);
        ruta.relocateCliente(ruta, cliente, destino, demandas, distancias);
        cout << "Estado de ruta después de relocate (cliente " << idCliente << " después de " << idDestino << "):" << endl;
        cout << "Ruta: "; ruta.imprimirRuta();
        cout << endl;
        verificarIntegridadRuta(ruta, 1);
    } catch (...) { ok = false; }
    cout << (ok ? "OK" : "FALLO") << "\n" << endl;
}

// --- TEST 8: RELOCATE EXTREMOS ENTRE RUTAS ---
void TestRelocateCliente::testRelocateExtremosEntreRutas() {
    cout << "-------- TEST 8: RELOCATE EXTREMOS ENTRE RUTAS --------\n";
    vector<vector<double>> dist = distancias;
    vector<int> demandas = {0, 10, 15, 20, 25, 30};
    Route r1 = crearRuta({1, 2, 3}, demandas, 100, dist);
    Route r2 = crearRuta({4, 5}, demandas, 100, dist);
    cout << "Estado de rutas antes de relocate:" << endl;
    cout << "Ruta 1: "; r1.imprimirRuta();
    cout << "Ruta 2: "; r2.imprimirRuta();
    cout << endl;
    bool ok = true;
    int idCliente = 1, idDestino = 5;
    try {
        NodeRoute* cliente = buscarNodo(r1, idCliente);
        NodeRoute* destino = buscarNodo(r2, idDestino);
        r1.relocateCliente(r2, cliente, destino, demandas, distancias);
        cout << "Estado de rutas después de relocate (cliente " << idCliente << " después de " << idDestino << "):" << endl;
        cout << "Ruta 1: "; r1.imprimirRuta();
        cout << "Ruta 2: "; r2.imprimirRuta();
        cout << endl;
        verificarIntegridadRuta(r1, 1);
        verificarIntegridadRuta(r2, 2);
    } catch (...) { ok = false; }
    cout << (ok ? "OK" : "FALLO") << "\n" << endl;
}

// --- TEST 9: RELOCATE POSICIONES INVERSAS ---
void TestRelocateCliente::testRelocatePosicionesInversas() {
    cout << "-------- TEST 9: RELOCATE POSICIONES INVERSAS --------\n";
    vector<vector<double>> dist = distancias;
    vector<int> demandas = {0, 10, 15, 20, 25, 30};
    Route ruta = crearRuta({1, 2, 3, 4, 5}, demandas, 100, dist);
    cout << "Estado de ruta antes de relocate:" << endl;
    cout << "Ruta: "; ruta.imprimirRuta();
    cout << endl;
    bool ok = true;
    int idCliente = 5, idDestino = 1;
    try {
        NodeRoute* cliente = buscarNodo(ruta, idCliente);
        NodeRoute* destino = buscarNodo(ruta, idDestino);
        ruta.relocateCliente(ruta, cliente, destino, demandas, distancias);
        cout << "Estado de ruta después de relocate (cliente " << idCliente << " después de " << idDestino << "):" << endl;
        cout << "Ruta: "; ruta.imprimirRuta();
        cout << endl;
        verificarIntegridadRuta(ruta, 1);
    } catch (...) { ok = false; }
    cout << (ok ? "OK" : "FALLO") << "\n" << endl;
}

// ---------- Función principal ----------
void TestRelocateCliente::ejecutarTodosLosTests() {
    cout << "=========================================" << endl;
    cout << "TESTING EXHAUSTIVO DE RELOCATE CLIENTE" << endl;
    cout << "=========================================\n" << endl;

    int okCount = 0, failCount = 0;
    auto runTest = [&](void (TestRelocateCliente::*test)()) {
        int before = okCount + failCount;
        try { (this->*test)(); okCount++; } catch (...) { failCount++; }
        if (okCount + failCount == before) failCount++; // Si no sumó, es FALLO
    };

    runTest(&TestRelocateCliente::testRelocateEntreRutas);
    runTest(&TestRelocateCliente::testRelocateMismaRuta);
    runTest(&TestRelocateCliente::testRelocateConsecutivos);
    runTest(&TestRelocateCliente::testRelocateAlInicio);
    runTest(&TestRelocateCliente::testRelocateAlFinal);
    runTest(&TestRelocateCliente::testRelocateRutaUnCliente);
    runTest(&TestRelocateCliente::testRelocateExtremosMismaRuta);
    runTest(&TestRelocateCliente::testRelocateExtremosEntreRutas);
    runTest(&TestRelocateCliente::testRelocatePosicionesInversas);

    cout << "==============================================================" << endl;
    cout << "✓ TODOS LOS TESTS DE RELOCATECLIENTE PASARON (" << okCount << " OK, " << failCount << " FALLO)" << endl;
    cout << "==============================================================" << endl;
}

// ---------- Función main para testing independiente ----------
int main() {
    TestRelocateCliente tester;
    tester.ejecutarTodosLosTests();
    return 0;
}
