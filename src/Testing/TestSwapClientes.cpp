#include "TestSwapClientes.h"

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

double TestSwapClientes::calcularCostoAnterior(NodeRoute* cliente) {
    if (!cliente || !cliente->anterior || !cliente->siguiente) return 0;
    return distancias[cliente->anterior->id][cliente->id] + 
            distancias[cliente->id][cliente->siguiente->id];
}

double TestSwapClientes::calcularCostoNuevo(NodeRoute* cliente, NodeRoute* nuevoAnterior, NodeRoute* nuevoSiguiente) {
    if (!cliente || !nuevoAnterior || !nuevoSiguiente) return 0;
    return distancias[nuevoAnterior->id][cliente->id] + 
            distancias[cliente->id][nuevoSiguiente->id];
}

void TestSwapClientes::imprimirRutaBonita(const Route& ruta, const string& nombre) {
    cout << nombre << endl;
    cout << "Ruta: ";
    const NodeRoute* actual = ruta.getRaiz();
    while (actual != nullptr) {
        cout << actual->id;
        if (actual->siguiente != nullptr) {
            cout << " -> ";
        }
        actual = actual->siguiente;
    }
    cout << endl;
    cout << "Demanda total: " << ruta.getDemandaTotal()
         << " | Capacidad restante: " << ruta.getCapacidadRestante()
         << " | Distancia total: " << ruta.getDistanciaTotal() << endl;
    cout << endl;
}

void TestSwapClientes::verificarIntegridadRuta(const Route& ruta, const string& nombre) {
    cout << "🔍 Verificando integridad de " << nombre << "..." << endl;
    
    // Verificar que la ruta es válida
    assert(ruta.getRaiz() != nullptr);
    assert(ruta.getUltimo() != nullptr);
    assert(ruta.getRaiz()->siguiente != nullptr);
    assert(ruta.getUltimo()->anterior != nullptr);
    
    // Verificar que la demanda total es correcta
    vector<NodeRoute*> clientes = ruta.getAllClientes();
    int demandaCalculada = 0;
    for (NodeRoute* cliente : clientes) {
        demandaCalculada += cliente->demanda;
    }
    assert(demandaCalculada == ruta.getDemandaTotal());
    
    // Verificar que todos los nodos tienen enlaces correctos
    const NodeRoute* actual = ruta.getRaiz();
    while (actual != nullptr) {
        if (actual->siguiente != nullptr) {
            assert(actual->siguiente->anterior == actual);
        }
        if (actual->anterior != nullptr) {
            assert(actual->anterior->siguiente == actual);
        }
        actual = actual->siguiente;
    }
    
    // Verificar que el primer cliente tiene al depot como anterior
    if (ruta.getRaiz()->siguiente != ruta.getUltimo()) {
        assert(ruta.getRaiz()->siguiente->anterior == ruta.getRaiz());
    }
    
    // Verificar que el último cliente tiene al depot como siguiente
    if (ruta.getUltimo()->anterior != ruta.getRaiz()) {
        assert(ruta.getUltimo()->anterior->siguiente == ruta.getUltimo());
    }
    
    // Verificar que la capacidad restante es correcta
    assert(ruta.getCapacidadRestante() == ruta.getCapacidadTotal() - ruta.getDemandaTotal());
    
    cout << "✓ Integridad verificada exitosamente" << endl;
}

TestSwapClientes::TestSwapClientes() {
    inicializarDistancias();
}

