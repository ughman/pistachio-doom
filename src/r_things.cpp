#include "List.hpp"

extern "C"
{
#include "r_local.h"

#include "doomstat.h"

	void R_DrawSprite(vissprite_t *);
	void R_DrawPlayerSprites();
}

List <vissprite_t> Vissprites;

extern "C" void R_ClearSprites()
{
	Vissprites.Clear();
}

extern "C" vissprite_t *R_NewVisSprite()
{
	Vissprites.Add(vissprite_t());
	return &Vissprites.Back->Value;
}

extern "C" void R_SortVisSprites()
{
	// TODO :: More efficient sorting
	bool Retry;
	do
	{
		Retry = false;
		for (Link <vissprite_t> *it = Vissprites.Front;it && it->Next;it = it->Next)
		{
			if (it->Value.scale > it->Next->Value.scale)
			{
				Link <vissprite_t> *Other = it->Next;
				Link <vissprite_t> *Prev = it->Prev;
				Link <vissprite_t> *Next = it->Next->Next;
				Other->Prev = Prev;
				Other->Next = it;
				it->Prev = Other;
				it->Next = Next;
				if (Prev)
				{
					Prev->Next = Other;
				}
				else
				{
					it->Owner->Front = Other;
				}
				if (Next)
				{
					Next->Prev = it;
				}
				else
				{
					it->Owner->Back = it;
				}
				Retry = true;
			}
		}
	}
	while (Retry);
}

extern "C" void R_DrawMasked()
{
	R_SortVisSprites();
	for (Link <vissprite_t> *it = Vissprites.Front;it;it = it->Next)
	{
		R_DrawSprite(&it->Value);
	}
	for (drawseg_t *ds = ds_p - 1;ds >= drawsegs;ds--)
	{
		if (ds->maskedtexturecol)
		{
			R_RenderMaskedSegRange(ds,ds->x1,ds->x2);
		}
	}
	if (!viewangleoffset)
	{
		R_DrawPlayerSprites();
	}
}
