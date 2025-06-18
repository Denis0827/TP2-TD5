#include <iostream>
#include "VRPLIBReader.h"
#include "VecinoMasCercanoSolver.h"

using namespace std;

void imprimir_ruta(const Route& ruta, const VRPLIBReader& instancia, double capacidad_maxima) {
    double demanda_total = 0.0;
    cout << "Ruta: ";

    for (size_t i = 0; i < ruta.nodes.size() - 1; ++i) {
        int actual = ruta.nodes[i];
        int siguiente = ruta.nodes[i + 1];
        double distancia = instancia.distancia(actual, siguiente);
        double demanda = instancia.demanda(actual);
        demanda_total += demanda;

        cout << actual << " (dist: " << distancia
             << ", dem: " << demanda << ") -> ";
    }

    int ultimo = ruta.nodes.back();
    double demanda_ultima = instancia.demanda(ultimo);
    demanda_total += demanda_ultima;

    cout << ultimo << " (dem: " << demanda_ultima << ")" << endl;

    double capacidad_restante = capacidad_maxima - demanda_total;
    cout << "Demanda total: " << demanda_total
         << " | Capacidad restante: " << capacidad_restante << "\n" << endl;
}

TEST(vecinomascercano_tests, test_solucion_vecinomascercano_mostrar_rutas) {
    VRPLIBReader instancia;
    instancia.leer("../instancias/A-n32-k5.vrp"); // Cambia la ruta a tu archivo real

    double capacidad = instancia.capacidad_maxima();
    vector<Route> rutas = vecinoMasCercano(instancia);

    cout << "Cantidad de rutas generadas: " << rutas.size() << endl << endl;

    vector<bool> cliente_visitado(instancia.cantidad_clientes() + 1, false);
    int depot_id = instancia.depot_id();

    for (const Route& ruta : rutas) {
        imprimir_ruta(ruta, instancia, capacidad);

        for (int i = 1; i < ruta.nodes.size() - 1; ++i) {
            int cliente = ruta.nodes[i];
            ASSERT_NE(cliente, depot_id) << "El depósito no debe estar entre medio.";
            ASSERT_FALSE(cliente_visitado[cliente]) << "Cliente visitado más de una vez.";
            cliente_visitado[cliente] = true;
        }
    }

    for (int i = 1; i <= instancia.cantidad_clientes(); ++i) {
        ASSERT_TRUE(cliente_visitado[i]) << "Cliente " << i << " no fue visitado.";
    }
}
