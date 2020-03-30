#ifndef AMARA_DEBUGGING
#define AMARA_DEBUGGING

#include "amara.h"

namespace Amara {
    class Error {
        public:
            breakGame() {
                Amara::Error* e = nullptr;
                e->breakGame();
            }
    };

    void log(std::string msg) {
        std::cout << msg << std::endl;
    }

    void error(std::string msg) {
        std::cout << "Amara::Error: " << msg << std::endl;
        Amara::Error* e = nullptr;
        e->breakGame();
    }

    void breakGame() {
        Amara::Error* e = nullptr;
        e->breakGame();
    }
}

#endif