// CASO 1: Nodos consecutivos en la misma ruta
void TestSwapClientes::testSwapConsecutivosMismaRuta() {
    cout << "\n❓ TEST: Nodos consecutivos en la misma ruta" << endl;
    cout << "═══════════════════════════════════════════════════════════════" << endl;
    
    Route ruta(100, 0); // Capacidad 100, depot 0
    ruta.agregarClienteFinal(1, 10, distancias[0][1], 0, distancias[1][0]);
    ruta.agregarClienteFinal(2, 15, distancias[0][2], distancias[1][2], distancias[2][0]);
    ruta.agregarClienteFinal(3, 20, distancias[0][3], distancias[2][3], distancias[3][0]);
    
    imprimirRutaBonita(ruta, "Ruta Original");
    
    vector<NodeRoute*> clientes = ruta.getAllClientes();
    NodeRoute* cliente1 = clientes[0]; // Cliente 1
    NodeRoute* cliente2 = clientes[1]; // Cliente 2 (consecutivo)
    
    cout << "🔄 Realizando swap entre cliente " << cliente1->id << " y cliente " << cliente2->id << "..." << endl;
    
    double costo_anterior_1 = calcularCostoAnterior(cliente1);
    double costo_anterior_2 = calcularCostoAnterior(cliente2);
    double costo_nuevo_1 = calcularCostoNuevo(cliente1, cliente2->anterior, cliente2->siguiente);
    double costo_nuevo_2 = calcularCostoNuevo(cliente2, cliente1->anterior, cliente1->siguiente);
    
    int demanda_original = ruta.getDemandaTotal();
    
    ruta.swapClientes(ruta, cliente1, cliente2, cliente1->demanda, cliente2->demanda,
                     costo_anterior_1, costo_anterior_2, costo_nuevo_1, costo_nuevo_2);
    
    imprimirRutaBonita(ruta, "Ruta Después del Swap");
    
    // Verificaciones
    assert(ruta.getDemandaTotal() == demanda_original); // Demanda no cambia en misma ruta
    verificarIntegridadRuta(ruta, "ruta después del swap");
    
    cout << "Test de nodos consecutivos en misma ruta PASÓ ✅" << endl;
}

// CASO 2: Nodos de distintas rutas
void TestSwapClientes::testSwapDistintasRutas() {
    cout << "\n❓ TEST: Nodos de distintas rutas" << endl;
    cout << "═══════════════════════════════════════════════════════════════" << endl;
    
    Route ruta1(100, 0);
    Route ruta2(100, 0);
    
    // Ruta 1: 0-1-2-0
    ruta1.agregarClienteFinal(1, 10, distancias[0][1], 0, distancias[1][0]);
    ruta1.agregarClienteFinal(2, 15, distancias[0][2], distancias[1][2], distancias[2][0]);
    
    // Ruta 2: 0-3-4-0
    ruta2.agregarClienteFinal(3, 20, distancias[0][3], 0, distancias[3][0]);
    ruta2.agregarClienteFinal(4, 25, distancias[0][4], distancias[3][4], distancias[4][0]);
    
    imprimirRutaBonita(ruta1, "Ruta 1 Original");
    imprimirRutaBonita(ruta2, "Ruta 2 Original");
    
    vector<NodeRoute*> clientes1 = ruta1.getAllClientes();
    vector<NodeRoute*> clientes2 = ruta2.getAllClientes();
    
    NodeRoute* cliente1 = clientes1[0]; // Cliente 1 de ruta 1
    NodeRoute* cliente3 = clientes2[0]; // Cliente 3 de ruta 2
    
    cout << "🔄 Realizando swap entre cliente " << cliente1->id << " (Ruta 1) y cliente " << cliente3->id << " (Ruta 2)..." << endl;
    
    double costo_anterior_1 = calcularCostoAnterior(cliente1);
    double costo_anterior_3 = calcularCostoAnterior(cliente3);
    double costo_nuevo_1 = calcularCostoNuevo(cliente1, cliente3->anterior, cliente3->siguiente);
    double costo_nuevo_3 = calcularCostoNuevo(cliente3, cliente1->anterior, cliente1->siguiente);
    
    int demanda_original_1 = ruta1.getDemandaTotal();
    int demanda_original_2 = ruta2.getDemandaTotal();
    
    ruta1.swapClientes(ruta2, cliente1, cliente3, cliente1->demanda, cliente3->demanda,
                      costo_anterior_1, costo_anterior_3, costo_nuevo_1, costo_nuevo_3);
    
    imprimirRutaBonita(ruta1, "Ruta 1 Después del Swap");
    imprimirRutaBonita(ruta2, "Ruta 2 Después del Swap");
    
    // Verificaciones
    assert(ruta1.getDemandaTotal() == demanda_original_1 - cliente1->demanda + cliente3->demanda);
    assert(ruta2.getDemandaTotal() == demanda_original_2 - cliente3->demanda + cliente1->demanda);
    verificarIntegridadRuta(ruta1, "ruta 1 después del swap");
    verificarIntegridadRuta(ruta2, "ruta 2 después del swap");
    
    cout << "Test de nodos de distintas rutas PASÓ ✅" << endl;
}

