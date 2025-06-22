#ifndef HEURISTICAS_H
#define HEURISTICAS_H

#include "../Modelado_Clase/VRPLIBReader.h"
#include "../Modelado_Clase/Saving.h"
#include "../Modelado_Clase/Route.h"

#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <tuple>

#include <fstream>
#include <iomanip>
#include <sstream>

using namespace std;

class Heuristicas {
    private: 
        VRPLIBReader _instancia;

    public:
        Heuristicas(const string& instanciaPath); // O(1)
        VRPLIBReader getInstancia(); // O(1)

        vector<Route> clarkeWright(); // O(N^2*logN)
        vector<Route> nearestNeighbor(); // O(N^2)
        vector<Route> relocate(const vector<Route>& solucion_inicial);
        vector<Route> swap(const vector<Route>& solucion_inicial);
};

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
        const NodeRoute* actual = ruta.getRaizSinMod();
        while (actual && actual->siguiente) {
            const Node& from = nodos[actual->id - 1];
            const Node& to = nodos[actual->siguiente->id - 1];
            out << from.x << "," << from.y << "," << to.x << "," << to.y << "," << actual->id << "," << actual->siguiente->id << "," << ruta.getClientePadreId() << "\n";
            actual = actual->siguiente;
        }
    }
    out.close();
}

#endif // HEURISTICAS_H