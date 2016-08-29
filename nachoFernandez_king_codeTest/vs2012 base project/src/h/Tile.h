#pragma once


#include <king/Engine.h>

namespace KingsTest {

	class Tile
	{
	public:
		Tile(King::Engine::Texture color, int x, int y);
		~Tile();

		King::Engine::Texture GetColor() const;
		int GetX() const;
		int GetY() const;
		bool IsDestroyed() const;

		void SetX(int x);
		void SetY(int y);
		void SetCoords(int x, int y);
		void SetDestroyed(bool status);
		void SetColor(King::Engine::Texture newColor);

		Tile& operator=(const Tile& right);
		
	private:
		King::Engine::Texture mColor;
		bool mDestroyed = false;
		int mX, mY, value;
	};
}
