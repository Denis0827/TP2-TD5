#include "Solution.h"

Solution::Solution() {
    this->_rutas = {}; // O(1)
    this->_cantidad_rutas = 0; // O(1)
    this->_cantidad_camiones = 0;
    this->_ultimo_id = 0;
    this->_algoritmo = "";
    this->_instanciaCVRP = "";
}

Solution::Solution(int cantidad_camiones, string algoritmo, string instancia) {
    this->_rutas = {}; // O(1)
    this->_cantidad_rutas = 0; // O(1)
    this->_cantidad_camiones = cantidad_camiones;
    this->_ultimo_id = 0;
    this->_algoritmo = algoritmo;
    this->_instanciaCVRP = instancia;
}

bool Solution::esFactible() const {
    return this->_cantidad_camiones == 0 || this->_cantidad_rutas <= this->_cantidad_camiones;
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

    for (int i = 0; i < static_cast<int>(this->_rutas.size()); i++) {
        cout << "Ruta " << id++ << ": ";
        Route* ruta = get<1>(this->_rutas[i]);
        ruta->imprimirRuta();
        distanciaTotalGlobal += ruta->getDistanciaTotal();
    }
    cout << "Distancia total sumada de todas las rutas: " << distanciaTotalGlobal << endl;
}

void Solution::exportarSolutionParcial(const vector<Node>& nodos, int numero_iteracion) {
    string algoritmo;

    if (this->_algoritmo == "ClarkeWright") {
        algoritmo = "CW";
    } else if (this->_algoritmo == "NearestNeighbor") {
        algoritmo = "NN";
    } else if (this->_algoritmo == "ClarkeWright + Swap (FirstImprovement)") {
        algoritmo = "CWSwap_FI";
    } else if (this->_algoritmo == "ClarkeWright + Swap (BestImprovement)") {
        algoritmo = "CWSwap_BI";
    } else if (this->_algoritmo == "NearestNeighbor + Swap (FirstImprovement)") {
        algoritmo = "NNSwap_FI";
    } else if (this->_algoritmo == "NearestNeighbor + Swap (BestImprovement)") {
        algoritmo = "NNSwap_BI";
    }

    std::ostringstream filename;
    filename << "animaciones/csv_exportados/" << this->_instanciaCVRP << "_" << algoritmo << "_" << (numero_iteracion+1) << ".csv";
    std::ofstream out(filename.str());

    double distanciaTotalGlobal = 0.0;
    for (int i = 0; i < this->_cantidad_rutas; i++) {
        distanciaTotalGlobal += get<1>(this->_rutas[i])->getDistanciaTotal();
    }

    out << "# Instancia: " << this->_instanciaCVRP << std::endl;
    out << "# Algoritmo: " << this->_algoritmo << std::endl;
    out << "# Distancia total: " << distanciaTotalGlobal << std::endl;

    // Primero, exporta todos los nodos como puntos aislados
    for (const auto& nodo : nodos) {
        out << nodo.x << "," << nodo.y << "," << nodo.x << "," << nodo.y << "," << nodo.id << "," << nodo.id << ",0\n";
    }

    // Luego, exporta las aristas de las rutas
    for (const auto& ruta_tuple : this->_rutas) {
        Route* ruta = get<1>(ruta_tuple);
        const NodeRoute* actual = ruta->getRaiz();
        while (actual && actual->siguiente) {
            const Node& from = nodos[actual->id - 1];
            const Node& to = nodos[actual->siguiente->id - 1];
            out << from.x << "," << from.y << "," << to.x << "," << to.y << "," << actual->id << "," << actual->siguiente->id << "," << ruta->getClientePadreId() << "\n";
            actual = actual->siguiente;
        }
    }
    out.close();
}

// Método para obtener una ruta por índice
Route* Solution::getRutaByIndex(int index) {
    if (index >= 0 && index < this->_cantidad_rutas) {
        return get<1>(this->_rutas[index]);
    }
    return nullptr;
}

// Método para obtener el costo total de la solución
double Solution::getCostoTotal() const {
    double costoTotal = 0.0;
    for (int i = 0; i < this->_cantidad_rutas; i++) {
        Route* ruta = get<1>(this->_rutas[i]);
        costoTotal += ruta->getDistanciaTotal();
    }
    return costoTotal;
}

// Constructor de copia
Solution::Solution(const Solution& other) {
    this->_cantidad_rutas = other._cantidad_rutas;
    this->_cantidad_camiones = other._cantidad_camiones;
    this->_ultimo_id = other._ultimo_id;
    this->_algoritmo = other._algoritmo;
    
    // Copiar las rutas (copia profunda)
    for (int i = 0; i < other._cantidad_rutas; i++) {
        Route* rutaOriginal = get<1>(other._rutas[i]);
        Route* nuevaRuta = new Route(*rutaOriginal); // Necesitarás implementar constructor de copia en Route
        this->_rutas.push_back(make_tuple(get<0>(other._rutas[i]), nuevaRuta));
    }
}

// Operador de asignación
Solution& Solution::operator=(const Solution& other) {
    if (this != &other) {
        // Limpiar rutas existentes
        for (int i = 0; i < this->_cantidad_rutas; i++) {
            delete get<1>(this->_rutas[i]);
        }
        this->_rutas.clear();
        
        // Copiar del otro objeto
        this->_cantidad_rutas = other._cantidad_rutas;
        this->_cantidad_camiones = other._cantidad_camiones;
        this->_ultimo_id = other._ultimo_id;
        this->_algoritmo = other._algoritmo;
        
        // Copiar las rutas
        for (int i = 0; i < other._cantidad_rutas; i++) {
            Route* rutaOriginal = get<1>(other._rutas[i]);
            Route* nuevaRuta = new Route(*rutaOriginal);
            this->_rutas.push_back(make_tuple(get<0>(other._rutas[i]), nuevaRuta));
        }
    }
    return *this;
}

// Destructor
Solution::~Solution() {
    for (int i = 0; i < this->_cantidad_rutas; i++) {
        delete get<1>(this->_rutas[i]);
    }
}

// --- Getter Implementations ---

vector<tuple<int, Route*>> Solution::getRutas() const { return this->_rutas; } // O(1)
int Solution::getCantidadRutas() const { return this->_cantidad_rutas; } // O(1)
int Solution::getCantidadCamiones() const { return this->_cantidad_camiones; } // O(1)
string Solution::getAlgoritmo() const { return this->_algoritmo; } // O(1)