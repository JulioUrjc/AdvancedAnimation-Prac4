#ifndef FULLSCREEN_UI_H
#define FULLSCREEN_UI_H
#include <Box2D/Box2D.h>
#include <string>

class Arrow;

// Handles drawing and selection of full screen UI.
class FullscreenUI{
	public:
		// Currently selected Arrow widget.
		enum Selection{	e_SelectionTestPrevious = 0, e_SelectionTestNext, e_SelectionParameterPrevious,	e_SelectionParameterNext, e_SelectionNone,};

		FullscreenUI();

		void Reset();
		bool GetEnabled() const { return m_enabled; }
		void SetEnabled(bool enable) { m_enabled = enable; }
		uint32 Mouse(const int32 button, const int32 state, const int32 previousState, const b2Vec2 &mousePosition);
		uint32 GetSelection() const { return m_selection; }

		void SetParticleParameterSelectionEnabled(const bool enable){ m_particleParameterSelectionEnabled = enable; }
		bool GetParticleParameterSelectionEnabled() const{ return m_particleParameterSelectionEnabled; }
		void Draw(const std::string &footer);

		void SetViewParameters(const b2Vec2 *viewCenter, const b2Vec2 *extents);

	private:
		void DrawArrows();
		void DrawFooterText(const std::string &text);

	private:
		bool m_enabled;	
		uint32 m_selection;	
		bool m_particleParameterSelectionEnabled;	
		float32 m_aspectRatio;

		static const bool k_enabledDefault;	
		static const float32 k_arrowScale;	
		static const float32 k_arrowScaleSmall;	
		static const float32 k_arrowSpacing;
		static const float32 k_arrowTestX;
		static const float32 k_arrowParticleParameterX;
		static Arrow s_arrows[];
		static const uint32 k_numArrows;
};

#endif  // FULLSCREEN_UI_H
