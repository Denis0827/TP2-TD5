#ifndef CLARKE_WRIGHT_SOLVER_H
#define CLARKE_WRIGHT_SOLVER_H

#include "VRPLIBReader.h"
#include "Route.h"
#include <vector>

struct Saving {
    int i, j;
    double saving;
    bool operator<(const Saving& other) const;
};

std::vector<Route> clarkeWright(const VRPLIBReader& instancia);

#endif // CLARKE_WRIGHT_SOLVER_H
