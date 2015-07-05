#ifndef SCENE1_H
#define SCENE1_H

#include <iostream>
using namespace std;

class Scene1 : public Test{
	public:
		Scene1(){
			Zoom = 0.15f;
			m_density = k_densityDefault;
			m_position = k_containerHalfHeight;
			m_position2 = k_containerHalfHeight;
			m_position3 = k_containerHalfHeight;
			m_particleGroup = NULL;
			m_temperature = k_temperatureDefault;
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
			// Create the particles.
			ResetParticles();
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

		// Destroy and recreate all particles.
		void ResetParticles(){
			if (m_particleGroup != NULL){ m_particleGroup->DestroyParticles();	m_particleGroup = NULL;	}

			//m_particleSystem->SetRadius(k_containerHalfWidth / 20.0f);
			m_particleSystem->SetRadius(k_containerHalfWidth / 20.0f);
			{
				b2PolygonShape shape;
				//shape.SetAsBox(m_density * k_containerHalfWidth, m_density * k_containerHalfHeight,	b2Vec2(0, k_containerHalfHeight), 0);
				shape.SetAsBox(9.005f, 0.05f, b2Vec2(0, 0.07f), 0);
				b2ParticleGroupDef pd;
				pd.flags = b2_powderParticle;
				//pd.flags = b2_waterParticle;
				pd.SetColor(255,0,0,255);
				pd.SetPosition(0,0);
				pd.shape = &shape;
				m_particleGroup = m_particleSystem->CreateParticleGroup(pd);
				b2Vec2* velocities = m_particleSystem->GetVelocityBuffer() + m_particleGroup->GetBufferIndex();
				for (int i = 0; i < m_particleGroup->GetParticleCount(); ++i){
					b2Vec2& v = *(velocities + i);
					v.Set(RandomFloat() + 1.0f, RandomFloat() + 1.0f);
					v.Normalize();
					v *= m_temperature;
				}
			}
		}

	virtual void Step(Settings* settings){
		Test::Step(settings);
		// Number of particles below (bottom) the barrier.
		// int32 top = 0;
		int32 bottom = 0;
		int32 bottom2 = 0;
		int32 bottom3 = 0;
		const int32 index = m_particleGroup->GetBufferIndex();
		b2Vec2* const velocities = m_particleSystem->GetVelocityBuffer() + index;
		b2Vec2* const positions = m_particleSystem->GetPositionBuffer() + index;

		for (int32 i = 0; i < m_particleGroup->GetParticleCount(); i++){
			// Add energy to particles based upon the temperature.
			b2Vec2& v = velocities[i];
			v.Normalize();
			v *= m_temperature;

			// Keep particles below the divider barrier position.
			b2Vec2& p = positions[i];
			if (p.y < m_position && p.x<2.5 && p.x>-2.5) bottom++;	//if (p.y > m_position)  top++; else bottom++;
			if (p.y < m_position2 && p.x>2.5) bottom2++;	//if (p.y > m_position2)  top++; else bottom2++;
			if (p.y < m_position3 && p.x<-2.5) bottom3++;	//if (p.y > m_position3)  top++; else bottom3++;
		}

		m_debugDraw.DrawString(700, 60, "Num Particles: %i", bottom);
		m_debugDraw.DrawString(700, 75, "Barrier Pos: %f", m_position);
		m_debugDraw.DrawString(700, 90, "Num Particles2: %i", bottom2);
		m_debugDraw.DrawString(700, 105, "Barrier Pos2: %f", m_position2);
		m_debugDraw.DrawString(700, 120, "Num Particles3: %i", bottom3);
		m_debugDraw.DrawString(700, 135, "Barrier Pos3: %f", m_position3);
	}

	// Reset the particles and the barrier.
	void Reset(){ DisableBarrier();	ResetParticles(); EnableBarrier(); }

	// Move the divider / barrier.
	void MoveDivider(const float32 newPosition){
		m_position = b2Clamp(newPosition, 0.0f, k_containerHeight );
		Reset();
	}
	// Move the divider / barrier2.
	void MoveDivider2(const float32 newPosition){
		m_position2 = b2Clamp(newPosition, 0.0f, k_containerHeight);
		Reset();
	}
	// Move the divider / barrier3.
	void MoveDivider3(const float32 newPosition){
		m_position3 = b2Clamp(newPosition, 0.0f, k_containerHeight);
		Reset();
	}

	virtual void Keyboard(unsigned char key){
		switch (key){
			case 'a':
				ToggleBarrier();
				cout << "Enable / disable the barrier "<< endl;
				break;
			case 's':
				m_density = b2Min(m_density * k_densityStep, k_densityMax); 
				Reset();
				cout <<"Increase the particle density" << endl;
				break;
			case 'd':
				m_density = b2Max(m_density / k_densityStep, k_densityMin);
				Reset();
				cout <<"Reduce the particle density" << endl;
				break;
			case 'f':
				MoveDivider(m_position + k_barrierMovementIncrement);
				cout <<"Move the location of the divider up" << endl;
				break;
			case 'g':
				MoveDivider(m_position - k_barrierMovementIncrement);
				cout <<"Move the location of the divider down" << endl;
				break;
			case 'v':
				MoveDivider2(m_position2 + k_barrierMovementIncrement);
				cout << "Move the location of the divider up" << endl;
				break;
			case 'b':
				MoveDivider2(m_position2 - k_barrierMovementIncrement);
				cout << "Move the location of the divider down" << endl;
				break;
			case 'n':
				MoveDivider3(m_position3 + k_barrierMovementIncrement);
				cout << "Move the location of the divider up" << endl;
				break;
			case 'm':
				MoveDivider3(m_position3 - k_barrierMovementIncrement);
				cout << "Move the location of the divider down" << endl;
				break;
			case 'h':
				m_temperature = b2Max(m_temperature - k_temperatureStep, k_temperatureMin);
				Reset();
				cout <<"Reduce the temperature (velocity of particles)" << endl;
				break;
			case 'j':
				m_temperature = b2Min(m_temperature + k_temperatureStep, k_temperatureMax);
				Reset();
				cout <<"Increase the temperature (velocity of particles)" << endl;
				break;
			case 'k':
				//ColorParticleGroup(m_particleGroup,0);
				//Reset();
				cout << "Change Color" << endl;
				break;
				//SetGroupFlags(uint32 flags);
			default:
				Test::Keyboard(key);
				break;
		}
	}

	// Determine whether a point is in the container.
	bool InContainer(const b2Vec2& p) const{
		return p.x >= -k_containerHalfWidth && p.x <= k_containerHalfWidth && p.y >= 0.0f && p.y <= k_containerHalfHeight * 2.0f;
	}

	float32 GetDefaultViewZoom() const{ return Zoom; }
	static Test* Create(){ return new Scene1; }

	private:
		float32 m_density;
		float32 m_position;
		float32 m_position2;
		float32 m_position3;
		float32 m_temperature;
		b2Body* m_barrierBody;
		b2Body* m_barrierBody2;
		b2Body* m_barrierBody3;
		b2ParticleGroup* m_particleGroup;

	private:
		static const float32 k_containerWidth;
		static const float32 k_containerHeight;
		static const float32 k_containerHalfWidth;
		static const float32 k_containerHalfHeight;
		static const float32 k_barrierHeight;
		static const float32 k_barrierMovementIncrement;
		static const float32 k_densityStep;
		static const float32 k_densityMin;
		static const float32 k_densityMax;
		static const float32 k_densityDefault;
		static const float32 k_temperatureStep;
		static const float32 k_temperatureMin;
		static const float32 k_temperatureMax;
		static const float32 k_temperatureDefault;
};

const float32 Scene1::k_containerWidth = 2.0f;
const float32 Scene1::k_containerHeight = 5.0f;

const float32 Scene1::k_containerHalfWidth = Scene1::k_containerWidth / 2.0f;
const float32 Scene1::k_containerHalfHeight = Scene1::k_containerHeight / 2.0f;
const float32 Scene1::k_barrierHeight = Scene1::k_containerHalfHeight / 100.0f;
const float32 Scene1::k_barrierMovementIncrement = Scene1::k_containerHalfHeight * 0.1f;

const float32 Scene1::k_densityStep = 1.25;
const float32 Scene1::k_densityMin = 0.01f;
const float32 Scene1::k_densityMax = 0.8f;
const float32 Scene1::k_densityDefault = 0.25f;
const float32 Scene1::k_temperatureStep = 0.2f;
const float32 Scene1::k_temperatureMin = 0.4f;
const float32 Scene1::k_temperatureMax = 10.0f;
const float32 Scene1::k_temperatureDefault = 5.0f;
#endif