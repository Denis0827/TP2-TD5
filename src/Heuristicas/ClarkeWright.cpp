#include "Heuristicas.h"

// Función auxiliar para obtener un vector de Saving dada un vector de distancias inicial,
// ordenado de mayor a menor
vector<Saving> calcularSavings(const vector<vector<double>>& distancias, int depotId) {
    vector<Saving> savings; // O(1)
    int n = distancias.size(); // O(1)

    for (int i = 1; i < static_cast<int>(n); i++) { // arranco desde 1 porque no existe id = 0
        if (i != depotId) { // O(1) no calculo saving para depot
            for (int j = i + 1; j < static_cast<int>(n); j++) { // para los anteriores de i, ya habré chequeado en otra iteración anterior
                if (j != depotId) { // O(1)
                    double sij = distancias[depotId][i] + distancias[depotId][j] - distancias[i][j]; // O(1) calcula el ahorro
                    savings.push_back({i, j, sij}); // O(1)
                }
            }
        }
    }
    // La cantidad de iteraciones depende de cuántos pares distintos se pueden formar a partir de un conjunto de 
    // n elementos, sin repetir y sin importar el orden -> N*(N-1)/2
    // Complejidad total del ciclo: O(N*(N-1)/2) -> O(N^2)

    sort(savings.begin(), savings.end()); // ordena de mayor a menor
    // El método sort tiene complejidad O(N*logN), como savings tiene un tamaño N^2, la complejidad es de O(N^2*logN)

    return savings; // O(1)
}
// Complejidad total: O(N^2*logN)

// Dado un cliente, encuentra la raíz padre de la ruta donde se encuentra el cliente
// Compresión de ruta: para cada hijo de la raíz, actualizamos su padre a la raíz si no está hecho
// Ejemplo: dado 1->1, 2->1, 3->2, 4->3, si ejecuto findPadre(3) termino teniendo 1->1, 2->1, 3->1, 4->1
int findPadre(vector<int>& padres_ruta, int cliente) {
    if (padres_ruta[cliente] != cliente) {
        padres_ruta[cliente] = findPadre(padres_ruta, padres_ruta[cliente]); // compresión de ruta para todos los clientes conectados
    } 
    return padres_ruta[cliente]; // es la raiz
}
// Complejidad total: O(α(n)), donde α(n) es la función inversa de Ackermann
// Resultado propuesto por Tarjan en "Efficiency of a Good But Not Linear Set Union Algorithm"

// unionFind nos ayudará a decidir cómo unir después las rutas de manera más óptima
// devuelve una tupla con las dos raices, donde la primera es de la ruta a la que vamos a unir ambas rutas
// y la segunda es de la ruta que borraremos
tuple<int, int> unionFind(vector<int>& padres_ruta, vector<int>& rango_cliente, int i, int j) {
    int raiz_i = findPadre(padres_ruta, i); // O(α(n))
    int raiz_j = findPadre(padres_ruta, j); // O(α(n))
    int rango_i = rango_cliente[raiz_i]; // O(1)
    int rango_j = rango_cliente[raiz_j]; // O(1)

    if (rango_i < rango_j) { // O(1) la idea de UnionFind es reducir tiempos de cómputo, agregamos la ruta chica a la grande
        padres_ruta[raiz_i] = raiz_j; // O(1)
        // ahora el primer cliente de la ruta borrada está conectado a la ruta grande unida, entonces en el próximo
        // UnionFind los clientes de la ruta borrada empezarán a apuntar también a la ruta grande con findPadre()
        return make_tuple(raiz_j, raiz_i); // O(1)
    } else if (rango_i > rango_j) { // O(1)
        padres_ruta[raiz_j] = raiz_i; // O(1)
        return make_tuple(raiz_i, raiz_j); // O(1)
    } else {
        padres_ruta[raiz_j] = raiz_i; // O(1)
        rango_cliente[raiz_i]++; // O(1) unimos dos rutas con igual rango, aumentamos en 1
        return make_tuple(raiz_i, raiz_j); // O(1)
    }
}
// Complejidad total: O(α(n))

bool chequeoSolapamiento(const Route& ruta_i, const Route& ruta_j, int i, int j) {
    return ruta_i.getClientePadreId() != ruta_j.getClientePadreId() && // O(1) si son iguales son la misma ruta
        ruta_i.getClienteFinalId() == i && ruta_j.getClientePadreId() == j; // O(1) solapamiento
}
// Complejidad total: O(1)

