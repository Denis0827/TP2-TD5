#include "Heuristicas.h"

// Función auxiliar para obtener un vector de Saving dada un vector de distancias inicial,
// ordenado de mayor a menor
vector<Saving> calcularSavings(const vector<vector<double>>& distancias, int depotId) {
    vector<Saving> savings;
    int n = distancias.size();

    for (int i = 1; i < static_cast<int>(n); i++) {
        if (i != depotId) {
            for (int j = i + 1; j < static_cast<int>(n); j++) {
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

void imprimirSavings(const std::vector<Saving>& savings) {
    for (const auto& s : savings) {
        std::cout << "i: " << s.i << ", j: " << s.j << ", saving: " << s.saving << std::endl;
    }
}

auto imprimirRutasMapa = [](const unordered_map<int, tuple<Route, NodoCliente*>>& rutas) {
    std::cout << "Estado actual de las rutas en el mapa:" << std::endl;
    for (const auto& par : rutas) {
        std::cout << "Cliente clave: " << par.first
                  << " | Padre: " << get<1>(par.second)
                  << " | Ruta: ";
        NodoCliente* actual = get<0>(par.second).raiz;
        while (actual != nullptr) {
            std::cout << actual->id;
            if (actual->siguiente != nullptr) {
                std::cout << "(->" << actual->siguiente->id << ") ";
            } else {
                std::cout << "(->null) ";
            }
            actual = actual->siguiente;
        }
        std::cout << "| Demanda total: " << get<0>(par.second).demandaTotal << std::endl;
    }
    std::cout << "-------------------------" << std::endl;
};

vector<Route> Heuristicas::clarkeWright() {
    const vector<vector<double>>& distancias = this->_instancia.getDistanceMatrix();
    const vector<int>& demandas = this->_instancia.getDemands();
    int depot = this->_instancia.getDepotId(); 
    int capacidad = this->_instancia.getCapacity();
    int n = this->_instancia.getDimension();
    int k = this->_instancia.getNumVehicles();

    unordered_map<int, tuple<Route, NodoCliente*>> rutaCliente; 
    // cada cliente tiene su ruta al principio, junto con un valor con su id dentro de la tupla
    // mientras se actualiza el mapa, si el cliente está ahora en otra ruta, la ruta se vacía,
    // y el valor de id de la tupla se actualiza al id del cliente cuya ruta tiene incluido ese cliente
    
    for (int i = 1; i <= n; i++) {
        if (i != depot) {
            // inicializamos cada ruta como {depot, i, depot}
            NodoCliente* raiz = new NodoCliente{depot, nullptr, nullptr};
            
            NodoCliente* nodo2 = new NodoCliente{i, raiz, nullptr};   
            raiz->siguiente = nodo2; 

            NodoCliente* nodo3 = new NodoCliente{depot, nodo2, nullptr}; 
            nodo2->siguiente = nodo3;    
            NodoCliente* ultimo = nodo3;

            double distancia_total = distancias[depot][i] * 2;
            Route ruta = Route{raiz, ultimo, demandas[i], capacidad - demandas[i], distancia_total};
            rutaCliente[i] = make_tuple(ruta, raiz);
        }
    }

    vector<Saving> savings = calcularSavings(distancias, depot);

    imprimirSavings(savings);
    std::cout << "===" << endl;

    bool combinar;

    for (int s = 0; s < static_cast<int>(savings.size()); s++) { // recorre los savings desde el mayor al menor
        int i = savings[s].i;
        int j = savings[s].j;

        combinar = false;
        Route ruta_i;
        Route ruta_j;

        // chequeo solapamiento
        if ((get<0>(rutaCliente[get<1>(rutaCliente[i])->siguiente->id])).ultimo->anterior->id == i && get<1>(rutaCliente[j])->siguiente->id == j) {
            combinar = true;
            
            cout << (get<0>(rutaCliente[get<1>(rutaCliente[i])->siguiente->id])).ultimo->anterior->anterior->id << endl;

            cout << (get<0>(rutaCliente[get<1>(rutaCliente[i])->siguiente->id])).ultimo->anterior->id << endl;
            ruta_i = get<0>(rutaCliente[get<1>(rutaCliente[i])->siguiente->id]);
            ruta_j = get<0>(rutaCliente[j]);
        }

        if (combinar) { 
            std::cout << "[if 1] Considerando clientes i=" << i << " y j=" << j << std::endl;
            // si no se encuentran en el mapa, ya están mapeados en otra ruta distinta y no los considero

            // chequeo de solapamiento
            //if (ruta_i.ultimo->anterior->id == i // si el cliente i es el último a visitar en la ruta_i le puedo seguir agregando rutas
            //    && ruta_j.raiz->siguiente->id == j) { // si j es el primer cliente que se visita en esta ruta se puede agregar la ruta_j a la ruta_i
            std::cout << "[if 2] Solapamiento válido para i=" << i << " y j=" << j << std::endl;

            int demandaAgregada = ruta_i.demandaTotal + ruta_j.demandaTotal;
            if (demandaAgregada <= capacidad) { // si la nueva demanda no supera la capacidad agregamos ruta_j a ruta_i
                std::cout << "[if 3] Demanda agregada (" << demandaAgregada << ") <= capacidad (" << capacidad << ") para i=" << i << " y j=" << j << std::endl;
                NodoCliente* ultimo_ruta_i = ruta_i.ultimo;
                NodoCliente* raiz_ruta_j = ruta_j.raiz;

                int demandaRutaJ = ruta_j.demandaTotal;
                int distanciaRutaJ = ruta_j.distanciaTotal;

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

                ruta_i.demandaTotal = demandaAgregada;
                ruta_i.capacidadRestante -= demandaRutaJ;
                ruta_i.distanciaTotal += distanciaRutaJ + distancias[i][j] - distancias[depot][i] - distancias[depot][j];
                
                NodoCliente* padre_i = get<1>(rutaCliente[i]);
                if (padre_i->siguiente->id == i) {
                    rutaCliente[i] = make_tuple(ruta_i, padre_i);
                } else {
                    rutaCliente[padre_i->siguiente->id] = make_tuple(ruta_i, padre_i);
                }

                
                rutaCliente[j] = make_tuple(Route(), padre_i);

                imprimirRutasMapa(rutaCliente);
                cout << "=====" << endl;
            }
        }
    }
    
    vector<Route> solucion = {};
    for (auto it = rutaCliente.begin(); it != rutaCliente.end(); ++it) {
        solucion.push_back(get<0>(it->second));
    }

    // Si k = 0, significa que no hay restricción de vehículos
    if (k != 0 && static_cast<int>(solucion.size()) > k) {
        return solucion; // si la cantidad de vehículos en la solución sobrepasa el límite de vehículos, no existe sol óptima
    } else {
        return solucion;
    }
    
}