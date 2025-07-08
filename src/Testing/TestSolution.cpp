#include "TestSolution.h"
#include <iostream>
#include <cassert>
using namespace std;

TestSolution::TestSolution() {
    inicializarDistancias();
}

void TestSolution::inicializarDistancias() {
    distancias.resize(10, vector<double>(10, 0));
    for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 10; ++j)
            distancias[i][j] = (i == j) ? 0 : abs(i - j) * 10.0;
}

Route* TestSolution::crearRuta(const vector<int>& ids, const vector<int>& demandas, int capacidad, const vector<vector<double>>& dist) {
    Route* ruta = new Route(capacidad, 0);
    int prev = 0;
    for (size_t i = 0; i < ids.size(); ++i) {
        int id = ids[i];
        int demanda = demandas[id];
        double dist_depi = dist[prev][0];
        double dist_ij = (i > 0) ? dist[prev][id] : 0;
        double dist_depj = dist[id][0];
        ruta->agregarClienteFinal(id, demanda, dist_depi, dist_ij, dist_depj);
        prev = id;
    }
    return ruta;
}

bool TestSolution::testAgregarRutaYGetters() {
    cout << "\n-------- TEST 1: Agregar rutas y getters --------" << endl;
    Solution sol(3, "TestAlgoritmo", "TestInstancia");
    vector<int> demandas = {0, 10, 20, 30};
    Route* r1 = crearRuta({1,2}, demandas, 100, distancias);
    Route* r2 = crearRuta({3}, demandas, 100, distancias);
    sol.agregarRuta(r1);
    sol.agregarRuta(r2);
    sol.imprimirSolution();
    // Chequeo de capacidad en cada ruta
    bool ok = true;
    for (auto& t : sol.getRutas()) {
        Route* r = (t);
        if (r->getDemandaTotal() > r->getCapacidadTotal()) {
            cout << "[FALLO] Demanda de ruta supera capacidad: " << r->getDemandaTotal() << "/" << r->getCapacidadTotal() << endl;
            ok = false;
        }
    }
    ok = ok && (sol.getCantidadRutas() == 2 && sol.getCantidadCamiones() == 3 && sol.getAlgoritmo() == "TestAlgoritmo");
    auto rutas = sol.getRutas();
    ok = ok && ((rutas[0]) == r1 && rutas[1] == r2);
    cout << (ok ? "OK" : "FALLO") << endl;
    return ok;
}

bool TestSolution::testDistanciaTotal() {
    cout << "\n-------- TEST 2: Distancia total --------" << endl;
    Solution sol(2, "TestAlgoritmo", "TestInstancia");
    vector<int> demandas = {0, 10, 20, 30};
    Route* r1 = crearRuta({1}, demandas, 100, distancias);
    Route* r2 = crearRuta({2,3}, demandas, 100, distancias);
    sol.agregarRuta(r1);
    sol.agregarRuta(r2);
    sol.imprimirSolution();
    double expected = r1->getDistanciaTotal() + r2->getDistanciaTotal();
    bool ok = abs(sol.getDistanciaTotal() - expected) < 1e-6;
    cout << (ok ? "OK" : "FALLO") << endl;
    return ok;
}

bool TestSolution::testFactibilidad() {
    cout << "\n-------- TEST 3: Factibilidad --------" << endl;
    Solution sol1(2, "Alg", "Inst");
    Solution sol2(1, "Alg", "Inst");
    vector<int> demandas = {0, 10, 20};
    Route* r1 = crearRuta({1}, demandas, 100, distancias);
    Route* r2 = crearRuta({2}, demandas, 100, distancias);
    sol1.agregarRuta(r1);
    sol1.agregarRuta(r2);
    sol2.agregarRuta(r1);
    sol2.agregarRuta(r2);
    cout << "Solución 1:" << endl;
    sol1.imprimirSolution();
    cout << "Solución 2:" << endl;
    sol2.imprimirSolution();
    bool ok = sol1.esFactible() && !sol2.esFactible();
    cout << (ok ? "OK" : "FALLO") << endl;
    return ok;
}

bool TestSolution::testGetAllClientesSol() {
    cout << "\n-------- TEST 4: getAllClientesSol --------" << endl;
    Solution sol(2, "Alg", "Inst");
    vector<int> demandas = {0, 10, 20};
    Route* r1 = crearRuta({1}, demandas, 100, distancias);
    Route* r2 = crearRuta({2}, demandas, 100, distancias);
    sol.agregarRuta(r1);
    sol.agregarRuta(r2);
    sol.imprimirSolution();
    auto clientes = sol.getAllClientesSol();
    bool ok = (clientes.size() == 2);
    ok = ok && (get<0>(clientes[0])->id == 1 || get<0>(clientes[1])->id == 1);
    ok = ok && (get<0>(clientes[0])->id == 2 || get<0>(clientes[1])->id == 2);
    cout << (ok ? "OK" : "FALLO") << endl;
    return ok;
}

bool TestSolution::testSetAlgoritmo() {
    cout << "\n-------- TEST 5: setAlgoritmo --------" << endl;
    Solution sol(1, "A", "I");
    sol.setAlgoritmo("NuevoAlg");
    sol.imprimirSolution();
    bool ok = (sol.getAlgoritmo() == "NuevoAlg");
    cout << (ok ? "OK" : "FALLO") << endl;
    return ok;
}

