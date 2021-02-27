
		Flappy Bird DOS clone
		Version: 0.02
		Engine: WIP-2DFX
		Code: Roy Massaad
		Date: Feb 14 2021

		Requirements:
		-86Box (real performance), DosBox
		-486DX2 66mhz+, Emulated or Real Machine + VGA card 320x200 256 colors

		Tools and Libraries:
		WGT 4.5 Wordup Graphics ToolKit GPL, Borland C++ 3.1
                http://web.archive.org/web/20001027023929/http://www.egerter.com/opensource.htm
                http://web.archive.org/web/20000606010204/http://www.egerter.com/index3.htm

		License: GPL 3

		TODO:
		-Soundbaster Audio SDK, FLIC movie & MIDI support (currently experimenting)
		-EMS expanded memory use (can't use DOS extenders with BC31)
		-Port to Watcom c++ environment (better compiler/protected mode, but
		 worse vi editor..?)
		-Use inline assembly for better collision code?
		-Check for memory leaks
		-Add/upgrade to more 'modern' engine features (scene graph with flag
		 properties (_visible, draw() and update() on objects centralized loading
		 of images with no memory duplicates)
		-Mouse support
		-Tile Support
		-Custom fonts
		-Write custom vga drawing routines in assembly ? (WGT seems fast enough)
		-Turn the core code into a general purpose 2d game engine
		-Separate the project into more files

		NOTES:
		-Space bar to jump, x to reset, q to quit (lowercase)
		-Smooth scrolling under a 486 dx..
		-We might get a cool DOS engine at some point out of all this eventually
		-BC31 is very old and lacks alot of IDE & c++ features
		-You need to setup BC3 + download and link WGT45 libs/includes/help
		-Edit the project files settings to match your dev environment
		-This was developed inside 86Box, files copied to windows98 vm over msnet client 3
		-Tried to use libs+OOP when possible, back in the day c++ for gaming was a
		bad idea for performance reasons, x86 assembly was used to speed things up
		-Why i did this? Nostalgia of course! plus flappy bird is easy to code
		-PCX is the image format supported, WGT uses it with Palettes
		-The hitbox of the bird is slighly smaller than the image on purpose
		-If you manage to run it on real 486 hardware let me know!
                -Why not Watcom/Djgpp with VI/RHIDE first? I have memories of Borland C++ IDE as a kid
		-Feel free to help pimp it out

