#include "Solution.h"

Solution::Solution() {
    this->_rutas = {}; // O(1)
    this->_cantidad_rutas = 0; // O(1)
    this->_cantidad_camiones = 0; // O(1)
    this->_ultimo_id = 0; // O(1)
    this->_algoritmo = ""; // O(1)
    this->_instanciaCVRP = ""; // O(1)
}
// Complejidad total: O(1)

Solution::Solution(int cantidad_camiones, string algoritmo, string instancia) {
    this->_rutas = {}; // O(1)
    this->_cantidad_rutas = 0; // O(1)
    this->_cantidad_camiones = cantidad_camiones; // O(1)
    this->_ultimo_id = 0; // O(1)
    this->_algoritmo = algoritmo; // O(1)
    this->_instanciaCVRP = instancia; // O(1)
}
// Complejidad total: O(1)

bool Solution::esFactible() const {
    return this->_cantidad_camiones == 0 || this->_cantidad_rutas <= this->_cantidad_camiones; // O(1)
}
// Complejidad total: O(1)

vector<tuple<NodeRoute*, Route*>> Solution::getAllClientesSol() const {
    vector<tuple<NodeRoute*, Route*>> clientes; // O(1)
    for (int i = 0; i < this->_cantidad_rutas; i++) { // O(K)
        Route* ruta = this->_rutas[i]; // O(1)
        NodeRoute* actual = ruta->getRaizModify()->siguiente; // O(1)
        while (actual != ruta->getUltimo()) { // O(M) por ruta
            clientes.push_back(make_tuple(actual, ruta)); // O(1)
            actual = actual->siguiente; // O(1)
        }
    }
    return clientes; // O(1)
}
// Complejidad total: O(N), donde N es el total de clientes en todas las rutas

double Solution::getDistanciaTotal() const {
    double distancia = 0.0; // O(1)
    for (int i = 0; i < this->_cantidad_rutas; i++) { // O(K)
        Route* ruta = this->_rutas[i]; // O(1)
        distancia += ruta->getDistanciaTotal(); // O(1)
    }
    return distancia; // O(1)
}
// Complejidad total: O(K)

void Solution::setAlgoritmo(string algoritmo) {
    this->_algoritmo = algoritmo; // O(1)
}
// Complejidad total: O(1)

void Solution::agregarRuta(Route* ruta) {
    this->_rutas.push_back(ruta); // O(1) amortizado
    this->_cantidad_rutas++; // O(1)
    this->_ultimo_id++; // O(1)
}
// Complejidad total: O(1)

void Solution::eliminarRutasVacias() {
    vector<Route*> nuevasRutas; // O(1)
    for (int i = 0; i < this->_cantidad_rutas; i++) { // O(K)
        Route* ruta = this->_rutas[i]; // O(1)
        if (ruta->getAllClientes().size() > 0) { // O(M) por ruta
            nuevasRutas.push_back(ruta); // O(1)
        }
    }
    this->_rutas = nuevasRutas; // O(1)
    this->_cantidad_rutas = this->_rutas.size(); // O(1)
}
// Complejidad total: O(N), siendo N la cantidad total de clientes

void Solution::imprimirSolution() {
    if (!this->esFactible()) { // O(1)
        cout << "No existe ninguna solución óptima utilizando este algoritmo" << endl; // O(1)
    }

    int id = 1; // O(1)
    double distanciaTotalGlobal = 0.0; // O(1)

    for (int i = 0; i < static_cast<int>(this->_rutas.size()); i++) { // O(K)
        cout << "Ruta " << id++ << ": "; // O(1)
        Route* ruta = this->_rutas[i]; // O(1)
        ruta->imprimirRuta(); // O(M) — recorrido de clientes en la ruta
        distanciaTotalGlobal += ruta->getDistanciaTotal(); // O(1)
    }
    cout << "Distancia total sumada de todas las rutas: " << distanciaTotalGlobal << endl; // O(1)
}
// Complejidad total: O(N)

void Solution::exportarSolutionParcial(const vector<Node>& nodos, int numero_iteracion) {
    string algoritmo; // O(1)

    // Mapear el nombre del algoritmo — O(1)
    if (this->_algoritmo == "ClarkeWright") algoritmo = "CW";
    else if (this->_algoritmo == "NearestNeighbor") algoritmo = "NN";
    else if (this->_algoritmo == "ClarkeWright + Swap (FirstImprovement)") algoritmo = "CWSwap_FI";
    else if (this->_algoritmo == "ClarkeWright + Swap (BestImprovement)") algoritmo = "CWSwap_BI";
    else if (this->_algoritmo == "NearestNeighbor + Swap (FirstImprovement)") algoritmo = "NNSwap_FI";
    else if (this->_algoritmo == "NearestNeighbor + Swap (BestImprovement)") algoritmo = "NNSwap_BI";
    else if (this->_algoritmo == "ClarkeWright + Relocate (FirstImprovement)") algoritmo = "CWRelocate_FI";
    else if (this->_algoritmo == "ClarkeWright + Relocate (BestImprovement)") algoritmo = "CWRelocate_BI";
    else if (this->_algoritmo == "NearestNeighbor + Relocate (FirstImprovement)") algoritmo = "NNRelocate_FI";
    else if (this->_algoritmo == "NearestNeighbor + Relocate (BestImprovement)") algoritmo = "NNRelocate_BI";
    else if (this->_algoritmo == "NearestNeighborRandomized") algoritmo = "NNRandom";
    else if (this->_algoritmo == "GRASP") algoritmo = "GRASP";

    std::ostringstream filename; // O(1)
    filename << "animaciones/csv_exportados/" << this->_instanciaCVRP << "_" << algoritmo << "_" << (numero_iteracion+1) << ".csv"; // O(1)
    std::ofstream out(filename.str()); // O(1)

    double distanciaTotalGlobal = 0.0; // O(1)
    for (int i = 0; i < this->_cantidad_rutas; i++) { // O(K)
        distanciaTotalGlobal += this->_rutas[i]->getDistanciaTotal(); // O(1)
    }

    // Exportar todos los nodos como puntos aislados
    for (const auto& nodo : nodos) { // O(N)
        out << nodo.x << "," << nodo.y << "," << nodo.x << "," << nodo.y << "," << nodo.id << "," << nodo.id << ",0\n"; // O(1)
    }

    // Exportar aristas (enlaces entre nodos)
    for (const auto& ruta_tuple : this->_rutas) { // O(K)
        Route* ruta = ruta_tuple; // O(1)
        const NodeRoute* actual = ruta->getRaiz(); // O(1)
        while (actual && actual->siguiente) { // O(M) por ruta
            const Node& from = nodos[actual->id - 1]; // O(1)
            const Node& to = nodos[actual->siguiente->id - 1]; // O(1)
            out << from.x << "," << from.y << "," << to.x << "," << to.y << "," << actual->id << "," << actual->siguiente->id << "," << ruta->getClientePadreId() << "\n"; // O(1)
            actual = actual->siguiente; // O(1)
        }
    }

    out.close(); // O(1)
}
// Complejidad total: O(N + E), donde N es la cantidad de nodos y E ≈ número de aristas (rutas)

// --- Getter Implementations ---

vector<Route*> Solution::getRutas() const { return this->_rutas; } // O(1)
int Solution::getCantidadRutas() const { return this->_cantidad_rutas; } // O(1)
int Solution::getCantidadCamiones() const { return this->_cantidad_camiones; } // O(1)
string Solution::getAlgoritmo() const { return this->_algoritmo; } // O(1)