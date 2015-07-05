#include "../Framework/Test.h"
#include "../Framework/Render.h"
#include "../Framework/Main.h"
#include "../Framework/ParticleParameter.h"
#include "../Framework/ParticleEmitter.h"

#include "GL/freeglut.h"


#include "LiquidTimer.h"
#include "Scene1.h"
#include "Scene2.h"
#include "Scene3.h"

int32 p1, p2, p3;

TestEntry g_testEntries[] ={
	{"Scene1", Scene1::Create },
	{"Scene2", Scene2::Create },
	{"Scene3", Scene3::Create},
	{"Liquid Timer", LiquidTimer::Create},
	{NULL, NULL}
};
