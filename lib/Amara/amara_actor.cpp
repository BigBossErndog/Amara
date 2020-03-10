#ifndef AMARA_ACTOR
#define AMARA_ACTOR

#include "amara.h"

namespace Amara {
    class Actor: public Amara::Entity {
        public:
            vector<Amara::Script*> scripts;

            Actor(): Amara::Entity() {

            }

            void recite(Amara::Script* script) {
                scripts.push_back(script);
                script->prepare(this);
            }

            void reciteScripts() {
                Amara::Script* script;
                for (auto it = scripts.begin(); it != scripts.end(); it++) {
                    script = *it;
                    script->script(this);
                    if (script->finished) {
                        scripts.erase(it--);
                        if (script->deleteOnFinish) {
                            delete script;
                        }
                    }
                }
            }

            void run() {
                reciteScripts();
                Amara::Entity::run();
            }
    };
}

#endif
