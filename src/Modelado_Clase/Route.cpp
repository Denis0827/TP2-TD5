#include "Route.h"

Route::Route() {
    this->_raiz = nullptr; // O(1)
    this->_ultimo = nullptr; // O(1)
    this->_demandaTotal = 0; // O(1)
    this->_capacidad = 0; // O(1)
    this->_distanciaTotal = 0; // O(1)
}

Route::Route(int capacidad, int depot) {
    this->_demandaTotal = 0; // O(1)
    this->_capacidad = capacidad; // O(1)
    this->_distanciaTotal = 0; // O(1)

    // Agregamos el depot
    this->_raiz = new NodeRoute{depot, 0, nullptr, nullptr}; // O(1)
    this->_ultimo = new NodeRoute{depot, 0, this->_raiz, nullptr}; // O(1)
    this->_raiz->siguiente = this->_ultimo; // O(1)
}

vector<NodeRoute*> Route::getAllClientes() const {
    vector<NodeRoute*> clientes;
    NodeRoute* actual = this->_raiz->siguiente;
    while (actual != this->_ultimo) {
        clientes.push_back(actual);
        actual = actual->siguiente;
    }
    return clientes;
}

// Pre: debe estar el depot como raiz y ultimo en la ruta
void Route::agregarClienteInicio(int id, int demanda, double dist_depi, double dist_ij, double dist_depj) {
    NodeRoute* primerCliente = this->_raiz->siguiente; // O(1)
    NodeRoute* nuevo = new NodeRoute{id, demanda, this->_raiz, primerCliente}; // O(1)

    this->_raiz->siguiente = nuevo; // O(1)
    if (primerCliente != this->_ultimo) { // O(1)
        primerCliente->anterior = nuevo; // O(1)
        this->_distanciaTotal += dist_depi + dist_ij - dist_depj; // O(1)
    } else { // caso particular donde solo tenemos el depot en raiz y ultimo
        this->_ultimo->anterior = nuevo; // O(1)
        this->_distanciaTotal += dist_depi * 2; // {depot, id, depot}
    }

    this->_demandaTotal += demanda; // O(1)
}

// Pre: debe estar el depot como raiz y ultimo en la ruta
void Route::agregarClienteFinal(int id, int demanda, double dist_depi, double dist_ij, double dist_depj) {
    NodeRoute* ultimoCliente = this->_ultimo->anterior; // O(1)
    NodeRoute* nuevo = new NodeRoute{id, demanda, ultimoCliente, this->_ultimo}; // O(1)

    this->_ultimo->anterior = nuevo; // O(1)
    if (ultimoCliente != this->_raiz) { // O(1)
        ultimoCliente->siguiente = nuevo; // O(1)
        this->_distanciaTotal += dist_ij + dist_depj - dist_depi; // O(1) formula de costos
    } else { // caso particular donde solo tenemos el depot en raiz y ultimo
        this->_raiz->siguiente = nuevo; // O(1)
        this->_distanciaTotal += dist_depi * 2; // {depot, id, depot}
    }

    this->_demandaTotal += demanda;
}

