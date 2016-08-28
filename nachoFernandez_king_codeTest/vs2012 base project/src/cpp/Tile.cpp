#include "Tile.h"


using namespace std;

namespace KingsTest {

	

	Tile::Tile(King::Engine::Texture color, int x, int y) 
		: mColor(color), mX(x), mY(y)
	{

	}


	Tile::~Tile()
	{
	}

	King::Engine::Texture Tile::GetColor()
	{
		return mColor;
	}

	int Tile::GetX()
	{
		return mX;
	}

	int Tile::GetY()
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

	Tile & Tile::operator=(const Tile & right)
	{
		mColor = right.mColor;
		mX = right.mX;
		mY = right.mY;
		return *this;
	}
	
}
