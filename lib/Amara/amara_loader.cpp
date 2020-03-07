#ifndef AMARA_LOADER
#define AMARA_LOADER

#include "amara.h"

namespace Amara {
    class Game;

    class Loader {
        public:
            Amara::Game* game = nullptr;
			Amara::GameProperties* properties = nullptr;
            SDL_Window* gWindow = NULL;
			SDL_Surface* gSurface = NULL;
			SDL_Renderer* gRenderer = NULL;

            unordered_map<string, Amara::Asset*> assets;
			string selfkey;

            Loader(Amara::GameProperties* gameProperties) {
				properties = gameProperties;
				game = properties->game;
				gWindow = properties->gWindow;
                gSurface = properties->gSurface;
                gRenderer = properties->gRenderer;
                assets.clear();
				assets["control"] = new Amara::Asset("control", IMAGE, nullptr);
            }

            Amara::Asset* get(string key) {
                unordered_map<string, Amara::Asset*>::iterator got = assets.find(key);
                if (got != assets.end()) {
                    return got->second;
                }
                return nullptr;
            }

            /*
			 * Slow image.
			 */
			bool surface(string key, string path, bool replace) {
				if (get(key) != nullptr && !replace) {
					cout << "Loader Error: Key %s has already been used.\n" << key << endl;
					return false;
				}
				bool success = true;
				// Final optimized image.
				SDL_Surface* optimizedSurface = NULL;

				//Load image at specified path.
				SDL_Surface* loadedSurface = IMG_Load(path.c_str());
				if (loadedSurface == NULL) {
					cout << "Unable to load image %s! Error: %s\n" << path << IMG_GetError() << endl;
					success = false;
				}
				else {
					// Convert surface to screen format.
					optimizedSurface = SDL_ConvertSurface(loadedSurface, gSurface->format, NULL);

					if (optimizedSurface == NULL) {
						cout << "Unable to optimize image %s. SDL Error: %s\n" << path << SDL_GetError() << endl;
						success = false;
					}

					//Get rid of old loaded surface
					SDL_FreeSurface(loadedSurface);
				}

				if (success) {
					cout << "Loaded: " << key << endl;
					Amara::Asset* newAsset = new Amara::Asset(key, SURFACE, optimizedSurface);
					assets[key] = newAsset;
				}

				return success;
			}

			bool surface(string key, string path) {
				return surface(key, path, false);
			}

            /*
			 * Fast texture image.
			 */
			bool image(string key, string path, bool replace) {
				if (get(key) != nullptr && !replace) {
					cout << "Loader Error: Key " << key << " has already been used.\n" << endl;
					return false;
				}
				bool success = true;

				SDL_Texture* newTexture = NULL;
				// Load image
				SDL_Surface* loadedSurface = IMG_Load(path.c_str());



				if (loadedSurface == NULL && !replace) {
					cout << "Unable to load image " << path << ". Error: " << IMG_GetError() << endl;
					success = false;
				}
				else {
					// Create texture from surface pixels
					newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
					if (newTexture == NULL) {
						cout << "Unable to create texture from " << path << ". SDL Error: \n" << SDL_GetError() << endl;
					}

					//Get rid of old loaded surface
					SDL_FreeSurface(loadedSurface);
				}


				if (success) {
					cout << "Loaded: " << key << endl;
					Amara::Asset* newAsset = new Amara::ImageTexture(key, IMAGE, newTexture);
					assets[key] = newAsset;
				}

				return success;
			}

			bool image(string key, string path) {
				return image(key, path, false);
			}

            /*
			 *  Spritesheet handles frame width and height.
			 */
			bool spritesheet(string key, string path, int frwidth, int frheight, bool replace) {
				if (get(key) != nullptr && !replace) {
					cout << "Loader Error: Key " << key << " has already been used." << key << endl;
					return false;
				}
				bool success = true;

				SDL_Texture* newTexture = NULL;

				// Load image
				SDL_Surface* loadedSurface = IMG_Load(path.c_str());
				if (loadedSurface == NULL) {
					cout << "Unable to load image " << path << ". Error: " << IMG_GetError() << endl;
					success = false;
				}
				else {
					// Create texture from surface pixels
					newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
					if (newTexture == NULL) {
						cout << "Unable to create texture from " << path << ". SDL Error: " << SDL_GetError() << endl;
					}

					//Get rid of old loaded surface
					SDL_FreeSurface(loadedSurface);
				}

				if (success) {
					cout << "Loaded: " << key << endl;
					Amara::Spritesheet* newAsset = new Amara::Spritesheet(key, SPRITESHEET, newTexture, frwidth, frheight);
					if (newAsset->type == SPRITESHEET) Amara::log("IT IS A SPRITESHEET");
					assets[key] = newAsset;
				}

				return success;
			}

			bool spritesheet(string key, string path, int frwidth, int frheight) {
				return spritesheet(key, path, frwidth, frheight, false);
			}

            /*
			 * Loads a TrueTypeFont.
			 */
			bool ttf(string key, string path, int size, SDL_Color color, int style, bool replace) {
				if (get(key) != nullptr && !replace) {
					printf("Loader Error: Key %s has already been used.\n", key);
					return false;
				}
				bool success = true;

				// Creating the font
				FC_Font* gFont = FC_CreateFont();
				FC_LoadFont(gFont, gRenderer, path.c_str(), size, color, style);

				if (success) {
					cout << "Loaded: " << key << endl;
					Amara::Asset* newAsset = new Amara::Asset(key, TTF, gFont);
					assets[key] = newAsset;
				}

				return success;
			}

			bool ttf(string key, string path, int size, SDL_Color color, int style) {
				return ttf(key, path, size, color, style, false);
			}

			bool ttf(string key, string path, int size, SDL_Color color) {
				return ttf(key, path, size, color, TTF_STYLE_NORMAL);
			}

			bool ttf(string key, string path, int size) {
				return ttf(key, path, size, FC_MakeColor(0,0,0,255));
			}

            bool sound(string key, string path, bool replace) {
				if (get(key) != nullptr && !replace) {
					cout << "Loader Error: Key %s has already been used.\n" << key << endl;
					return false;
				}
				bool success = true;

				Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
				if (sound == NULL) {
					cout << "Failed to load sound effect. SDL_mixer Error: %s\n" <<  Mix_GetError() << endl;
					success = false;
				}
				else {
					cout << "Loaded: " << key << endl;
					Amara::Asset* newAsset = new Amara::Asset(key, SOUND, sound);
					assets[key] = newAsset;
				}

				return success;
			}

			bool sound(string key, string path) {
				return sound(key, path, false);
			}


			bool music(string key, string path, bool replace) {
				if (get(key) != nullptr && !replace) {
					cout << "Loader Error: Key " << key << " has already been used." << key << endl;
					return false;
				}
				bool success = true;

				Mix_Music* music = Mix_LoadMUS(path.c_str());
				if (music == NULL) {
					cout << "Failed to load music. SDL_mixer Error: %s\n" << Mix_GetError() << endl;
					success = false;
				}
				else {
					cout << "Loaded: " << key << endl;
					Amara::Asset* newAsset = new Amara::Asset(key, MUSIC, music);
					assets[key] = newAsset;
				}

				return success;
			}

            bool music(string key, string path) {
				return music(key, path, false);
			}
    };
}

#endif