// Pre: para las distancias, i es el último cliente de la ruta actual, el cliente j es el primero de otraRuta
void Route::unirRutas(Route& otraRuta, double dist_ij, double dist_depi, double dis_depj) {
    NodeRoute* ultimo_ruta_i = this->_ultimo; // O(1)
    NodeRoute* raiz_ruta_j = otraRuta._raiz; // O(1)

    // conectamos el último cliente de ruta_i con el primero de ruta_j
    raiz_ruta_j->siguiente->anterior = ultimo_ruta_i->anterior; // O(1)
    ultimo_ruta_i->anterior->siguiente = raiz_ruta_j->siguiente; // O(1)
    this->_ultimo = otraRuta._ultimo; // O(1) la ruta_i recorre hasta el final de la ruta_j

    ultimo_ruta_i->anterior = nullptr; // O(1)
    ultimo_ruta_i->siguiente = nullptr; // O(1)
    delete(ultimo_ruta_i); // O(1) desconectamos y eliminamos el último nodo de ruta_i
    raiz_ruta_j->anterior = nullptr; // O(1)
    raiz_ruta_j->siguiente = nullptr; // O(1)
    delete(raiz_ruta_j); // O(1) desconectamos y eliminamos el primer nodo de ruta_j

    this->_demandaTotal += otraRuta._demandaTotal; // O(1)
    this->_distanciaTotal += otraRuta._distanciaTotal + dist_ij - dist_depi - dis_depj; // O(1)
}
void Route::swapClientes(Route& otraRuta, NodeRoute* clienteA, NodeRoute* clienteB, const vector<vector<double>>& distancias) {
    int demandaA = clienteA->demanda; // O(1)
    int demandaB = clienteB->demanda; // O(1)

    int cliente_anterior_A = clienteA->anterior->id; // O(1)
    int cliente_siguiente_A = clienteA->siguiente->id; // O(1)
    int cliente_anterior_B = clienteB->anterior->id; // O(1)
    int cliente_siguiente_B = clienteB->siguiente->id; // O(1)

    double costo_anterior_A = distancias[cliente_anterior_A][clienteA->id] + distancias[clienteA->id][cliente_siguiente_A]; // O(1)
    double costo_anterior_B = distancias[cliente_anterior_B][clienteB->id] + distancias[clienteB->id][cliente_siguiente_B]; // O(1)

    double costo_nuevo_A, costo_nuevo_B; // O(1)

    if (clienteA->siguiente != clienteB && clienteB->siguiente != clienteA) {
        costo_nuevo_A = distancias[cliente_anterior_B][clienteA->id] + distancias[clienteA->id][cliente_siguiente_B]; // O(1)
        costo_nuevo_B = distancias[cliente_anterior_A][clienteB->id] + distancias[clienteB->id][cliente_siguiente_A]; // O(1)
    } else {
        if (clienteB == clienteA->siguiente) {
            costo_nuevo_A = distancias[clienteA->id][clienteB->id] + distancias[clienteA->id][cliente_siguiente_B]; // O(1)
            costo_nuevo_B = distancias[cliente_anterior_A][clienteB->id] + distancias[clienteB->id][clienteA->id]; // O(1)
        } else {
            costo_nuevo_A = distancias[clienteA->id][clienteB->id] + distancias[clienteA->id][cliente_anterior_B]; // O(1)
            costo_nuevo_B = distancias[cliente_siguiente_A][clienteB->id] + distancias[clienteB->id][clienteA->id]; // O(1)
        }
    }

    NodeRoute* clienteA_anterior = clienteA->anterior; // O(1)
    NodeRoute* clienteA_siguiente = clienteA->siguiente; // O(1)
    NodeRoute* clienteB_anterior = clienteB->anterior; // O(1)
    NodeRoute* clienteB_siguiente = clienteB->siguiente; // O(1)

    // Reasignación de punteros (O(1) cada una)
    if (clienteA->siguiente != clienteB && clienteB->siguiente != clienteA) {
        clienteA->anterior = clienteB->anterior; // O(1)
        clienteA->siguiente = clienteB->siguiente; // O(1)
        clienteB->anterior = clienteA_anterior; // O(1)
        clienteB->siguiente = clienteA_siguiente; // O(1)

        clienteA_anterior->siguiente = clienteB; // O(1)
        clienteA_siguiente->anterior = clienteB; // O(1)
        clienteB_anterior->siguiente = clienteA; // O(1)
        clienteB_siguiente->anterior = clienteA; // O(1)
    } else {
        if (clienteA->siguiente == clienteB) {
            clienteA->anterior = clienteB; // O(1)
            clienteA->siguiente = clienteB_siguiente; // O(1)
            clienteB->anterior = clienteA_anterior; // O(1)
            clienteB->siguiente = clienteA; // O(1)

            clienteA_anterior->siguiente = clienteB; // O(1)
            clienteB_siguiente->anterior = clienteA; // O(1)
        } else {
            clienteA->anterior = clienteB_anterior; // O(1)
            clienteA->siguiente = clienteB; // O(1)
            clienteB->anterior = clienteA; // O(1)
            clienteB->siguiente = clienteA_siguiente; // O(1)

            clienteA_siguiente->anterior = clienteB; // O(1)
            clienteB_anterior->siguiente = clienteA; // O(1)
        }
    }

    this->_demandaTotal += demandaB - demandaA; // O(1)
    otraRuta._demandaTotal += demandaA - demandaB; // O(1)

    bool misma_ruta = this->getRaiz()->siguiente->id == otraRuta.getRaiz()->siguiente->id; // O(1)

    if (misma_ruta) {
        this->_distanciaTotal += (costo_nuevo_A + costo_nuevo_B) - (costo_anterior_A + costo_anterior_B); // O(1)
    } else {
        this->_distanciaTotal += costo_nuevo_B - costo_anterior_A; // O(1)
        otraRuta._distanciaTotal += costo_nuevo_A - costo_anterior_B; // O(1)
    }
}