Solution Heuristicas::clarkeWright() {
    const vector<vector<double>>& distancias = this->_instancia.getDistanceMatrix(); // O(1)
    const vector<int>& demandas = this->_instancia.getDemands(); // O(1)
    int depot = this->_instancia.getDepotId(); // O(1)
    int capacidad = this->_instancia.getCapacity(); // O(1)
    int n = this->_instancia.getDimension(); // O(1)
    int k = this->_instancia.getNumVehicles(); // O(1)

    unordered_map<int, Route*> rutaCliente; // O(1)
    
    for (int i = 1; i <= n; i++) { // O(N)
        if (i != depot) { // O(1)
            Route* ruta = new Route(capacidad, depot); // O(1)
            ruta->agregarClienteInicio(i, demandas[i], distancias[depot][i]); // O(1)
            rutaCliente[i] = ruta; // O(1) en promedio, teniendo en cuenta que las claves utilizan la función estándar de hash para int y están bien distribuidas {1, 2, 3, ... ,n}
        }
    }
    // Complejidad total del ciclo: O(N) en promedio

    vector<Saving> savings = calcularSavings(distancias, depot); // O(N^2*logN)
    vector<int> padres_ruta(n + 1); // O(1)
    vector<int> rango_cliente(n + 1, 0); // representa el costo de recorrido hasta llegar a la raiz, el rango de un cliente
    // aumenta si unimos dos rutas con el mismo rango

    for (int i = 0; i < static_cast<int>(n + 1); i++) { // O(N)
        padres_ruta[i] = i; // O(1) al inicio cada cliente i es la raiz de la ruta donde esta el cliente i
    }
    // Complejidad total del ciclo: O(N)

    int paso = 0; // Contador de pasos para los CSV

    // Exporta el estado inicial (todas las rutas individuales)
    {
        std::vector<Route> rutas_iniciales;
        for (const auto& par : rutaCliente) rutas_iniciales.push_back(*par.second);
        //exportarRutasPaso(rutas_iniciales, this->_instancia.getNodes(), paso++);
    }

    for (int s = 0; s < static_cast<int>(savings.size()); s++) { // O(N^2) recorre los savings desde el mayor al menor
        int i = savings[s].i; // O(1)
        int j = savings[s].j; // O(1)

        Route* ruta_i = rutaCliente[findPadre(padres_ruta, i)]; // O(α(n) encuentra la ruta de la raiz
        // acá aprovecha a actualizar los padres de la subruta para que apunten bien a la ruta que corresponde
        Route* ruta_j = rutaCliente[findPadre(padres_ruta, j)]; // O(α(n))
        bool solapado = true; // O(1)
        bool invertir = false; // O(1)

        if (chequeoSolapamiento(*ruta_i, *ruta_j, i, j)) { // O(1)
            solapado = false; // O(1)
        } else if (chequeoSolapamiento(*ruta_j, *ruta_i, j, i)) { // O(1) chequeo si hay solapamiento invertido
            solapado = false; // O(1)
            invertir = true; // O(1)
        }

        if (!solapado) { // O(1)
            int demandaAgregada = ruta_i->getDemandaTotal() + ruta_j->getDemandaTotal(); // O(1)
            if (demandaAgregada <= capacidad) { // O(1) si la nueva demanda no supera la capacidad agregamos ruta_j a ruta_i
                tuple<int, int> res_unionFind = unionFind(padres_ruta, rango_cliente, i, j); // O(α(n))
                int cliente_padre = get<0>(res_unionFind); // O(1)
                int cliente_borrar = get<1>(res_unionFind); // O(1)            

                if (!invertir) { // O(1)
                    ruta_i->unirRutas(*ruta_j, distancias[i][j], distancias[depot][i], distancias[depot][j]); // O(1)
                    // seteamos la ruta unida al cliente padre que debe tener la ruta
                    rutaCliente[cliente_padre] = ruta_i; // O(1) en promedio
                } else { // lo mismo pero invertido
                    ruta_j->unirRutas(*ruta_i, distancias[i][j], distancias[depot][j], distancias[depot][i]); // O(1)
                    rutaCliente[cliente_padre] = ruta_j; // O(1) en promedio
                }
                // la otra ruta la borramos
                rutaCliente.erase(cliente_borrar); // O(1) en promedio, teniendo en cuenta que solo existe un valor con esa clave

                // <<<<<<<< EXPORTA EL ESTADO ACTUAL DESPUÉS DE CADA UNIÓN EXITOSA >>>>>>>>
                std::vector<Route> rutas_actuales;
                for (const auto& par : rutaCliente) rutas_actuales.push_back(*par.second);
                //exportarRutasPaso(rutas_actuales, this->_instancia.getNodes(), paso++);            
            } 
        } 
    }
    // Complejidad total del ciclo: O(N^2) en promedio
    
    Solution solucion = Solution(k, "ClarkeWright");
    for (auto it = rutaCliente.begin(); it != rutaCliente.end(); ++it) {
        solucion.agregarRuta(it->second);
    }

    return solucion;
}
// Complejidad total ClarkeWright: O(N^2*logN)