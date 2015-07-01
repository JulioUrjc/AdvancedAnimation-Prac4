#ifndef ARROW_H
#define ARROW_H
#include <Box2D/Box2D.h>

class Arrow{
	public:
		// Present angles used to rotate the arrow in different orientations.
		enum Angle { e_angleRight = 0, e_angleLeft = 180,};

		Arrow(const float32 angle, const float32 scale, const b2Vec2 &position, const uint32 identifier, const b2Vec2 *viewCenter, const b2Vec2 *extents);
		uint32 GetIdentifier() const { return m_identifier; }	
		uint32 Hit(const b2Vec2 &position, uint32 notSelectedIdentifier) const;
		void Draw(const uint32 selectedIdentifier) const;
		void SetViewParameters(const b2Vec2 *viewCenter, const b2Vec2 *extents);

	protected:
		float32 CalculateScale() const;
		b2Vec2* CalculateViewportPosition(b2Vec2 * const viewportPosition) const;
		static void DrawArrow(const b2Color& color, const float32 angle, const float32 scale, const b2Vec2 &position);

	private:	
		float32 m_angle;	
		float32 m_scale;	
		b2Vec2 m_position;	
		uint32 m_identifier;	
		const b2Vec2 *m_viewCenter;
		const b2Vec2 *m_extents;

	public:
		static const float32 k_size;

	private:
		static const b2Color k_activeColor;
		static const b2Color k_passiveColor;
};
#endif  // ARROW_H
