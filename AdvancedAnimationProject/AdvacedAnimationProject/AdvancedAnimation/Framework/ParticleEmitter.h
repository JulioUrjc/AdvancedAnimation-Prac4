#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H
#include <math.h>
#include <set>

// Callback used to notify the user of created particles.
class EmittedParticleCallback {
	public:
		EmittedParticleCallback() {}
		virtual ~EmittedParticleCallback() {}

		virtual void ParticleCreated(b2ParticleSystem * const system, const int32 particleIndex) = 0;
};

// Emit particles from a circular region.
class RadialEmitter{
	public:
		// Initialize a particle emitter.
		RadialEmitter() : m_particleSystem(NULL), m_callback(NULL), m_speed(0.0f),
			m_emitRate(1.0f), m_emitRemainder(0.0f), m_flags(b2_waterParticle),	m_group(NULL){}

		~RadialEmitter(){ SetGroup(NULL); }

		void SetPosition(const b2Vec2& origin){ m_origin = origin; }
		const b2Vec2& GetPosition() const{ return m_origin; }
		void SetSize(const b2Vec2& size){ m_halfSize = size * 0.5f; }
		b2Vec2 GetSize() const{	return m_halfSize * 2.0f; }
		void SetVelocity(const b2Vec2& velocity){ m_startingVelocity = velocity; }
		const b2Vec2& GetVelocity() const{ return m_startingVelocity; }
		void SetSpeed(const float32 speed){	m_speed = speed; }		
		float32 GetSpeed() const{ return m_speed; }
		void SetParticleFlags(uint32 flags){ m_flags = flags; }		
		uint32 GetParticleFlags() const{ return m_flags;	}
		void SetColor(const b2ParticleColor& color){ m_color = color; }
		const b2ParticleColor& GetColor() const{ return m_color; }	
		void SetEmitRate(const float32 emitRate){ m_emitRate = emitRate; }		
		float32 GetEmitRate() const{ return m_emitRate;	}	
		void SetParticleSystem(b2ParticleSystem * const particleSystem){ m_particleSystem = particleSystem; }
		b2ParticleSystem* GetParticleSystem() const{ return m_particleSystem; }
		void SetCallback(EmittedParticleCallback* const callback){ m_callback = callback; }
		EmittedParticleCallback* GetCallback() const{ return m_callback;	}
		b2ParticleGroup* GetGroup() const{ return m_group; }

		void SetGroup(b2ParticleGroup * const group){
			if (m_group){
				m_group->SetGroupFlags(m_group->GetGroupFlags() &  ~b2_particleGroupCanBeEmpty);
			}
			m_group = group;
			if (m_group){
				m_group->SetGroupFlags(m_group->GetGroupFlags() |  b2_particleGroupCanBeEmpty);
			}
		}

		int32 Step(const float32 dt, int32* const particleIndices, const int32 particleIndicesCount){
			b2Assert(m_particleSystem);
			int32 numberOfParticlesCreated = 0;
			// How many (fractional) particles should we have emitted this frame?
			m_emitRemainder += m_emitRate * dt;

			b2ParticleDef pd;
			pd.color = m_color;
			pd.flags = m_flags;
			pd.group = m_group;

			// Keep emitting particles on this frame until we only have a fractional particle left.
			while (m_emitRemainder > 1.0f) {
				m_emitRemainder -= 1.0f;

				// Randomly pick a position within the emitter's radius.
				const float32 angle = Random() * 2.0f * b2_pi;
				// Distance from the center of the circle.
				const float32 distance = Random();
				b2Vec2 positionOnUnitCircle(sin(angle), cos(angle));

				// Initial position.
				pd.position.Set(
					m_origin.x + positionOnUnitCircle.x * distance * m_halfSize.x,
					m_origin.y + positionOnUnitCircle.y * distance * m_halfSize.y);
				// Send it flying
				pd.velocity = m_startingVelocity;
				if (m_speed != 0.0f){
					pd.velocity += positionOnUnitCircle * m_speed;
				}

				const int32 particleIndex = m_particleSystem->CreateParticle(pd);
				if (m_callback){
					m_callback->ParticleCreated(m_particleSystem, particleIndex);
				}
				if (particleIndices && numberOfParticlesCreated < particleIndicesCount){
					particleIndices[numberOfParticlesCreated] = particleIndex;
				}
				++numberOfParticlesCreated;
			}
			return numberOfParticlesCreated;
		}

		private:
			static float32 Random(){ return ((float32)rand() / (float32)RAND_MAX); } // number 0.0f..1.0f.

		private:
			
			b2ParticleSystem *m_particleSystem;		// Pointer to global world			
			EmittedParticleCallback *m_callback;	// Called for each created particle.		
			b2Vec2 m_origin;						// Center of particle emitter		
			b2Vec2 m_startingVelocity;				// Launch direction.			
			float32 m_speed;						// Speed particles are emitted		
			b2Vec2 m_halfSize;						// Half width / height of particle emitter		
			float32 m_emitRate;						// Particles per second		
			b2ParticleColor m_color;				// Initial color of particle emitted.		
			float32 m_emitRemainder;				// Number particles to emit on the next frame	
			uint32 m_flags;							// Flags for created particles, see b2ParticleFlag.	
			b2ParticleGroup* m_group;				// Group to put newly created particles in.
};

#endif  // PARTICLE_EMITTER_H


