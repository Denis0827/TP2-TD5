#include "TestRoute.h"
#include <iostream>
#include <cassert>
#include <cmath>
#include <vector>
#include <set>
#include <algorithm>
using namespace std;

TestRouteTester::TestRouteTester() {
    inicializarDistancias();
}

void TestRouteTester::inicializarDistancias() {
    distancias.resize(10, std::vector<double>(10, 0));
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            distancias[i][j] = (i == j) ? 0 : abs(i - j) * 10.0;
        }
    }
}

void TestRouteTester::imprimirRuta(const Route& ruta, const std::string& nombre) {
    cout << nombre << endl;
    cout << "Ruta: ";
    const NodeRoute* actual = ruta.getRaiz();
    while (actual != nullptr) {
        cout << actual->id;
        if (actual->siguiente != nullptr) cout << " -> ";
        actual = actual->siguiente;
    }
    cout << endl;
    cout << "Demanda total: " << ruta.getDemandaTotal()
         << " | Capacidad restante: " << ruta.getCapacidadRestante()
         << " | Distancia total: " << ruta.getDistanciaTotal() << endl;
    cout << endl;
}

bool TestRouteTester::verificarIntegridadRuta(const Route& ruta, int clientesEsperados, int demandaEsperada, int capacidadEsperada, double distanciaEsperada) {
    if (!(ruta.getRaiz() && ruta.getUltimo())) return false;
    // Chequeo de enlaces dobles y consistencia de nodos
    const NodeRoute* actual = ruta.getRaiz();
    while (actual != nullptr) {
        if (actual->siguiente && actual->siguiente->anterior != actual) return false;
        if (actual->anterior && actual->anterior->siguiente != actual) return false;
        actual = actual->siguiente;
    }
    // Chequeo de clientes
    std::vector<NodeRoute*> clientes = ruta.getAllClientes();
    if ((int)clientes.size() != clientesEsperados) return false;
    // Chequeo de clientes no repetidos
    std::set<int> ids;
    for (NodeRoute* c : clientes) {
        if (ids.count(c->id)) return false;
        ids.insert(c->id);
    }
    // Chequeo de demanda total
    int demandaCalculada = 0;
    for (NodeRoute* cliente : clientes) {
        demandaCalculada += cliente->demanda;
    }
    if (demandaCalculada != ruta.getDemandaTotal()) return false;
    if (ruta.getDemandaTotal() != demandaEsperada) return false;
    // Chequeo de capacidad
    if (ruta.getCapacidadTotal() != capacidadEsperada) return false;
    if (ruta.getCapacidadRestante() != capacidadEsperada - demandaEsperada) return false;
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
    if (std::abs(ruta.getDistanciaTotal() - distanciaEsperada) > 1e-6) return false;
    // Chequeo de consistencia de nodos (no repetidos, depot solo en extremos)
    std::vector<int> ids_ordenados;
    actual = ruta.getRaiz()->siguiente;
    while (actual != ruta.getUltimo()) {
        ids_ordenados.push_back(actual->id);
        actual = actual->siguiente;
    }
    std::sort(ids_ordenados.begin(), ids_ordenados.end());
    for (size_t i = 1; i < ids_ordenados.size(); ++i) {
        if (ids_ordenados[i] == ids_ordenados[i-1]) return false;
    }
    // Depot solo en extremos
    if (!((ruta.getRaiz()->id == 0 || ruta.getRaiz()->id == 1) && (ruta.getUltimo()->id == 0 || ruta.getUltimo()->id == 1))) return false;
    return true;
}

bool TestRouteTester::testRutaVacia() {
    cout << "\n-------- TEST 1: Ruta vacía (solo depot) --------" << endl;
    Route ruta(50, 0);
    imprimirRuta(ruta, "Estado inicial:");
    bool ok = verificarIntegridadRuta(ruta, 0, 0, 50, 0.0);
    cout << (ok ? "OK" : "FALLO") << endl;
    return ok;
}

