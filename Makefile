CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -I./src/Modelado_Clase -I./src/Heuristicas

SRC = src/Heuristicas/testing.cpp \
      src/Heuristicas/InitHeuristicas.cpp \
      src/Heuristicas/ClarkeWright.cpp \
	src/Heuristicas/NearestNeighbor.cpp \
      src/Modelado_Clase/VRPLIBReader.cpp \
      src/Modelado_Clase/Route.cpp

HEADERS = src/Heuristicas/Heuristicas.h \
          src/Heuristicas/ClarkeWrightSolver.h \
          src/Modelado_Clase/VRPLIBReader.h \
          src/Modelado_Clase/Saving.h \
          src/Modelado_Clase/Route.h \
	    src/Modelado_Clase/NodeRoute.h

OBJ = $(SRC:.cpp=.o)
EXEC = heuristicas

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)