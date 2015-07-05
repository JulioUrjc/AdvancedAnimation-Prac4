#define ENABLE_GLUI 1
#if ENABLE_GLUI
	#include "glui/glui.h"
#else
	#include "GL/freeglut.h"
#endif

#include "Render.h"
#include "Test.h"
#include "FullscreenUI.h"

#include <stdio.h>
#include <algorithm>
#include <string>
#include <sstream>
#include <iostream>

namespace TestMain{
		namespace{
			int32 ENABLE_GLUI2 = 1;
			int32 testIndex = 0;
			int32 testSelection = 0;
			int32 testo = 0;
			GLUI_String texto = "Hello World!";
			int32 testCount = 0;
			TestEntry* entry;
			Test* test;
			Settings settings;
			int32 width = 940;
			int32 height = 640;
			int32 framePeriod = 16;
			int32 mainWindow;
			float settingsHz = 60.0;

			#if ENABLE_GLUI
					GLUI *glui = NULL;
			#endif

			float32 viewZoom = 3.0f;
			int tx, ty, tw, th;
			bool rMouseDown = false;
			int32 previousMouseState = -1;
			b2Vec2 lastp;
			b2Vec2 extents;
		
			FullscreenUI fullscreenUI;		
			ParticleParameter particleParameter;
		}


		void SetRestartOnParticleParameterChange(bool enable){
			particleParameter.SetRestartOnChange(enable);
		}

		uint32 SetParticleParameterValue(uint32 value){
			const int32 index = particleParameter.FindIndexByValue(value);
	
			particleParameter.Set(index >= 0 ? index : 0);
			return particleParameter.GetValue();
		}

		uint32 GetParticleParameterValue(){
			fullscreenUI.SetParticleParameterSelectionEnabled(true);
			return particleParameter.GetValue();
		}


		void SetParticleParameters(const ParticleParameter::Definition * const particleParameterDef, const uint32 particleParameterDefCount){
			particleParameter.SetDefinition(particleParameterDef, particleParameterDefCount);
		}

		static void Resize(int32 w, int32 h){
			width = w;
			height = h;

			#if ENABLE_GLUI				
						GLUI_Master.get_viewport_area(&tx, &ty, &tw, &th);										
			#else
						tx = 0;
						ty = 0;
						tw = glutGet(GLUT_WINDOW_WIDTH);
						th = glutGet(GLUT_WINDOW_HEIGHT);
			#endif 

			glViewport(tx, ty, tw, th);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			float32 ratio = th ? float32(tw) / float32(th) : 1;

			extents = ratio >= 1 ? b2Vec2(ratio * 25.0f, 25.0f) : b2Vec2(25.0f, 25.0f / ratio);
			extents *= viewZoom;

			b2Vec2 lower = settings.viewCenter - extents;
			b2Vec2 upper = settings.viewCenter + extents;
			LoadOrtho2DMatrix(lower.x, upper.x, lower.y, upper.y);

			if (fullscreenUI.GetEnabled()){	fullscreenUI.SetViewParameters(&settings.viewCenter, &extents); }
		}

		static b2Vec2 ConvertScreenToWorld(int32 x, int32 y){
			float32 u = x / float32(tw);
			float32 v = (th - y) / float32(th);

			b2Vec2 lower = settings.viewCenter - extents;
			b2Vec2 upper = settings.viewCenter + extents;

			b2Vec2 p;
			p.x = (1.0f - u) * lower.x + u * upper.x;
			p.y = (1.0f - v) * lower.y + v * upper.y;
			return p;
		}


		static void Timer(int){
			// This is used to control the frame rate (60Hz).
			glutSetWindow(mainWindow);
			glutPostRedisplay();
			glutTimerFunc(framePeriod, Timer, 0);
		}

