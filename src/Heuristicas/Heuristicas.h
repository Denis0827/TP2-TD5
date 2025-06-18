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

using namespace std;

class Heuristicas {
    private: 
        VRPLIBReader _instancia;

    public:
        Heuristicas(const string& instanciaPath);
        VRPLIBReader getInstancia();

        vector<Route> clarkeWright();
        vector<Route> vecinoMasCercano();
        vector<Route> relocate(const vector<Route>& solucion_inicial);
        vector<Route> swap(const vector<Route>& solucion_inicial);

};

#endif // HEURISTICAS_H