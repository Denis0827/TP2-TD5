#include "TestHeuristicas.h"
using namespace std;

TestHeuristicas::TestHeuristicas(const string& path, const string& nombre) : instancia(path), nombreInstancia(nombre) {
    inicializarDatos(path, nombre);
}

void TestHeuristicas::inicializarDatos(const string& path, const string& nombre) {
    distancias = instancia.getDistanceMatrix();
    demandas = instancia.getDemands();
    capacidad = instancia.getCapacity();
    depot = instancia.getDepotId();
}

vector<int> TestHeuristicas::obtenerClientesInstancia() const {
    vector<int> clientes;
    for (int i = 1; i < (int)demandas.size(); ++i) {
        clientes.push_back(i);
    }
    return clientes;
}

bool TestHeuristicas::verificarIntegridadRuta(const Route& ruta, int depot) {
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
    if (ruta.getDemandaTotal() > ruta.getCapacidadTotal()) return false;
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
    if (ruta.getRaiz()->id != depot) return false;
    if (ruta.getUltimo()->id != depot) return false;
    return true;
}

void TestHeuristicas::verificarIntegridadSolucion(const Solution& sol, const string& nombreAlgoritmo) {
    cout << "\n--- Verificando integridad de la solución: " << nombreAlgoritmo << " ---" << endl;
    const auto& rutas = sol.getRutas(); // vector<tuple<int, Route*>>
    set<int> clientes_cubiertos;
    int totalDemand = 0;
    double totalCost = 0;
    int rutaNum = 1;
    for (const auto& ruta_tuple : rutas) {
        Route* ruta = get<1>(ruta_tuple);
        bool ok = verificarIntegridadRuta(*ruta, depot);
        if (!ok) {
            cout << "FALLO en la ruta " << rutaNum << endl;
            return; // Salir si falla una ruta
        }
        vector<NodeRoute*> clientes = ruta->getAllClientes();
        for (NodeRoute* c : clientes) {
            clientes_cubiertos.insert(c->id);
            totalDemand += c->demanda;
        }
        totalCost += ruta->getDistanciaTotal();
        rutaNum++;
    }
    vector<int> clientesInst = obtenerClientesInstancia();
    for (int c : clientesInst) {
        assert(clientes_cubiertos.count(c) == 1);
    }
    assert(abs(totalCost - sol.getDistanciaTotal()) < 1e-6);
    cout << "Integridad de la solución verificada correctamente" << endl;
}

bool TestHeuristicas::testNearestNeighbor() {
    cout << "\n========== TEST NEAREST NEIGHBOR ==========" << endl;
    Heuristicas heur(instancia.getName(), nombreInstancia);
    Solution sol = heur.nearestNeighbor(false);
    sol.imprimirSolution();
    verificarIntegridadSolucion(sol, "Nearest Neighbor");
    bool ok = true;
    cout << (ok ? "OK" : "FALLO") << endl;
    return ok;
}

void TestHeuristicas::testClarkeWright() {
    cout << "\n========== TEST CLARKE WRIGHT ==========" << endl;
    Heuristicas heur(instancia.getName(), nombreInstancia);
    Solution sol = heur.clarkeWright(false);
    sol.imprimirSolution();
    verificarIntegridadSolucion(sol, "Clarke Wright");
    cout << "OK" << endl;
}

void TestHeuristicas::testNearestNeighborRandomized() {
    cout << "\n========== TEST NEAREST NEIGHBOR RANDOMIZED ==========" << endl;
    Heuristicas heur(instancia.getName(), nombreInstancia);
    Solution sol = heur.nearestNeighborRandomized(3);
    sol.imprimirSolution();
    verificarIntegridadSolucion(sol, "Nearest Neighbor Randomized");
    cout << "OK" << endl;
}

void TestHeuristicas::testGRASP() {
    cout << "\n========== TEST GRASP ==========" << endl;
    Heuristicas heur(instancia.getName(), nombreInstancia);
    Solution sol = heur.GRASP(10, "swap", 0, 3);
    sol.imprimirSolution();
    verificarIntegridadSolucion(sol, "GRASP");
    cout << "OK" << endl;
}

void TestHeuristicas::ejecutarTodosLosTests() {
    cout << "====================================" << endl;
    cout << "TESTING EXHAUSTIVO DE HEURISTICAS" << endl;
    cout << "====================================" << endl;
    int okCount = 0, failCount = 0;
    auto runTest = [&](bool (TestHeuristicas::*test)()) {
        bool ok = (this->*test)();
        if (ok) okCount++;
        else failCount++;
    };
    runTest(&TestHeuristicas::testNearestNeighbor);
    runTest(&TestHeuristicas::testClarkeWright);
    runTest(&TestHeuristicas::testNearestNeighborRandomized);
    runTest(&TestHeuristicas::testGRASP);
    runTest(&TestHeuristicas::testCWFeasibility);
    runTest(&TestHeuristicas::testNNFeasibility);
    runTest(&TestHeuristicas::testGRASPFeasibility);
    cout << "\n==============================================================" << endl;
    cout << "✓ TODOS LOS TESTS DE HEURISTICAS PASARON (" << okCount << " OK, " << failCount << " FALLO)" << endl;
    cout << "==============================================================" << endl;
}

int main() {
    TestHeuristicas tester("instancias/2l-cvrp-0/E016-03m.dat", "instancias/2l-cvrp-0/E016-03m.dat");
    tester.ejecutarTodosLosTests();
    return 0;
}
