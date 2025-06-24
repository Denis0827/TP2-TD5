/*
#include "Swap.h"
#include <algorithm>
#include <unordered_map>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool swap_clientes(vector<Route>& rutas, const vector<vector<double>>& distancias, const vector<int>& demandas, int capacidad) {
    int r = rutas.size();

    for (int i = 0; i < r; ++i) {
        for (int j = i; j < r; ++j) {  // permitimos i == j para intra-ruta
            Route& rutaA = rutas[i];
            Route& rutaB = rutas[j];

            for (size_t posA = 1; posA < rutaA.nodes.size() - 1; ++posA) {
                size_t inicioB;
                if (i == j) {
                    inicioB = posA + 1; // para intra-ruta
                } else {
                    inicioB = 1; // para inter-ruta
                }

                for (size_t posB = inicioB; posB < rutaB.nodes.size() - 1; ++posB) {
                    int clienteA = rutaA.nodes[posA];
                    int clienteB = rutaB.nodes[posB];

                    bool swapValido = true;

                    // En inter-ruta, controlar capacidad
                    if (i != j) {
                        int demandaA = demandas[clienteA];
                        int demandaB = demandas[clienteB];

                        int nuevaDemandaA = rutaA.totalDemand - demandaA + demandaB;
                        int nuevaDemandaB = rutaB.totalDemand - demandaB + demandaA;

                        if (nuevaDemandaA > capacidad || nuevaDemandaB > capacidad) {
                            swapValido = false;
                        }
                    }

                    if (swapValido) {
                        // Calcular costo antes y después
                        double costoAntes = 0.0;
                        double costoDespues = 0.0;

                        if (posA > 0 && posA + 1 < rutaA.nodes.size()) { // para no incluir al deposito
                            costoAntes += distancias[rutaA.nodes[posA - 1]][clienteA] +
                                          distancias[clienteA][rutaA.nodes[posA + 1]];
                            costoDespues += distancias[rutaA.nodes[posA - 1]][clienteB] +
                                            distancias[clienteB][rutaA.nodes[posA + 1]];
                        }

                        if (posB > 0 && posB + 1 < rutaB.nodes.size()) {
                            costoAntes += distancias[rutaB.nodes[posB - 1]][clienteB] +
                                          distancias[clienteB][rutaB.nodes[posB + 1]];
                            costoDespues += distancias[rutaB.nodes[posB - 1]][clienteA] +
                                            distancias[clienteA][rutaB.nodes[posB + 1]];
                        }

                        if (costoDespues < costoAntes) {
                            swap(rutaA.nodes[posA], rutaB.nodes[posB]);

                            if (i != j) {
                                int demandaA = demandas[clienteA];
                                int demandaB = demandas[clienteB];
                                rutaA.totalDemand = rutaA.totalDemand - demandaA + demandaB;
                                rutaB.totalDemand = rutaB.totalDemand - demandaB + demandaA;
                            }

                            return true;  // mejora encontrada
                        }
                    }
                }
            }
        }
    }

    return false;
}

############################################################################################

1. Recorre todas las combinaciones de rutas (incluyendo la misma ruta).

2. Extrae los clientes de cada ruta (excluyendo depósitos).

3. Prueba todos los pares de clientes posibles (uno de cada ruta, o dos de la misma ruta).

4. Evalúa si es válido hacer el intercambio:

    - Si son rutas distintas, verifica que no se supere la capacidad del camión.

5. Calcula el costo antes y después del swap usando las distancias.

6. Si el swap reduce el costo, lo realiza:

    - Intercambia los id de los nodos.

    - Recalcula y actualiza la demanda total de las rutas afectadas.

    - Devuelve true (se hizo una mejora).

7. Si no encuentra ninguna mejora después de probar todos los swaps posibles, devuelve false.

*/

#include "NodeRoute.h"
#include "Route.h"
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

