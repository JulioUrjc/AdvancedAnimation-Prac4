#ifndef SCENE3_H
#define SCENE3_H

#include <iostream>
using namespace std;

class Scene3 : public Test{
	public:
		Scene3(){
			Zoom = 0.15f;

			porcentaje3 = 0;
			porcentaje = 50;
			porcentaje2 = 100;

			m_position = 0;
			m_position2 = 0;
			m_position3 = 0;

			// Configure particle system parameters.
			m_particleSystem->SetRadius(0.065f);
			m_particleSystem->SetMaxParticleCount(k_maxParticleCount);
			m_particleSystem->SetDestructionByAge(true);

			// Initialize the particle emitter1.
			{
				const float32 faucetLength = m_particleSystem->GetRadius() * 2.0f * k_faucetLength;
				m_emitter.SetParticleSystem(m_particleSystem);
				m_emitter.SetPosition(b2Vec2(0.0f,0.15f));
				m_emitter.SetVelocity(b2Vec2(0.1f, 0.0f));
				m_emitter.SetSize(b2Vec2(0.0f, faucetLength));
				m_emitter.SetColor(b2ParticleColor(255, 0, 0, 255));
				m_emitter.SetEmitRate(120.0f);
				m_emitter.SetParticleFlags(TestMain::GetParticleParameterValue());
			}

			// Initialize the particle emitter2.
			{
				const float32 faucetLength = m_particleSystem->GetRadius() * 2.0f * k_faucetLength;
				m_emitter2.SetParticleSystem(m_particleSystem);
				m_emitter2.SetPosition(b2Vec2(2.5f, 0.15f));
				m_emitter2.SetVelocity(b2Vec2(0.1f, 0.0f));
				m_emitter2.SetSize(b2Vec2(0.0f, faucetLength));
				m_emitter2.SetColor(b2ParticleColor(255, 0, 0, 255));
				m_emitter2.SetEmitRate(120.0f);
				m_emitter2.SetParticleFlags(TestMain::GetParticleParameterValue());
			}

			// Initialize the particle emitter3.
			{
				const float32 faucetLength = m_particleSystem->GetRadius() * 2.0f * k_faucetLength;
				m_emitter3.SetParticleSystem(m_particleSystem);
				m_emitter3.SetPosition(b2Vec2(-2.5f, 0.15f));
				m_emitter3.SetVelocity(b2Vec2(0.1f, 0.0f));
				m_emitter3.SetSize(b2Vec2(0.0f, faucetLength));
				m_emitter3.SetColor(b2ParticleColor(255, 0, 0, 255));
				m_emitter3.SetEmitRate(120.0f);
				m_emitter3.SetParticleFlags(TestMain::GetParticleParameterValue());
			}
			
			m_barrierBody = NULL;
			m_barrierBody2 = NULL;
			m_barrierBody3 = NULL;

			m_world->SetGravity(b2Vec2(0, -10));

			// Create the container 1.
			{
				b2BodyDef bd;
				b2Body* ground = m_world->CreateBody(&bd);
				b2ChainShape shape;
				const b2Vec2 vertices[4] = { b2Vec2(-k_containerHalfWidth, 0), b2Vec2(k_containerHalfWidth, 0),
				b2Vec2(k_containerHalfWidth, k_containerHeight),b2Vec2(-k_containerHalfWidth, k_containerHeight) };
				shape.CreateLoop(vertices, 4);
				b2FixtureDef def;
				def.shape = &shape;
				def.density = 0;
				def.restitution = 1.0;
				ground->CreateFixture(&def);
			}

			// Create the container 2.
			{
				b2BodyDef bd;
				b2Body* ground = m_world->CreateBody(&bd);
				b2ChainShape shape;
				const b2Vec2 vertices[4] = { b2Vec2(-k_containerHalfWidth + 2.5f, 0), b2Vec2(k_containerHalfWidth + 2.5f, 0),
				b2Vec2(k_containerHalfWidth + 2.5f, k_containerHeight), b2Vec2(-k_containerHalfWidth + 2.5f, k_containerHeight) };
				shape.CreateLoop(vertices, 4);
				b2FixtureDef def;
				def.shape = &shape;
				def.density = 0;
				def.restitution = 1.0;
				ground->CreateFixture(&def);
			}

			// Create the container 3.
			{
				b2BodyDef bd;
				b2Body* ground = m_world->CreateBody(&bd);
				b2ChainShape shape;
				const b2Vec2 vertices[4] = { b2Vec2(-k_containerHalfWidth - 2.5f, 0), b2Vec2(k_containerHalfWidth - 2.5f, 0),
				b2Vec2(k_containerHalfWidth - 2.5f, k_containerHeight), b2Vec2(-k_containerHalfWidth - 2.5f, k_containerHeight) };
				shape.CreateLoop(vertices, 4);
				b2FixtureDef def;
				def.shape = &shape;
				def.density = 0;
				def.restitution = 1.0;
				ground->CreateFixture(&def);
			}

			// Enable the barrier.
			EnableBarrier();			

			//Initial barrier positions
			MoveDivider(m_position + k_barrierMovementIncrement*correspondiente(porcentaje), 0); 
			MoveDivider2(m_position2 + k_barrierMovementIncrement*correspondiente(porcentaje2), 0);
			MoveDivider3(m_position3 + k_barrierMovementIncrement*correspondiente(porcentaje3), 0);

			// Don't restart the test when changing particle types.
			TestMain::SetRestartOnParticleParameterChange(false);
			// Limit the set of particle types.
			TestMain::SetParticleParameters(k_paramDef, k_paramDefCount);
			// Create the particles.
			//ResetParticles();
		}