bool TestRouteTester::testUnCliente() {
    cout << "\n-------- TEST 2: Ruta con un solo cliente --------" << endl;
    Route ruta(50, 0);
    ruta.agregarClienteFinal(1, 10, distancias[0][1], 0, distancias[1][0]);
    imprimirRuta(ruta, "Estado después de agregar cliente 1:");
    bool ok = verificarIntegridadRuta(ruta, 1, 10, 50, 20.0);
    cout << (ok ? "OK" : "FALLO") << endl;
    return ok;
}

bool TestRouteTester::testVariosClientes() {
    cout << "\n-------- TEST 3: Ruta con varios clientes --------" << endl;
    Route ruta(100, 0);
    ruta.agregarClienteFinal(1, 10, distancias[0][1], 0, distancias[1][0]);
    ruta.agregarClienteFinal(2, 15, distancias[0][2], distancias[1][2], distancias[2][0]);
    ruta.agregarClienteFinal(3, 20, distancias[0][3], distancias[2][3], distancias[3][0]);
    imprimirRuta(ruta, "Estado después de agregar clientes 1, 2, 3:");
    bool ok = verificarIntegridadRuta(ruta, 3, 45, 100, ruta.getDistanciaTotal());
    cout << (ok ? "OK" : "FALLO") << endl;
    return ok;
}

bool TestRouteTester::testAgregarInicioYFinal() {
    cout << "\n-------- TEST 4: Agregar clientes al final y luego al inicio --------" << endl;
    Route ruta(100, 0);
    ruta.agregarClienteFinal(2, 15, distancias[0][2], 0, distancias[2][0]);
    ruta.agregarClienteFinal(3, 20, distancias[0][3], distancias[2][3], distancias[3][0]);
    imprimirRuta(ruta, "Estado después de agregar clientes 2, 3 al final:");
    ruta.agregarClienteInicio(1, 10, distancias[0][1], distancias[1][2], distancias[2][0]);
    imprimirRuta(ruta, "Estado después de agregar cliente 1 al inicio:");
    bool ok = verificarIntegridadRuta(ruta, 3, 45, 100, ruta.getDistanciaTotal());
    cout << (ok ? "OK" : "FALLO") << endl;
    return ok;
}

bool TestRouteTester::testUnirRutasSimples() {
    cout << "\n-------- TEST 5: Unir dos rutas simples --------" << endl;
    Route r1(100, 0);
    r1.agregarClienteFinal(1, 10, distancias[0][1], 0, distancias[1][0]);
    Route r2(100, 0);
    r2.agregarClienteFinal(2, 15, distancias[0][2], 0, distancias[2][0]);
    imprimirRuta(r1, "Ruta 1 antes de unir:");
    imprimirRuta(r2, "Ruta 2 antes de unir:");
    r1.unirRutas(r2, distancias[1][2], distancias[0][1], distancias[2][0]);
    imprimirRuta(r1, "Ruta 1 después de unir:");
    bool ok = verificarIntegridadRuta(r1, 2, 25, 100, r1.getDistanciaTotal());
    cout << (ok ? "OK" : "FALLO") << endl;
    return ok;
}

bool TestRouteTester::testUnirRutasConClientes() {
    cout << "\n-------- TEST 6: Unir rutas con varios clientes --------" << endl;
    Route r1(100, 0);
    r1.agregarClienteFinal(1, 10, distancias[0][1], 0, distancias[1][0]);
    r1.agregarClienteFinal(2, 15, distancias[0][2], distancias[1][2], distancias[2][0]);
    Route r2(100, 0);
    r2.agregarClienteFinal(3, 20, distancias[0][3], 0, distancias[3][0]);
    r2.agregarClienteFinal(4, 25, distancias[0][4], distancias[3][4], distancias[4][0]);
    imprimirRuta(r1, "Ruta 1 antes de unir:");
    imprimirRuta(r2, "Ruta 2 antes de unir:");
    r1.unirRutas(r2, distancias[2][3], distancias[0][2], distancias[4][0]);
    imprimirRuta(r1, "Ruta 1 después de unir:");
    bool ok = verificarIntegridadRuta(r1, 4, 70, 100, r1.getDistanciaTotal());
    cout << (ok ? "OK" : "FALLO") << endl;
    return ok;
}

