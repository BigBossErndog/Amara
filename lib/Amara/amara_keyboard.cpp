#ifndef AMARA_KEYBOARD
#define AMARA_KEYBOARD

#include "amara.h"

namespace Amara {
    class Key;

    class Keyboard {
        public:
            unordered_map<Amara::Keycode, Amara::Key*> keys;
            vector<SDL_Keycode> tappedKeycodes;

            Amara::Key* lastKeyDown = nullptr;
			Amara::Keycode lastKeycodeDown;

            Keyboard() {
                tappedKeycodes.clear();
            }

            Amara::Key* addKey(SDL_Keycode keyCode) {
                Amara::Key* newKey = new Amara::Key(keyCode);
                keys[keyCode] = newKey;
                return newKey;
            }

            void press(Amara::Keycode keyCode) {
                unordered_map<Amara::Keycode, Amara::Key*>::iterator it;
                Amara::Key* key;
                for (it = keys.begin(); it != keys.end(); it++) {
                    key = it->second;
                    if (key->keyCode == keyCode) {
                        key->press();
                        lastKeyDown = key;
                    }
                }
                tappedKeycodes.push_back(keyCode);

                lastKeycodeDown = keyCode;
            }

            void release(Amara::Keycode keyCode) {
                unordered_map<Amara::Keycode, Amara::Key*>::iterator it;
                Amara::Key* key;
                for (it = keys.begin(); it != keys.end(); it++) {
                    key = it->second;
                    if (key->keyCode == keyCode) {
                        key->release();
                    }
                }
            }

            Amara::Key* get(Amara::Keycode key) {
                if (keys.find(key) != keys.end()) {
                    return keys[key];
                }
                return nullptr;
            }

            void manage() {
                unordered_map<Amara::Keycode, Amara::Key*>::iterator it;
                Amara::Key* key;
                for (it = keys.begin(); it != keys.end(); it++) {
                    key = it->second;
                    key->manage();
                }
                tappedKeycodes.clear();
            }
    };
}

#endif
