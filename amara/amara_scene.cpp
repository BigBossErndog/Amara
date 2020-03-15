#ifndef AMARA_SCENE
#define AMARA_SCENE

#include "amara.h"

namespace Amara {
    class Game;
    class ScenePlugin;
    class AssetManager;

    class Scene: public Amara::Actor {
        public:
            string key;
            Amara::Game* game = nullptr;
            Amara::LoadManager* load = nullptr;
            Amara::ScenePlugin* scenePlugin = nullptr;
            Amara::InputManager* input = nullptr;
            Amara::ControlScheme* controls = nullptr;
            Amara::AssetManager* assets = nullptr;

            Amara::Camera* mainCamera = nullptr;
            vector<Amara::Camera*> cameras;

            bool initialLoaded = false;
            bool initialCreated = false;

            Scene(): Actor() {

            }

            virtual void setup(Amara::GameProperties* gameProperties, Amara::ScenePlugin* gScenePlugin) final {
                properties = gameProperties;

                game = properties->game;
                input = properties->input;
                controls = properties->controls;
                assets = properties->assets;

                if (load != nullptr) {
                    delete load;
                }
                load = new Amara::LoadManager(properties);

                scenePlugin = gScenePlugin;
            }

            void init() {
                initialLoaded = false;
                
                load->reset();

                for (Amara::Camera* cam : cameras) {
                    delete cam;
                }
                cameras.clear();
                mainCamera = nullptr;

                for (Amara::Entity* entity : entities) {
                    delete entity;
                }
                entities.clear();

                mainCamera = new Amara::Camera();
                cameras.push_back(mainCamera);
                mainCamera->init(properties, this, &entities);

                preload();
                cout << "START LOADING TASKS: " << load->tasks.size() << " loading tasks." << endl;
            }

            Amara::Entity* add(Amara::Entity* entity) {
                entities.push_back(entity);
                entity->init(properties, this, this);
                return entity;
            }

            Amara::Camera* add(Amara::Camera* cam) {
                cameras.push_back(cam);
                cam->init(properties, this, &entities);
                return cam;
            }

            void run() {
                properties->currentScene = this;

                if (!initialLoaded) {
                    load->run();
                    if (!load->stillLoading) {
                        initialLoaded = true;
                        create();
                    }
                }
                else {
                    update();
                    reciteScripts();
                    
                    for (Amara::Entity* entity : entities) {
                        entity->run();
                    }

                    for (Amara::Camera* cam : cameras) {
                        cam->run();
                    }
                }
            }

            void draw() {
                properties->currentScene = this;
				properties->scrollX = 0;
				properties->scrollY = 0;

                stable_sort(cameras.begin(), cameras.end(), sortEntities());
                stable_sort(entities.begin(), entities.end(), sortEntities());

                for (Amara::Camera* cam : cameras) {
                    cam->draw(0, 0, properties->resolution->width, properties->resolution->height);
                }
            }

            virtual void preload() {}
            virtual void create() {}
            virtual void update() {}

            ~Scene() {
                delete load;
            }
    };
}

#endif
