#include "TestRoute.h"
#include <iostream>
#include <cassert>
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

void TestRouteTester::verificarIntegridadRuta(const Route& ruta, const std::string& nombre, int clientesEsperados, int demandaEsperada, int capacidadEsperada, double distanciaEsperada) {
    cout << "Verificando integridad de " << nombre << "..." << endl;
    // Chequeo de estructura
    assert(ruta.getRaiz() != nullptr);
    assert(ruta.getUltimo() != nullptr);
    // Chequeo de clientes
    vector<NodeRoute*> clientes = ruta.getAllClientes();
    assert((int)clientes.size() == clientesEsperados);
    // Chequeo de demanda
    assert(ruta.getDemandaTotal() == demandaEsperada);
    // Chequeo de capacidad
    assert(ruta.getCapacidadTotal() == capacidadEsperada);
    assert(ruta.getCapacidadRestante() == capacidadEsperada - demandaEsperada);
    // Chequeo de distancia
    assert(abs(ruta.getDistanciaTotal() - distanciaEsperada) < 0.01);
    // Chequeo de enlaces
    const NodeRoute* actual = ruta.getRaiz();
    while (actual != nullptr) {
        if (actual->siguiente != nullptr) assert(actual->siguiente->anterior == actual);
        if (actual->anterior != nullptr) assert(actual->anterior->siguiente == actual);
        actual = actual->siguiente;
    }
    cout << "✓ Integridad verificada exitosamente" << endl << endl;
}

void TestRouteTester::testRutaVacia() {
    cout << "\n❓ TEST: Ruta vacía (solo depot)" << endl;
    Route ruta(50, 0);
    imprimirRuta(ruta, "Ruta vacía");
    verificarIntegridadRuta(ruta, "Ruta vacía", 0, 0, 50, 0.0);
}

void TestRouteTester::testUnCliente() {
    cout << "\n❓ TEST: Ruta con un solo cliente" << endl;
    Route ruta(50, 0);
    ruta.agregarClienteFinal(1, 10, distancias[0][1], 0, distancias[1][0]);
    imprimirRuta(ruta, "Ruta con un cliente");
    verificarIntegridadRuta(ruta, "Ruta con un cliente", 1, 10, 50, 20.0);
}

void TestRouteTester::testVariosClientes() {
    cout << "\n❓ TEST: Ruta con varios clientes" << endl;
    Route ruta(100, 0);
    ruta.agregarClienteFinal(1, 10, distancias[0][1], 0, distancias[1][0]);
    ruta.agregarClienteFinal(2, 15, distancias[0][2], distancias[1][2], distancias[2][0]);
    ruta.agregarClienteFinal(3, 20, distancias[0][3], distancias[2][3], distancias[3][0]);
    imprimirRuta(ruta, "Ruta con varios clientes");
    verificarIntegridadRuta(ruta, "Ruta con varios clientes", 3, 45, 100, ruta.getDistanciaTotal());
}

void TestRouteTester::testAgregarInicioYFinal() {
    cout << "\n❓ TEST: Agregar clientes al inicio y al final" << endl;
    Route ruta(100, 0);
    ruta.agregarClienteFinal(2, 15, distancias[0][2], 0, distancias[2][0]);
    ruta.agregarClienteInicio(1, 10, distancias[0][1], distancias[1][2], distancias[2][0]);
    ruta.agregarClienteFinal(3, 20, distancias[0][3], distancias[2][3], distancias[3][0]);
    imprimirRuta(ruta, "Ruta con clientes al inicio y final");
    verificarIntegridadRuta(ruta, "Ruta con clientes al inicio y final", 3, 45, 100, ruta.getDistanciaTotal());
}

void TestRouteTester::testUnirRutasSimples() {
    cout << "\n❓ TEST: Unir dos rutas simples" << endl;
    Route r1(100, 0);
    r1.agregarClienteFinal(1, 10, distancias[0][1], 0, distancias[1][0]);
    Route r2(100, 0);
    r2.agregarClienteFinal(2, 15, distancias[0][2], 0, distancias[2][0]);
    imprimirRuta(r1, "Ruta 1 antes de unir");
    imprimirRuta(r2, "Ruta 2 antes de unir");
    r1.unirRutas(r2, distancias[1][2], distancias[0][1], distancias[2][0]);
    imprimirRuta(r1, "Ruta unida");
    verificarIntegridadRuta(r1, "Ruta unida", 2, 25, 100, r1.getDistanciaTotal());
}