bool swap_clientes(vector<Route>& rutas, const vector<vector<double>>& distancias, const vector<int>& demandas, int capacidad) {
    int r = rutas.size();

    for (int i = 0; i < r; ++i) {
        for (int j = i; j < r; ++j) {  // permitimos i == j para intra-ruta
            Route& rutaA = rutas[i];
            Route& rutaB = rutas[j];

            // Obtener todos los nodos cliente de rutaA (excluyendo depots)
            vector<NodeRoute*> nodosA;
            NodeRoute* actualA = rutaA.getRaiz();
            if (actualA != nullptr) actualA = actualA->siguiente; // saltar primer depot
            while (actualA != nullptr && actualA->siguiente != nullptr) {
                nodosA.push_back(actualA);
                actualA = actualA->siguiente;
            }

            // Obtener todos los nodos cliente de rutaB (excluyendo depots)
            vector<NodeRoute*> nodosB;
            NodeRoute* actualB = rutaB.getRaiz();
            if (actualB != nullptr) actualB = actualB->siguiente; // saltar primer depot
            while (actualB != nullptr && actualB->siguiente != nullptr) {
                nodosB.push_back(actualB);
                actualB = actualB->siguiente;
            }

            for (size_t posA = 0; posA < nodosA.size(); ++posA) {
                size_t inicioB;
                if (i == j) {
                    inicioB = posA + 1; // para intra-ruta
                } else {
                    inicioB = 0; // para inter-ruta
                }

                for (size_t posB = inicioB; posB < nodosB.size(); ++posB) {
                    NodeRoute* nodoA = nodosA[posA];
                    NodeRoute* nodoB = nodosB[posB];
                    
                    int clienteA = nodoA->id;
                    int clienteB = nodoB->id;

                    bool swapValido = true;

                    // En inter-ruta, controlar capacidad
                    if (i != j) {
                        int demandaA = demandas[clienteA];
                        int demandaB = demandas[clienteB];

                        int nuevaDemandaA = rutaA.getDemandaTotal() - demandaA + demandaB;
                        int nuevaDemandaB = rutaB.getDemandaTotal() - demandaB + demandaA;

                        if (nuevaDemandaA > capacidad || nuevaDemandaB > capacidad) {
                            swapValido = false;
                        }
                    }

                    if (swapValido) {
                        // Calcular costo antes del swap
                        double costoAntes = 0.0;
                        
                        // Costo de aristas conectadas al nodo A
                        if (nodoA->anterior != nullptr) {
                            costoAntes += distancias[nodoA->anterior->id][clienteA];
                        }
                        if (nodoA->siguiente != nullptr) {
                            costoAntes += distancias[clienteA][nodoA->siguiente->id];
                        }
                        
                        // Costo de aristas conectadas al nodo B
                        if (nodoB->anterior != nullptr) {
                            costoAntes += distancias[nodoB->anterior->id][clienteB];
                        }
                        if (nodoB->siguiente != nullptr) {
                            costoAntes += distancias[clienteB][nodoB->siguiente->id];
                        }

                        // Calcular costo después del swap
                        double costoDespues = 0.0;
                        
                        // Nuevo costo para la posición de nodo A con cliente B
                        if (nodoA->anterior != nullptr) {
                            costoDespues += distancias[nodoA->anterior->id][clienteB];
                        }
                        if (nodoA->siguiente != nullptr) {
                            costoDespues += distancias[clienteB][nodoA->siguiente->id];
                        }
                        
                        // Nuevo costo para la posición de nodo B con cliente A
                        if (nodoB->anterior != nullptr) {
                            costoDespues += distancias[nodoB->anterior->id][clienteA];
                        }
                        if (nodoB->siguiente != nullptr) {
                            costoDespues += distancias[clienteA][nodoB->siguiente->id];
                        }

                        if (costoDespues < costoAntes) {
                            // Realizar el swap intercambiando los IDs
                            nodoA->id = clienteB;
                            nodoB->id = clienteA;

                            // Actualizar demandas totales en las rutas si son inter-ruta
                            if (i != j) {
                                
                                int nuevaDemandaA = 0;
                                NodeRoute* temp = rutaA.getRaiz();
                                if (temp != nullptr) temp = temp->siguiente; // saltar depot inicial
                                while (temp != nullptr && temp->siguiente != nullptr) { // no es ruta vacía y corta antes de llegar al depot
                                    nuevaDemandaA += demandas[temp->id];
                                    temp = temp->siguiente;
                                }
                                
                                int nuevaDemandaB = 0;
                                temp = rutaB.getRaiz();
                                if (temp != nullptr) temp = temp->siguiente; // saltar depot inicial
                                while (temp != nullptr && temp->siguiente != nullptr) {
                                    nuevaDemandaB += demandas[temp->id];
                                    temp = temp->siguiente;
                                }
                                rutaA.setDemandaTotal(nuevaDemandaA);
                                rutaB.setDemandaTotal(nuevaDemandaB);
                            }

                            return true;  // mejora encontrada
                        }
                    }
                }
            }
        }
    }

    return false;
}