// Complejidad total: O(1)
// Justificación: todas las operaciones son asignaciones, accesos por punteros o cálculos simples de costo,
// sin bucles ni estructuras dinámicas. La función realiza un intercambio entre dos nodos en tiempo constante.
void Route::relocateCliente(Route& otraRuta, NodeRoute* cliente, NodeRoute* destinoPrev, const vector<int>& demandas, const vector<vector<double>>& distancias) {
    int cliente_i = cliente->id; // O(1)
    int cliente_p = cliente->anterior->id; // O(1)
    int cliente_q = cliente->siguiente->id; // O(1)
    int cliente_j = destinoPrev->id; // O(1)
    NodeRoute* destinoNext = destinoPrev->siguiente; // O(1)
    int cliente_k = destinoNext->id; // O(1)

    double costo_actual = distancias[cliente_p][cliente_i] + distancias[cliente_i][cliente_q] + distancias[cliente_j][cliente_k]; // O(1)
    double costo_nuevo = distancias[cliente_p][cliente_q] + distancias[cliente_j][cliente_i] + distancias[cliente_i][cliente_k]; // O(1)

    // Quitar cliente de su lugar actual (ajuste de punteros)
    cliente->anterior->siguiente = cliente->siguiente; // O(1)
    cliente->siguiente->anterior = cliente->anterior; // O(1)

    // Insertar cliente en nueva posición (ajuste de punteros)
    cliente->anterior = destinoPrev; // O(1)
    cliente->siguiente = destinoNext; // O(1)
    destinoPrev->siguiente = cliente; // O(1)
    destinoNext->anterior = cliente; // O(1)

    if (this != &otraRuta) {
        this->_demandaTotal -= demandas[cliente_i]; // O(1)
        otraRuta._demandaTotal += demandas[cliente_i]; // O(1)
        this->_distanciaTotal += distancias[cliente_p][cliente_q] - distancias[cliente_p][cliente_i] - distancias[cliente_i][cliente_q]; // O(1)
        otraRuta._distanciaTotal += distancias[cliente_j][cliente_i] + distancias[cliente_i][cliente_k] - distancias[cliente_j][cliente_k]; // O(1)
    } else {
        this->_distanciaTotal += costo_nuevo - costo_actual; // O(1)
    }
}

// Complejidad total: O(1)
// Justificación: todas las acciones se realizan en tiempo constante: enlaces de punteros,
// actualización de demanda y cálculo de costo. No se recorren listas ni estructuras complejas.
void Route::imprimirRuta() const {
    cout << "Ruta: "; // O(1)
    const NodeRoute* actual = this->getRaiz(); // O(1)
    while (actual != nullptr) { // O(N)
        cout << actual->id << " "; // O(1)
        actual = actual->siguiente; // O(1)
    }
    cout << "| Demanda total: " << this->getDemandaTotal() // O(1)
              << " | Capacidad restante: " << this->getCapacidadRestante() // O(1)
              << " | Distancia total: " << this->getDistanciaTotal() // O(1)
              << endl; // O(1)
}

// --- Getter Implementations ---

int Route::getClientePadreId() const { return this->_raiz->siguiente->id; } // O(1)
int Route::getClienteFinalId() const { return this->_ultimo->anterior->id; } // O(1)
int Route::getDemandaTotal() const { return this->_demandaTotal; } // O(1)
int Route::getCapacidadTotal() const { return this->_capacidad; } // O(1)
int Route::getCapacidadRestante() const { return this->_capacidad - this->_demandaTotal; } // O(1)
double Route::getDistanciaTotal() const { return this->_distanciaTotal; } // O(1)
const NodeRoute* Route::getRaiz() const { return this->_raiz; } // O(1)
const NodeRoute* Route::getUltimo() const { return this->_ultimo; } // O(1)
NodeRoute* Route::getRaizModify() { return this->_raiz; } // O(1)
NodeRoute* Route::getUltimoModify() { return this->_ultimo; } // O(1)