#ifndef PARTICLE_PARAMETER_H
#define PARTICLE_PARAMETER_H
#include <Box2D/Box2D.h>

// Manages a list of parameter values that can be iterated through using simple UI elements.
class ParticleParameter{
	public:
		enum Options{ OptionStrictContacts = 1 << 0, OptionDrawShapes = 1 << 1,	OptionDrawParticles = 1 << 2,
					  OptionDrawJoints = 1 << 3, OptionDrawAABBs = 1 << 4, OptionDrawContactPoints = 1 << 5,
					  OptionDrawContactNormals = 1 << 6, OptionDrawContactImpulse = 1 << 7,	OptionDrawFrictionImpulse = 1 << 8,
					  OptionDrawCOMs = 1 << 9, OptionDrawStats = 1 << 10, OptionDrawProfile = 1 << 11
		};

		static const uint32 k_DefaultOptions;

		struct Value{	
			uint32 value;
			uint32 options;
			const char *name;
		};

		struct Definition{
			const Value *values;
			uint32 numValues;

			// Calculate the mask (bitwise OR) of all values in the definition.
			uint32 CalculateValueMask() const{
				uint32 mask = 0;
				for (uint32 i = 0; i < numValues; i++){
					mask |= values[i].value;
				}
				return mask;
			}
		};

	public:
		ParticleParameter() { Reset(); }
		void Reset();

		void SetDefinition(const Definition *definition, uint32 definitionCount);
		uint32 Get() const { return m_index; }	
		void Set(uint32 index);
		void Increment();
		void Decrement();
		bool Changed(bool * const restart);

		uint32 GetValue() const{ b2Assert(m_value);	return m_value->value; }
		const char* GetName() const{ b2Assert(m_value);	return m_value->name; }
		uint32 GetOptions() const{ b2Assert(m_value); return m_value->options; }	
		void SetRestartOnChange(bool enable) { m_restartOnChange = enable; }
		bool GetRestartOnChange() const { return m_restartOnChange; }
		int32 FindIndexByValue(uint32 value) const;

	protected:
		const Value* FindParticleParameterValue() const;

	private:
		uint32 m_index;
		bool m_changed;
		bool m_restartOnChange;
		const Value *m_value;
		const Definition *m_definition;
		uint32 m_definitionCount;
		uint32 m_valueCount;

	public:	
		static const Value *k_particleTypesPtr;
		static const uint32 k_particleTypesCount;	
		static const Value k_particleTypes[];
		static const Definition k_defaultDefinition[];
};

#endif  // PARTICLE_PARAMETER_H
