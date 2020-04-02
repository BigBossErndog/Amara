#include <amara.h>

using namespace Amara;
using namespace std;

class TestScene: public Scene {
    public:
        void preload() {
            
        }

        void create() {
            controls->addKey("up", KEY_UP);
            controls->addKey("down", KEY_DOWN);
            controls->addKey("left", KEY_LEFT);
            controls->addKey("right", KEY_RIGHT);

            controls->addButton("up", BUTTON_DPAD_UP);
            controls->addButton("down", BUTTON_DPAD_DOWN);
            controls->addButton("left", BUTTON_DPAD_LEFT);
            controls->addButton("right", BUTTON_DPAD_RIGHT);

            controls->addButton("confirm", BUTTON_A);
        }

        void update() {
            if (controls->justDown("up")) {
                log("UP PRESSED");
            }
            if (controls->justDown("down")) {
                log("DOWN PRESSED");
            }
            if (controls->justDown("left")) {
                log("LEFT PRESSED");
            }
            if (controls->justDown("right")) {
                log("RIGHT PRESSED");
            }
            if (controls->justDown("confirm")) {
                cout << input->gamepads->numConnected() << endl;
            }
        }
};

int main(int argc, char** args) {
    Game* game = new Game("Amara Game");
    game->init(480, 360);

    game->scenes->add("test", new TestScene());
    game->start("test");

    // game->scenes->add("enterSceneKey", new GiveSceneInstance());
    // game->start("enterStartingScene");

    return 0;
}
