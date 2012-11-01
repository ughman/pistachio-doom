#include "Legacy.hpp"
#include "IO/Archive.hpp"

extern "C" void D_AddFile(char *file)
{
	using (IO::Archive *Archive = IO::Archive::Load(file))
	{
		Pistachio->Data->Add(Archive);
	}
	end_using(Archive);
}
