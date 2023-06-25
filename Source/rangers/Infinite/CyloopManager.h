#pragma once

#include "SonicParameters.h"

class CyloopManager
{
	public:
		static void init();
		static void changeCyloopColor(CyloopLocusParameter& locus);
	private:
		static void changeColorF(colorF& color, float red, float green, float blue);
};