		int correspondiente(int p){
			return p/5;
		}

		// Disable the barrier.
		void DisableBarrier(){ 
			if (m_barrierBody){ m_world->DestroyBody(m_barrierBody); m_barrierBody = NULL; } 
			if (m_barrierBody2){ m_world->DestroyBody(m_barrierBody2); m_barrierBody2 = NULL; }
			if (m_barrierBody3){ m_world->DestroyBody(m_barrierBody3); m_barrierBody3 = NULL; }
		}

		// Enable the barrier.
		void EnableBarrier(){
			if (!m_barrierBody){
				b2BodyDef bd;
				m_barrierBody = m_world->CreateBody(&bd);
				b2PolygonShape barrierShape;
				barrierShape.SetAsBox(k_containerHalfWidth, k_barrierHeight, b2Vec2(0, m_position), 0);
				b2FixtureDef def;
				def.shape = &barrierShape;
				def.density = 0;
				def.restitution = 1.0;
				m_barrierBody->CreateFixture(&def);
			}
			if (!m_barrierBody2){
				b2BodyDef bd2;
				m_barrierBody2 = m_world->CreateBody(&bd2);
				b2PolygonShape barrierShape2;
				barrierShape2.SetAsBox(k_containerHalfWidth, k_barrierHeight, b2Vec2(2.5f, m_position2), 0);
				b2FixtureDef def2;
				def2.shape = &barrierShape2;
				def2.density = 0;
				def2.restitution = 1.0;
				m_barrierBody2->CreateFixture(&def2);
			}
			if (!m_barrierBody3){
				b2BodyDef bd;
				m_barrierBody3 = m_world->CreateBody(&bd);
				b2PolygonShape barrierShape;
				barrierShape.SetAsBox(k_containerHalfWidth, k_barrierHeight, b2Vec2(-2.5f, m_position3), 0);
				b2FixtureDef def;
				def.shape = &barrierShape;
				def.density = 0;
				def.restitution = 1.0;
				m_barrierBody3->CreateFixture(&def);
			}
		}

		// Enable / disable the barrier.
		void ToggleBarrier(){ if (m_barrierBody){ DisableBarrier(); }else{ EnableBarrier(); } }

		

