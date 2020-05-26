#pragma once
#ifndef AMARA_ASSETMANAGER
#define AMARA_ASSETMANAGER

#include "amara.h"

namespace Amara {
    class AssetManager {
        public:
            Amara::GameProperties* properties = nullptr;
            Amara::Loader* loader = nullptr;

            AssetManager(Amara::GameProperties* gameProperties) {
                properties = gameProperties;
                loader = properties->loader;
            }

            Amara::Asset* get(std::string key) {
                return loader->get(key);
            }

            bool add(SDL_Texture* tx, std::string key, bool replace) {
                return loader->add(tx, key, replace);
            }
            bool add(SDL_Texture* tx, std::string key) {
                return add(tx, key, true);
            }

            bool add(SDL_Texture* tx, int frwidth, int frheight, std::string key, bool replace) {
                return loader->add(tx, frwidth, frheight, key, replace);
            }
            bool add(SDL_Texture* tx, int frwidth, int frheight, std::string key) {
                return add(tx, frwidth, frheight, key, true);
            }

            Amara::Animation* addAnim(std::string textureKey, std::string animKey, std::vector<int> frames, int frameRate, bool loop) {
                Amara::Asset* asset = get(textureKey);
                if (asset == nullptr) {
                    std::cout << "Couldn't find animation \"" << textureKey << "\"" << std::endl;
                    return nullptr;
                }
                if (asset->type == SPRITESHEET) {
                    return ((Amara::Spritesheet*)asset)->addAnim(animKey, frames, frameRate, loop);
                }
            }

            Amara::Animation* addAnim(std::string textureKey, std::string animKey, int frame) {
                return addAnim(textureKey, animKey, {frame}, 1, false);
            }

            void addAnim(std::vector<std::string> textureKeys, std::string animKey, std::vector<int> frames, int frameRate, bool loop) {
                for (std::string tKey : textureKeys) {
                    addAnim(tKey, animKey, frames, frameRate, loop);
                }
            }

            void addAnim(std::vector<std::string> textureKeys, std::string animKey, int frame) {
                addAnim(textureKeys, animKey, {frame}, 1, false);
            }

            void addAnim(std::string textureKey, std::string animKey, int startFrame, int numFrames, int frameRate, bool loop) {
                std::vector<int> frames;
                frames.clear();
                for (int i = 0; i < numFrames; i++) {
                    frames.push_back(startFrame + i);
                }
                addAnim(textureKey, animKey, frames, frameRate, loop);
            }
    };
}

#endif