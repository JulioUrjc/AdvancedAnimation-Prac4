#include "Arrow.h"
#include "Render.h"

#include "GL/freeglut.h"

const float32 Arrow::k_size = 3.5f;
const b2Color Arrow::k_activeColor(0.0f, 1.0f, 0.0f);
const b2Color Arrow::k_passiveColor(0.0f, 0.0f, 1.0f);

Arrow::Arrow(const float32 angle, const float32 scale, const b2Vec2 &position, const uint32 identifier, const b2Vec2 *viewCenter, const b2Vec2 *extents){
	m_angle = angle;
	m_scale = scale;
	m_position = position;
	m_identifier = identifier;
	SetViewParameters(viewCenter, extents);
}

// Determine whether the specified position is within the bounding
uint32 Arrow::Hit(const b2Vec2 &position, uint32 notSelectedIdentifier) const{
	const float32 arrowExtent =	CalculateScale() * Arrow::k_size * 0.5f;
	b2Vec2 arrowPosition;
	// Translate position into this arrow's space.
	const b2Vec2 relativePosition =	position - *CalculateViewportPosition(&arrowPosition);
	// Determine whether position is within the arrow's bounding box.
	if (fabs(relativePosition.x) < arrowExtent && fabs(relativePosition.y) < arrowExtent){ return m_identifier; }
	return notSelectedIdentifier;
}

// Draw this arrow using Arrow::k_activeColor
void Arrow::Draw(const uint32 selectedIdentifier) const{
	b2Vec2 position;
	DrawArrow(selectedIdentifier == m_identifier ? Arrow::k_activeColor : Arrow::k_passiveColor, m_angle, CalculateScale(), *CalculateViewportPosition(&position));
}

// Set the view center and extents.
void Arrow::SetViewParameters(const b2Vec2 *viewCenter, const b2Vec2 *extents){
	b2Assert(viewCenter);
	m_viewCenter = viewCenter;
	b2Assert(extents);
	m_extents = extents;
}

// Calculate the scaling factor for the arrow given the current viewport extents.
float32 Arrow::CalculateScale() const{
	const float32 minDimension = std::min(m_extents->x, m_extents->y) * 2.0f;	
	return (minDimension / Arrow::k_size) * m_scale;
}

// Calculate the viewport position of the arrow.
b2Vec2* Arrow::CalculateViewportPosition(b2Vec2 * const viewportPosition) const{
	b2Assert(viewportPosition);
	viewportPosition->x = m_viewCenter->x + (m_extents->x * m_position.x);
	viewportPosition->y = m_viewCenter->y + (m_extents->y * m_position.y);
	return viewportPosition;
}

// Draw a colored arrow rotated by angle, scaled by scale and at the viewport relative position using DebugDraw.
void Arrow::DrawArrow(const b2Color &color, const float32 angle, const float32 scale, const b2Vec2 &position){
	static const b2Vec2 square[] = { b2Vec2(0.25f, 1.0f), b2Vec2(0.25f, -1.0f),	b2Vec2(-1.75f, -1.0f), b2Vec2(-1.75f, 1.0f)};
	static const b2Vec2 triangle[] = { b2Vec2(0.25f, 1.75f), b2Vec2(1.75f, 0.0f), b2Vec2(0.25f, -1.75f)};
	// Build the transformation matrix.
	glPushMatrix();
	glTranslatef(position.x, position.y, 0.0f);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	glScalef(scale, scale, 1.0f);
	// Draw the arrow.
	DebugDraw dbgDraw;
	dbgDraw.DrawFlatPolygon(square, B2_ARRAY_SIZE(square), color);
	dbgDraw.DrawFlatPolygon(triangle, B2_ARRAY_SIZE(triangle), color);
	glPopMatrix();
}