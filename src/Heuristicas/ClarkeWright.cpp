#include "Heuristicas.h"

// Función auxiliar para obtener un vector de Saving dada un vector de distancias inicial,
// ordenado de mayor a menor
vector<Saving> calcularSavings(const vector<vector<double>>& distancias, int depotId) {
    vector<Saving> savings;
    int n = distancias.size();

    for (int i = 0; i < static_cast<int>(n); ++i) {
        if (i != depotId) {
            for (int j = i + 1; j < n; ++j) {
                if (j != depotId) {
                    double sij = distancias[depotId][i] + distancias[depotId][j] - distancias[i][j]; // calcula el ahorro
                    savings.push_back({i, j, sij});
                }
            }
        }
    }

    sort(savings.begin(), savings.end()); // ordena de mayor a menor

    return savings;
}

vector<Route> Heuristicas::clarkeWright() {
    const vector<vector<double>>& distancias = this->_instancia.getDistanceMatrix();
    const vector<int>& demandas = this->_instancia.getDemands();
    int depot = this->_instancia.getDepotId(); 
    int capacidad = this->_instancia.getCapacity();
    int n = this->_instancia.getDimension();
    int k = this->_instancia.getNumVehicles();

    unordered_map<int, Route> rutaCliente;
    for (int i = 1; i <= n; i++) {
        if (i != depot) {
            // inicializamos cada ruta como {depot, i, depot}
            NodoCliente* raiz = new NodoCliente{depot, nullptr, nullptr};
            
            NodoCliente* nodo2 = new NodoCliente{i, raiz, nullptr};   
            raiz->siguiente = nodo2; 

            NodoCliente* nodo3 = new NodoCliente{depot, nodo2, nullptr}; 
            nodo2->siguiente = nodo3;    
            NodoCliente* ultimo = nodo3;

            Route ruta = Route{raiz, ultimo, demandas[i], capacidad};
            rutaCliente[i] = ruta;
        }
    }

    vector<Saving> savings = calcularSavings(distancias, depot);

    for (int s = 0; s < static_cast<int>(savings.size()); s++) { // recorre los savings desde el mayor al menor
        int i = savings[s].i;
        int j = savings[s].j;

        if (rutaCliente.find(i) != rutaCliente.end() 
            && rutaCliente.find(j) != rutaCliente.end()) { 
            // si no se encuentran en el mapa, ya están mapeados en otra ruta distinta y no los considero
            Route ruta_i = rutaCliente[i];
            Route ruta_j = rutaCliente[j];

            // chequeo de solapamiento
            if (ruta_i.ultimo->anterior->id == i // si el cliente i es el último a visitar en la ruta_i le puedo seguir agregando rutas
                && ruta_j.raiz->siguiente->id == j) { // si j es el primer cliente que se visita en esta ruta se puede agregar la ruta_j a la ruta_i
                int demandaAgregada = ruta_i.totalDemand + ruta_j.totalDemand;
                if (demandaAgregada <= capacidad) { // si la nueva demanda no supera la capacidad agregamos ruta_j a ruta_i
                    NodoCliente* ultimo_ruta_i = ruta_i.ultimo;
                    NodoCliente* raiz_ruta_j = ruta_j.raiz;

                    // conectamos el último cliente de ruta_i con el primero de ruta_j
                    ruta_j.raiz->siguiente->anterior = ruta_i.ultimo->anterior; 
                    ruta_i.ultimo->anterior->siguiente = ruta_j.raiz->siguiente;
                    ruta_i.ultimo = ruta_j.ultimo; // la ruta_i recorre hasta el final de la ruta_j
                    
                    ultimo_ruta_i->anterior = nullptr;
                    ultimo_ruta_i->siguiente = nullptr;
                    delete(ultimo_ruta_i); // desconectamos y eliminamos el último nodo de ruta_i
                    raiz_ruta_j->anterior = nullptr;
                    raiz_ruta_j->siguiente = nullptr;
                    delete(raiz_ruta_j); // desconectamos y eliminamos el primer nodo de ruta_j

                    ruta_i.totalDemand = demandaAgregada;
                    rutaCliente[i] = ruta_i;
                    rutaCliente.erase(j); // eliminamos ruta_j del mapa cuando ya es parte de ruta_i
                }
            }
        }
    }
    
    vector<Route> solucion = {};
    for (auto it = rutaCliente.begin(); it != rutaCliente.end(); ++it) {
        solucion.push_back(it->second);
    }

    // Si k = 0, significa que no hay restricción de vehículos
    if (k != 0 && static_cast<int>(solucion.size()) > k) {
        return {}; // si la cantidad de vehículos en la solución sobrepasa el límite de vehículos, no existe sol óptima
    } else {
        return solucion;
    }
    
}