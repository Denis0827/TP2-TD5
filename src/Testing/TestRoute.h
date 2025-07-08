#ifndef TESTROUTE_H
#define TESTROUTE_H

#include "../Modelado_Clase/Route.h"
#include <vector>
#include <string>

class TestRoute {
    private:
        std::vector<std::vector<double>> distancias;
        void inicializarDistancias();
        void imprimirRuta(const Route& ruta, const std::string& nombre);
        bool verificarIntegridadRuta(const Route& ruta, int clientesEsperados, int demandaEsperada, int capacidadEsperada, double distanciaEsperada, int rutaNum);

    public:
        TestRoute();
        bool testRutaVacia();
        bool testUnCliente();
        bool testVariosClientes();
        bool testAgregarInicioYFinal();
        bool testUnirRutasSimples();
        bool testUnirRutasConClientes();
        bool testUnirRutasInverso();
        bool testUnirRutasVacias();
        bool testCapacidadYDemanda();
        bool testGettersRuta();
        void ejecutarTodosLosTests();
    };

#endif // TESTROUTE_H
