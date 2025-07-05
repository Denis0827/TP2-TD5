// testing_nn_simple.cpp
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

// --- Función auxiliar para ordenar clientes por distancia ---
vector<int> ordenarPorDistancias(const vector<double>& distancias) {
    int n = distancias.size();
    vector<pair<double,int>> dist_con_id;
    for (int i = 1; i < n; i++) {
        dist_con_id.push_back({distancias[i], i});
    }
    sort(dist_con_id.begin(), dist_con_id.end());
    vector<int> clientes_ordenados;
    for (auto& p : dist_con_id) {
        clientes_ordenados.push_back(p.second);
    }
    return clientes_ordenados;
}

// --- Función para obtener candidatos RCL ---
vector<int> obtenerCandidatosRCL(const vector<double>& distanciasCliente, int id,
                                const vector<int>& visitados, const vector<int>& demandas,
                                int capacidadRestante, double alpha) {
    vector<pair<double,int>> candidatos;
    for (int i = 1; i < (int)distanciasCliente.size(); i++) {
        if (!visitados[i] && demandas[i] <= capacidadRestante && i != id) {
            candidatos.push_back({distanciasCliente[i], i});
        }
    }
    if (candidatos.empty()) return {};
    sort(candidatos.begin(), candidatos.end());
    double minDist = candidatos[0].first;
    double maxDist = candidatos.back().first;
    double threshold = minDist + alpha*(maxDist - minDist);
    vector<int> rcl;
    for (auto& c : candidatos) {
        if (c.first <= threshold) rcl.push_back(c.second);
    }
    return rcl;
}

// --- Función para seleccionar primer cliente desde RCL ---
int seleccionarPrimerClienteRCL(const vector<int>& clientes_ordenados,
                               const vector<int>& visitados, double alpha) {
    vector<int> candidatos_disponibles;
    for (int c : clientes_ordenados) {
        if (!visitados[c]) candidatos_disponibles.push_back(c);
    }
    if (candidatos_disponibles.empty()) return -1;
    int rcl_size = max(1, (int)(candidatos_disponibles.size()*alpha));
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, rcl_size - 1);
    return candidatos_disponibles[dis(gen)];
}

// --- Función para correr un test de construcción de ruta ---
void testConstruccionRuta(const vector<vector<double>>& dist, 
                         const vector<int>& demandas, int capacidad, double alpha) {
    int n = dist.size();
    vector<int> visitados(n, 0);
    visitados[0] = 1; // depot visitado

    vector<int> clientes_ordenados = ordenarPorDistancias(dist[0]);

    cout << "\n--- Test construcción ruta con alpha = " << alpha << " ---\n";
    cout << "Clientes ordenados desde depot: ";
    for (int c : clientes_ordenados) cout << c << " ";
    cout << "\n";

    int primer_cliente = seleccionarPrimerClienteRCL(clientes_ordenados, visitados, alpha);
    if (primer_cliente == -1) {
        cout << "No hay clientes disponibles para iniciar la ruta.\n";
        return;
    }
    cout << "Primer cliente seleccionado: " << primer_cliente << "\n";

    visitados[primer_cliente] = 1;
    int capacidad_restante = capacidad - demandas[primer_cliente];
    int actual = primer_cliente;

    cout << "Ruta construida: " << actual << " ";

    while (true) {
        vector<int> candidatos_rcl = obtenerCandidatosRCL(dist[actual], actual, visitados, demandas, capacidad_restante, alpha);
        if (candidatos_rcl.empty()) break;

        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, candidatos_rcl.size() - 1);
        int elegido = candidatos_rcl[dis(gen)];

        cout << elegido << " ";
        visitados[elegido] = 1;
        capacidad_restante -= demandas[elegido];
        actual = elegido;
    }

    cout << "\nCapacidad restante: " << capacidad_restante << "\n";
}

int main() {
    // Matriz ejemplo 1
    vector<vector<double>> dist1 = {
        {0, 1, 2, 3, 4},
        {1, 0, 1, 4, 5},
        {2, 1, 0, 1, 2},
        {3, 4, 1, 0, 1},
        {4, 5, 2, 1, 0}
    };
    vector<int> demandas1 = {0, 2, 2, 4, 3};
    int capacidad1 = 7;

    // Test con distintos valores alpha
    testConstruccionRuta(dist1, demandas1, capacidad1, 0.0);  // greedy puro
    testConstruccionRuta(dist1, demandas1, capacidad1, 0.5);  // mix random-greedy
    testConstruccionRuta(dist1, demandas1, capacidad1, 1.0);  // totalmente random

    // Matriz ejemplo 2, distinta estructura y demandas
    vector<vector<double>> dist2 = {
        {0, 3, 1, 4, 7, 5},
        {3, 0, 6, 2, 3, 8},
        {1, 6, 0, 5, 9, 3},
        {4, 2, 5, 0, 1, 2},
        {7, 3, 9, 1, 0, 4},
        {5, 8, 3, 2, 4, 0}
    };
    vector<int> demandas2 = {0, 1, 2, 2, 1, 3};
    int capacidad2 = 6;

    testConstruccionRuta(dist2, demandas2, capacidad2, 0.3);
    testConstruccionRuta(dist2, demandas2, capacidad2, 0.7);

    return 0;
}