// CASO 3: Nodos de la misma ruta pero no consecutivos
void TestSwapClientes::testSwapNoConsecutivosMismaRuta() {
    cout << "\n❓ TEST: Nodos de la misma ruta pero no consecutivos" << endl;
    cout << "═══════════════════════════════════════════════════════════════" << endl;
    
    Route ruta(100, 0);
    ruta.agregarClienteFinal(1, 10, distancias[0][1], 0, distancias[1][0]);
    ruta.agregarClienteFinal(2, 15, distancias[0][2], distancias[1][2], distancias[2][0]);
    ruta.agregarClienteFinal(3, 20, distancias[0][3], distancias[2][3], distancias[3][0]);
    ruta.agregarClienteFinal(4, 25, distancias[0][4], distancias[3][4], distancias[4][0]);
    
    imprimirRutaBonita(ruta, "Ruta Original");
    
    vector<NodeRoute*> clientes = ruta.getAllClientes();
    NodeRoute* cliente1 = clientes[0]; // Cliente 1
    NodeRoute* cliente3 = clientes[2]; // Cliente 3 (no consecutivo con 1)
    
    cout << "🔄 Realizando swap entre cliente " << cliente1->id << " y cliente " << cliente3->id << " (no consecutivos)..." << endl;
    
    double costo_anterior_1 = calcularCostoAnterior(cliente1);
    double costo_anterior_3 = calcularCostoAnterior(cliente3);
    double costo_nuevo_1 = calcularCostoNuevo(cliente1, cliente3->anterior, cliente3->siguiente);
    double costo_nuevo_3 = calcularCostoNuevo(cliente3, cliente1->anterior, cliente1->siguiente);
    
    int demanda_original = ruta.getDemandaTotal();
    
    ruta.swapClientes(ruta, cliente1, cliente3, cliente1->demanda, cliente3->demanda,
                     costo_anterior_1, costo_anterior_3, costo_nuevo_1, costo_nuevo_3);
    
    imprimirRutaBonita(ruta, "Ruta Después del Swap");
    
    // Verificaciones
    assert(ruta.getDemandaTotal() == demanda_original); // Demanda no cambia en misma ruta
    verificarIntegridadRuta(ruta, "ruta después del swap");
    
    cout << "Test de nodos no consecutivos en misma ruta PASÓ ✅" << endl;
}

// CASO 4: Caso borde - Swap con nodos en extremos de rutas
void TestSwapClientes::testSwapExtremosRutas() {
    cout << "\n❓ TEST: Swap con nodos en extremos de rutas" << endl;
    cout << "═══════════════════════════════════════════════════════════════" << endl;
    
    Route ruta1(100, 0);
    Route ruta2(100, 0);
    
    // Ruta 1: 0-1-2-0
    ruta1.agregarClienteFinal(1, 10, distancias[0][1], 0, distancias[1][0]);
    ruta1.agregarClienteFinal(2, 15, distancias[0][2], distancias[1][2], distancias[2][0]);
    
    // Ruta 2: 0-3-4-0
    ruta2.agregarClienteFinal(3, 20, distancias[0][3], 0, distancias[3][0]);
    ruta2.agregarClienteFinal(4, 25, distancias[0][4], distancias[3][4], distancias[4][0]);
    
    imprimirRutaBonita(ruta1, "Ruta 1 Original");
    imprimirRutaBonita(ruta2, "Ruta 2 Original");
    
    vector<NodeRoute*> clientes1 = ruta1.getAllClientes();
    vector<NodeRoute*> clientes2 = ruta2.getAllClientes();
    
    NodeRoute* primerCliente1 = clientes1[0]; // Primer cliente de ruta 1
    NodeRoute* ultimoCliente2 = clientes2[1]; // Último cliente de ruta 2
    
    cout << "🔄 Realizando swap entre primer cliente " << primerCliente1->id << " (Ruta 1) y último cliente " << ultimoCliente2->id << " (Ruta 2)..." << endl;
    
    double costo_anterior_1 = calcularCostoAnterior(primerCliente1);
    double costo_anterior_4 = calcularCostoAnterior(ultimoCliente2);
    double costo_nuevo_1 = calcularCostoNuevo(primerCliente1, ultimoCliente2->anterior, ultimoCliente2->siguiente);
    double costo_nuevo_4 = calcularCostoNuevo(ultimoCliente2, primerCliente1->anterior, primerCliente1->siguiente);
    
    ruta1.swapClientes(ruta2, primerCliente1, ultimoCliente2, primerCliente1->demanda, ultimoCliente2->demanda,
                      costo_anterior_1, costo_anterior_4, costo_nuevo_1, costo_nuevo_4);
    
    imprimirRutaBonita(ruta1, "Ruta 1 Después del Swap");
    imprimirRutaBonita(ruta2, "Ruta 2 Después del Swap");
    
    verificarIntegridadRuta(ruta1, "ruta 1 después del swap");
    verificarIntegridadRuta(ruta2, "ruta 2 después del swap");
    
    cout << "Test de swap con extremos de rutas PASÓ ✅" << endl;
}