		b2ParticleColor colorPorcentaje(int p){
			int intervalo = (p>0) + (p>20) + (p>40) + (p>60) + (p>80);
			switch (intervalo){
				case 0: return b2ParticleColor(0, 0, 0, 0); // Transparente
				case 1:	return b2ParticleColor(0, 255, 0, 255); // green
				case 2: return b2ParticleColor(255, 0, 255, 255);
				case 3: return b2ParticleColor(0, 0, 255, 255); // blue
				case 4: return b2ParticleColor(255, 255, 0, 255);
				case 5: return b2ParticleColor(255, 0, 0, 255); //red
				default:
					return b2ParticleColor(255, 255, 255, 255);
					break;
			}
		}


		virtual void Step(Settings* settings){
			const float32 dt = 1.0f / settings->hz;
			Test::Step(settings);
			m_particleColorOffset += dt;
			// Keep m_particleColorOffset in the range 0.0f..k_ParticleColorsCount.
			if (m_particleColorOffset >= (float32)k_ParticleColorsCount){ m_particleColorOffset -= (float32)k_ParticleColorsCount; }
			// Propagate the currently selected particle flags.
			m_emitter.SetParticleFlags(TestMain::GetParticleParameterValue());
			m_emitter.SetColor(colorPorcentaje(porcentaje));
			m_emitter.Step(dt, NULL, 0); // Create the particles.

			m_emitter2.SetParticleFlags(TestMain::GetParticleParameterValue());
			m_emitter2.SetColor(colorPorcentaje(porcentaje2));
			m_emitter2.Step(dt, NULL, 0); // Create the particles.

			m_emitter3.SetParticleFlags(TestMain::GetParticleParameterValue());
			m_emitter3.SetColor(colorPorcentaje(porcentaje3));
			m_emitter3.Step(dt, NULL, 0); // Create the particles.
	
			m_debugDraw.DrawString(140, 60, "Porcentaje: %i", porcentaje3);	
			m_debugDraw.DrawString(400, 60, "Porcentaje: %i", porcentaje);
			m_debugDraw.DrawString(650, 60, "Porcentaje: %i", porcentaje2);

			m_debugDraw.DrawString(140, 80, "Posicion1: %.2f", m_position3);
			m_debugDraw.DrawString(400, 80, "Posicion2: %.2f", m_position);
			m_debugDraw.DrawString(650, 80, "Posicion3: %.2f", m_position2);
		}

		// Reset the particles and the barrier.
		void Reset(){ DisableBarrier();	/*ResetParticles();*/ EnableBarrier(); }

		// Move the divider / barrier.
		void MoveDivider(const float32 newPosition, const int32 inc){
			m_position = b2Clamp(newPosition, 0.0f, k_containerHeight );
			porcentaje = b2Clamp(porcentaje + inc, 0, 100);
			Reset();
		}
		// Move the divider / barrier2.
		void MoveDivider2(const float32 newPosition, const int32 inc){
			m_position2 = b2Clamp(newPosition, 0.0f, k_containerHeight);
			porcentaje2 = b2Clamp(porcentaje2 + inc, 0, 100);
			Reset();
		}
		// Move the divider / barrier3.
		void MoveDivider3(const float32 newPosition, const int32 inc){
			m_position3 = b2Clamp(newPosition, 0.0f, k_containerHeight);
			porcentaje3 = b2Clamp(porcentaje3+inc, 0, 100);
			Reset();
		}

	virtual void Keyboard(unsigned char key){
		switch (key){
			case 'a':
				ToggleBarrier();
				cout << "Enable / disable the barrier "<< endl;
				break;
			case 'f':
				MoveDivider(m_position + k_barrierMovementIncrement,5);
				cout <<"Move the location of the divider up" << endl;
				break;
			case 'g':
				MoveDivider(m_position - k_barrierMovementIncrement,-5);
				cout <<"Move the location of the divider down" << endl;
				break;
			case 'v':
				MoveDivider2(m_position2 + k_barrierMovementIncrement,5);
				cout << "Move the location of the divider up" << endl;
				break;
			case 'b':
				MoveDivider2(m_position2 - k_barrierMovementIncrement,-5);
				cout << "Move the location of the divider down" << endl;
				break;
			case 'n':
				MoveDivider3(m_position3 + k_barrierMovementIncrement,5);
				cout << "Move the location of the divider up" << endl;
				break;
			case 'm':
				MoveDivider3(m_position3 - k_barrierMovementIncrement,-5);
				cout << "Move the location of the divider down" << endl;
				break;
			case 'k':
				
				cout << "Change Color" << endl;
				break;
				//SetGroupFlags(uint32 flags);
			default:
				Test::Keyboard(key);
				break;
		}
	}

