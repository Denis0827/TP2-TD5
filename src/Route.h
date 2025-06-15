#ifndef ROUTE_H
#define ROUTE_H
#include <vector>

struct Route {
    std::vector<int> nodes; // el primer y último nodo debe ser el del depot
    int totalDemand;
};

#endif // ROUTE_H