// CASO 5: Caso borde - Ruta con un solo cliente
void TestSwapClientes::testSwapRutaUnSoloCliente() {
    cout << "\n❓ TEST: Swap con ruta que tiene un solo cliente" << endl;
    cout << "═══════════════════════════════════════════════════════════════" << endl;
    
    Route ruta1(100, 0);
    Route ruta2(100, 0);
    
    // Ruta 1: 0-1-0 (un solo cliente)
    ruta1.agregarClienteFinal(1, 10, distancias[0][1], 0, distancias[1][0]);
    
    // Ruta 2: 0-2-3-0 (dos clientes)
    ruta2.agregarClienteFinal(2, 15, distancias[0][2], 0, distancias[2][0]);
    ruta2.agregarClienteFinal(3, 20, distancias[0][3], distancias[2][3], distancias[3][0]);
    
    imprimirRutaBonita(ruta1, "Ruta 1 Original (Un Cliente)");
    imprimirRutaBonita(ruta2, "Ruta 2 Original (Dos Clientes)");
    
    vector<NodeRoute*> clientes1 = ruta1.getAllClientes();
    vector<NodeRoute*> clientes2 = ruta2.getAllClientes();
    
    NodeRoute* cliente1 = clientes1[0];
    NodeRoute* cliente2 = clientes2[0];
    
    cout << "🔄 Realizando swap entre cliente " << cliente1->id << " (Ruta 1 - un cliente) y cliente " << cliente2->id << " (Ruta 2)..." << endl;
    
    double costo_anterior_1 = calcularCostoAnterior(cliente1);
    double costo_anterior_2 = calcularCostoAnterior(cliente2);
    double costo_nuevo_1 = calcularCostoNuevo(cliente1, cliente2->anterior, cliente2->siguiente);
    double costo_nuevo_2 = calcularCostoNuevo(cliente2, cliente1->anterior, cliente1->siguiente);
    
    ruta1.swapClientes(ruta2, cliente1, cliente2, cliente1->demanda, cliente2->demanda,
                      costo_anterior_1, costo_anterior_2, costo_nuevo_1, costo_nuevo_2);
    
    imprimirRutaBonita(ruta1, "Ruta 1 Después del Swap");
    imprimirRutaBonita(ruta2, "Ruta 2 Después del Swap");
    
    verificarIntegridadRuta(ruta1, "ruta 1 después del swap");
    verificarIntegridadRuta(ruta2, "ruta 2 después del swap");
    
    cout << "Test de swap con ruta de un solo cliente PASÓ ✅" << endl;
}

