#ifndef RELOCATE_H
#define RELOCATE_H

#include "VRPLIBReader.h"
#include "Route.h"
#include <vector>

std::vector<Route> relocate(const VRPLIBReader& instancia, std::vector<Route>& solucion_inicial);

#endif // RELOCATE_H
