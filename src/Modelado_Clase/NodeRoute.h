#ifndef NODEROUTE_H
#define NODEROUTE_H

struct NodeRoute {
    int id;
    NodeRoute* anterior;
    NodeRoute* siguiente;
};

#endif // NODEROUTE_H