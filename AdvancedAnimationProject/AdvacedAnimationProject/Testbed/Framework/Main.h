#ifndef MAIN_H
#define MAIN_H
#include <Box2D/Box2D.h>
#include "ParticleParameter.h"

namespace TestMain{

	void SetRestartOnParticleParameterChange(bool enable);
	uint32 SetParticleParameterValue(uint32 value);
	uint32 GetParticleParameterValue();
	void SetParticleParameters(const ParticleParameter::Definition * const particleParameterDef, const uint32 particleParameterDefCount);
}  

#endif  // MAIN_H
