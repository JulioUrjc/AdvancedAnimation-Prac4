#ifndef MAXWELL_H
#define MAXWELL_H

	class Maxwell : public Test{

			public:
				Maxwell(){
					m_density = k_densityDefault;
					m_position = k_containerHalfHeight;
					m_particleGroup = NULL;
					m_temperature = k_temperatureDefault;
					m_barrierBody = NULL;

					m_world->SetGravity(b2Vec2(0, 0));

					// Create the container.
					{
						b2BodyDef bd;
						b2Body* ground = m_world->CreateBody(&bd);
						b2ChainShape shape;
						const b2Vec2 vertices[4] = {
							b2Vec2(-k_containerHalfWidth, 0),
							b2Vec2(k_containerHalfWidth, 0),
							b2Vec2(k_containerHalfWidth, k_containerHeight),
							b2Vec2(-k_containerHalfWidth, k_containerHeight)};
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
					if (m_barrierBody){
						m_world->DestroyBody(m_barrierBody);
						m_barrierBody = NULL;
					}
				}

				// Enable the barrier.
				void EnableBarrier(){
					if (!m_barrierBody){
						b2BodyDef bd;
						m_barrierBody = m_world->CreateBody(&bd);
						b2PolygonShape barrierShape;
						barrierShape.SetAsBox(k_containerHalfWidth, k_barrierHeight,
											  b2Vec2(0, m_position), 0);
						b2FixtureDef def;
						def.shape = &barrierShape;
						def.density = 0;
						def.restitution = 1.0;
						m_barrierBody->CreateFixture(&def);
					}
				}

				// Enable / disable the barrier.
				void ToggleBarrier(){
					if (m_barrierBody){	DisableBarrier();}
					else{ EnableBarrier(); }
				}

				// Destroy and recreate all particles.
				void ResetParticles(){
						if (m_particleGroup != NULL){
							m_particleGroup->DestroyParticles();
							m_particleGroup = NULL;
						}

						m_particleSystem->SetRadius(k_containerHalfWidth / 20.0f);
						{
							b2PolygonShape shape;
							shape.SetAsBox(m_density * k_containerHalfWidth,
										   m_density * k_containerHalfHeight,
										   b2Vec2(0, k_containerHalfHeight), 0);
							b2ParticleGroupDef pd;
							pd.flags = b2_powderParticle;
							pd.shape = &shape;
							m_particleGroup = m_particleSystem->CreateParticleGroup(pd);
							b2Vec2* velocities =
								m_particleSystem->GetVelocityBuffer() +
								m_particleGroup->GetBufferIndex();
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

					// Number of particles above (top) and below (bottom) the barrier.
					int32 top = 0;
					int32 bottom = 0;
					const int32 index = m_particleGroup->GetBufferIndex();
					b2Vec2* const velocities =	m_particleSystem->GetVelocityBuffer() + index;
					b2Vec2* const positions =	m_particleSystem->GetPositionBuffer() + index;

					for (int32 i = 0; i < m_particleGroup->GetParticleCount(); i++){
						// Add energy to particles based upon the temperature.
						b2Vec2& v = velocities[i];
						v.Normalize();
						v *= m_temperature;

						// Keep track of the number of particles above / below the divider / barrier position.
						b2Vec2& p = positions[i];
						if (p.y > m_position)  top++; else bottom++;
					}

					// Calculate a score based upon the difference in pressure between the
					// upper and lower divisions of the container.
					const float32 topPressure = top / (k_containerHeight - m_position);
					const float32 botPressure = bottom / m_position;
					m_debugDraw.DrawString(10, 75, "Score: %f",	topPressure > 0.0f ? botPressure / topPressure - 1.0f : 0.0f);
				}

				// Reset the particles and the barrier.
				void Reset(){
					DisableBarrier();
					ResetParticles();
					EnableBarrier();
				}

				// Move the divider / barrier.
				void MoveDivider(const float32 newPosition){
					m_position = b2Clamp(newPosition, k_barrierMovementIncrement, k_containerHeight - k_barrierMovementIncrement);
					Reset();
				}

				virtual void Keyboard(unsigned char key){
					switch(key){
						case 'a':			
							ToggleBarrier();	// Enable / disable the barrier.
							break;
						case '=':		
							m_density = b2Min(m_density * k_densityStep, k_densityMax); // Increase the particle density.
							Reset();
							break;
						case '-':			
							m_density = b2Max(m_density / k_densityStep, k_densityMin); // Reduce the particle density.
							Reset();
							break;
						case '.':		
							MoveDivider(m_position + k_barrierMovementIncrement); // Move the location of the divider up.
							break;
						case ',':			
							MoveDivider(m_position - k_barrierMovementIncrement); // Move the location of the divider down.
							break;
						case ';':		
							m_temperature = b2Max(m_temperature - k_temperatureStep, k_temperatureMin); // Reduce the temperature (velocity of particles).
							Reset();
							break;
						case '\'':			
							m_temperature = b2Min(m_temperature + k_temperatureStep, k_temperatureMax); // Increase the temperature (velocity of particles).
							Reset();
							break;
						default:
							Test::Keyboard(key);
							break;
					}
				}

				// Determine whether a point is in the container.
				bool InContainer(const b2Vec2& p) const{
					return p.x >= -k_containerHalfWidth && p.x <= k_containerHalfWidth && p.y >= 0.0f && p.y <= k_containerHalfHeight * 2.0f;
				}

				float32 GetDefaultViewZoom() const{
					return 0.1f;
				}

				static Test* Create(){
					return new Maxwell;
				}

			private:
				float32 m_density;
				float32 m_position;
				float32 m_temperature;
				b2Body* m_barrierBody;
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

	const float32 Maxwell::k_containerWidth = 2.0f;
	const float32 Maxwell::k_containerHeight = 4.0f;
	const float32 Maxwell::k_containerHalfWidth = Maxwell::k_containerWidth / 2.0f;
	const float32 Maxwell::k_containerHalfHeight = Maxwell::k_containerHeight / 2.0f;
	const float32 Maxwell::k_barrierHeight =Maxwell::k_containerHalfHeight / 100.0f;
	const float32 Maxwell::k_barrierMovementIncrement =	Maxwell::k_containerHalfHeight * 0.1f;
	const float32 Maxwell::k_densityStep = 1.25;
	const float32 Maxwell::k_densityMin = 0.01f;
	const float32 Maxwell::k_densityMax = 0.8f;
	const float32 Maxwell::k_densityDefault = 0.25f;
	const float32 Maxwell::k_temperatureStep = 0.2f;
	const float32 Maxwell::k_temperatureMin = 0.4f;
	const float32 Maxwell::k_temperatureMax = 10.0f;
	const float32 Maxwell::k_temperatureDefault = 5.0f;
#endif

/*virtual void MouseDown(const b2Vec2& p){
if (!InContainer(p)){
Test::MouseDown(p);
}
}*/

/*virtual void MouseUp(const b2Vec2& p){
if (InContainer(p)){
ToggleBarrier();
}else{
MoveDivider(p.y);
Test::MouseUp(p);
}
}*/