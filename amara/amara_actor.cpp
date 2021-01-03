#pragma once
#ifndef AMARA_ACTOR
#define AMARA_ACTOR

#include "amara.h"

namespace Amara {
    class Tween;
    class Actor: public Amara::Entity {
        public:
            std::vector<Amara::Script*> scripts;
            bool actingPaused = false;

            Actor(): Amara::Entity() {}

            virtual Amara::Script* recite(Amara::Script* script) {
                scripts.push_back(script);
                script->init(properties, this);
                script->prepare();
                script->prepare(this);
                return script;
            }

            void reciteScripts() {
                if (scripts.size() == 0 || actingPaused) return;
                for (Amara::Script* script: scripts) {
                    if (!script->finished) {
                        script->script();
                        script->script(this);
                    }
                }

                std::vector<Script*> chained;
                chained.clear();

                Amara::Script* script;
                for (auto it = scripts.begin(); it != scripts.end(); ++it) {
                    script = *it;
                    if (script->finished) {
                        if (script->chainedScript != nullptr) {
                            chained.push_back(script->chainedScript);
                        }
                        scripts.erase(it--);
                        if (script->deleteOnFinish) {
                            delete script;
                        }
                    }
                }
                for (Amara::Script* chain: chained) {
                    recite(chain);
                }
            }

            bool stillActing() {
                return (scripts.size() > 0);
            }

            void run() {
                reciteScripts();
                Amara::Entity::run();
            }

            Amara::Entity* add(Amara::Entity* entity) {
                return Amara::Entity::add(entity);
            }

            void clearScripts() {
                for (Amara::Script* script: scripts) {
                    if (script->deleteOnFinish) {
                        delete script;
                    }
                }
                scripts.clear();
            }

			void clearScript(std::string gid) {
				Amara::Script* script;
                for (auto it = scripts.begin(); it != scripts.end(); ++it) {
                    script = *it;
                    if (script->id.compare(gid) == 0) {
                        if (script->chainedScript != nullptr) {
                            recite(script->chainedScript);
                        }
                        scripts.erase(it--);
                        if (script->deleteOnFinish) {
                            delete script;
                        }
						return;
                    }
                }
			}

			Amara::Script* getScript(std::string gid) {
				for (Amara::Script* script: scripts) {
					if (script->id.compare(gid) == 0) {
						return script;
					}
				}
				return nullptr;
			}

			void cancelScripts() {
				for (Amara::Script* script: scripts) {
					script->cancel();
					script->cancel(this);
                    if (script->deleteOnFinish) {
                        delete script;
                    }
                }
                scripts.clear();
			}

            void pauseActing() {
                actingPaused = true;
            }
            void resumeActing() {
                actingPaused = false;
            }

            virtual ~Actor() {
                clearScripts();
            }
    };
}

#endif
