#pragma once

#include "Component.h"
#include "Vector2D.h"

typedef class Sprite Sprite;
typedef class SpriteSource SpriteSource;
typedef struct AEGfxTexture AEGfxTexture;
typedef struct AEGfxVertexList AEGfxVertexList;
typedef class Transform Transform;

class Tilemap : public Component {
public:
	Tilemap(const char* spritesheetFilename, const char* tilemapFilename, const char* collisionMapFilename,
		int onScreenOffsetX, int onScreenOffsetY, int onScreenWidth, int onScreenHeight,
		int spritesheetWidth, int spritesheetHeight);
	~Tilemap();
	void Draw();
	Vector2D getPosOnMap(Vector2D screenPos, Vector2D *offsetFromTile = nullptr);
	Vector2D getPosOnScreen(Vector2D tilePos);
	bool isObjectCollidingWithMap(Vector2D objectPosition, Vector2D objectScale);
	void setOffset(int onScreenOffsetX, int onScreenOffsetY);

	int getTileWidth();
	int getTileHeight();
	int getTilemapWidth();
	int getTilemapHeight();

private:
	void readFiles(const char* tilemapFilename, const char* collisionMapFilename);

	int getTilenum(int x, int y);
	int getCollides(int x, int y);

	int offsetX, offsetY; // on-screen offset from top-left
	int width, height;    // on-screen width/height
	int tileWidth, tileHeight; // on-screen size of tiles
	int *tilemap;         // 2d array of tile nums
	int tilemapWidth, tilemapHeight; // width/height of tilemap
	bool *collisionMap;   // 2d array of colliding sprites

	Sprite* sprite;
	SpriteSource* spriteSource;
	AEGfxTexture* texture;
	AEGfxVertexList* meshQuad;
	Transform* transform;
};