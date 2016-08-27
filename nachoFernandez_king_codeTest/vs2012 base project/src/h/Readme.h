/**********************************************************************************************

	C++ Programming Test

	The test consists of making a small, match-3 game on an 8x8 board. 
	The player can swap the gems on the board either by dragging the 
	mouse from one gem to another, adjacent one, or by clicking them 
	in sequence.


		-- The Tiles --

	The Tiles are represented as a simple class that contains the color 
	and value of the tile, instead of just being a pointer to a member 
	of the Texture enum.
	This allows for new tile-related features to be added more easily.
	
		-- The Board --
	
	The 8x8 board has been implemented as a  3-dimensional pointer, for 
	two main reasons:
	  - First of all, I find it easier to draw and visualize the problem this way.
	  - I wanted to keep memory under control, and I thought that, 
		although a bit more cumbersome, the "extra" third dimension 
		would allow me to allocate and deallocate memory more precisely, 
		especially when tiles are deleted, after the player successfully 
		achieves a match; and equally, to re-generate tiles in the board.

		-- Board Generation --

	Board generation is done when the game is created, though a namespace-wide 
	static function. I thought of using classes at first but I came to the
	conclusion that the class would only contain a static function, so 
	the related overhead didn't seem worth it.
	
		-- Corrections to Board Generation --

	The generic random grid generator does not perform any match checks 
	after finishing. This was solved by adding a second generator that
	did so.

		-- Constants.h --

	I have used the constants.h file to store constants in it (thanks,
	Captain Obvious) as well as diverse simple functions that I 
	thought would be used in several parts of the code.












**/