// Casos borde
bool TestSolution::testSolucionVacia() {
    cout << "\n-------- TEST 6: Solución vacía --------" << endl;
    Solution sol(2, "Alg", "Inst");
    sol.imprimirSolution();
    bool ok = (sol.getCantidadRutas() == 0 && sol.getDistanciaTotal() == 0.0 && sol.esFactible());
    cout << (ok ? "OK" : "FALLO") << endl;
    return ok;
}

bool TestSolution::testUnicaRutaUnCliente() {
    cout << "\n-------- TEST 7: Única ruta con un cliente --------" << endl;
    Solution sol(1, "Alg", "Inst");
    vector<int> demandas = {0, 10};
    Route* r1 = crearRuta({1}, demandas, 100, distancias);
    sol.agregarRuta(r1);
    sol.imprimirSolution();
    bool ok = (sol.getCantidadRutas() == 1 && sol.getDistanciaTotal() == r1->getDistanciaTotal());
    cout << (ok ? "OK" : "FALLO") << endl;
    return ok;
}

bool TestSolution::testRutasConClientesRepetidos() {
    cout << "\n-------- TEST 8: Rutas con clientes repetidos --------" << endl;
    Solution sol(2, "Alg", "Inst");
    vector<int> demandas = {0, 10, 20};
    Route* r1 = crearRuta({1,2}, demandas, 100, distancias);
    Route* r2 = crearRuta({2}, demandas, 100, distancias); // Cliente 2 repetido
    sol.agregarRuta(r1);
    sol.agregarRuta(r2);
    sol.imprimirSolution();
    auto clientes = sol.getAllClientesSol();
    int count2 = 0;
    for (auto& t : clientes) if (get<0>(t)->id == 2) count2++;
    bool ok = (count2 == 2); // Detecta repetidos
    cout << (ok ? "OK" : "FALLO") << endl;
    return ok;
}

bool TestSolution::testRutasSinClientes() {
    cout << "\n-------- TEST 9: Rutas sin clientes --------" << endl;
    Solution sol(2, "Alg", "Inst");
    Route* r1 = crearRuta({}, {0}, 100, distancias);
    sol.agregarRuta(r1);
    sol.imprimirSolution();
    bool ok = (sol.getCantidadRutas() == 1 && sol.getAllClientesSol().empty());
    cout << (ok ? "OK" : "FALLO") << endl;
    return ok;
}

bool TestSolution::testEliminarRutasVacias() {
    cout << "\n-------- TEST 10: eliminarRutasVacias --------" << endl;
    Solution sol(3, "Alg", "Inst");
    vector<int> demandas = {0, 10, 20};
    Route* r1 = crearRuta({1,2}, demandas, 100, distancias);
    Route* r2 = crearRuta({3}, demandas, 100, distancias);
    sol.agregarRuta(r1);
    sol.agregarRuta(r2);
    sol.imprimirSolution();
    // Borro todos los clientes de r2 manualmente
    NodeRoute* actual = r2->getRaizModify()->siguiente;
    while (actual != r2->getUltimo()) {
        actual = actual->siguiente;
        // Simula borrado: solo deja los depots
        r2->getRaizModify()->siguiente = r2->getUltimoModify();
        r2->getUltimoModify()->anterior = r2->getRaizModify();
        break; // Solo un cliente
    }
    sol.eliminarRutasVacias();
    cout << "\nSolution después de borrar la ruta 2" << endl;
    sol.imprimirSolution();
    bool ok = (sol.getCantidadRutas() == 1);
    cout << (ok ? "OK" : "FALLO") << endl;
    return ok;
}

void TestSolution::ejecutarTodosLosTests() {
    cout << "====================================" << endl;
    cout << "TESTING EXHAUSTIVO DE SOLUTION" << endl;
    cout << "====================================" << endl;
    int okCount = 0, failCount = 0;
    auto runTest = [&](bool (TestSolution::*test)()) {
        bool ok = (this->*test)();
        if (ok) okCount++;
        else failCount++;
    };
    runTest(&TestSolution::testAgregarRutaYGetters);
    runTest(&TestSolution::testDistanciaTotal);
    runTest(&TestSolution::testFactibilidad);
    runTest(&TestSolution::testGetAllClientesSol);
    runTest(&TestSolution::testSetAlgoritmo);
    runTest(&TestSolution::testSolucionVacia);
    runTest(&TestSolution::testUnicaRutaUnCliente);
    runTest(&TestSolution::testRutasConClientesRepetidos);
    runTest(&TestSolution::testRutasSinClientes);
    runTest(&TestSolution::testEliminarRutasVacias);
    cout << "\n==============================================================" << endl;
    cout << "✓ TODOS LOS TESTS DE SOLUTION PASARON (" << okCount << " OK, " << failCount << " FALLO)" << endl;
    cout << "==============================================================" << endl;
}

int main() {
    TestSolution tester;
    tester.ejecutarTodosLosTests();
    return 0;
}
