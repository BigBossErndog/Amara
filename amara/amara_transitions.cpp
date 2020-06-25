#pragma once
#ifndef AMARA_TRANSITIONS
#define AMARA_TRANSITIONS

#include "amara.h"

namespace Amara {
    class FillTransition: public Amara::SceneTransition {
        public:
            SDL_Color recColor;
            SDL_Color color = {0, 0, 0, 255};

            SDL_Rect drawnRect;
            SDL_Rect viewport;

            float fadeInSpeed = 0.1;
            float fadeOutSpeed = 0.1;

            FillTransition(std::string gNextScene): Amara::SceneTransition(gNextScene) {}

            FillTransition(std::string gNextScene, float inSpeed, float outSpeed): Amara::FillTransition(gNextScene) {
                fadeInSpeed = inSpeed;
                fadeOutSpeed = outSpeed;
            }

            FillTransition(std::string gNextScene, float speed): FillTransition(gNextScene, speed, speed) {}

            void setColor(int r, int g, int b, int a) {
                color.r = r;
                color.g = g;
                color.b = b;
                color.a = a;
            }
            void setColor(int r, int g, int b) {
                setColor(r, g, b, 255);
            }

            void setSpeed(float inSpeed, float outSpeed) {
                fadeInSpeed = inSpeed;
                fadeOutSpeed = outSpeed;
            }
            void setSpeed(float speed) {
                setSpeed(speed, speed);
            }

            void create() {
                alpha = 0;
            }

            void update() {
                start();
                if (evt()) {
                    alpha += fadeInSpeed;
                    if (alpha >= 1) {
                        alpha = 1;
                        nextEvt();
                    }
                }
                startNextScene();
                waitForPermission();
                if (evt()) {
                    alpha -= fadeOutSpeed;
                    if (alpha <= 0) {
                        alpha = 0;
                        nextEvt();
                    }
                }
                finishEvt();
            }

            void draw(int vx, int vy, int vw, int vh) {
                Amara::GameProperties* properties = Amara::Actor::properties;
                SDL_GetRenderDrawColor(properties->gRenderer, &recColor.r, &recColor.g, &recColor.b, &recColor.a);

                if (alpha < 0) alpha = 0;
                if (alpha > 1) alpha = 1;

                viewport.x = vx;
                viewport.y = vy;
                viewport.w = vw;
                viewport.h = vh;
                SDL_RenderSetViewport(properties->gRenderer, &viewport);

                drawnRect.x = 0;
                drawnRect.y = 0;
                drawnRect.w = vw;
                drawnRect.h = vh;

                int newAlpha = (float)color.a * alpha;
                SDL_SetRenderDrawBlendMode(properties->gRenderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(properties->gRenderer, color.r, color.g, color.b, newAlpha);
                SDL_RenderFillRect(properties->gRenderer, &drawnRect);

                SDL_SetRenderDrawColor(properties->gRenderer, recColor.r, recColor.g, recColor.b, recColor.a);
                Amara::Actor::draw(vx, vy, vw, vh);
            }
    };
}

#include "amara.h"
#endif