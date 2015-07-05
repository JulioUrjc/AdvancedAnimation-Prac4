#include "FullscreenUI.h"
#include "Arrow.h"

#include <GL/freeglut.h>

const bool FullscreenUI::k_enabledDefault = false;

const float32 FullscreenUI::k_arrowScale = 0.15f;
const float32 FullscreenUI::k_arrowScaleSmall = 0.075f;
const float32 FullscreenUI::k_arrowSpacing = 0.005f;
const float32 FullscreenUI::k_arrowTestX = FullscreenUI::k_arrowScale + FullscreenUI::k_arrowSpacing;
const float32 FullscreenUI::k_arrowParticleParameterX =	FullscreenUI::k_arrowTestX + FullscreenUI::k_arrowScale +
														FullscreenUI::k_arrowScaleSmall + FullscreenUI::k_arrowSpacing;

Arrow FullscreenUI::s_arrows[] = {
	// Test selection arrows.
	Arrow(Arrow::e_angleRight, FullscreenUI::k_arrowScale, b2Vec2(1.0f - FullscreenUI::k_arrowTestX, 0.0f), e_SelectionTestNext, &b2Vec2_zero, &b2Vec2_zero),
	Arrow(Arrow::e_angleLeft, FullscreenUI::k_arrowScale, b2Vec2(-1.0f + FullscreenUI::k_arrowTestX, 0.0f), e_SelectionTestPrevious, &b2Vec2_zero, &b2Vec2_zero),
	Arrow(Arrow::e_angleRight, FullscreenUI::k_arrowScaleSmall, b2Vec2(1.0f - FullscreenUI::k_arrowParticleParameterX, 0.0f), e_SelectionParameterNext, &b2Vec2_zero, &b2Vec2_zero),
	Arrow(Arrow::e_angleLeft, FullscreenUI::k_arrowScaleSmall, b2Vec2(-1.0f + FullscreenUI::k_arrowParticleParameterX, 0.0f), e_SelectionParameterPrevious, &b2Vec2_zero, &b2Vec2_zero),
};
const uint32 FullscreenUI::k_numArrows = B2_ARRAY_SIZE(FullscreenUI::s_arrows);

FullscreenUI::FullscreenUI(){
	Reset();
	m_enabled = k_enabledDefault;
}

void FullscreenUI::Reset(){
	m_selection = e_SelectionNone;
	m_particleParameterSelectionEnabled = false;
	m_aspectRatio = 1;
}


uint32 FullscreenUI::Mouse(const int32 button, const int32 state, const int32 previousState, const b2Vec2 &mousePosition){
	if (GetEnabled() && button == GLUT_LEFT_BUTTON){
		if (state == GLUT_DOWN && previousState != GLUT_DOWN){
			for (uint32 i = 0; i < k_numArrows && m_selection == e_SelectionNone; ++i){
				m_selection = s_arrows[i].Hit(mousePosition, e_SelectionNone);
			}
		}
		
		if (state == GLUT_UP){
			const uint32 selectedArrow = m_selection;
			m_selection = e_SelectionNone;
			return selectedArrow;
		}
	}
	return e_SelectionNone;
}

void FullscreenUI::Draw(const std::string &footer){
	if (m_enabled){
		DrawArrows();
		DrawFooterText(footer);
	}
}

void FullscreenUI::SetViewParameters(const b2Vec2 *viewCenter, const b2Vec2 *extents){
	for (uint32 i = 0; i < k_numArrows; ++i){
		Arrow &arrow = s_arrows[i];
		arrow.SetViewParameters(viewCenter, extents);
	}
	m_aspectRatio = extents->y > 0 ? extents->x / extents->y : 1;
}

// Draw s_arrows.
void FullscreenUI::DrawArrows(){
	for (uint32 i = 0; i < k_numArrows; ++i){
		const Arrow &arrow = s_arrows[i];
		const uint32 id = arrow.GetIdentifier();
		if ((id != e_SelectionParameterPrevious && id != e_SelectionParameterNext) || m_particleParameterSelectionEnabled){
			arrow.Draw(m_selection);
		}
	}
}

// Draw text at the bottom of the screen.
void FullscreenUI::DrawFooterText(const std::string &text){
	glColor4f(1, 1, 1, 1);
	glPushMatrix();
	glLoadIdentity();
	if (m_aspectRatio > 1){	glScalef( 1 / m_aspectRatio, 1, 1); }else{ glScalef(1, m_aspectRatio, 1);	}
	glScalef(0.04f, 0.04f, 1);
	glTranslatef(0, -20, 0);
	glTranslatef(text.length() * -0.75f, -4, 0);
	glScalef(0.015f, 0.015f, 1);
	glutStrokeString(GLUT_STROKE_MONO_ROMAN, (const unsigned char *)text.c_str());
	glPopMatrix();
}