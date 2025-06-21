#include "Heuristicas.h"

// Función auxiliar para obtener un vector de Saving dada un vector de distancias inicial,
// ordenado de mayor a menor
vector<Saving> calcularSavings(const vector<vector<double>>& distancias, int depotId) {
    vector<Saving> savings;
    int n = distancias.size();

    for (int i = 1; i < static_cast<int>(n); i++) { // arranco desde 1 porque no existe id = 0
        if (i != depotId) { // no calculo saving para depot
            for (int j = i + 1; j < static_cast<int>(n); j++) { // para los anteriores de i, ya habré chequeado en otra iteración anterior
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

// Dado un cliente, encuentra la raíz padre de la ruta donde se encuentra el cliente
// Compresión de ruta: para cada hijo de la raíz, actualizamos su padre a la raíz si no está hecho
// Ejemplo: dado 1->1, 2->1, 3->2, 4->3, si ejecuto findPadre(3) termino teniendo 1->1, 2->1, 3->1, 4->1
int findPadre(vector<int>& padres_ruta, int cliente) {
    if (padres_ruta[cliente] != cliente) {
        padres_ruta[cliente] = findPadre(padres_ruta, padres_ruta[cliente]); // compresión de ruta para todos los clientes conectados
    } 
    return padres_ruta[cliente]; // es la raiz
}

// unionFind nos ayudará a decidir cómo unir después las rutas de manera más óptima
// devuelve una tupla con las dos raices, donde la primera es de la ruta a la que vamos a unir ambas rutas
// y la segunda es de la ruta que borraremos
tuple<int, int> unionFind(vector<int>& padres_ruta, vector<int>& rango_cliente, int i, int j) {
    int raiz_i = findPadre(padres_ruta, i);
    int raiz_j = findPadre(padres_ruta, j);
    int rango_i = rango_cliente[raiz_i];
    int rango_j = rango_cliente[raiz_j];

    if (rango_i < rango_j) { // la idea de UnionFind es reducir tiempos de cómputo, agregamos la ruta chica a la grande
        padres_ruta[raiz_i] = raiz_j; 
        // ahora el primer cliente de la ruta borrada está conectado a la ruta grande unida, entonces en el próximo
        // UnionFind los clientes de la ruta borrada empezarán a apuntar también a la ruta grande con findPadre()
        return make_tuple(raiz_j, raiz_i);
    } else if (rango_i > rango_j) {
        padres_ruta[raiz_j] = raiz_i;
        return make_tuple(raiz_i, raiz_j);
    } else {
        padres_ruta[raiz_j] = raiz_i;
        rango_cliente[raiz_i]++; // unimos dos rutas con igual rango, aumentamos en 1
        return make_tuple(raiz_i, raiz_j);
    }
}

bool chequeoSolapamiento(Route ruta_i, Route ruta_j, int i, int j) {
    return ruta_i.raiz->siguiente->id != ruta_j.raiz->siguiente->id && // si son iguales son la misma ruta
        ruta_i.ultimo->anterior->id == i && ruta_j.raiz->siguiente->id == j; // solapamiento
}

// Función auxiliar para unir ruta_i y ruta_j en ruta_i
// Pre: el cliente i es el último de la ruta_i (sin depot), el cliente j es el primero de la ruta_j (sin depot)
void unionRutas(const vector<vector<double>>& distancias, Route& ruta_i, Route& ruta_j, int i, int j, int depot) {
    NodoCliente* ultimo_ruta_i = ruta_i.ultimo;
    NodoCliente* raiz_ruta_j = ruta_j.raiz;

    int demanda_ruta_i = ruta_i.demandaTotal;
    int demanda_ruta_j = ruta_j.demandaTotal;
    double distancia_ruta_j = ruta_j.distanciaTotal;

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

    ruta_i.demandaTotal = demanda_ruta_i + demanda_ruta_j;
    ruta_i.capacidadRestante -= demanda_ruta_j;
    ruta_i.distanciaTotal += distancia_ruta_j + distancias[i][j] - distancias[depot][i] - distancias[depot][j];
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

            double distancia_total = distancias[depot][i] * 2; // la distancia es la misma en la ida y vuelta
            Route ruta = Route{raiz = raiz, ultimo = ultimo, demandas[i], capacidad - demandas[i], distancia_total};
            rutaCliente[i] = ruta;
        }
    }

    vector<Saving> savings = calcularSavings(distancias, depot);
    vector<int> padres_ruta(n + 1, 0); // cada cliente i es la raiz de la ruta donde esta el cliente i
    vector<int> rango_cliente(n + 1, 0); // representa el costo de recorrido hasta llegar a la raiz, el rango de un cliente
    // aumenta si unimos dos rutas con el mismo rango

    for (int i = 0; i < static_cast<int>(padres_ruta.size()); i++) {
        padres_ruta[i] = i; // al inicio cada cliente apunta a su propia ruta
    }

    for (int s = 0; s < static_cast<int>(savings.size()); s++) { // recorre los savings desde el mayor al menor
        int i = savings[s].i;
        int j = savings[s].j;

        Route ruta_i = rutaCliente[findPadre(padres_ruta, i)]; // encuentra la ruta de la raiz
        // acá aprovecha a actualizar los padres de la subruta para que apunten bien a la ruta que corresponde
        Route ruta_j = rutaCliente[findPadre(padres_ruta, j)];
        bool solapado = true;
        bool invertir = false;

        if (chequeoSolapamiento(ruta_i, ruta_j, i, j)) {
            solapado = false;
        } else if (chequeoSolapamiento(ruta_j, ruta_i, j, i)) { // chequeo si hay solapamiento invertido
            solapado = false;
            invertir = true;
        }

        if (!solapado) { 
            int demandaAgregada = ruta_i.demandaTotal + ruta_j.demandaTotal;
            if (demandaAgregada <= capacidad) { // si la nueva demanda no supera la capacidad agregamos ruta_j a ruta_i
                tuple<int, int> res_unionFind = unionFind(padres_ruta, rango_cliente, i, j);
                int cliente_padre = get<0>(res_unionFind);
                int cliente_borrar = get<1>(res_unionFind);                  

                if (!invertir) {
                    unionRutas(distancias, ruta_i, ruta_j, i, j, depot);
                    rutaCliente[cliente_padre] = ruta_i;
                    rutaCliente.erase(cliente_borrar);
                } else {
                    unionRutas(distancias, ruta_j, ruta_i, j, i, depot);
                    rutaCliente[cliente_padre] = ruta_j;
                    rutaCliente.erase(cliente_borrar);
                }
            } 
        } 
    }
    
    vector<Route> solucion = {};
    for (auto it = rutaCliente.begin(); it != rutaCliente.end(); ++it) {
        solucion.push_back((it->second));
    }

    // Si k = 0, significa que no hay restricción de vehículos
    if (k != 0 && static_cast<int>(solucion.size()) > k) {
        return {}; // si la cantidad de vehículos en la solución sobrepasa el límite de vehículos, no existe sol óptima
    } else {
        return solucion;
    }
    
}