// CASO 6: Caso borde - Swap entre rutas con capacidades diferentes
void TestSwapClientes::testSwapCapacidadesDiferentes() {
    cout << "\n❓ TEST: Swap entre rutas con capacidades diferentes" << endl;
    cout << "═══════════════════════════════════════════════════════════════" << endl;
    
    Route ruta1(50, 0);  // Capacidad menor
    Route ruta2(100, 0); // Capacidad mayor
    
    // Ruta 1: 0-1-2-0
    ruta1.agregarClienteFinal(1, 20, distancias[0][1], 0, distancias[1][0]);
    ruta1.agregarClienteFinal(2, 25, distancias[0][2], distancias[1][2], distancias[2][0]);
    
    // Ruta 2: 0-3-4-0
    ruta2.agregarClienteFinal(3, 10, distancias[0][3], 0, distancias[3][0]);
    ruta2.agregarClienteFinal(4, 15, distancias[0][4], distancias[3][4], distancias[4][0]);
    
    imprimirRutaBonita(ruta1, "Ruta 1 Original (Capacidad 50)");
    imprimirRutaBonita(ruta2, "Ruta 2 Original (Capacidad 100)");
    
    vector<NodeRoute*> clientes1 = ruta1.getAllClientes();
    vector<NodeRoute*> clientes2 = ruta2.getAllClientes();
    
    NodeRoute* cliente1 = clientes1[0];
    NodeRoute* cliente3 = clientes2[0];
    
    cout << "🔄 Realizando swap entre cliente " << cliente1->id << " (Ruta 1 - capacidad 50) y cliente " << cliente3->id << " (Ruta 2 - capacidad 100)..." << endl;
    
    double costo_anterior_1 = calcularCostoAnterior(cliente1);
    double costo_anterior_3 = calcularCostoAnterior(cliente3);
    double costo_nuevo_1 = calcularCostoNuevo(cliente1, cliente3->anterior, cliente3->siguiente);
    double costo_nuevo_3 = calcularCostoNuevo(cliente3, cliente1->anterior, cliente1->siguiente);
    
    ruta1.swapClientes(ruta2, cliente1, cliente3, cliente1->demanda, cliente3->demanda,
                      costo_anterior_1, costo_anterior_3, costo_nuevo_1, costo_nuevo_3);
    
    imprimirRutaBonita(ruta1, "Ruta 1 Después del Swap");
    imprimirRutaBonita(ruta2, "Ruta 2 Después del Swap");
    
    verificarIntegridadRuta(ruta1, "ruta 1 después del swap");
    verificarIntegridadRuta(ruta2, "ruta 2 después del swap");
    
    cout << "Test de swap entre rutas con capacidades diferentes PASÓ ✅" << endl;
}

void TestSwapClientes::ejecutarTodosLosTests() {
    cout << "\n🚀 INICIANDO TESTS EXHAUSTIVOS PARA SwapClientes" << endl;
    cout << "═══════════════════════════════════════════════════════════════" << endl;
    cout << "📋 Casos de prueba que se ejecutarán:" << endl;
    cout << "   1. Nodos consecutivos en la misma ruta" << endl;
    cout << "   2. Nodos de distintas rutas" << endl;
    cout << "   3. Nodos de la misma ruta pero no consecutivos" << endl;
    cout << "   4. Swap con nodos en extremos de rutas" << endl;
    cout << "   5. Ruta con un solo cliente" << endl;
    cout << "   6. Rutas con capacidades diferentes" << endl;
    cout << "═══════════════════════════════════════════════════════════════" << endl;
    
    try {
        testSwapConsecutivosMismaRuta();
        testSwapDistintasRutas();
        testSwapNoConsecutivosMismaRuta();
        testSwapExtremosRutas();
        testSwapRutaUnSoloCliente();
        testSwapCapacidadesDiferentes();
        
        cout << "\n🎉 ¡TODOS LOS TESTS PASARON EXITOSAMENTE!" << endl;
        cout << "═══════════════════════════════════════════════════════════════" << endl;
        cout << "✅ Cobertura de casos bordes completada:" << endl;
        cout << "   ✓ Nodos consecutivos en la misma ruta" << endl;
        cout << "   ✓ Nodos de distintas rutas" << endl;
        cout << "   ✓ Nodos de la misma ruta pero no consecutivos" << endl;
        cout << "   ✓ Casos bordes adicionales (extremos, rutas con un cliente, capacidades diferentes)" << endl;
        cout << "═══════════════════════════════════════════════════════════════" << endl;
        
    } catch (const exception& e) {
        cout << "\n❌ ERROR en los tests: " << e.what() << endl;
        cout << "═══════════════════════════════════════════════════════════════" << endl;
    }
}

int main() {
    TestSwapClientes tester;
    tester.ejecutarTodosLosTests();
    return 0;
} 