#include <iostream>
#include <iomanip>
#include "VRPLIBReader.h"
#include "ClarkeWrightSolver.h"

void testClarkeWright(const std::string& instanciaPath) {
    VRPLIBReader reader(instanciaPath); // ya parsea en el constructor

    std::vector<Route> rutas = clarkeWright(reader);

    const auto& distancias = reader.getDistanceMatrix();  // corregido
    const auto& demandas = reader.getDemands();           // corregido
    int capacidadMax = reader.getCapacity();              // corregido

    std::cout << "=== Solución Clarke-Wright para la instancia " << instanciaPath << " ===\n";

    for (size_t r = 0; r < rutas.size(); ++r) {
        const auto& ruta = rutas[r];
        int demandaAcumulada = 0;
        std::cout << "Ruta #" << (r + 1) << ":\n";

        for (size_t i = 0; i < ruta.nodes.size() - 1; ++i) {
            int actual = ruta.nodes[i];
            int siguiente = ruta.nodes[i + 1];
            int demanda = demandas[actual];
            double distancia = distancias[actual][siguiente];
            demandaAcumulada += demanda;

            std::cout << "  Cliente " << actual
                      << " --(" << std::fixed << std::setprecision(2)
                      << distancia << " km)--> ";
        }

        int ultimo = ruta.nodes.back();
        std::cout << "Cliente " << ultimo << "\n";

        std::cout << "  Demanda total: " << demandaAcumulada
                  << " / " << capacidadMax
                  << " | Capacidad restante: " << (capacidadMax - demandaAcumulada) << "\n\n";
    }
}
