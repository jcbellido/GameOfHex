#pragma once


#include <king/Engine.h>
#include <Constants.h>

namespace KingsTest {

	class Tile
	{
	public:
		Tile(King::Engine::Texture color, int x, int y);
		~Tile();

		King::Engine::Texture GetColor();
		int GetX();
		int GetY();

		void SetX(int x);
		void SetY(int y);
		void SetCoords(int x, int y);

		Tile& operator=(const Tile& right);
		
	private:
		King::Engine::Texture mColor;
		int mX, mY;
	};

	/*
	bool operator==(const Tile &left, const Tile &right)
	{
		return left.GetColor() == right.GetColor();
	}
	static bool TileCompare(Tile &t1, Tile &t2)
	{
		return t1.GetColor() == t2.GetColor();
		
	}*/
}
