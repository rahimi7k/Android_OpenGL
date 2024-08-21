#ifndef GAME_ASSETMANAGER_H
#define GAME_ASSETMANAGER_H

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "Log.h"
#include "Matrix4x4.h"


class AssetManager {

private:
	// Static member to store the asset manager



public:
	static AAssetManager* assetManager;

	// Get the asset manager
	static AAssetManager* getAssetManager() {
		return assetManager;
	}

	// Set the asset manager
	static void setAssetManager(AAssetManager* assetManager) {
		AssetManager::assetManager = assetManager;
	}






        static unsigned char* loadImage(const char* path, int &width, int &height, int &channels_in_file, int desired_channels);
		static void freeImage(unsigned char* imageData);


	static Matrix4x4 projectionMatrix;


};

#endif //GAME_ASSETMANAGER_H
