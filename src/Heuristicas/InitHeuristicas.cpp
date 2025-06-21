#include "Heuristicas.h"

Heuristicas::Heuristicas(const string& instanciaPath)
    : _instancia(instanciaPath) // inicializa _instancia usando el constructor adecuado
{}

VRPLIBReader Heuristicas::getInstancia() {
    return this->_instancia;
}