		static void SimulationLoop(){
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();			
			settings.hz = settingsHz;
			Resize(width, height);
			test->Step(&settings);
			
			bool restartTest;
			const bool changed = particleParameter.Changed(&restartTest);
			B2_NOT_USED(changed);

			if (fullscreenUI.GetEnabled()){
				const uint32 options = particleParameter.GetOptions();

				settings.strictContacts = options & ParticleParameter::OptionStrictContacts;
				settings.drawStats 		= options & ParticleParameter::OptionDrawStats;
				settings.drawProfile	= options & ParticleParameter::OptionDrawProfile;
				settings.drawShapes 	= options & ParticleParameter::OptionDrawShapes ? 1 : 0;
				settings.drawParticles 	= options & ParticleParameter::OptionDrawParticles ? 1 : 0;
				settings.drawAABBs		= options & ParticleParameter::OptionDrawAABBs ? 1 : 0;
				settings.drawCOMs 		= options & ParticleParameter::OptionDrawCOMs ? 1 : 0;

				// Draw the full screen UI with
				std::string msg = entry->name;
				if (fullscreenUI.GetParticleParameterSelectionEnabled()){
					msg += " : ";
					msg += particleParameter.GetName();
				}

				std::stringstream ss;
				ss << int(1000.0f / ComputeFPS());
				msg += " / " + ss.str() + " fps";
				fullscreenUI.Draw(msg);
			}

			test->DrawTitle(entry->name);

			glutSwapBuffers();

			if (testSelection != testIndex || restartTest){
				fullscreenUI.Reset();
				if (!restartTest) particleParameter.Reset();

				testIndex = testSelection;
				delete test;
				entry = g_testEntries + testIndex;
				test = entry->createFcn();
				viewZoom = test->GetDefaultViewZoom();
				settings.viewCenter.Set(0.0f, 20.0f * viewZoom);
				Resize(width, height);
			}

			// print world step time stats every 600 frames
			static int s_printCount = 0;
			static b2Stat st;
			st.Record(settings.stepTimeOut);

			const int STAT_PRINT_INTERVAL = 600;
			if ( settings.printStepTimeStats && st.GetCount() == STAT_PRINT_INTERVAL ){
				//printf("World Step Time samples %i-%i: %fmin %fmax %favg (ms)\n", s_printCount*STAT_PRINT_INTERVAL, (s_printCount+1)*STAT_PRINT_INTERVAL-1, st.GetMin(), st.GetMax(), st.GetMean());
				st.Clear();
				s_printCount++;
			}
		}

		#if ENABLE_GLUI
				static void Exit(int code){
		#ifdef FREEGLUT
					glutLeaveMainLoop();
		#endif
					exit(code);
				}
		#endif 
		
		#if ENABLE_GLUI
				static void Restart(int){
					delete test;
					entry = g_testEntries + testIndex;
					test = entry->createFcn();
					Resize(width, height);
				}
		#endif  

		#if ENABLE_GLUI
				static void Pause(int){
					settings.pause = !settings.pause;
				}
		#endif 

		#if ENABLE_GLUI
				static void SingleStep(int){
					settings.pause = 1;
					settings.singleStep = 1;
				}
		#endif

		static void Keyboard(unsigned char key, int x, int y){
			B2_NOT_USED(x);
			B2_NOT_USED(y);

			switch (key){
				case 27:
					glutLeaveMainLoop();
					exit(0);
					break;	
				case '1':
					viewZoom = b2Min(1.1f * viewZoom, 20.0f);		// Press '1' to zoom out.
					Resize(width, height);
					break;	
				case '2':
					viewZoom = b2Max(0.9f * viewZoom, 0.02f);		// Press '2' to zoom in.
					Resize(width, height);
					break;
				case 'r':
					delete test;									// Press 'r' to reset.
					test = entry->createFcn();
					break;	
				case '3':
																	// Press 3 to launch.
					break;
				case '4':
					settings.pause = !settings.pause;
					break;	
				case '5':
					--testSelection;								// Press 5 to prev test.
					if (testSelection < 0){ testSelection = testCount - 1; }
					#if ENABLE_GLUI
							if (glui) glui->sync_live();
					#endif 
					break;	
				case '6':
					++testSelection;								// Press 6 to next test.
					if (testSelection == testCount){ testSelection = 0; }
					#if ENABLE_GLUI
							if (glui) glui->sync_live();
					#endif 
					break;
				case '7':
					fullscreenUI.SetEnabled(!fullscreenUI.GetEnabled()); // Press 7 to enable / disable the fullscreen UI.
					break;	
				case '8':
					particleParameter.Decrement();					// Press 8 to select the previous particle parameter setting.
					break;	
				case '9':
					particleParameter.Increment();					// Press 9 to select the next particle parameter setting.
					break;
				case '0':
					ENABLE_GLUI2 = !ENABLE_GLUI2;
					if (!ENABLE_GLUI2){
						#if ENABLE_GLUI
							GLUI_Master.close_all();
						#endif
					}else{
						#if ENABLE_GLUI
							glui = GLUI_Master.create_glui_subwindow(mainWindow, GLUI_SUBWINDOW_RIGHT);

							glui->add_statictext("Escenas");
							GLUI_Listbox* testList = glui->add_listbox("", &testSelection);
							glui->add_separator();

							GLUI_Spinner* nextStep = glui->add_spinner("Step", GLUI_SPINNER_FLOAT, &settingsHz);
							nextStep->set_float_limits(5.0f, 200.0f);

							glui->add_separator();

							GLUI_Panel* drawPanel = glui->add_panel("Draw");
							glui->add_checkbox_to_panel(drawPanel, "Shapes", &settings.drawShapes);
							glui->add_checkbox_to_panel(drawPanel, "Particles", &settings.drawParticles);
							glui->add_checkbox_to_panel(drawPanel, "AABBs", &settings.drawAABBs);
							glui->add_checkbox_to_panel(drawPanel, "Statistics", &settings.drawStats);
							glui->add_checkbox_to_panel(drawPanel, "Profile", &settings.drawProfile);

							int32 testCount = 0;
							TestEntry* e = g_testEntries;
							while (e->createFcn){
								testList->add_item(testCount, e->name);
								++testCount;
								++e;
							}
							glui->add_separator();
							glui->add_button("Pause", 0, Pause);
							glui->add_button("Single Step", 0, SingleStep);
							glui->add_button("Restart", 0, Restart);
							glui->add_button("Quit", 0, (GLUI_Update_CB)Exit);

							glui->set_main_gfx_window(mainWindow);
						#endif
					}
					break;
				default:
					if (test){ test->Keyboard(key);	}
			}
		}

