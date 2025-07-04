#include "Solution.h"

Solution::Solution() {
    this->_rutas = {}; // O(1)
    this->_cantidad_rutas = 0; // O(1)
    this->_cantidad_camiones = 0;
    this->_ultimo_id = 0;
    this->_algoritmo = "";
}

Solution::Solution(int cantidad_camiones, string algoritmo) {
    this->_rutas = {}; // O(1)
    this->_cantidad_rutas = 0; // O(1)
    this->_cantidad_camiones = cantidad_camiones;
    this->_ultimo_id = 0;
    this->_algoritmo = algoritmo;
}

bool Solution::esFactible() const {
    return this->_cantidad_rutas <= this->_cantidad_camiones;
}

vector<tuple<NodeRoute*, Route*>> Solution::getAllClientesSol() const {
    vector<tuple<NodeRoute*, Route*>> clientes;
    for (int i = 0; i < this->_cantidad_rutas; i++) {
        Route* ruta = get<1>(this->_rutas[i]);
        // Recorremos los clientes de la ruta (excluyendo el depósito)
        NodeRoute* actual = ruta->getRaizModify()->siguiente;
        while (actual != ruta->getUltimo()) {
            clientes.push_back(make_tuple(actual, ruta));
            actual = actual->siguiente;
        }
    }
    return clientes;
}

void Solution::setAlgoritmo(string algoritmo) {
    this->_algoritmo = algoritmo;
}

void Solution::agregarRuta(Route* ruta) {
    this->_rutas.push_back(make_tuple(this->_ultimo_id + 1, ruta));
    this->_cantidad_rutas++;
    this->_ultimo_id++;
} 

void Solution::imprimirSolution() {
    if (!this->esFactible()) {
        cout << "No existe ninguna solución óptima utilizando este algoritmo" << endl;
    }

    int id = 1;
    double distanciaTotalGlobal = 0.0;

    for (int i = 0; i < this->_rutas.size(); i++) {
        cout << "Ruta " << id++ << ": ";
        Route* ruta = get<1>(this->_rutas[i]);
        ruta->imprimirRuta();
        distanciaTotalGlobal += ruta->getDistanciaTotal();
    }
    cout << "Distancia total sumada de todas las rutas: " << distanciaTotalGlobal << endl;
}

void exportarRutasPaso(const std::vector<Route>& solucion, const std::vector<Node>& nodos, int paso) {
    std::ostringstream filename;
    filename << "rutas_" << std::setw(3) << std::setfill('0') << paso << ".csv";
    std::ofstream out(filename.str());

    // Primero, exporta todos los nodos como puntos aislados
    for (const auto& nodo : nodos) {
        // Nodo como origen y destino para que el script Python lo dibuje como punto
        out << nodo.x << "," << nodo.y << "," << nodo.x << "," << nodo.y << "," << nodo.id << "," << nodo.id << ",0\n";
    }

    // Luego, exporta las aristas de las rutas
    for (const auto& ruta : solucion) {
        const NodeRoute* actual = ruta.getRaiz();
        while (actual && actual->siguiente) {
            const Node& from = nodos[actual->id - 1];
            const Node& to = nodos[actual->siguiente->id - 1];
            out << from.x << "," << from.y << "," << to.x << "," << to.y << "," << actual->id << "," << actual->siguiente->id << "," << ruta.getClientePadreId() << "\n";
            actual = actual->siguiente;
        }
    }
    out.close();
}

// --- Getter Implementations ---

vector<tuple<int, Route*>> Solution::getRutas() const { return this->_rutas; } // O(1)
int Solution::getCantidadRutas() const { return this->_cantidad_rutas; } // O(1)
int Solution::getCantidadCamiones() const { return this->_cantidad_camiones; } // O(1)
string Solution::getAlgoritmo() const { return this->_algoritmo; } // O(1)