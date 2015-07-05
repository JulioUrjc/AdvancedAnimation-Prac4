#ifndef SCENE4_H
#define SCENE4_H

#include <iostream>
using namespace std;

class Scene4 : public Test{
	public:
		Scene4(){
			Zoom = 25.0f;

			porcentaje3 = 20;
			porcentaje = 50;
			porcentaje2 = 100;
			porcentaje4 = 70;

			m_position = 0;
			m_position2 = 0;
			m_position3 = 0;
			// Setup particle parameters.

			// Configure particle system parameters.
			m_particleSystem->SetRadius(8.065f);
			m_particleSystem->SetMaxParticleCount(100000);
			m_particleSystem->SetDestructionByAge(true);

			// Initialize the particle emitter1.
			{
				const float32 faucetLength = m_particleSystem->GetRadius() * 2.0f * k_faucetLength;
				m_emitter.SetParticleSystem(m_particleSystem);
				m_emitter.SetPosition(b2Vec2(250.0f, 550.0f));
				m_emitter.SetVelocity(b2Vec2(-0.1f, 0.0f));
				m_emitter.SetSize(b2Vec2(0.0f, faucetLength));
				m_emitter.SetColor(b2ParticleColor(255, 0, 0, 255));
				m_emitter.SetEmitRate(400.0f);
				m_emitter.SetParticleFlags(TestMain::GetParticleParameterValue());
			}

			// Initialize the particle emitter2.
			{
				const float32 faucetLength = m_particleSystem->GetRadius() * 2.0f * k_faucetLength;
				m_emitter2.SetParticleSystem(m_particleSystem);
				m_emitter2.SetPosition(b2Vec2(2.5f, 400.0f));
				m_emitter2.SetVelocity(b2Vec2(0.1f, 0.0f));
				m_emitter2.SetSize(b2Vec2(0.0f, faucetLength));
				m_emitter2.SetColor(b2ParticleColor(255, 0, 0, 255));
				m_emitter2.SetEmitRate(400.0f);
				m_emitter2.SetParticleFlags(TestMain::GetParticleParameterValue());
			}

			// Initialize the particle emitter3.
			{
				const float32 faucetLength = m_particleSystem->GetRadius() * 2.0f * k_faucetLength;
				m_emitter3.SetParticleSystem(m_particleSystem);
				m_emitter3.SetPosition(b2Vec2(-300.5f, 350.0f));
				m_emitter3.SetVelocity(b2Vec2(0.1f, 0.0f));
				m_emitter3.SetSize(b2Vec2(0.0f, faucetLength));
				m_emitter3.SetColor(b2ParticleColor(255, 0, 0, 255));
				m_emitter3.SetEmitRate(400.0f);
				m_emitter3.SetParticleFlags(TestMain::GetParticleParameterValue());
			}

			// Initialize the particle emitter4.
			{
				const float32 faucetLength = m_particleSystem->GetRadius() * 2.0f * k_faucetLength;
				m_emitter4.SetParticleSystem(m_particleSystem);
				m_emitter4.SetPosition(b2Vec2(350.5f, 200.0f));
				m_emitter4.SetVelocity(b2Vec2(0.1f, 0.0f));
				m_emitter4.SetSize(b2Vec2(0.0f, faucetLength));
				m_emitter4.SetColor(b2ParticleColor(255, 0, 0, 255));
				m_emitter4.SetEmitRate(400.0f);
				m_emitter4.SetParticleFlags(TestMain::GetParticleParameterValue());
			}
			
			m_barrierBody = NULL;
			m_barrierBody2 = NULL;
			m_barrierBody3 = NULL;

			m_world->SetGravity(b2Vec2(0, -10));

			// Create brain container
			{
				b2BodyDef bd;
				b2Body* ground = m_world->CreateBody(&bd);
				b2ChainShape bodyBrain;
				b2Vec2 brain[93] = {
					b2Vec2(633.0f, 601.0f), b2Vec2(621.0f, 565.0f), b2Vec2(594.0f, 561.0f), b2Vec2(537.0f, 573.0f), b2Vec2(531.0f, 562.0f),
					b2Vec2(505.0f, 555.0f), b2Vec2(498.0f, 574.0f), b2Vec2(354.0f, 567.0f), b2Vec2(286.0f, 538.0f), b2Vec2(250.0f, 529.0f),
					b2Vec2(214.0f, 514.0f), b2Vec2(189.0f, 475.0f), b2Vec2(169.0f, 409.0f), b2Vec2(190.0f, 387.0f), b2Vec2(171.0f, 354.0f),
					b2Vec2(181.0f, 318.0f), b2Vec2(184.0f, 291.0f), b2Vec2(199.0f, 288.0f), b2Vec2(192.0f, 274.0f), b2Vec2(216.0f, 229.0f),
					b2Vec2(249.0f, 199.0f), b2Vec2(277.0f, 204.0f), b2Vec2(277.0f, 165.0f), b2Vec2(312.0f, 133.0f), b2Vec2(403.0f, 108.0f),
					b2Vec2(444.0f, 88.0f), b2Vec2(483.0f, 91.0f), b2Vec2(493.0f, 100.0f), b2Vec2(501.0f, 85.0f), b2Vec2(544.0f, 66.0f),
					b2Vec2(570.0f, 82.0f), b2Vec2(586.0f, 115.0f), b2Vec2(603.0f, 78.0f), b2Vec2(666.0f, 73.0f), b2Vec2(718.0f, 67.0f),
					b2Vec2(756.0f, 85.0f), b2Vec2(766.0f, 139.0f), b2Vec2(795.0f, 88.0f), b2Vec2(820.0f, 84.0f), b2Vec2(874.0f, 118.0f),
					b2Vec2(901.0f, 160.0f), b2Vec2(918.0f, 172.0f), b2Vec2(924.0f, 139.0f), b2Vec2(954.0f, 168.0f), b2Vec2(925.0f, 241.0f),
					b2Vec2(949.0f, 255.0f), b2Vec2(978.0f, 186.0f), b2Vec2(1039.0f, 253.0f), b2Vec2(1018.0f, 316.0f), b2Vec2(1033.0f, 325.0f),
					b2Vec2(1045.0f, 289.0f), b2Vec2(1069.0f, 300.0f), b2Vec2(1075.0f, 325.0f), b2Vec2(1107.0f, 369.0f), b2Vec2(1099.0f, 397.0f),
					b2Vec2(1117.0f, 411.0f), b2Vec2(1095.0f, 450.0f), b2Vec2(1105.0f, 483.0f), b2Vec2(1132.0f, 492.0f), b2Vec2(1129.0f, 513.0f),
					b2Vec2(1119.0f, 528.0f), b2Vec2(1132.0f, 537.0f), b2Vec2(1137.0f, 580.0f), b2Vec2(1129.0f, 595.0f), b2Vec2(1132.0f, 619.0f),
					b2Vec2(1116.0f, 637.0f), b2Vec2(1128.0f, 675.0f), b2Vec2(1110.0f, 724.0f), b2Vec2(1083.0f, 742.0f), b2Vec2(1081.0f, 762.0f),
					b2Vec2(1062.0f, 777.0f), b2Vec2(1032.0f, 768.0f), b2Vec2(1023.0f, 742.0f), b2Vec2(1027.0f, 717.0f), b2Vec2(987.0f, 745.0f),
					b2Vec2(906.0f, 655.0f), b2Vec2(910.0f, 628.0f), b2Vec2(894.0f, 637.0f), b2Vec2(871.0f, 598.0f), b2Vec2(895.0f, 591.0f),
					b2Vec2(889.0f, 574.0f), b2Vec2(888.0f, 558.0f), b2Vec2(862.0f, 577.0f), b2Vec2(846.0f, 589.0f), b2Vec2(819.0f, 553.0f),
					b2Vec2(819.0f, 528.0f), b2Vec2(789.0f, 534.0f), b2Vec2(760.0f, 514.0f), b2Vec2(745.0f, 529.0f), b2Vec2(724.0f, 541.0f),
					b2Vec2(693.0f, 585.0f), b2Vec2(679.0f, 606.0f), b2Vec2(632.0f, 600.0f)
				};

				for (int i = 0; i < 93; ++i){
					brain[i] -= b2Vec2(600.0f, 800.0f);
					brain[i] = brain[i].Skew4();
				}

				bodyBrain.CreateLoop(brain, 93);
				b2FixtureDef def;
				def.shape = &bodyBrain;
				ground->CreateFixture(&def);
			}

			// Create brodman
			{
				b2BodyDef bd;
				b2Body* ground = m_world->CreateBody(&bd);
				b2ChainShape bodybrodman;
				b2Vec2 brodman[4] = {
					b2Vec2(286.0f, 538.0f), b2Vec2(586.0f, 115.0f), 
					b2Vec2(819.0f, 528.0f), b2Vec2(1095.0f, 450.0f)					
				};

				for (int i = 0; i < 4; ++i){
					brodman[i] -= b2Vec2(600.0f, 800.0f);
					brodman[i] = brodman[i].Skew4();
				}

				bodybrodman.CreateChain(brodman, 4);
				b2FixtureDef def;
				def.shape = &bodybrodman;
				ground->CreateFixture(&def);
			}

			// Enable the barrier.
			//EnableBarrier();			

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

			m_emitter4.SetParticleFlags(TestMain::GetParticleParameterValue());
			m_emitter4.SetColor(colorPorcentaje(porcentaje4));
			m_emitter4.Step(dt, NULL, 0); // Create the particles.
	
			m_debugDraw.DrawString(140, 60, "Areas de brodman");	
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
	static Test* Create(){ return new Scene4; }

	private:
		float32 m_particleColorOffset;
		RadialEmitter m_emitter;
		RadialEmitter m_emitter2;
		RadialEmitter m_emitter3;
		RadialEmitter m_emitter4;
		int32 porcentaje;
		int32 porcentaje2;
		int32 porcentaje3;
		int32 porcentaje4;

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

const float32 Scene4::k_faucetLength = 2.0f;
const float32 Scene4::k_faucetWidth = 0.1f;
const float32 Scene4::k_faucetHeight = 15.0f;

const float32 Scene4::k_containerWidth = 2.0f;
const float32 Scene4::k_containerHeight = 5.0f;

const float32 Scene4::k_containerHalfWidth = Scene4::k_containerWidth / 2.0f;
const float32 Scene4::k_containerHalfHeight = Scene4::k_containerHeight / 2.0f;
const float32 Scene4::k_barrierHeight = Scene4::k_containerHalfHeight / 100.0f;
const float32 Scene4::k_barrierMovementIncrement = Scene4::k_containerHalfHeight * 0.1f;


const int32 Scene4::k_maxParticleCount = 10000;
const ParticleParameter::Value Scene4::k_paramValues[] = {
	{ b2_waterParticle, ParticleParameter::k_DefaultOptions, "water" },
	{ b2_waterParticle, ParticleParameter::k_DefaultOptions | ParticleParameter::OptionStrictContacts, "water (strict)" },
	{ b2_viscousParticle, ParticleParameter::k_DefaultOptions, "viscous" },
	{ b2_powderParticle, ParticleParameter::k_DefaultOptions, "powder" },
	{ b2_tensileParticle, ParticleParameter::k_DefaultOptions, "tensile" },
	{ b2_colorMixingParticle, ParticleParameter::k_DefaultOptions, "color mixing" },
	{ b2_staticPressureParticle, ParticleParameter::k_DefaultOptions, "static pressure" }, };

const ParticleParameter::Definition Scene4::k_paramDef[] = { { Scene4::k_paramValues, B2_ARRAY_SIZE(Scene4::k_paramValues) }, };
const uint32 Scene4::k_paramDefCount = B2_ARRAY_SIZE(Scene4::k_paramDef);

#endif