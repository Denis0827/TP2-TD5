#include "Route.h"

Route::Route() {
    this->_raiz = nullptr; // O(1)
    this->_ultimo = nullptr; // O(1)
    this->_demandaTotal = 0; // O(1)
    this->_capacidad = 0; // O(1)
    this->_distanciaTotal = 0; // O(1)
}

Route::Route(int capacidad) {
    this->_raiz = nullptr; // O(1)
    this->_ultimo = nullptr; // O(1)
    this->_demandaTotal = 0; // O(1)
    this->_capacidad = capacidad; // O(1)
    this->_distanciaTotal = 0; // O(1)
}

void Route::agregarDepot(int depot) {
    this->_raiz = new NodeRoute{depot, 0, nullptr, nullptr}; // O(1)
    this->_ultimo = new NodeRoute{depot, 0, this->_raiz, nullptr}; // O(1)
    this->_raiz->siguiente = this->_ultimo; // O(1)
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

void Route::swapNodes(Route& otraRuta, NodeRoute* nodoA, NodeRoute* nodoB, const vector<vector<double>>& distancias) {
    bool excede_capacidad = false;
    int demandaA = nodoA->demanda;
    int demandaB = nodoB->demanda;

    // si las rutas no tienen el mismo primer cliente, entonces son distintas rutas
    // chequeo si el swap es valido, es decir, si cambiar el cliente de ruta no me
    // rompe la capacidad
    if (this->_raiz->siguiente->id != otraRuta._raiz->siguiente->id) {
        int nuevaDemandaA = this->_demandaTotal - demandaA + demandaB;
        int nuevaDemandaB = otraRuta.getDemandaTotal() - demandaB + demandaA;

        if (nuevaDemandaA > this->_capacidad || nuevaDemandaB > otraRuta.getCapacidadTotal()) {
            excede_capacidad = true;
        }
    }

    double costo_anterior_A = 0.0;
    double costo_anterior_B = 0.0;
    double costo_nuevo_A = 0.0;
    double costo_nuevo_B = 0.0;

    // si los nodos estan en la misma ruta, no habria conflicto de capacidad al swapear
    if (!excede_capacidad) {
        int cliente_anterior_A = nodoA->anterior->id;
        int cliente_siguiente_A = nodoA->siguiente->id;
        int cliente_anterior_B = nodoB->anterior->id;
        int cliente_siguiente_B = nodoB->siguiente->id;

        costo_anterior_A = distancias[cliente_anterior_A][nodoA->id] + 
                        distancias[nodoA->id][cliente_siguiente_A]; 
        costo_anterior_B = distancias[cliente_anterior_B][nodoB->id] +
                        distancias[nodoB->id][cliente_siguiente_B];

        if (nodoA->siguiente != nodoB) {
            costo_nuevo_A = distancias[cliente_anterior_A][nodoB->id] +
                        distancias[nodoB->id][nodoA->id];
            costo_nuevo_B = distancias[nodoB->id][nodoA->id] +
                        distancias[nodoA->id][cliente_anterior_B];
        } else {
            costo_nuevo_A = distancias[nodoA->id][nodoB->id] +
                    distancias[nodoA->id][cliente_siguiente_B];
            costo_nuevo_B = distancias[cliente_anterior_A][nodoB->id] + 
                    distancias[nodoB->id][nodoA->id];
        }
    }

    if (costo_nuevo_A + costo_nuevo_B < costo_anterior_A + costo_anterior_B) {
        NodeRoute* nodoA_anterior = nodoA->anterior;
        NodeRoute* nodoA_siguiente = nodoA->siguiente;
        NodeRoute* nodoB_anterior = nodoB->anterior;
        NodeRoute* nodoB_siguiente = nodoB->siguiente;

        if (nodoA->siguiente != nodoB) {
            nodoA->anterior = nodoB->anterior;
            nodoA->siguiente = nodoB->siguiente;
            nodoB->anterior = nodoA_anterior;
            nodoB->siguiente = nodoA_siguiente;
        } else {
            nodoA_anterior->siguiente = nodoB;
            nodoB_siguiente->anterior = nodoA;
            nodoA->anterior = nodoB;
            nodoA->siguiente = nodoB_siguiente;
            nodoB->anterior = nodoA_anterior;
            nodoB->siguiente = nodoA;
        }

        this->_demandaTotal += demandaB - demandaA; 
        otraRuta._demandaTotal += demandaA - demandaB;
        this->_distanciaTotal += costo_nuevo_A - costo_anterior_A;
        otraRuta._distanciaTotal += costo_nuevo_B - costo_anterior_B;
    }
}

void Route::imprimirRuta() const {
    cout << "Ruta: "; // O(1)
    const NodeRoute* actual = this->getRaizSinMod(); // O(1)
    while (actual != nullptr) { // O(N)
        cout << actual->id << " "; // O(1)
        actual = actual->siguiente; // O(1)
    }
    cout << "| Demanda total: " << this->getDemandaTotal() // O(1)
              << " | Capacidad restante: " << this->getCapacidadRestante() // O(1)
              << " | Distancia total: " << this->getDistanciaTotal() // O(1)
              << endl; // O(1)
}



void Route::setDemandaTotal(int nuevaDemanda) {
    this->_demandaTotal = nuevaDemanda;
}

// --- Getter Implementations ---

int Route::getClientePadreId() const { return this->_raiz->siguiente->id; } // O(1)
int Route::getClienteFinalId() const { return this->_ultimo->anterior->id; } // O(1)
int Route::getDemandaTotal() const { return this->_demandaTotal; } // O(1)
int Route::getCapacidadTotal() const { return this->_capacidad; } // O(1)
int Route::getCapacidadRestante() const { return this->_capacidad - this->_demandaTotal; } // O(1)
double Route::getDistanciaTotal() const { return this->_distanciaTotal; } // O(1)
NodeRoute* Route::getRaiz() { return this->_raiz; } // O(1)
NodeRoute* Route::getUltimo() { return this->_ultimo; } // O(1)
const NodeRoute* Route::getRaizSinMod() const { return this->_raiz; } // O(1)
const NodeRoute* Route::getUltimoSinMod() const { return this->_ultimo; } // O(!)