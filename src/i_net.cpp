#include "Memory.hpp"

extern "C"
{
#include "i_system.h"
#include "d_event.h"
#include "d_net.h"
#include "m_argv.h"

#include "doomstat.h"

#include "i_net.h"
}

extern "C" void I_InitNetwork()
{
	doomcom = new doomcom_t;
	Memory::Set(doomcom,0,sizeof(doomcom_t));
	doomcom->ticdup = 1;
	netgame = false;
	doomcom->id = DOOMCOM_ID;
	doomcom->numplayers = doomcom->numnodes = 1;
	// TODO
}

extern "C" void I_NetCmd()
{
	// TODO
}
