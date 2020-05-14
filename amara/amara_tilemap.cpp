#pragma once
#ifndef AMARA_TILEMAP
#define AMARA_TILEMAP

#include "amara.h"

namespace Amara {
    class Tilemap: public Amara::Actor, public Amara::WallFinder {
        public:
            std::string textureKey;
            std::string tiledJsonKey;
            nlohmann::json tiledJson;

            int width = 0;
            int height = 0;
            
            int widthInPixels = 0;
            int heightInPixels = 0;

            int tileWidth = 0;
            int tileHeight = 0;

            std::unordered_map<std::string, Amara::TilemapLayer*> layers;
            std::vector<Amara::TilemapLayer*> walls;
            std::unordered_map<int, Amara::Direction> wallTypes;

            Tilemap(): Amara::Actor() {}

            Tilemap(float gx, float gy, std::string gTextureKey) {
                x = gx;
                y = gy;
                textureKey = gTextureKey;
            }
            
            Tilemap(float gx, float gy, std::string gTextureKey, std::string gTiledJsonKey): Tilemap(gx, gy, gTextureKey) {
                tiledJsonKey = gTiledJsonKey;
            }

            Tilemap(std::string gTextureKey, std::string gTiledJsonKey): Tilemap(0, 0, gTextureKey, gTiledJsonKey) {}

            virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene, Amara::Entity* givenParent) {
                Amara::Actor::init(gameProperties, givenScene, givenParent);
                if (!tiledJsonKey.empty()) {
                    tiledJson = ((Amara::JsonFile*)load->get(tiledJsonKey))->jsonObj;

                    if (tiledJson != nullptr) {
                        if (tiledJson["width"] > width) width = tiledJson["width"];
                        if (tiledJson["height"] > height) height = tiledJson["height"];
                        tileWidth = tiledJson["tilewidth"];
                        tileHeight = tiledJson["tileheight"];
                    }
                }
            }

            void setTiledJson(std::string gTiledJsonKey) {
                tiledJsonKey = gTiledJsonKey;
                if (!tiledJsonKey.empty()) {
                    tiledJson = ((Amara::JsonFile*)load->get(tiledJsonKey))->jsonObj;

                    if (tiledJson != nullptr) {
                        if (tiledJson["width"] > width) width = tiledJson["width"];
                        if (tiledJson["height"] > height) height = tiledJson["height"];
                        tileWidth = tiledJson["tilewidth"];
                        tileHeight = tiledJson["tileheight"];
                        widthInPixels = tileWidth * width;
                        heightInPixels = tileHeight * height;
                    }
                }
            }

            bool setTexture(std::string gTextureKey) {
                Amara::Loader* load = properties->loader;
                Amara::ImageTexture* texture = (Amara::ImageTexture*)(load->get(gTextureKey));
                if (texture != nullptr) {
                    textureKey = texture->key;
                    return true;
                }
                else {
                    std::cout << "Texture with key: \"" << gTextureKey << "\" was not found." << std::endl;
                }
                return false;
            }

            Amara::TilemapLayer* add(Amara::TilemapLayer* gLayer) {
                Amara::Actor::add(gLayer);
                return gLayer;
            }

            Amara::TilemapLayer* createLayer(int mapWidth, int mapHeight, int tileWidth, int tileHeight) {
                Amara::TilemapLayer* newLayer;
                ((Amara::Entity*)scene)->add(newLayer = new Amara::TilemapLayer(mapWidth, mapHeight, tileWidth, tileHeight));
                newLayer->setTexture(textureKey);
                if (newLayer->width > width) width = newLayer->width;
                if (newLayer->height > height) height = newLayer->height;
                return newLayer;
            }

            Amara::TilemapLayer* createLayer(std::string layerKey) {
                if (tiledJsonKey.empty()) return nullptr;
                std::cout << "Creating Layer: \"" << layerKey << "\" with tilemap \"" << tiledJsonKey << "\"" << std::endl;
                Amara::TilemapLayer* newLayer;
                ((Amara::Entity*)scene)->add(newLayer = new Amara::TilemapLayer(textureKey, tiledJsonKey, layerKey));
                layers[layerKey] = newLayer;
                if (newLayer->width > width) width = newLayer->width;
                if (newLayer->height > height) height = newLayer->height;
                tileWidth = newLayer->tileWidth;
                tileHeight = newLayer->tileHeight;
                widthInPixels = tileWidth * width;
                heightInPixels = tileHeight * height;
                return newLayer;
            }

            Amara::TilemapLayer* getLayer(std::string layerKey) {
                if (layers.find(layerKey) != layers.end()) {
                    return layers[layerKey];
                }
                return nullptr;
            }

            std::vector<Amara::TilemapLayer*> setWalls(std::vector<std::string> wallKeys) {
                walls.clear();
                Amara::TilemapLayer* layer;
                for (std::string layerKey: wallKeys) {
                    layer = getLayer(layerKey);
                    if (layer) {
                        walls.push_back(layer);
                    }
                }
                return walls;
            }

            virtual bool isWall(int gx, int gy) {
                Amara::Tile tile;
                for (Amara::TilemapLayer* layer: walls) {
                    tile = layer->getTileAt(gx, gy);
                    if (tile.id >= 0) {
                        return true;
                    }
                }
                return false;
            }

            virtual bool isWall(int gx, int gy, Amara::Direction dir) {
                Amara::Tile tile;
                for (Amara::TilemapLayer* layer: walls) {
                    tile = layer->getTileAt(gx, gy);
                    if (wallTypes.find(tile.id) != wallTypes.end()) {
                        if (wallTypes[tile.id] == dir) {
                            return true;
                        }
                    }
                    else if (tile.id >= 0) {
                        return true;
                    }
                }
                return false;
            }
            
            void setWallId(int id, Amara::Direction dir) {
                wallTypes[id] = dir;
            }

            void createAllLayers() {
                if (tiledJsonKey.empty()) return;
                int numLayers = tiledJson["layers"].size();
                for (size_t l = 0; l < numLayers; l++) {
                    createLayer(tiledJson["layers"][l]["name"]);
                }
            }

            void run() {
                Amara::Actor::run();
            }

            void draw(int vx, int vy, int vw, int vh) {
                Amara::Actor::draw(vx, vy, vw, vh);
            }

            virtual int getMapWidth() {
                return width;
            }
            virtual int getMapHeight() {
                return height;
            }

            void setCameraBounds(Amara::Camera* cam) {
                cam->setBounds(x, y, widthInPixels, heightInPixels);
            }
    };
}

#endif