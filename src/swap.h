#ifndef SWAP_H
#define SWAP_H

#include "Route.h"
#include <vector>

bool swap_clientes(std::vector<Route>& rutas, const std::vector<std::vector<double>>& distancias, const std::vector<int>& demandas, int capacidad);

#endif // SWAP_H
