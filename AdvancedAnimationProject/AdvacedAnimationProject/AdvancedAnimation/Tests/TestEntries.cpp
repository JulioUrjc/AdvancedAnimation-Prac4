#include "../Framework/Test.h"
#include "../Framework/Render.h"
#include "../Framework/Main.h"
#include "../Framework/ParticleParameter.h"
#include "../Framework/ParticleEmitter.h"

#include "GL/freeglut.h"

#include "Scene1.h"
#include "Scene2.h"
#include "Scene3.h"
#include "Scene4.h"

TestEntry g_testEntries[] ={
	{"Scene1", Scene1::Create },
	{"Scene2", Scene2::Create },
	{"Scene3", Scene3::Create },
	{"Scene4", Scene4::Create },
	{NULL, NULL}
};
