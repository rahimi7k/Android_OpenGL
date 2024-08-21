#include "AssetManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Matrix4x4.h"

// Initialize the static member
AAssetManager* AssetManager::assetManager = nullptr;



JNIEnv* Log::env = nullptr;
unsigned int Log::count = 10;


Matrix4x4 AssetManager::projectionMatrix = Matrix4x4::identity();






unsigned char* AssetManager::loadImage(const char* path, int &width, int &height, int &channels_in_file, int desired_channels) {

	AAsset* asset = AAssetManager_open(assetManager, path, AASSET_MODE_BUFFER);
	if (asset == nullptr) {
		return nullptr;
	}
	off_t assetLength = AAsset_getLength(asset);
	unsigned char* assetBuffer = (unsigned char*) malloc(assetLength);
	AAsset_read(asset, assetBuffer, assetLength);
	AAsset_close(asset);

	// Set stb_image to flip images vertically
	stbi_set_flip_vertically_on_load(true);

	unsigned char* imageData = stbi_load_from_memory(assetBuffer, assetLength, &width, &height, &channels_in_file, desired_channels);

	free(assetBuffer);

	return imageData;
}

void AssetManager::freeImage(unsigned char* imageData) {
	stbi_image_free(imageData);
	imageData = nullptr;
}


