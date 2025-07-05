#ifndef TESTROUTE_H
#define TESTROUTE_H

#include "../Modelado_Clase/Route.h"
#include <vector>
#include <string>

class TestRouteTester {
    private:
        std::vector<std::vector<double>> distancias;
        void inicializarDistancias();
        void imprimirRuta(const Route& ruta, const std::string& nombre);
        void verificarIntegridadRuta(const Route& ruta, const std::string& nombre, int clientesEsperados, 
            int demandaEsperada, int capacidadEsperada, double distanciaEsperada);

    public:
        TestRouteTester();
        void testRutaVacia();
        void testUnCliente();
        void testVariosClientes();
        void testAgregarInicioYFinal();
        void testUnirRutasSimples();
        void testUnirRutasConClientes();
        void testUnirRutasVacias();
        void testCapacidadYDemanda();
        void ejecutarTodosLosTests();
    };

#endif // TESTROUTE_H