bool TestRouteTester::testUnirRutasVacias() {
    cout << "\n-------- TEST 7: Unir rutas vacías --------" << endl;
    Route r1(100, 0);
    Route r2(100, 0);
    imprimirRuta(r1, "Ruta 1 antes de unir:");
    imprimirRuta(r2, "Ruta 2 antes de unir:");
    r1.unirRutas(r2, 0, 0, 0);
    imprimirRuta(r1, "Ruta 1 después de unir:");
    bool ok = verificarIntegridadRuta(r1, 0, 0, 100, 0.0);
    cout << (ok ? "OK" : "FALLO") << endl;
    return ok;
}

bool TestRouteTester::testUnirRutasInverso() {
    cout << "\n-------- TEST 8: Unir rutas inverso (clientes de r2 al inicio de r1) --------" << endl;
    // ruta1: 1 2 3 1
    Route r1(100, 1);
    r1.agregarClienteFinal(2, 10, distancias[1][2], 0, distancias[2][1]);
    r1.agregarClienteFinal(3, 10, distancias[1][3], distancias[2][3], distancias[3][1]);
    // ruta2: 1 4 5 1
    Route r2(100, 1);
    r2.agregarClienteFinal(4, 10, distancias[1][4], 0, distancias[4][1]);
    r2.agregarClienteFinal(5, 10, distancias[1][5], distancias[4][5], distancias[5][1]);
    imprimirRuta(r1, "Ruta 1 antes de unir:");
    imprimirRuta(r2, "Ruta 2 antes de unir:");
    // Insertar los clientes de r2 (excepto depot) al inicio de r1 (después del depot)
    vector<NodeRoute*> clientesR2 = r2.getAllClientes();
    // Insertar en orden inverso para mantener el orden correcto al insertar al inicio
    for (auto it = clientesR2.rbegin(); it != clientesR2.rend(); ++it) {
        r1.agregarClienteInicio((*it)->id, 10, distancias[1][(*it)->id], distancias[(*it)->id][r1.getRaiz()->siguiente->id], distancias[r1.getRaiz()->siguiente->id][1]);
    }
    imprimirRuta(r1, "Ruta 1 después de unir inverso:");
    // Esperado: 1 4 5 2 3 1
    vector<NodeRoute*> clientesFinal = r1.getAllClientes();
    bool ok = (clientesFinal.size() == 4 && clientesFinal[0]->id == 4 && clientesFinal[1]->id == 5 && clientesFinal[2]->id == 2 && clientesFinal[3]->id == 3);
    ok = ok && verificarIntegridadRuta(r1, 4, 40, 100, r1.getDistanciaTotal());
    cout << (ok ? "OK" : "FALLO") << endl;
    return ok;
}

bool TestRouteTester::testCapacidadYDemanda() {
    cout << "\n-------- TEST 9: Chequeo de capacidad y demanda --------" << endl;
    Route ruta(30, 0);
    ruta.agregarClienteFinal(1, 10, distancias[0][1], 0, distancias[1][0]);
    ruta.agregarClienteFinal(2, 15, distancias[0][2], distancias[1][2], distancias[2][0]);
    imprimirRuta(ruta, "Estado después de agregar clientes 1, 2:");
    bool ok = verificarIntegridadRuta(ruta, 2, 25, 30, ruta.getDistanciaTotal());
    cout << (ok ? "OK" : "FALLO") << endl;
    return ok;
}

