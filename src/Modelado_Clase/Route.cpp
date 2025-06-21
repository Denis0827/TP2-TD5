#include "Route.h"

Route::Route() {
    this->_raiz = nullptr;
    this->_ultimo = nullptr;
    this->_demandaTotal = 0;
    this->_capacidad = 0;
    this->_distanciaTotal = 0;  
}

Route::Route(int capacidad) {
    this->_raiz = nullptr;
    this->_ultimo = nullptr;
    this->_demandaTotal = 0;
    this->_capacidad = capacidad;
    this->_distanciaTotal = 0;
}

int Route::getClientePadreId() const {
    return this->_raiz->siguiente->id;
}

int Route::getClienteFinalId() const {
    return this->_ultimo->anterior->id;
}

int Route::getDemandaTotal() const {
    return this->_demandaTotal;
}

int Route::getCapacidadRestante() const {
    return this->_capacidad - this->_demandaTotal;
}

double Route::getDistanciaTotal() const {
    return this->_distanciaTotal;
}

NodoCliente* Route::getRaiz() {
    return this->_raiz;
}

NodoCliente* Route::getUltimo() {
    return this->_ultimo;
}

const NodoCliente* Route::getRaizSinMod() const {
    return this->_raiz;
}

const NodoCliente* Route::getUltimoSinMod() const {
    return this->_ultimo;
}

void Route::agregarDepot(int depot) {
    this->_raiz = new NodoCliente{depot, nullptr, nullptr};
    this->_ultimo = new NodoCliente{depot, this->_raiz, nullptr};
    this->_raiz->siguiente = this->_ultimo;
}

// Pre: debe estar el depot como raiz y ultimo en la ruta
void Route::agregarClienteInicio(int id, int demanda, double distancia) {
    NodoCliente* primerCliente = this->_raiz->siguiente;
    NodoCliente* nuevo = new NodoCliente{id, this->_raiz, primerCliente};

    this->_raiz->siguiente = nuevo;
    if (primerCliente != this->_ultimo) {
        primerCliente->anterior = nuevo;
        this->_distanciaTotal += distancia;
    } else { // caso particular donde solo tenemos el depot en raiz y ultimo
        this->_ultimo->anterior = nuevo;
        this->_distanciaTotal += distancia * 2; // {depot, id, depot}
    }

    this->_demandaTotal += demanda;
}

// Pre: debe estar el depot como raiz y ultimo en la ruta
void Route::agregarClienteFinal(int id, int demanda, double dist_ij, double dist_depj, double dist_depi) {
    NodoCliente* ultimoCliente = this->_ultimo->anterior;
    NodoCliente* nuevo = new NodoCliente{id, ultimoCliente, this->_ultimo};

    this->_ultimo->anterior = nuevo;
    if (ultimoCliente != this->_raiz) {
        ultimoCliente->siguiente = nuevo;
        this->_distanciaTotal += dist_ij + dist_depj - dist_depi;
    } else { // caso particular donde solo tenemos el depot en raiz y ultimo
        this->_raiz->siguiente = nuevo;
        this->_distanciaTotal += dist_depi * 2; // {depot, id, depot}
    }

    this->_demandaTotal += demanda;
}

// Pre: para las distancias, i es el último cliente de la ruta actual, el cliente j es el primero de otraRuta
void Route::unirRutas(Route& otraRuta, double dist_ij, double dist_depi, double dis_depj) {
    NodoCliente* ultimo_ruta_i = this->_ultimo;
    NodoCliente* raiz_ruta_j = otraRuta.getRaiz();

    // conectamos el último cliente de ruta_i con el primero de ruta_j
    raiz_ruta_j->siguiente->anterior = ultimo_ruta_i->anterior;
    ultimo_ruta_i->anterior->siguiente = raiz_ruta_j->siguiente;
    this->_ultimo = otraRuta.getUltimo(); // la ruta_i recorre hasta el final de la ruta_j

    ultimo_ruta_i->anterior = nullptr;
    ultimo_ruta_i->siguiente = nullptr;
    delete(ultimo_ruta_i); // desconectamos y eliminamos el último nodo de ruta_i
    raiz_ruta_j->anterior = nullptr;
    raiz_ruta_j->siguiente = nullptr;
    delete(raiz_ruta_j); // desconectamos y eliminamos el primer nodo de ruta_j

    this->_demandaTotal += otraRuta.getDemandaTotal();
    this->_distanciaTotal += otraRuta.getDistanciaTotal() + dist_ij - dist_depi - dis_depj; 
}

void Route::imprimirRuta() const {
    cout << "Ruta: ";
    const NodoCliente* actual = this->getRaizSinMod();
    while (actual != nullptr) {
        cout << actual->id << " ";
        actual = actual->siguiente;
    }
    cout << "| Demanda total: " << this->getDemandaTotal()
              << " | Capacidad restante: " << this->getCapacidadRestante()
              << " | Distancia total: " << this->getDistanciaTotal()
              << endl;
}