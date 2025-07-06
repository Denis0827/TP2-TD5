CXX = g++
CXXFLAGS = -g -std=c++17 -Wall -Wextra -O2 -I./src/Modelado_Clase -I./src/Heuristicas -I./src/Testing

SRC = src/Heuristicas/ClarkeWright.cpp \
      src/Heuristicas/NearestNeighbor.cpp \
      src/Heuristicas/Swap.cpp \
      src/Modelado_Clase/VRPLIBReader.cpp \
      src/Heuristicas/Relocate.cpp \
      src/Modelado_Clase/Route.cpp \
      src/Modelado_Clase/Solution.cpp

HEADERS = src/Heuristicas/Heuristicas.h \
          src/Modelado_Clase/VRPLIBReader.h \
          src/Modelado_Clase/Saving.h \
          src/Modelado_Clase/Route.h \
	    src/Modelado_Clase/NodeRoute.h \
          src/Modelado_Clase/Solution.h

main: src/Heuristicas/testing.cpp $(SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o main src/Heuristicas/testing.cpp src/Heuristicas/ClarkeWright.cpp src/Heuristicas/NearestNeighbor.cpp src/Heuristicas/Swap.cpp src/Heuristicas/Relocate.cpp src/Modelado_Clase/VRPLIBReader.cpp src/Modelado_Clase/Solution.cpp src/Modelado_Clase/Route.cpp

test_swap_clientes: src/Testing/TestSwapClientes.cpp src/Testing/TestSwapClientes.h $(SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o test_swap_clientes src/Testing/TestSwapClientes.cpp src/Modelado_Clase/Route.cpp

test_relocate_cliente: src/Testing/TestRelocateCliente.cpp src/Testing/TestRelocateCliente.h $(SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o test_relocate_cliente src/Testing/TestRelocateCliente.cpp src/Modelado_Clase/Route.cpp

test_route: src/Testing/TestRoute.cpp src/Testing/TestRoute.h $(SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ src/Modelado_Clase/Route.cpp

# -- Ejecutar test específico (compila y ejecuta en un solo paso) --
run_main:
	$(MAKE) main && ./main

run_test_route:
	$(MAKE) test_route && ./test_route

run_test_swap_clientes:
	$(MAKE) test_swap_clientes && ./test_swap_clientes

run_test_relocate_cliente:
	$(MAKE) test_relocate_cliente && ./test_relocate_cliente

run_testing_nn_simple:
	$(MAKE) testing_nn_simple && ./testing_nn_simple

# -- Limpieza --
clean:
	rm -f main test_route test_swap_clientes test_relocate_cliente testing_nn_simple