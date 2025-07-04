CXX = g++
CXXFLAGS = -g -std=c++17 -Wall -Wextra -O2 -I./src/Modelado_Clase -I./src/Heuristicas

SRC = src/Heuristicas/testing_relocate.cpp \
      src/Heuristicas/ClarkeWright.cpp \
	src/Heuristicas/NearestNeighbor.cpp \
      src/Heuristicas/Swap.cpp \
      src/Modelado_Clase/VRPLIBReader.cpp \
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

clean:
	rm -f $(OBJ) $(EXEC)


testing_relocate: src/Heuristicas/testing_relocate.cpp src/Modelado_Clase/Route.cpp
	$(CXX) $(CXXFLAGS) -o testing_relocate src/Heuristicas/testing_relocate.cpp src/Modelado_Clase/Route.cpp