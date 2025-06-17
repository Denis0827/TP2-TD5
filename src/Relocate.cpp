#include "Relocate.h"
#include <algorithm>
#include <unordered_map>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<Route> relocate(const VRPLIBReader& instancia, vector<Route>& solucion_inicial) {
    vector<Route> solucion = solucion_inicial; 
    int cantidad_vehiculos = solucion_inicial.size();
    const vector<vector<double>>& distancias = instancia.getDistanceMatrix();
    const vector<int>& demandas = instancia.getDemands();
    int capacidad = instancia.getCapacity();
    int n = instancia.getDimension();

    int clientesNoVisitados = n - 1;
    vector<int> visitados (n + 1, 0);

    for (int i = 0; i < cantidad_vehiculos; i++){
        int cliente_i = solucion_inicial[i].nodes[1];
        int cliente_p = solucion_inicial[i].nodes[0];
        int cliente_q = solucion_inicial[i].nodes[2];

        int costo_pi = distancias[cliente_p][cliente_i];
        int costo_iq = distancias[cliente_i][cliente_q];
        int costo_pq = distancias[cliente_p][cliente_q];

        int cliente_j = 0;
        int cliente_k = 0;

        int costo_jk = 0;
        int costo_ji = 0;
        int costo_ik = 0;

        for (int s = 0; s < cantidad_vehiculos; s++) {
            int cant_clientes_vehiculo_s = solucion_inicial[s].nodes.size() - 2; // elimino los depot del inicio y final
            for (int j = 0; j <= cant_clientes_vehiculo_s; j++) {
                // verifico que cliente_j no sea igual a cliente_i ni cliente_p
                if (solucion_inicial[s].nodes[j] != cliente_i && solucion_inicial[s].nodes[j] != cliente_p) { 
                    cliente_j = solucion_inicial[s].nodes[j];
                    cliente_k = solucion_inicial[s].nodes[j + 1];
                    costo_jk = distancias[cliente_j][cliente_k];
                    costo_ji = distancias[cliente_j][cliente_i];
                    costo_ik = distancias[cliente_i][cliente_k];
                    
                    // verificacion de mejora
                }
            }
        }



        insert

    }
}