		static void KeyboardSpecial(int key, int x, int y){
			B2_NOT_USED(x);
			B2_NOT_USED(y);

			int mod = glutGetModifiers();

			switch (key){	
				case GLUT_KEY_LEFT:					// Press left to pan left.
					if (mod == GLUT_ACTIVE_CTRL){
						b2Vec2 newOrigin(2.0f, 0.0f);
						test->ShiftOrigin(newOrigin);
					}else{
						settings.viewCenter.x -= 0.5f;
						Resize(width, height);
					}
					break;	
				case GLUT_KEY_RIGHT:				// Press right to pan right.
					if (mod == GLUT_ACTIVE_CTRL){
						b2Vec2 newOrigin(-2.0f, 0.0f);
						test->ShiftOrigin(newOrigin);
					}else{
						settings.viewCenter.x += 0.5f;
						Resize(width, height);
					}
					break;		
				case GLUT_KEY_DOWN:					// Press down to pan down.
					if (mod == GLUT_ACTIVE_CTRL){
						b2Vec2 newOrigin(0.0f, 2.0f);
						test->ShiftOrigin(newOrigin);
					}else{
						settings.viewCenter.y -= 0.5f;
						Resize(width, height);
					}
					break;		
				case GLUT_KEY_UP:					// Press up to pan up.
					if (mod == GLUT_ACTIVE_CTRL){
						b2Vec2 newOrigin(0.0f, -2.0f);
						test->ShiftOrigin(newOrigin);
					}else{
						settings.viewCenter.y += 0.5f;
						Resize(width, height);
					}
					break;			
				case GLUT_KEY_HOME:					// Press home to reset the view.
					viewZoom = 1.0f;
					settings.viewCenter.Set(0.0f, 20.0f);
					Resize(width, height);
					break;
				}
		}

		static void KeyboardUp(unsigned char key, int x, int y){
			B2_NOT_USED(x);
			B2_NOT_USED(y);

			if (test){ test->KeyboardUp(key); }
		}

		static void Mouse(int32 button, int32 state, int32 x, int32 y){
			if (button == GLUT_LEFT_BUTTON){
					int mod = glutGetModifiers();
					b2Vec2 p = ConvertScreenToWorld(x, y);

					switch (fullscreenUI.Mouse(button, state, previousMouseState, p)){
						case FullscreenUI::e_SelectionTestPrevious:
							testSelection = std::max(0, testSelection - 1);
							break;
						case FullscreenUI::e_SelectionTestNext:
							if (g_testEntries[testSelection + 1].name) testSelection++;
							break;
						case FullscreenUI::e_SelectionParameterPrevious:
							particleParameter.Decrement();
							break;
						case FullscreenUI::e_SelectionParameterNext:
							particleParameter.Increment();
							break;
						default:
							break;
					}

					if (state == GLUT_DOWN){
						b2Vec2 p = ConvertScreenToWorld(x, y);
						if (mod == GLUT_ACTIVE_SHIFT){ test->ShiftMouseDown(p);}else{ test->MouseDown(p);	}
					}

					if (state == GLUT_UP){ test->MouseUp(p);	}
			}else if (button == GLUT_RIGHT_BUTTON){
					if (state == GLUT_DOWN){
						lastp = ConvertScreenToWorld(x, y);
						rMouseDown = true;
					}

					if (state == GLUT_UP){ rMouseDown = false; }
			}
			previousMouseState = state;
		}

