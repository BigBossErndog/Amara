#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <time.h>
#include <limits>
#include <map>
#include <unordered_map>
#include <vector>
#include <deque>
#include <algorithm>
#include <functional>
#include <math.h>

using namespace std;

#ifndef AMARA_SDL
#define AMARA_SDL
    #include <SDL.h>
    #include <SDL_image.h>
    #include <SDL_ttf.h>
    #include <SDL_mixer.h>
    #include <SDL_FontCache.c>
#endif

#include "amara_game.cpp"
#include "amara_sceneManager.cpp"
#include "amara_scenePlugin.cpp"
#include "amara_scene.cpp"
#include "amara_ltimer.cpp"

#include "amara_controlScheme.cpp"
#include "amara_control.cpp"

#include "amara_mouse.cpp"

#include "amara_keyboard.cpp"
#include "amara_key.cpp"
#include "amara_keyCodes.cpp"

#include "amara_scene.cpp"

#include "amara_image.cpp"

#include "amara_camera.cpp"
#include "amara_actor.cpp"
#include "amara_script.cpp"
#include "amara_entity.cpp"

#include "amara_loadManager.cpp"
#include "amara_loader.cpp"
#include "amara_asset.cpp"

#include "amara_geometry.cpp"
#include "amara_stateManager.cpp"

#include "amara_inputManager.cpp"

#include "amara_debugging.cpp"
#include "amara_taskManager.cpp"
#include "amara_gameProperties.cpp"
