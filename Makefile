CXX = g++
CXXFLAGS = -g -std=c++17 -Wall -Wextra -O2 -I./src/Modelado_Clase -I./src/Heuristicas

SRC = src/Heuristicas/testing_relocate.cpp \
      src/Heuristicas/ClarkeWright.cpp \
	src/Heuristicas/NearestNeighbor.cpp \
      src/Heuristicas/Swap.cpp \
      src/Modelado_Clase/VRPLIBReader.cpp \
      src/Heuristicas/Relocate.cpp \
      src/Modelado_Clase/Route.cpp \
      src/Modelado_Clase/Solution.cpp

HEADERS = src/Heuristicas/Heuristicas.h \
          src/Heuristicas/ClarkeWrightSolver.h \
          src/Modelado_Clase/VRPLIBReader.h \
          src/Modelado_Clase/Saving.h \
          src/Modelado_Clase/Route.h \
	    src/Modelado_Clase/NodeRoute.h \
          src/Modelado_Clase/Solution.h

OBJ = $(SRC:.cpp=.o)
EXEC = heuristicas

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@


testing_relocate: src/Heuristicas/testing_relocate.cpp src/Modelado_Clase/Route.cpp
	$(CXX) $(CXXFLAGS) -o testing_relocate src/Heuristicas/testing_relocate.cpp src/Modelado_Clase/Route.cpp

test_swap_clientes: src/Testing/TestSwapClientes.cpp src/Testing/TestSwapClientes.h src/Modelado_Clase/Route.cpp
	$(CXX) $(CXXFLAGS) -o test_swap_clientes src/Testing/TestSwapClientes.cpp src/Modelado_Clase/Route.cpp

test_route: src/Testing/TestRoute.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ src/Modelado_Clase/Route.cpp

# ===============================
# Ejecutar test específico (compila y ejecuta en un solo paso)
# ===============================
run_test_route:
	$(MAKE) test_route && ./test_route

run_test_swap:
	$(MAKE) test_swap_clientes && ./test_swap_clientes

run_testing_relocate:
	$(MAKE) testing_relocate && ./testing_relocate

run_testing_nn_simple:
	$(MAKE) testing_nn_simple && ./testing_nn_simple

# ===============================
# Limpieza
# ===============================
clean:
	rm -f $(OBJ) $(EXEC) test_route test_swap_clientes testing_relocate testing_nn_simple