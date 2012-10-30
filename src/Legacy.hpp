#ifndef Legacy_hpp
#define Legacy_hpp

#include "Core/System.hpp"

extern Core::System *Pistachio;

#define LEGACY_ENTER(Sys) do { Core::System *TempPistachio = Pistachio; Pistachio = Sys; try { do {} while (false)
#define LEGACY_LEAVE() } catch (...) { Pistachio = TempPistachio; throw; } Pistachio = TempPistachio; } while (false)

#endif
