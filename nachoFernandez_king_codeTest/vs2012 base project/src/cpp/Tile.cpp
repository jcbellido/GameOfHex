#include "Tile.h"


using namespace std;

namespace KingsTest {

	

	Tile::Tile(King::Engine::Texture color, int x, int y) 
		: mColor(color), mX(x), mY(y)
	{
		value = 10;
	}


	Tile::~Tile()
	{
	}

	King::Engine::Texture Tile::GetColor() const
	{
		return mColor;
	}

	int Tile::GetX() const
	{
		return mX;
	}

	int Tile::GetY() const
	{
		return mY;
	}

	void Tile::SetX(int x) 
	{
		mX = x;
	}

	void Tile::SetY(int y)
	{
		mY = y;
	}

	void Tile::SetCoords(int x, int y)
	{
		mX = x; mY = y;
	}

	void Tile::SetDestroyed(bool status)
	{
		mDestroyed = status;
	}

	void Tile::SetColor(King::Engine::Texture newColor)
	{
		mColor = newColor;
	}

	bool Tile::IsDestroyed() const
	{
		return mDestroyed;
	}

	Tile & Tile::operator=(const Tile & right)
	{
		mColor = right.mColor;
		mX = right.mX;
		mY = right.mY;
		return *this;
	}
	
}
