#include "Engine.h"
#include "Physical.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>

// --- SDL2-specific code ------------------------------------------------

bool Engine::InitOSWrapper() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	atexit(SDL_Quit); // SDL will be shut down automatically on app exit
	return true;
}

void Engine::SetupInputs() {
	// not needed
}

void Engine::ToggleFullscreen() {
	if (isFullscreen) {
		iWidth=GH_SCREEN_WIDTH;
		iHeight=GH_SCREEN_HEIGHT;
		SDL_SetWindowFullscreen(window,0);
		SDL_SetWindowSize(window,iWidth,iHeight);
	}
	else {
		SDL_DisplayMode DM;
		SDL_GetCurrentDisplayMode(0, &DM);
		iWidth=DM.w;
		iHeight=DM.h;
		SDL_SetWindowFullscreen(window,SDL_WINDOW_FULLSCREEN);
	}
	isFullscreen = !isFullscreen;
}

void Engine::CreateGLWindow() {
	iWidth = GH_SCREEN_WIDTH;
	iHeight = GH_SCREEN_HEIGHT;

	window = SDL_CreateWindow(
		GH_TITLE,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		iWidth, iHeight,
		SDL_WINDOW_OPENGL|SDL_WINDOW_ALLOW_HIGHDPI|
			(GH_START_FULLSCREEN ? SDL_WINDOW_FULLSCREEN : 0)
	);
	if (!window) {
		SDL_Log("Unable to create GL window: %s", SDL_GetError());
		return;
	}

	// window successfully created

	glContext = SDL_GL_CreateContext(window);
	if (!glContext) {
		SDL_Log("Unable to create GL context: %s", SDL_GetError());
		return;
	}

	// GL context is created and is current for the calling thread

	isWindowGood = true;
}

void Engine::DestroyGLWindow() {
	if (glContext) {
		SDL_GL_DeleteContext(glContext);
	}
	if (window) {
		SDL_DestroyWindow(window);
	}
}

void Engine::EnableVSync() {

	// try adaptive vsync first, if not supported, go full vsync
	if (SDL_GL_SetSwapInterval(-1) != 0) {
		SDL_GL_SetSwapInterval(1);
	}
}

int Engine::EnterMessageLoop() {
	if (GH_HIDE_MOUSE) {
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}
	//Setup the timer
	ticks_per_step = timer.SecondsToTicks(GH_DT);
	int64_t cur_ticks = timer.GetTicks();
	GH_FRAME = 0;

	SDL_Event event;
	while(true) {
		/*const char* c = std::to_string(rand() % 100).c_str();
		write(1, c, strlen(c));
		write(1, "\ncheck for event\n", 17);*/
		if (SDL_PollEvent(&event)) {
			const char* c = std::to_string(event.type).c_str();
			write(1, "\n", 1);
			write(1, c, strlen(c));
			write(1, "\nevent\n", 7);
			if (event.type == SDL_QUIT) {
				break;
			}else if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
				write(1, "key down\n", 9);
				int keycode = event.key.keysym.sym;
				auto mod = event.key.keysym.mod;
				write(1, (char*) keycode, 1);
				write(1, "\n", 1);
				if (keycode == SDLK_ESCAPE) {
					break;
				}else if (keycode == SDLK_RETURN && (mod & KMOD_LALT || mod & KMOD_RALT)) {
					ToggleFullscreen();
				}else if (keycode >= 'a' && keycode <= 'z'){
					input.key[toupper(keycode)] = true;
				}else{
					int i = keycode-49;
					if(i>=0&&i<=6){
						LoadScene(i);
					}
				}
			}else if (event.type == SDL_KEYUP) {
				write(1, "key up\n", 7);
				auto keycode = event.key.keysym.sym;
				if (keycode >= 'a' && keycode <= 'z'){
					input.key[toupper(keycode)] = false;
				}
			}else if(event.type==SDL_MOUSEMOTION){
				write(1, "mouse", 5);
				input.UpdateMouse();
				continue;
			}
		}

		PeriodicRender(cur_ticks);
		SDL_GL_SwapWindow(window);
	}

	return 0;
}

void UpdateMouse(){

}