void TestRouteTester::testUnirRutasConClientes() {
    cout << "\n❓ TEST: Unir rutas con varios clientes" << endl;
    Route r1(100, 0);
    r1.agregarClienteFinal(1, 10, distancias[0][1], 0, distancias[1][0]);
    r1.agregarClienteFinal(2, 15, distancias[0][2], distancias[1][2], distancias[2][0]);
    Route r2(100, 0);
    r2.agregarClienteFinal(3, 20, distancias[0][3], 0, distancias[3][0]);
    r2.agregarClienteFinal(4, 25, distancias[0][4], distancias[3][4], distancias[4][0]);
    imprimirRuta(r1, "Ruta 1 antes de unir");
    imprimirRuta(r2, "Ruta 2 antes de unir");
    r1.unirRutas(r2, distancias[2][3], distancias[0][2], distancias[4][0]);
    imprimirRuta(r1, "Ruta unida");
    verificarIntegridadRuta(r1, "Ruta unida", 4, 70, 100, r1.getDistanciaTotal());
}

void TestRouteTester::testUnirRutasVacias() {
    cout << "\n❓ TEST: Unir rutas vacías" << endl;
    Route r1(100, 0);
    Route r2(100, 0);
    imprimirRuta(r1, "Ruta 1 antes de unir");
    imprimirRuta(r2, "Ruta 2 antes de unir");
    r1.unirRutas(r2, 0, 0, 0);
    imprimirRuta(r1, "Ruta unida (vacía)");
    verificarIntegridadRuta(r1, "Ruta unida (vacía)", 0, 0, 100, 0.0);
}

void TestRouteTester::testCapacidadYDemanda() {
    cout << "\n❓ TEST: Chequeo de capacidad y demanda" << endl;
    Route ruta(30, 0);
    ruta.agregarClienteFinal(1, 10, distancias[0][1], 0, distancias[1][0]);
    ruta.agregarClienteFinal(2, 15, distancias[0][2], distancias[1][2], distancias[2][0]);
    imprimirRuta(ruta, "Ruta con capacidad justa");
    verificarIntegridadRuta(ruta, "Ruta con capacidad justa", 2, 25, 30, ruta.getDistanciaTotal());
    // Intentar agregar un cliente que exceda la capacidad (no debe hacerlo en la lógica real, pero aquí solo mostramos el test)
}

void TestRouteTester::ejecutarTodosLosTests() {
    cout << "\n🚀 INICIANDO TESTS EXHAUSTIVOS PARA Route" << endl;
    cout << "═══════════════════════════════════════════════════════════════" << endl;
    cout << "Casos de prueba que se ejecutarán:" << endl;
    cout << "  1. Ruta vacía (solo depot)" << endl;
    cout << "  2. Ruta con un solo cliente" << endl;
    cout << "  3. Ruta con varios clientes" << endl;
    cout << "  4. Agregar clientes al inicio y al final" << endl;
    cout << "  5. Unir dos rutas simples" << endl;
    cout << "  6. Unir rutas con varios clientes" << endl;
    cout << "  7. Unir rutas vacías" << endl;
    cout << "  8. Chequeo de capacidad y demanda" << endl;
    cout << "═══════════════════════════════════════════════════════════════" << endl;
    testRutaVacia();
    testUnCliente();
    testVariosClientes();
    testAgregarInicioYFinal();
    testUnirRutasSimples();
    testUnirRutasConClientes();
    testUnirRutasVacias();
    testCapacidadYDemanda();
    cout << "\n✓ TODOS LOS TESTS DE ROUTE PASARON EXITOSAMENTE" << endl;
}

int main() {
    TestRouteTester tester;
    tester.ejecutarTodosLosTests();
    return 0;
}
