#ifndef AMARA_KEY
#define AMARA_KEY

#include "amara.h"

namespace Amara {
    class Key {
        public:
            string key;
            SDL_Keycode keyCode;

            bool isDown = false;
            bool justDown = false;
            bool tapped = false;
			bool justUp = false;
			bool held = false;
            bool activated = false;

            int downCounter = 0;
			int tapTime = 15;

            Key() {
                downCounter = 0;
            }

            Key(string keyname): Key() {
				key = keyname;
			}

            Key(string givenKey, SDL_Keycode givenKeyCode) {
                key = givenKey;
                keyCode = givenKeyCode;
            }

            void press() {
				if (!isDown) {
					isDown = true;
					downCounter = 0;
					justDown = true;
					held = false;
				}
				activated = true;
			}

			void release() {
				justUp = true;
				justDown = false;
				isDown = false;
				activated = false;
				held = false;
				if (downCounter < tapTime) {
					tapped = true;
				}
			}

            void manage() {
                justUp = false;
				justDown = false;
				if (isDown) {
					downCounter += 1;
					if (downCounter > tapTime) {
						held = true;
					}
				}
                activated = false;
            }

            bool is(string keyCheck) {
                if (key.compare(keyCheck) == 0) {
                    return true;
                }
                return false;
            }
    };
}

#endif
