#ifndef NODEROUTE_H
#define NODEROUTE_H

struct NodeRoute {
    int id;
    int demanda;
    NodeRoute* anterior;
    NodeRoute* siguiente;
};

#endif // NODEROUTE_H