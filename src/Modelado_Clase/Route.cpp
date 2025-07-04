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

bool Route::swapClientes(Route& otraRuta, NodeRoute* clienteA, NodeRoute* clienteB, const vector<vector<double>>& distancias) {
    // Inicialización de variables
    int demandaA = clienteA->demanda;
    int demandaB = clienteB->demanda;
    double costo_anterior_A, costo_anterior_B, costo_nuevo_A, costo_nuevo_B;

    bool misma_ruta = this->_raiz->siguiente->id == otraRuta._raiz->siguiente->id; // si las rutas no tienen el mismo primer cliente, entonces son distintas rutas
    bool excede_capacidad = false;

    // si los nodos estan en la misma ruta, no habria conflicto de capacidad al swapear
    // pero si estan en distintas rutas, debo chequear si el swap es valido, es decir, 
    // si cambiar el cliente de ruta no me rompe la capacidad
    if (!misma_ruta) {
        int nuevaDemandaA = this->_demandaTotal - demandaA + demandaB;
        int nuevaDemandaB = otraRuta.getDemandaTotal() - demandaB + demandaA;

        if (nuevaDemandaA > this->_capacidad || nuevaDemandaB > otraRuta.getCapacidadTotal()) {
            excede_capacidad = true;
        }
    }
    
    if (!excede_capacidad) { // ahora me asegure que no me rompe la capacidad en ningun caso
        int cliente_anterior_A = clienteA->anterior->id;
        int cliente_siguiente_A = clienteA->siguiente->id;
        int cliente_anterior_B = clienteB->anterior->id;
        int cliente_siguiente_B = clienteB->siguiente->id;

        // Ejemplo ilustrativo A - misma ruta no consecutivos
        // Ruta antes: 0 1 2 3 4 5 0 (swap 2,4)
        // Ruta despues: 0 1 4 3 2 5 0
        // Costo anterior: 1->2 + 2->3 + 3->4 + 4->5
        // Costo nuevo: 1->4 + 4->3 + 3->2 + 2->5

        // Ejemplo ilustrativo B - distintas rutas
        // Rutas antes: (0 1 2 3 0) (0 4 5 6 0) (swap 2,5)
        // Ruta despues: (0 1 5 3 0) (0 4 2 6 0)
        // Costo anterior: 1->2 + 2->3 + 4->5 + 5->6
        // Costo nuevo: 1->5 + 5->3 + 4->2 + 2->6

        // Podemos ver que en los casos de ejemplos A y B buscamos hacer la misma verificacion y asignacion

        // Ejemplo ilustrativo C - misma ruta consecutivos
        // Ruta antes: 0 1 2 3 4 0 (swap 2,3)
        // Ruta despues: 0 1 3 2 4 0
        // Costo anterior: 1->2 + 2->3 + 2->3 + 3->4
        // Costo nuevo: 1->3 + 3-> 2 + 3->2 + 2->4 (notar que 2->3 es igual a 3->2)

        costo_anterior_A = distancias[cliente_anterior_A][clienteA->id] + distancias[clienteA->id][cliente_siguiente_A];
        costo_anterior_B = distancias[cliente_anterior_B][clienteB->id] + distancias[clienteB->id][cliente_siguiente_B];

        if (clienteA->siguiente != clienteB && clienteB->siguiente != clienteA) { // nodos no consecutivos
            costo_nuevo_A = distancias[cliente_anterior_B][clienteA->id] + distancias[clienteA->id][cliente_siguiente_B];
            costo_nuevo_B = distancias[cliente_anterior_A][clienteB->id] + distancias[clienteB->id][cliente_siguiente_A];
        } else { // nodos consecutivos
            if (clienteB == clienteA->siguiente) {
                costo_nuevo_A = distancias[clienteA->id][clienteB->id] + distancias[clienteA->id][cliente_siguiente_B];
                costo_nuevo_B = distancias[cliente_anterior_A][clienteB->id] + distancias[clienteB->id][clienteA->id];
            } else {
                costo_nuevo_A = distancias[clienteA->id][clienteB->id] + distancias[clienteA->id][cliente_anterior_B];
                costo_nuevo_B = distancias[cliente_siguiente_A][clienteB->id] + distancias[clienteB->id][clienteA->id];
            }
        }
    }

    // Chequeo si hay alguna mejora de costo y realizo el swap
    if (!excede_capacidad && costo_nuevo_A + costo_nuevo_B < costo_anterior_A + costo_anterior_B) {
        NodeRoute* clienteA_anterior = clienteA->anterior;
        NodeRoute* clienteA_siguiente = clienteA->siguiente;
        NodeRoute* clienteB_anterior = clienteB->anterior;
        NodeRoute* clienteB_siguiente = clienteB->siguiente;

        // Realizamos el swap, teniendo cuidado con los enlaces entre clientes y tomando como guía los ejemplos
        // ilustrativos mostrados arriba
        if (clienteA->siguiente != clienteB) { // caso NO consecutivos
            clienteA->anterior = clienteB->anterior;
            clienteA->siguiente = clienteB->siguiente;
            clienteB->anterior = clienteA_anterior;
            clienteB->siguiente = clienteA_siguiente;
            
            clienteA_anterior->siguiente = clienteB;
            clienteA_siguiente->anterior = clienteB;
            clienteB_anterior->siguiente = clienteA;
            clienteB_siguiente->anterior = clienteA;
        } else { // caso SI consecutivos
            clienteA->anterior = clienteB;
            clienteA->siguiente = clienteB_siguiente;
            clienteB->anterior = clienteA_anterior;
            clienteB->siguiente = clienteA;

            clienteA_anterior->siguiente = clienteB;
            clienteB_siguiente->anterior = clienteA;
        }

        this->_demandaTotal += demandaB - demandaA;
        otraRuta._demandaTotal += demandaA - demandaB;
        // si fuesen la misma ruta se cancela la suma

        if (misma_ruta) {
            this->_distanciaTotal += (costo_nuevo_A + costo_nuevo_B) - (costo_anterior_A + costo_anterior_B);
        } else {
            this->_distanciaTotal += costo_nuevo_A - costo_anterior_A;
            otraRuta._distanciaTotal += costo_nuevo_B - costo_anterior_B;
        }
        return true;
    } else {
        return false;
    }
}

