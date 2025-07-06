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

void Route::swapClientes(Route& otraRuta, NodeRoute* clienteA, NodeRoute* clienteB, int demandaA, int demandaB, 
    double costo_anterior_A, double costo_anterior_B, double costo_nuevo_A, double costo_nuevo_B) {
    NodeRoute* clienteA_anterior = clienteA->anterior;
    NodeRoute* clienteA_siguiente = clienteA->siguiente;
    NodeRoute* clienteB_anterior = clienteB->anterior;
    NodeRoute* clienteB_siguiente = clienteB->siguiente;

    // Realizamos el swap, teniendo cuidado con los enlaces entre clientes y tomando como guía los ejemplos
    // ilustrativos mostrados arriba
    if (clienteA->siguiente != clienteB && clienteB->siguiente != clienteA) { // caso NO consecutivos
        clienteA->anterior = clienteB->anterior;
        clienteA->siguiente = clienteB->siguiente;
        clienteB->anterior = clienteA_anterior;
        clienteB->siguiente = clienteA_siguiente;
        
        clienteA_anterior->siguiente = clienteB;
        clienteA_siguiente->anterior = clienteB;
        clienteB_anterior->siguiente = clienteA;
        clienteB_siguiente->anterior = clienteA;
    } else { // caso SI consecutivos
        if (clienteA->siguiente == clienteB) {
            clienteA->anterior = clienteB;
            clienteA->siguiente = clienteB_siguiente;
            clienteB->anterior = clienteA_anterior;
            clienteB->siguiente = clienteA;

            clienteA_anterior->siguiente = clienteB;
            clienteB_siguiente->anterior = clienteA;
        } else {
            clienteA->anterior = clienteB_anterior;
            clienteA->siguiente = clienteB;
            clienteB->anterior = clienteA;
            clienteB->siguiente = clienteA_siguiente;

            clienteA_siguiente->anterior = clienteB;
            clienteB_anterior->siguiente = clienteA;
        }
    }

    this->_demandaTotal += demandaB - demandaA;
    otraRuta._demandaTotal += demandaA - demandaB;
    // si fuesen la misma ruta se cancela la suma

    bool misma_ruta = this->getRaiz()->siguiente->id == otraRuta.getRaiz()->siguiente->id; // si las rutas no tienen el mismo primer cliente, entonces son distintas rutas

    if (misma_ruta) {
        this->_distanciaTotal += (costo_nuevo_A + costo_nuevo_B) - (costo_anterior_A + costo_anterior_B);
    } else {
        this->_distanciaTotal += costo_nuevo_A - costo_anterior_A;
        otraRuta._distanciaTotal += costo_nuevo_B - costo_anterior_B;
    } 
}

bool Route::relocateCliente(Route& otraRuta, NodeRoute* cliente, NodeRoute* destinoPrev, const std::vector<int>& demandas, const vector<vector<double>>& distancias ) {
    if (!cliente || !cliente->anterior || !cliente->siguiente || !destinoPrev || !destinoPrev->siguiente) {
        return false;
    }

    int id_i = cliente->id;
    int id_p = cliente->anterior->id;
    int id_q = cliente->siguiente->id;
    int id_j = destinoPrev->id;
    NodeRoute* destinoNext = destinoPrev->siguiente;
    int id_k = destinoNext->id;

    // Si intenta insertarse a sí mismo o en una posición adyacente que no genera cambio
    if (cliente == destinoPrev || cliente == destinoNext) {
        return false;
    }

    double costo_actual = distancias[id_p][id_i] + distancias[id_i][id_q] + distancias[id_j][id_k];
    double costo_nuevo = distancias[id_p][id_q] + distancias[id_j][id_i] + distancias[id_i][id_k];

    int demanda_i = demandas[id_i];

    if (this != &otraRuta) {
        if (otraRuta._demandaTotal + demanda_i > otraRuta._capacidad) return false;
        if (costo_nuevo >= costo_actual) return false;
    }

    // Quitar cliente de su lugar
    cliente->anterior->siguiente = cliente->siguiente;
    cliente->siguiente->anterior = cliente->anterior;

    // Insertar cliente en nueva posición
    cliente->anterior = destinoPrev;
    cliente->siguiente = destinoNext;
    destinoPrev->siguiente = cliente;
    destinoNext->anterior = cliente;

    if (this != &otraRuta) {
        this->_demandaTotal -= demanda_i;
        otraRuta._demandaTotal += demanda_i;
        this->_distanciaTotal += distancias[id_p][id_q] - distancias[id_p][id_i] - distancias[id_i][id_q];
        otraRuta._distanciaTotal += distancias[id_j][id_i] + distancias[id_i][id_k] - distancias[id_j][id_k];
    } else {
        this->_distanciaTotal += costo_nuevo - costo_actual;
    }

    return true;
}


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

Route::Route(const Route& other) {
    this->_demandaTotal = other._demandaTotal;
    this->_capacidad = other._capacidad;
    this->_distanciaTotal = other._distanciaTotal;
    
    // Copiar la lista enlazada
    this->_raiz = nullptr;
    this->_ultimo = nullptr;
    
    if (other._raiz != nullptr) {
        // Copiar el primer nodo (depot)
        this->_raiz = new NodeRoute{other._raiz->id, other._raiz->demanda, nullptr, nullptr};
        
        NodeRoute* actualOrigen = other._raiz->siguiente;
        NodeRoute* actualDestino = this->_raiz;
        
        // Copiar todos los nodos intermedios
        while (actualOrigen != nullptr) {
            NodeRoute* nuevoNodo = new NodeRoute{actualOrigen->id, actualOrigen->demanda, actualDestino, nullptr};
            actualDestino->siguiente = nuevoNodo;
            
            if (actualOrigen == other._ultimo) {
                this->_ultimo = nuevoNodo;
            }
            
            actualDestino = nuevoNodo;
            actualOrigen = actualOrigen->siguiente;
        }
    }
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