/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			README.txt
	Purpose:			Describes the proper usage of my final project
	Language:			ASCII
	Platform:			Notepad++, Windows 10
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		10/10/2020

- End Header --------------------------------------------------------*/

To compile and run, open the solution in Visual studio 2019 and run from main.cpp. an
example level will be deserialized from json format, stored in the Resources folder.

If this software is not compiled on Windows 10 using Visual Studio 2019, I cannot
predict the results, though I see no reason why other recent versions of Visual
Studio should not work.

If the included external libraries (one directory up) and my basic provided textures 
are not included with the project, I cannot predict the results. Most likely linker 
errors and a failure to load the image.
Both should be present to compile the project.

***************************************

IMPORTANT NOTE: I never had the oppritunity to test this game on DigiPen's lab computers.
I developed it across two laptops, so I know it at least works on two independant
windows 10 machines.


 ----- Basic Gameplay -----
	 P A R I H E L I O N

The goal in parihelion is to destroy all enemy spacecraft, and avoid destruction yourself.
To do this, you have two weapons, and two forms of movement.

Note for professor Chacra:
Level 1 doesn't have much content on purpse, it's a bit of an introduction to the basic
gameplay mechanics.
The real meat of the game is on levels 2 and 3. Level 4 is mostly just for fun.
Also, timed events work but I didn't end up needing them for my gameplay.


Gameplay Note:
In the pause menu, there is a restart level button. There is no shame in using this!
If you get into a situation where you're, say, flying off into space with no hope
of getting back anytime soon, feel free to reset!

Movement:
The WSAD keys corrispond to left, right, forward, and backward *relative to the ship*. These
maneuvring thrusters don't have a whole lot of power behind them, but for tweaking your orbit
in levels beyond level 1, this can be a good thing.
The Q and E keys are your rotation controls. If you're not giving rotational input, your
ship will automatically try to dampen its spin.

Neither of these maneuvering thruster movements burn fuel.

Shift and Ctrl respectively throttle up and down the main engine. This is a powerful form of 
thrust which burns through your fuel meter. 
This is good for chasing down a target, matching velocity with a target, or doing large orbital
changes.
Watch out, though. If you burn through all your fuel, its possible to crash!

Weapons:
Coilgun
The coilgun is a point-and-click weapon. You hold left click and all coilgun turrets on your
ship will spew bullets in the direction you're aiming.
This is the go-to close-quarters weapon.
More damage is dealt if the projectiles are moving quickly relative to what they hit.

At high velocities, coilgun bullets will pierce through a target rather than bounce off.
This can do a lot more damage!

Guided thermonuclear missiles
Using guided missiles is a two-step process. First you must lock onto a target. You can
cycle through targets with the "T" key, and a spinning red indicator will appear over the
targeted enemy.
So long as you have misssiles remaining in your arsenal, you can then press spacebar.
A guided missile wlil be launched out its launch tube if you have one, and after a short
fuse period, it will activate and seek out its target. Guided missiles by default
detonate 1000 units (KM) from their target, allowing for the resulting explosion to wash over
the target.
Guided missiles CAN be shot down. This applies to enemy missiles too!

Guided missiles aren't very smart. They can and will hit a planet, and in extreme situations
which require orbital maneuvering, their use can become  extremely limited.


Types of AI (Besides Missiles)
There is stationary AI, chasing AI, and fleeing AI.
Stationary AI uses AICore and AIStationary, and will try to keep the orbit it is assigned at creation,
or otherwise remain stationary.
Chasing AI will chase the player down, even adjusting orbits if in gravity. If the player is ahead, it'll
descend into a lower orbit to catch up, and visa versa if the player is behind. Chasing AI is ignorant to
orbital direction, however.
At a certained defined distance, this AI will start to ignore orbital motion, and will try to move relative 
to the player while maintaining a certain distance.
Fleeing AI is the same as chasing AI, but inverted. This uses the "Inverted Behavior" flag.

AI's will shoot at the player within a certain range, and within a certain range will have a certain probability
of firing a guided missile at the player. All these values are data-driven.
The AI can blow itself up with these, however. That is worth noting.

 ----- The Debug Menu -----
 In the pause menu, there's a 'debug and cheats' menu.
 This has two options, debug drawing for collision, and clear all projectiles.
 I'd originally intended this to be a little more feature rich including a 'ufo mode' which turns off gravity 
 for the player and lets them move around the scene extremely easily as well as a 'god mode' which disables
 damage on the player.
 Due to time constraints, it is just the two options however.


 ----- The Resources Folder -----
 Resources contains many useful folders and files used for data driving the game.
 
 Config.json:
 Used in initial startup state for the game. Options such as max framerate and Windows
 size can be set in here. I have listed all the managers in there for future proofing,
 but not all of them will have paramaters to tweak.

Textures
Textures for the game. I'm using png files.

Levels
Level json files for the game. A level file can have a name, and a list of GameObjects.
GameObjects must have a name. GameObjects may have an Archetype, and a list of components.
Components which exist in the archetype may be overidden.

Level Archetypes
Building blocks for levels. Usually defined per-level.

Menus
Theres two menus folders, which can be confusing to those not familiar with the structure.
Archetypes/Menu contains archetypes for menuitems. Resources/Menus contains game object arrays
for ingame-menus such as the pause menu and victory/defeat menus.
The main menu and its associated heirarchy is located in Resources/Levels, with an additional
level /Tutorial for tutorial menus.

Archetypes
A special json file which designates a generic type of GameObject. An archetype cannot
be instantiated and therefore has no Name. It's basically just a bunch of components and
perhaps a render pass option to keep the level file clean.

Projectiles
A type of GameObject which can be dynamically spawned in mid-game with its own cap. These
must define a full GameObject and therefore have a Name parameter.

Particles
Similar to projectiles, but lower priority and purely for visual effect.
This ended up getting depricated due to time constraints.

Indicators
These are icons which are displayed over enemies and friendlies ingame, to help the player
better navigate the vastness of space.

 ----- Render passes -----
In this version of the game, there are two render passes. There is the final pass and
the HUD pass. The final pass includes the camera view and perspective projections. The
HUD pass displays objects in normalized device coordinates with no distortion.
HUD objects are rendered after the final pass, allowing for a 2D display on top of the
game world.



 ----- Quirks and Known Bugs -----
 - 	The dev console will always remain open in the background.
 -	When closing out of the game, the dev console will briefly complain about a missing camera.
	This can safely be ignored.
 - 	There may be a memory leak somewhere-- I'm actually not sure. I think I cleaned them
	all up, though.
 -	Strange things can happen if you get adventurous with pausing and unpausing before
	beginning a level. Nothing game-breaking though.
 -	I did a LOT of fixing of missile behavior, but its orthogonal correction value may
	still be a little bit high.
 -	Some visual effects and components on enemies might not loook right, even if they function
	just fine. I didn't have time to do much fine-tuning of positions of things like guns
	and plume effects.
 -	Bullets do not have health. I strongly considered adding that, but I did not end up having
	time. 
 -	100 isn't very many projectiles. Feel free to turn the "max projectiles" value up in resources\\config.json.
 -	This game is very physics-heavy so silly physics bugs are possible. Gravity specifically has a 
	ton of sanity-checks in place, but can still act a little funky in edge cases. Fortunately it's
	pretty unlikely you'll encounter this to any serious degree, other than maybe on level 4.


Some things I wanted to implement, but didn't have time:
 - A 'flash' effect when explosions are spawned.
 - Visual particles for things like impacts and player damage.
 - Dangerous debris when ships explode and take damage.
 - More interesting levels with more interesting hazards.
 
