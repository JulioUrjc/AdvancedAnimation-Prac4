#ifndef TEST_H
#define TEST_H

#include <Box2D/Box2D.h>
#include "Render.h"
#include "ParticleParameter.h"

#include <stdlib.h>

class Test;
struct Settings;

typedef Test* TestCreateFcn();

#define	RAND_LIMIT	32767
#define DRAW_STRING_NEW_LINE 25

/// Random number in range [-1,1]
inline float32 RandomFloat(){
	float32 r = (float32)(rand() & (RAND_LIMIT));
	r /= RAND_LIMIT;
	r = 2.0f * r - 1.0f;
	return r;
}

/// Random floating point number in range [lo, hi]
inline float32 RandomFloat(float32 lo, float32 hi){
	float32 r = (float32)(rand() & (RAND_LIMIT));
	r /= RAND_LIMIT;
	r = (hi - lo) * r + lo;
	return r;
}

/// Test settings. Some can be controlled in the GUI.
struct Settings{
	Settings(){
		viewCenter.Set(0.0f, 20.0f);
		hz = 60.0f;
		velocityIterations = 8;
		positionIterations = 3;
		// Particle iterations are needed for numerical stability in particle
		// simulations with small particles and relatively high gravity.
		// b2CalculateParticleIterations helps to determine the number.
		particleIterations = b2CalculateParticleIterations(10, 0.04f, 1 / hz);
		drawShapes = 1;
		drawParticles = 1;
		drawJoints = 0;
		drawAABBs = 0;
		drawContactPoints = 0;
		drawContactNormals = 0;
		drawContactImpulse = 0;
		drawFrictionImpulse = 0;
		drawCOMs = 0;
		drawStats = 0;
		drawProfile = 0;
		enableWarmStarting = 1;
		enableContinuous = 1;
		enableSubStepping = 0;
		enableSleep = 1;
		pause = 0;
		singleStep = 0;
		printStepTimeStats = 1;
		stepTimeOut = 0;
		strictContacts = 0;
	}

	b2Vec2 viewCenter;
	float32 hz;
	int32 velocityIterations;
	int32 positionIterations;
	int32 particleIterations;
	int32 drawShapes;
	int32 drawParticles;
	int32 drawJoints;
	int32 drawAABBs;
	int32 drawContactPoints;
	int32 drawContactNormals;
	int32 drawContactImpulse;
	int32 drawFrictionImpulse;
	int32 drawCOMs;
	int32 drawStats;
	int32 drawProfile;
	int32 enableWarmStarting;
	int32 enableContinuous;
	int32 enableSubStepping;
	int32 enableSleep;
	int32 pause;
	int32 singleStep;
	int32 printStepTimeStats;
	int32 strictContacts;

	/// Measures how long did the world step took, in ms
	float32 stepTimeOut;
};

struct TestEntry{
	const char *name;
	TestCreateFcn *createFcn;
};

extern TestEntry g_testEntries[];

class DestructionListener : public b2DestructionListener{
	public:
		void SayGoodbye(b2Fixture* fixture) { B2_NOT_USED(fixture); }
		void SayGoodbye(b2Joint* joint);
		void SayGoodbye(b2ParticleGroup* group);

		Test* test;
};

const int32 k_maxContactPoints = 2048;

struct ContactPoint{
	b2Fixture* fixtureA;
	b2Fixture* fixtureB;
	b2Vec2 normal;
	b2Vec2 position;
	b2PointState state;
	float32 normalImpulse;
	float32 tangentImpulse;
	float32 separation;
};

class Test : public b2ContactListener{
	public:
		Test();
		virtual ~Test();

		void DrawTitle(const char *string);
		virtual void Step(Settings* settings);
		virtual void Keyboard(unsigned char key) { B2_NOT_USED(key); }
		virtual void KeyboardUp(unsigned char key) { B2_NOT_USED(key); }
		void ShiftMouseDown(const b2Vec2& p);
		virtual void MouseDown(const b2Vec2& p);
		virtual void MouseUp(const b2Vec2& p);
		virtual void MouseMove(const b2Vec2& p);
		void LaunchBomb();
		void LaunchBomb(const b2Vec2& position, const b2Vec2& velocity);
	
		void SpawnBomb(const b2Vec2& worldPt);
		void CompleteBombSpawn(const b2Vec2& p);
		virtual void JointDestroyed(b2Joint* joint) { B2_NOT_USED(joint); }
		virtual void ParticleGroupDestroyed(b2ParticleGroup* group) { B2_NOT_USED(group); }
		virtual void BeginContact(b2Contact* contact) { B2_NOT_USED(contact); }
		virtual void EndContact(b2Contact* contact) { B2_NOT_USED(contact); }
		virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
		virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse){
			B2_NOT_USED(contact);
			B2_NOT_USED(impulse);
		}

		void ShiftOrigin(const b2Vec2& newOrigin);
		virtual float32 GetDefaultViewZoom() const;
		void SetDefaultViewZoom(float32 zoom);

		// Apply a preset range of colors to a particle group.
		// A different color out of k_ParticleColors is applied to each particlesPerColor particles in the specified group.
		// If particlesPerColor is 0, the particles in the group are divided into k_ParticleColorsCount equal sets of colored particles.
		void ColorParticleGroup(b2ParticleGroup * const group, uint32 particlesPerColor);

		void InitializeParticleParameters(const uint32 filterMask);
		void RestoreParticleParameters();

	protected:
		friend class DestructionListener;
		friend class BoundaryListener;
		friend class ContactListener;

		b2Body* m_groundBody;
		b2AABB m_worldAABB;
		ContactPoint m_points[k_maxContactPoints];
		int32 m_pointCount;
		DestructionListener m_destructionListener;
		DebugDraw m_debugDraw;
		int32 m_textLine;
		b2World* m_world;
		b2ParticleSystem* m_particleSystem;
		b2Body* m_bomb;
		b2MouseJoint* m_mouseJoint;
		b2Vec2 m_bombSpawnPoint;
		bool m_bombSpawning;
		b2Vec2 m_mouseWorld;
		bool m_mouseTracing;
		b2Vec2 m_mouseTracerPosition;
		b2Vec2 m_mouseTracerVelocity;
		int32 m_stepCount;
		float32 Zoom = 0.1f;

		b2Profile m_maxProfile;
		b2Profile m_totalProfile;

		ParticleParameter::Value* m_particleParameters;
		ParticleParameter::Definition m_particleParameterDef;

		static const b2ParticleColor k_ParticleColors[];
		static const uint32 k_ParticleColorsCount;
};

#endif