/*
bool relocateCliente(Route& otraRuta, NodeRoute* cliente, NodeRoute* destinoPrev, const std::vector<int>& demandas, const vector<vector<double>>& distancia ) {
    // entrada: dos rutas. un arco jk (donde voy a insertar el nodo) y dos nodos p y q
    // idea: me fijo q j!= p y k!=q. 

    int id_i = cliente->id;
    int id_p = cliente->anterior->id;
    int id_q = cliente->siguiente->id;
    int id_j = destinoPrev->id;
    int id_k = destinoNext->id;

    NodeRoute* destinoNext = destinoPrev -> id;
    double costo_actual = distancia[id_p][id_i] + distancia[id_i][id_q] + distancia[id_j][id_k];
    double costo_nuevo = distancia[id_p][id_q] + distancia[id_j][id_i] + distancia[id_i][id_k];

    int demanda_i = demandas[id_i];


    //chequeo que no se pase de la capacidad si es que son rutas distintas
    if(this != &otraRuta){
        int nueva_demanda_destino = otraRuta._demandaTotal + demanda_i;
        if (nueva_demanda_destino >otraRuta._capacidad)
            return false; // no es factible
    }

    // si mejora el costo, aplicamos el cambio
    if (costo_nuevo < costo_actual){
        // saco a  i   y conecto p -> q
        cliente -> anterior -> siguiente = cliente -> siguiente;
        cliente -> siguiente -> anterior = cliente -> anterior;

        // lo inserto entre k  y  j
        cliente -> anterior = destinoPrev;
        cliente -> siguiente = destinoNext;
        destinoPrev -> siguiente = cliente;
        destinoNext -> anterior = cliente;

        // actualizo demanda y distancias (individuales por ruta) si las rutas son distintas. (si estoy en la misma ruta la demanda queda igual)
        if (this != otraRuta){
            this -> _demandaTotal -= demanda_i;
            otraRuta._demandaTotal += demanda_i;

            this -> _distanciaTotal += distancia[id_p][id_q] - distancias[id_p][id_i] - distancias[id_i][id_q];  
            otraRuta._distanciaTotal += distancia[id_j][id_i] + distancias[id_i][id_k] - distancias[id_j][id_k];  
        } 

        //si es la misma ruta actualizo solo la distancia
        else{
            this -> _distanciaTotal += costo_nuevo - costo_actual;
        }

        return true;
    } 
    return false;
}
*/

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