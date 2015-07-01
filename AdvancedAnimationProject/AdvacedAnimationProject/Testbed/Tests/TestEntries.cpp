#include "../Framework/Test.h"
#include "../Framework/Render.h"
#include "../Framework/Main.h"
#include "../Framework/ParticleParameter.h"

#include "GL/freeglut.h"

#include "Sandbox.h"
#include "Maxwell.h"
#include "ParticlesSurfaceTension.h"
#include "LiquidTimer.h"
#include "Faucet.h"

TestEntry g_testEntries[] ={
	{ "Maxwell", Maxwell::Create },
	{"Liquid Timer", LiquidTimer::Create},
	{"Faucet", Faucet::Create},
	{ "Sandbox", Sandbox::Create },

	{NULL, NULL}
};
