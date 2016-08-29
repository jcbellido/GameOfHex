/**********************************************************************************************

	C++ Programming Test

	The test consists of making a small, match-3 game on an 8x8 board. The player 
	can swap the gems on the board either by dragging the mouse from one gem 
	to another, adjacent one, or by clicking them in sequence.


		-- Modifications to the given code and assets --

	I have added functionality to modify the size of the SDLWindow, mostly 
	because there was a small black stripe on the right that was annoying me.
	I have also modified the textures to be 40x40 so as to reduce complexity
	when translating from grid to pixel coordinates.
	I have added an extra texture so I had to modify the Engine constructor
	to allow for it, as well as extending the Texture enum

		-- The Tiles --

	The Tiles are represented as a simple class that contains the color 
	and value of the tile, as well as its state (destroyed or not), instead 
	of just being a pointer to a member of the Texture enum.
	This allows for new tile-related features to be added more easily, for 
	instance, bomb tiles, or tiles that are worth more.
	
		-- The Board --
	
	The 8x8 board has been implemented as an array of arrays of pointers to
	tiles, for three main reasons:
	  - First of all, I find it easier to draw and visualize the problem this way.
	  - I chose Tile* as the "leaf type" of the arrays because I wanted to be 
	    able to swap tiles without having to copy the contents of the tile itself.
		Since the code is prepared to be extended, even though now the class is quite 
		simple, it could easily grow in size and make those swaps a bit more expensive.
	  - Since the dimensions of the grid are fixed, std::arrays are perfect for this, 
	    and they take care of a lot of what happens backstage. It's also safer than 
		using C-like "pointer-based" arrays.

		-- Board Generation --

	Board generation is done when the game is created, through a namespace-wide static 
	function. I thought of using classes at first but I came to the conclusion that 
	the generator class would only contain a static function, so the related maintenance 
	didn't seem worth it.
	Which "algorithm" to use is determined by the enum value passed on board construction.
	Ideally, this could be modified via the level json file, so adding generators and
	using them in levels is data-driven.
	
		-- Corrections to Board Generation --

	The generic random grid generator does not perform any match checks after finishing. 
	This was solved by adding a second generator that did so, and swapped colors based on
	random selection of a check for one, or two, tiles backwards.

		-- Constants.h --

	I have used the constants.h file to store constants in it (thanks,
	Captain Obvious) as well as diverse simple functions that I 
	thought would be used in several parts of the code.












**/