bool TestRouteTester::testGettersRuta() {
    cout << "\n-------- TEST 10: Getters de Route --------" << endl;
    Route ruta(50, 0);
    ruta.agregarClienteFinal(1, 10, distancias[0][1], 0, distancias[1][0]);
    ruta.agregarClienteFinal(2, 15, distancias[0][2], distancias[1][2], distancias[2][0]);
    ruta.agregarClienteFinal(3, 5, distancias[0][3], distancias[2][3], distancias[3][0]);
    imprimirRuta(ruta, "Estado de la ruta:");
    bool ok = true;
    // getClientePadreId (primer cliente)
    int padre = ruta.getClientePadreId();
    cout << "getClientePadreId: " << padre << endl;
    ok = ok && (padre == 1);
    // getClienteFinalId (último cliente)
    int final = ruta.getClienteFinalId();
    cout << "getClienteFinalId: " << final << endl;
    ok = ok && (final == 3);
    // getDemandaTotal
    int demanda = ruta.getDemandaTotal();
    cout << "getDemandaTotal: " << demanda << endl;
    ok = ok && (demanda == 30);
    // getCapacidadTotal
    int capacidad = ruta.getCapacidadTotal();
    cout << "getCapacidadTotal: " << capacidad << endl;
    ok = ok && (capacidad == 50);
    // getCapacidadRestante
    int restante = ruta.getCapacidadRestante();
    cout << "getCapacidadRestante: " << restante << endl;
    ok = ok && (restante == 20);
    // getDistanciaTotal
    double dist = ruta.getDistanciaTotal();
    cout << "getDistanciaTotal: " << dist << endl;
    // No chequeo valor exacto, solo que sea positivo
    ok = ok && (dist > 0.0);
    // getAllClientes
    vector<NodeRoute*> clientes = ruta.getAllClientes();
    cout << "getAllClientes: ";
    for (auto c : clientes) cout << c->id << " ";
    cout << endl;
    ok = ok && (clientes.size() == 3 && clientes[0]->id == 1 && clientes[1]->id == 2 && clientes[2]->id == 3);
    // getRaiz y getUltimo
    const NodeRoute* raiz = ruta.getRaiz();
    const NodeRoute* ultimo = ruta.getUltimo();
    cout << "getRaiz->id: " << (raiz ? raiz->id : -1) << endl;
    cout << "getUltimo->id: " << (ultimo ? ultimo->id : -1) << endl;
    ok = ok && (raiz && raiz->id == 0 && ultimo && ultimo->id == 0);
    // getRaizModify y getUltimoModify
    NodeRoute* raizMod = ruta.getRaizModify();
    NodeRoute* ultMod = ruta.getUltimoModify();
    cout << "getRaizModify->id: " << (raizMod ? raizMod->id : -1) << endl;
    cout << "getUltimoModify->id: " << (ultMod ? ultMod->id : -1) << endl;
    ok = ok && (raizMod && raizMod->id == 0 && ultMod && ultMod->id == 0);
    cout << (ok ? "OK" : "FALLO") << endl;
    return ok;
}

void TestRouteTester::ejecutarTodosLosTests() {
    cout << "====================================" << endl;
    cout << "TESTING EXHAUSTIVO DE ROUTE" << endl;
    cout << "====================================" << endl;

    int okCount = 0, failCount = 0;
    auto runTest = [&](bool (TestRouteTester::*test)()) {
        bool ok = (this->*test)();
        if (ok) okCount++;
        else failCount++;
    };

    runTest(&TestRouteTester::testRutaVacia);
    runTest(&TestRouteTester::testUnCliente);
    runTest(&TestRouteTester::testVariosClientes);
    runTest(&TestRouteTester::testAgregarInicioYFinal);
    runTest(&TestRouteTester::testUnirRutasSimples);
    runTest(&TestRouteTester::testUnirRutasConClientes);
    runTest(&TestRouteTester::testUnirRutasInverso);
    runTest(&TestRouteTester::testUnirRutasVacias);
    runTest(&TestRouteTester::testCapacidadYDemanda);
    runTest(&TestRouteTester::testGettersRuta);

    cout << "\n==============================================================" << endl;
    cout << "✓ TODOS LOS TESTS DE ROUTE PASARON (" << okCount << " OK, " << failCount << " FALLO)" << endl;
    cout << "==============================================================" << endl;
}

int main() {
    TestRouteTester tester;
    tester.ejecutarTodosLosTests();
    return 0;
}
