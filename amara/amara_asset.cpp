#pragma once
#ifndef AMARA_ASSET
#define AMARA_ASSET

#include "amara.h"

namespace Amara {
    enum AssetType {
        JSONCONFIG,
        SURFACE,
        IMAGE,
        RADIALGRADIENTTEXTURE,
        SPRITESHEET,
        TTF,
        SOUND,
        MUSIC,
        JSONFILE,
        STRINGFILE,
        LINEBYLINE
    };

    class Asset {
        public:
            AssetType type;
            void* asset;
            std::string key;

            bool toRegenerate = false;

            Asset() {}

            Asset(std::string givenKey, AssetType givenType, void* givenAsset) {
                key = givenKey;
                type = givenType;
				asset = givenAsset;
            }

            virtual void regenerate(SDL_Renderer*) {}
    };

    class ImageTexture : public Amara::Asset {
        public:
            int width = 0;
            int height = 0;

            bool temp = false;

            SDL_Texture* asset = nullptr;

            ImageTexture(std::string key, AssetType givenType, SDL_Texture* givenAsset): Amara::Asset(key, givenType, givenAsset) {
                SDL_QueryTexture(givenAsset, NULL, NULL, &width, &height);
                asset = givenAsset;
            }

            ~ImageTexture() {
                SDL_DestroyTexture(asset);
            }
    };

    class RadialGradientTexture: public Amara::ImageTexture {
        public:
            SDL_Color innerColor;
            SDL_Color outerColor;
            float fadeStart;

            RadialGradientTexture(std::string key, AssetType givenType, SDL_Texture* givenAsset): Amara::ImageTexture(key, givenType, givenAsset) {}

            void configure(SDL_Color gInnerColor, SDL_Color gOuterColor, float gFadeStart) {
                innerColor = gInnerColor;
                outerColor = gOuterColor;
                fadeStart = gFadeStart;

                toRegenerate = true;
            }

            void regenerate(SDL_Renderer* gRenderer) {
                SDL_DestroyTexture(Amara::ImageTexture::asset);
                Amara::ImageTexture::asset = createRadialGradientTexture(gRenderer, width, height, innerColor, outerColor, fadeStart);
            }
    };

    class Spritesheet : public Amara::ImageTexture {
        public:
            int frameWidth = 0;
            int frameHeight = 0;

            std::unordered_map<std::string, Amara::Animation*> anims;

            Spritesheet(std::string key, AssetType givenType, SDL_Texture* newtexture, int newwidth, int newheight): Amara::ImageTexture(key, givenType, newtexture) {
                asset = newtexture;
                frameWidth = newwidth;
                frameHeight = newheight;
                if (frameWidth > width) {
                    frameWidth = width;
                }
                if (frameHeight > height) {
                    frameHeight = height;
                }
            }

            Amara::Animation* addAnim(std::string animKey, std::vector<int> frames, int frameRate, bool loop) {
                Amara::Animation* anim;
                std::unordered_map<std::string, Amara::Animation*>::iterator got = anims.find(animKey);
                if (got != anims.end()) {
                    anim = got->second;
                    anim->frames = frames;
                    anim->frameRate = frameRate;
                    anim->loop = loop;
                    return anim;
                }

                anim = new Amara::Animation(key, animKey, frames, frameRate, loop);
                anims[animKey] = anim;
                return anim;
            }
            Amara::Animation* addAnim(std::string animKey, int frame) {
                return addAnim(animKey, {frame}, 1, false);
            }

            Amara::Animation* getAnim(std::string animKey) {
                std::unordered_map<std::string, Amara::Animation*>::iterator got = anims.find(animKey);
                if (got != anims.end()) {
                    return got->second;
                }
                return nullptr;
            }
    };

    class StringFile: public Amara::Asset {
        public:
            std::string contents;

            StringFile(std::string givenKey, AssetType givenType, std::string gContents): Amara::Asset(givenKey, STRINGFILE, nullptr) {
                contents = gContents;
            }

            std::string getString() {
                return contents;
            }

            std::string toString() {
                return getString();
            }
    };

	class JsonFile: public Amara::Asset {
		public:
            nlohmann::json jsonObj;

			JsonFile(std::string givenKey, AssetType givenType, nlohmann::json gJson): Amara::Asset(givenKey, JSONFILE, nullptr) {
                jsonObj = gJson;
            }

            nlohmann::json& getJSON() {
                return jsonObj;
            }
	};

    class TTFAsset: public Amara::Asset {
        public:
            FC_Font* font = nullptr;
            std::string path;
            int size;
            SDL_Color color;
            int style;

            bool recFullscreen = false;

            TTFAsset(std::string givenKey, AssetType givenType, FC_Font* gFont): Amara::Asset(givenKey, TTF, gFont) {
                font = gFont;
                toRegenerate = true;
            }

            void reloadFontCache(SDL_Renderer* gRenderer) {
                FC_ClearFont(font);
                FC_LoadFont(font, gRenderer, path.c_str(), size, color, style);
            }

            void regenerate(SDL_Renderer* gRenderer) {
                reloadFontCache(gRenderer);
            }
    };

    class LineByLine: public Amara::Asset {
    public:
        int index = 0;
        std::vector<std::string> contents;

        LineByLine(std::string givenKey, AssetType givenType, std::vector<std::string> givenContents): Amara::Asset(givenKey, LINEBYLINE, nullptr){
            contents = givenContents;
        }

        void reset() {
            index = 0;
        }

        std::string getLine() {
            index += 1;
            if (index > contents.size()) {
                index -= 1;
                return "EOF";
            }
            else {
                return contents[index - 1];
            }
        }

        std::string getLine(int i) {
            return contents[i];
        }

        bool eof() {
            return index >= contents.size();
        }
    };
}

#endif