	// Determine whether a point is in the container.
	bool InContainer(const b2Vec2& p) const{ return p.x >= -k_containerHalfWidth && p.x <= k_containerHalfWidth && p.y >= 0.0f && p.y <= k_containerHalfHeight * 2.0f;}
	float32 GetDefaultViewZoom() const{ return Zoom; }
	static Test* Create(){ return new Scene3; }

	private:
		float32 m_particleColorOffset;
		RadialEmitter m_emitter;
		RadialEmitter m_emitter2;
		RadialEmitter m_emitter3;
		int32 porcentaje;
		int32 porcentaje2;
		int32 porcentaje3;

		float32 m_position ;
		float32 m_position2;
		float32 m_position3;
		
		b2Body* m_barrierBody;
		b2Body* m_barrierBody2;
		b2Body* m_barrierBody3;
		b2ParticleGroup* m_particleGroup;

	private:
		static const int32 k_maxParticleCount;
		static const ParticleParameter::Value k_paramValues[];
		static const ParticleParameter::Definition k_paramDef[];
		static const uint32 k_paramDefCount;

		static const float32 k_containerWidth;
		static const float32 k_containerHeight;
		static const float32 k_containerHalfWidth;
		static const float32 k_containerHalfHeight;
		static const float32 k_barrierHeight;
		static const float32 k_barrierMovementIncrement;

		static const float32 k_faucetLength;
		static const float32 k_faucetWidth;
		static const float32 k_faucetHeight;	
};

const float32 Scene3::k_faucetLength = 2.0f;
const float32 Scene3::k_faucetWidth = 0.1f;
const float32 Scene3::k_faucetHeight = 15.0f;

const float32 Scene3::k_containerWidth = 2.0f;
const float32 Scene3::k_containerHeight = 5.0f;

const float32 Scene3::k_containerHalfWidth = Scene3::k_containerWidth / 2.0f;
const float32 Scene3::k_containerHalfHeight = Scene3::k_containerHeight / 2.0f;
const float32 Scene3::k_barrierHeight = Scene3::k_containerHalfHeight / 100.0f;
const float32 Scene3::k_barrierMovementIncrement = Scene3::k_containerHalfHeight * 0.1f;

const int32 Scene3::k_maxParticleCount = 10000;
const ParticleParameter::Value Scene3::k_paramValues[] = {
	{ b2_waterParticle, ParticleParameter::k_DefaultOptions, "water" },
	{ b2_waterParticle, ParticleParameter::k_DefaultOptions | ParticleParameter::OptionStrictContacts, "water (strict)" },
	{ b2_viscousParticle, ParticleParameter::k_DefaultOptions, "viscous" },
	{ b2_powderParticle, ParticleParameter::k_DefaultOptions, "powder" },
	{ b2_tensileParticle, ParticleParameter::k_DefaultOptions, "tensile" },
	{ b2_colorMixingParticle, ParticleParameter::k_DefaultOptions, "color mixing" },
	{ b2_staticPressureParticle, ParticleParameter::k_DefaultOptions, "static pressure" }, };

const ParticleParameter::Definition Scene3::k_paramDef[] = { { Scene3::k_paramValues, B2_ARRAY_SIZE(Scene3::k_paramValues) }, };
const uint32 Scene3::k_paramDefCount = B2_ARRAY_SIZE(Scene3::k_paramDef);

#endif