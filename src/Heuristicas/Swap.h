#ifndef SWAP_H
#define SWAP_H

#include "Route.h"
#include "NodeRoute.h"
#include <vector>
using namespace std;

// Función principal de swap
bool swap_clientes(vector<Route>& rutas, const vector<vector<double>>& distancias, 
                  const vector<int>& demandas, int capacidad);

// Funciones auxiliares
vector<NodeRoute*> obtenerNodosClientes(Route& ruta);
double calcularCostoAristas(NodeRoute* nodo, const vector<vector<double>>& distancias);
double calcularCostoDespuesSwap(NodeRoute* nodoA, NodeRoute* nodoB, int nuevoClienteA, int nuevoClienteB, 
                               const vector<vector<double>>& distancias);
void realizarSwap(Route& rutaA, Route& rutaB, NodeRoute* nodoA, NodeRoute* nodoB, 
                 int clienteA, int clienteB, const vector<int>& demandas, 
                 const vector<vector<double>>& distancias);

#endif // SWAP_H