		static void MouseMotion(int32 x, int32 y){
			b2Vec2 p = ConvertScreenToWorld(x, y);

			if (fullscreenUI.GetSelection() == FullscreenUI::e_SelectionNone){ test->MouseMove(p); }

			if (rMouseDown){
				b2Vec2 diff = p - lastp;
				settings.viewCenter.x -= diff.x;
				settings.viewCenter.y -= diff.y;
				Resize(width, height);
				lastp = ConvertScreenToWorld(x, y);
			}
		}

		#ifdef FREEGLUT
			static void MouseWheel(int wheel, int direction, int x, int y){
				B2_NOT_USED(wheel);
				B2_NOT_USED(x);
				B2_NOT_USED(y);
				if (direction > 0){	viewZoom /= 1.1f; }else{ viewZoom *= 1.1f;}
				Resize(width, height);
			}
		#endif		

}  /**************************************** Namespace TestMain *******************************/

/// ************************************   MAIN  ***************************************///
int main(int argc, char** argv){
	using namespace TestMain;

	testCount = 0;
	while (g_testEntries[testCount].createFcn != NULL){	++testCount; }

	testIndex = b2Clamp(testIndex, 0, testCount-1);
	testSelection = testIndex;

	entry = g_testEntries + testIndex;
	if (entry && entry->createFcn) {
		test = entry->createFcn();
		testSelection = testIndex;
		testIndex = -1;
	}

	glutInit(&argc, argv);
	glutInitContextVersion(2, 0);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(width, height);
	char title[32];
	sprintf(title, "Advanced Animation Work");
	mainWindow = glutCreateWindow(title);

	glutDisplayFunc(SimulationLoop);

	#if ENABLE_GLUI
			GLUI_Master.set_glutReshapeFunc(Resize);
			GLUI_Master.set_glutKeyboardFunc(Keyboard);
			GLUI_Master.set_glutSpecialFunc(KeyboardSpecial);
			GLUI_Master.set_glutMouseFunc(Mouse);
	#else
			glutReshapeFunc(Resize);
			glutKeyboardFunc(Keyboard);
			glutSpecialUpFunc(KeyboardSpecial);
			glutMouseFunc(Mouse);		
	#endif

	#ifdef FREEGLUT
		glutMouseWheelFunc(MouseWheel);
	#endif
		glutMotionFunc(MouseMotion);
		glutKeyboardUpFunc(KeyboardUp);

	#if ENABLE_GLUI
			glui = GLUI_Master.create_glui_subwindow(mainWindow, GLUI_SUBWINDOW_RIGHT);

			glui->add_statictext("Escenas");
			GLUI_Listbox* testList = glui->add_listbox("", &testSelection);

			glui->add_separator();			
			
			GLUI_Spinner* nextStep = glui->add_spinner("Step", GLUI_SPINNER_FLOAT, &settingsHz);
			nextStep->set_float_limits(5.0f, 200.0f);
			
			glui->add_separator();

			GLUI_Panel* drawPanel = glui->add_panel("Draw");
			glui->add_checkbox_to_panel(drawPanel, "Shapes", &settings.drawShapes);
			glui->add_checkbox_to_panel(drawPanel, "Particles", &settings.drawParticles);	
			glui->add_checkbox_to_panel(drawPanel, "AABBs", &settings.drawAABBs);
			glui->add_checkbox_to_panel(drawPanel, "Statistics", &settings.drawStats);
			glui->add_checkbox_to_panel(drawPanel, "Profile", &settings.drawProfile);

			int32 testCount = 0;
			TestEntry* e = g_testEntries;
			while (e->createFcn){
				testList->add_item(testCount, e->name);
				++testCount;
				++e;
			}
			glui->add_separator();
			glui->add_button("Pause", 0, Pause);
			glui->add_button("Single Step", 0, SingleStep);
			glui->add_button("Restart", 0, Restart);
			glui->add_button("Quit", 0, (GLUI_Update_CB)Exit);

			glui->set_main_gfx_window(mainWindow);
	#endif

	
	fullscreenUI.SetViewParameters(&settings.viewCenter, &extents);
	glutTimerFunc(framePeriod, Timer, 0);
	glutMainLoop();

	return 0;
}