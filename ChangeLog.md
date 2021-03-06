# Glest ChangeLog

To see a full timeline of detailed changes please visit:
https://github.com/Glest/glest-source/commits/develop

v2.0

- Added the Specialists faction
- Now you can use shift to add and to remove from selected units (or use ctrl to unconditionally deselect)
- Now the Fog of War around a building being built clears out only after construction
- Now attack commands can be queued using the Shift key
- Renamed factions for consistency
- Now default command on right-click is customizable
- Middle mouse button now sends attack command, so you can micro-manage move and attack commands easily using right and middle mouse button

v1.0

- The new logo is polished
- Buildings will now still be built even when units are in the way
- Special effects added to fading/vanishing corpses
- Zoom-out restriction during network play removed
- Maximum unit selection cap has been removed
- Size of attack and move icons increased
- Forest tilesets are more animated
- You can leave and re-join network games, even if you experience lag (buggy)
- Console now hidden by default on Windows
- [Default command is changed to "attack" instead of "move", which is much more intuitive](https://github.com/Glest/glest-source/commit/2c4fc340de958f6b94b40ee62eabff015625e62b)
	(pressing "a" not needed anymore); ["m" key can be used for "move"](https://github.com/Glest/glest-source/commit/bf5a04be18f2b16b8a565acff4baabc5fbfed293).
- Updated UI to look more modern
- Improved OOS detection, which also helps detect cheating
- [Transparency of the alpha component of textures can now be set](https://glest.io/docs/modelling/textures.html#Transparency)/
- Updated [miniz](https://github.com/richgel999/miniz) library. Thanks
  to [richgel999](https://github.com/richgel999) and the miniz contributors.
- CMakeLists.txt files have had a major tune-up (doesn't affect Windows users)
- OpenBSD build instructions added and updated
- [INSTALL.md](https://github.com/Glest/glest-source/blob/develop/INSTALL.md) document created
- man pages are no longer generated each time a build is performed. They must be manually generated when the help usage output is changed.
- Upgraded [libircclient](https://sourceforge.net/projects/libircclient/) embedded library from 1.3 to 1.10
- Added support for Windows 10 SDK
- [16 maps added](https://github.com/Glest/glest-data/compare/4c67d4bfba21...9dfb37450e07)
- [[d1acd87](https://github.com/Glest/glest-source/commit/d1acd8742395bca8b37656e9e8065022902586fe)]Lua can now be used in scripts tag in faction tag within XML
- [[bugfix:835c6ef](https://github.com/Glest/glest-source/commit/835c6efd924ed7e62ccd43e064bc6b7f5cdecbfc)]Fixed "x" key getting stuck on Windows
- [[045b0b8](https://github.com/Glest/glest-source/commit/045b0b88e051f7163fbef42fa0adc63a326ca186)]Removed "MG_" and "xxx" (3 digit extension) on nicknames in lobby player list
- [Windows](https://github.com/Glest/glest-source/commits/develop/mk/windows) build system, dependencies, and documentation updated
- Now allows cross-version network games if the data is the same
- These units can now heal mobile units from their own factions that are in range: nurse (British, radius of 15), archmage_tower (Magic, radius of 8)
- Auto-healing radius changed from 8 to 15 (with the exception of Valhalla)
- Units standing near Valhalla (norse) are auto-healed (radius of 8)
- Supports importing units from other factions, such as by creating a folder named "fakir" in "tech" with a file "fakir.xml" with the following text:
	```<?xml version="1.0" standalone="no"?><unit><link tech-tree="glestpack" faction="persian"/></unit>```
- These units can now heal mobile units from their own factions that are in range (radius of 8):
	priest (Egypt)
	wartime mechanic (Romans)
	minstrel (Elves)
	engineer (Greece)
	magician (Persia)
- Hotkey changes:
	M -> H (Show console/chat history)
	H -> T (Toggle team chat)
	T -> Z (Cycle between store units)
	M is now the hotkey for "move" when units are selected
- [Replaced the sphinx with the scarab](https://github.com/Glest/glest-data/commit/60e734180ecb7f925144c94bef339e6130f9ae20)
(Egypt's starting units)
- 4 new tactical scenarios were added / Thanks @ [Julie Marchant](https://github.com/onpon4)
	Witches' Hunt
	Native Unrest
	Warlock Overlords
	Death Road
- [#13](https://github.com/Glest/glest-source/issues/13)
Observers no longer take up slots on a map. For instance, a map being
played by 4 players can have a 5th person observing; a map being played
by 8 people can have a 9th person observing. Up to 10 people can
connect, which mean you can have up to 6 observers with a 4-player map,
and 2 with an 8-player map.
- [#6](https://github.com/Glest/glest-source/issues/6) Players can now enter the multiplayer lobby unconditionally
- Replaced the Egypt graphics with Archmage's remaster: https://github.com/virtushda/EgyptRemaster

# MegaGlest ChangeLog

To see a full timeline of detail changes please visit:
https://github.com/MegaGlest/megaglest-source/commits/master

v3.13.0
- important sounds like "attack alarm" are always played
- display build/morph/upgrade time
- new AI tag <ai-build-size> for units(buildings) which allows fake size for buildings
  while placing them
- much better error handling and messages especially for modders and xml-related problems
- fix unwanted double event for some keys ( like alt+enter )
- map filter option for game setup on headless server
- option to select/command multiple buildings of same type ( uniform-selection )
- major cross platform OOS fix which especially showed up quite often when playing without
  fog of war
- you must prove basic game knowledge before you can play online by winning a special scenario
- less visible parts of black squares while rendering cliffs
- possibility to let units spawn units on death
- improved AI behavior per faction ( mostly magic )

v3.12.0
- Glest uses SDL2 now
- glowing meshes. ( models can glow like particles do )
- console message for finished upgrades
- new parameter to set servers titles
- 5 new maps
- improved network performance
- better stability in the lobby

v3.11.1
- no more false clicks when commanding via mini map
- map editor has a more useful and powerful map height randomizer
- map editor saves all maps with .mgm ending by default
- fixed map editor ( several linux distributions had trouble )
- for missing tilesets and techs optional download from mod center instead of server. (bugfix)
- healthbar default set to "if needed"
- nicer tilesets as defaults
- improved camera startposition on game start ( main building in center )
- attackBoosts: Ep + Hp regeneration is shown in description
- Glest can be build easier on MacOS ( sadly noone was willing to test )

v3.11.0
- healthbars with a lot of options
- multi shot / multi-projectiles with different timings,sounds and particle systems
- several camera shake effects
- new tileset "pine rock"
- some new maps
- improve switching settings when connected to a headless server
- support for team unit sharing
- team resource sharing
- tags feature  
- Attack boosts fixed in many ways
- Attack boosts and upgrades now support upgrading attack speed
- timed particles and mesh bound particles really work now.
- unit height independend particle positioning ( flat-particle-positions ) 
- CPU players multiplier are displayed in debug view
- You can have non commandable units
- Units spawned by an attack skill can get an attack command  
- Unit and Splash particles can be speeded up ( accelerated )
- Looting https://github.com/Glest/glest-source/pull/17
- HP and EP starting values
- <target-height> <burn-height> can be given https://docs.glest.org/XML/Unit#target-height
- +several bugfixes

v3.10.0 This version was skipped because of version number trouble.

v3.9.1
- Backward compatible with 3.9.0
- Many bug fixes as reported on the forums since 3.9.0
- Moved the project from sourceforge.net to github.com and from subversion to git
- Better handling of lagging clients in network games
- Fixed cmake scripts to correctly detect dependencies that were previously partially
  working on some distros and improved build support for Mac OS X
- Added numerous new LUA functions as documented at http://wiki.glest.org
- Code quality improvements as reported by cppcheck and coverity (and clang)
- Better color picking algorithm thanks to willvarfar makes color picking very stable
- Render order improved to allow more transparency effects
- Adjustable camera scroll speed
- Particle systems can be bound to meshes of models

v3.9.0
- we hopefully now really solved the last out of sync problems for cross platform games.
- animated tileset object support.
- new tileset texturing possibilities
- new tilesets birchforest, desert4 and updated mediterran using animated objects and
  new texture system
- greatly improved textures and animations for the roman faction
- new maps
- better network game performance / management to handle slower clients.
- easier ability to download game content from host and masterserver( if available there ).
- new arranged options menu with serveral sub menus
- menu gui improvements
- attack hotkey toggles through all attack types
- single player games can be sped up incrementally in steps. 
- color picking is greatly improved and the default selection mode now for better compatibility
- greater ability to translate game content into your native language. (including techtrees)
- Added Hebrew, Arabic, Vietnamese (and others).
- screenshots for savegames without annoying menu in screenshot.
- addition of google-breakpad to better track down bugs.
- many new lua functions for scenario modders.
- cell coordinates are shown in the mapeditor
- tilesets can set default air unit heights
- added ability for stand alone mod's to customize more of the engine like about screen.
- performance improvements.
- and as always many bugs were fixed

v3.8.0
(This version was never released, only as a beta.)

v3.7.1
- bugfixes reported in the bugs forum since 3.7.0

v3.7.0
- bugfixes reported in the bugs forum since 3.6.0.3
- display requirements when a player cannot execute a command
- improved IRC connection handling
- auto word wrapping for message boxes
- headless server can run in local lan mode (does not broadcast to master server)
- Added lua security sandbox
- third person view allows you to follow a selected unit. Select one unit anf press f4 to
  enable (deselect anf f4 to disable).
- cell markers allow players to create markers anywhere on the map and associate a note
  with them. All team players see the markers.
- quick sonar signal to tell team players where the action is happening
- admin player can disconnect other network players in game
- saving / loading games.
- when placing a new unit, we display a 'ghosted' model while units walk to the build
  location to show the space it will occupy
- video playback support (using libvlc)
- network multi-player scenarios
- campaign scenarios
- Integrated language translation support (translate / update your favorite language @
  https://www.transifex.com/projects/p/glest and pull the files into the game via
  advanced translation options to test your changes)
- play a sound when player mentions your name in chat mode
- more game data validation for commandline modders, see --help for more info
- many new lua commands for scenario modders
- enhanced AI management (logging and modding) and added more intellegence to decsions
- enhanced game data downloads now allow for downloading data from the mod center instead
  of game server if available.
- Quick keys in game lobby, SHIFT+<letter>+listbox arrow to jump to that letter in a list
  or Hold CTRL when clicking listbox for network control type to change all non human
  control values
- enhanced unicode support for non-ascii characters
- customizable unit, resource and upgrade language files
- tilesets support animated models
- scenario factions may now be 'neutral', meaning not on any players team.
- new command action mouse cursors
- resources are selectable showing info about the resource
- particle colors are now affected by the time of day
- new game hints are displayed while loading a new game
- linked faction support (and new techtree MagiTech for classic Glest play)
- added runtime checks to ensure cpu supports expected optimizations of the binary used
  (sse, x87)
- lots of code cleanup from static analysis such as cppcheck and valgrind
- experimental big endian and non x86 architecure support (need testers to give feedback)

v3.6.0
- bugfixes reported in the bugs forum from beta1 and 2
- iso639-1 style language loading example: glest --use-language=zh and auto language
  detect on first time launch
- new and updated translations (Greek, Japanese, Russian, Italian, Czech etc)
- auto complete player names in chat mode using tab character
- customized port # override via commandline (useful for multiple headless servers) example:
  glest --use-ports=x,y (x is internal port y is external port)
- new commandline option to support standalone modes example: glest --load-mod=x
  (x is the path to the mod)
- new lua functions added (to be documented on the MG wiki)
- lower CPU / RAM requirements in headless server mode
- attack-boosts can be named and shared for proper re-use and singleton usage.
- New font system and enhanced International language support (Chinese, Japanese, etc)
- New intro
- Enhanced AI plays smarter and responds more quickly
- Improved mod menu which always checks for updates for all mods now and more
- Modders can change intro, menus, faction AI behavior and many more things.
- New ability to enable in game team-switching for defecting to other teams in game
- Ability to toggle languages anywhere in the game using CTRL-L
- Numerous bug fixes as reported in the bugs forum
- Improved GlestPack including better balanced Romans faction
- New attack boost system (currently used by Romans faction) which allows new types of
  in game unit effects.
- New headless server mode allows dedicated servers to host games for network players
  (command line option --headless-server-mode)
- Added ability to 'pause' network games by press ESC in game to show a new popup-menu
  with in game options.
- Hosts can now set a title for their game for network games.
- Music fades in and out of game start/stop
- New low food warning indicators tells players when they are running out of food
- MacOSX support (thanks weltall)
- Players are no longer dropped in game lobby when toggling maps (player status may show
  as unassigned network node if they are ina slot that is not applicable to the selected map)
- Bugfixes for supported UPNP for auto router configuration when hosting games
- many more...

v3.5.2
- Lots of work to make Glest a Linux distro friendly package (Debian, Arch, etc)
- Added Support for Russian and Ukrainian with proper fonts in Linux if installed.
- Map editor now supports diagonal flipping
- Windows users now have a shortcut to the mod folder in their Glest programs Menu.
- Bug fixes including:
  - Fix corrupted g3d viewer screenshots, and automatic screenshot mode does not steal
    focus for Linux users
  - Proper network disconnect detection when connection drops in an abnormal way
  - Avoid crash when user has no soundcard installed
  - Better video handling for buggy opengl drivers that don't properly handle non power
    of two textures
  - Better support for non ASCII file paths (like Japanese, etc) for international users
    and Unicode enabled parts of the code

3.5.1
- Much better pathfinder performance
- Allow servers to temporary block specified network players from the game lobby
- Better Linux distro packaing support (by default glest works in standard linux
  paths and make install added)
- Game data cleanup (duplicate sound files have been merged in glestpack) and non free
  content replaced. (autumn and desert tilesets)
- Displayed up to 15 queued commands (observers can view all players queued commands)
- Allow factions in a techtree to share common data using the special tag in xml files
  $COMMONDATAPATH which points to the techtrees commondata/ folder.
- standardized games and tools startup scripts with names starting with 'startup_'
- updated translations for German, Italian and French
- bugfixes reported in 3.5.0
- AI enhancements (better repair, and safer build positions)
- Improved network code, fixes some out of synch issues.
- Display players svn revision# in network game lobby

3.5.0
- Menu changes to better group similar items
- Added Blender related tools in blender sub folder
- Modified binary and script names for Linux builds to avoid conflicts with original glest
- Added preview info in mod menu, scenario menu and tutorials
- End game stats have more info now
- Added more validations for modders for techtrees and scenarios (commandline options)
- Bugfix for OpenAL (now works in v 1.13)
- Updates to Romans faction
- Network messages generated by the game are now displayed in the local users selected language
- Support for custom credits (add the file data/core/menu/credits.txt)
- More AI player improvements
- Default screenshot format is now jpg (configurable in options menu)
- Fixed some performance problems with 'stuck' units
- improved resource selection
- performance improvements, now make it possible to play with more units and slower
  hardware than before.
- new maps with cliffs and higher terrain
- support for sending 7z tilesets and techtrees over network
- new game mod management console to download new game content or remove existing mod content
- multi-threaded pathfinding for better performance when many units are moving
- CPU AI player improvements (units get unstuck more often and stationary air units
  attack more efficiently)
- new tileset and resource particle support.
- added jpg screenshot support
- optional compressed GPU texture support
- improved harvesting and resource selection
- better support for multiple platform compiling
- Modifiable keyboard hotkeys
- bugfixes for map editor and g3d viewer to work more stable on multiple platforms
- LSB conform ".glest" usage


3.4.0
- new attack alarms use visual and audio cues to indicate when and where you or your
  team are under attack
- new auto router configuration for Internet hosted games will auto port forward (for
  UPNP enabled routers)
- new network file transfer system current allows servers to send missing maps and
  tilesets to clients
- new IRC client built into the Internet lobby (uses freenode's glest-lobby channel)
- improved sound handling for better stability and smoother game play
- CPU AI players now can have their resource multiplier manually changed
- new fog of war setting allows showing resources on the map at game start (thanks GAE team)
- many new commandline options (try --help to see)
- texture compression for video cards that support it
- improved AI players are much harder to defeat
- new Romans_beta faction in glestpack
- new language translations
- new tilesets, maps and a tutorials
- G3D Viewer enhancements including PNG screenshots with alpha transparency option
- performance improvements for both rendering and network play
- FreeBSD support has been added in this release
- no more selection problems!
- bugfixes discovered in 3.3.7.2


3.3.7.2
- This is primarily a bugfix release to correct noticable issues from 3.3.7

3.3.7.1
- This version included a bug and was withdrawn within minutes after release.

3.3.7
- Again more performance updates to rendering and game updating
- screenshots work again in windows ( bugfix )
- some new maps especially meant for coop multiplayer
- map preview in the game setup menus ( can be switched off in options for performance reasons )
- new features for unit particles ( for example see genies new green glow )
- observers ( implemented as special faction to choose )
- multibuild 
- teamates are shown with yellow cycle now ( instead of red )
- bugfix for AI not losing HP when food is out
- added more LUA functions 
- player number based map filter in cutom game menu
- toggle of music in custom menu while waiting for players
- advanced features switch to simplify the game setup
- faction preview ( a small version of the loading screen is displayed )
- working transparency textures for units! ( basically like trees in tilesets now )
- smooth shadow on/off while playing when needed for performance reasons.
- new unit property max-unit-count to limit the amount of living units of this type
- harvesting units of size bigger one work better now
- free BSD support ( thanks to M0llemeister! )
- first MacOS support ( alpha,  thanks to GeoVah! )
- several other minor changes/bug fixes

3.3.6
- More performance updates to rendering and game updating
- Improved network play for slower clients (new game options for lagging players)
  - Added the ability to toggle Pause or Disconnect for lagged clients per game
  - Better error detection of network state
- Updated client and server lobby screens to show more game information
- Added more per game changable settings for network games
- Updated support for GAE pathfinder (but should ONLY be used in single player 
  mode as network use is experimental)
- Added new commandline parameter for modders to validate their techtrees, usage:
  ./glest --validate-techtrees
- Added new commandline parameter to display openGL driver information, usage:
  ./glest --opengl-info
- Added support for Open Suse
- Fixed bugs where invalid IP addresses were used for NIC's that are not active
- Added the ability to turn OFF sounds via the options menu
- Some bug fixes and changes were made to the AI
- Fixed issue where command queues and multi-unit selecting was broken
- Fixed map editor for windows users where tooltips didn't show properly and memory corruption.

3.3.5.1
- Fixed slow rendering by reverting back to vanilla glest rendering
- Added much more error checking and network finesse
- Increased HTTP connection timeout from 5 to 10 seconds and made it configurable
- Added LAN IP Addresses on Custom Game Menu
- Disabled "Zoom out' on game end for Network Games (Created instability)
- Added more relaible TCP connection status checking
- Fixed mouse disappearing issues
- Made error messagebox persist between menus / screens
- Added ability to continue loading / playing if sound system cannot initialize

3.3.5
- major milestone:
  http://freegamer.blogspot.com/2010/06/glest-335-pre-release-special.html
- Thank you to: titi, tomreyn, silnarm, rcl, ultifd, wciow, claymore, 
  coldfusionstorm, FG_Julius (and crew), softcoder
- Added loads of Network features (See below)
- Added multi-platform network play support
- network performance improvements
- Added master server support to help the community find online Internet games
- Added Internet lobby to display Internet Game Hosts
- Added client lobby where clients may chat and configure their player
  settings before the game begins
- Added Network LAG checking for network games
- Added Server / Local based AI control for Network games
- Added customizable hotkey support
- Introduced native Linux 64 bit binaries and installer
- added queued commands (hold shift while continue clicking future unit commands)
- added new maps, tilesets and improved particle effects
- fixed many bugs found during the release cycle
- added particle viewing support in the G3d Viewer

3.3.4
- Fixed multiplayer related crashes from 3.3.3
- 'M'-key to redisplay faded messages
- Some fontwork again
- Windows is using OpenAL as default
- Waterbug fixed. Walking in water volume is now releated to the camera position.

3.3.3
- Font size adjustment can be set in the ingame options.
- Font colors can be toggled while playing using the 'C'-key.
- Bugfix for windows, ingame options can change resolutions now. 
- Font Helvetica is used for linux and font Verdana is used for windows
- Fontsizes can be set very detailed in glest.ini
- Fixed glest.ini

3.3.2
- Changed windows version to use SDL and also added support for OpenAL sound
  (DirectSound is still the default)
- Sound issues for windows users are related to a bad dsound.dll in the 
  mega-glest folder. Delete this file as it is not required since DirectX should
  already be installed on your Windows installation.
- Textures may now be in any of the following formats (tga, bmp, jpg and png)
- Scenarios, Techs and Factions all allow for a custom 'loading' page when 
  starting a game. Mega-Glest will look for a file called: loading_screen.*
  where .* is any supported graphic file type (tga, bmp, jpg and png). The 
  priority is first in the scenario folder (if loading a scenario) if not found
  it looks in the faction root folder and if not found it will look in the tech
  folder.
- Screen Resolutions can be changed in the ingame options menu now
- Added a LAN game auto-discovery feature. Clients may click the 'Find LAN Games'
  button to search on your local network for another Computer hosting a 
  Mega-Glest game (a server).
- Mega-Glest now actually uses the ServerPort setting in the glest.ini
  (the default port for glest is 61357)
- Added a read-only display ofthe configured Server Port # when joining a network
  game.
- Added the ability to configure Fog of War per game session.
- Fully working Photo Mode where you may take good photos of your mega-glest
  game / mao scenario by setting the value in glest.ini:
  PhotoMode=true
- Added the ability to use a playername instead of the hostname when playing 
  in a network game. Goto the options area (Options button on main screen) 
  and type your custom playername into the Network Player Name edit box.
- Added the ability to rotate units when creating a unit (before placing it) 
  by pressing the R key (for rotate)
- Added camera zoom in/ out (like in GAE) by using the scroll wheel on your 
  scroll mouse
- Added MANY new configurable items in glest.ini (some are optional and may not
  be in the ini file. Most options are described when you run the configurator
  application.
- Added the ability to run Glest full screen on multiple monitors 
  (currently linux only) via the two settings in glest.ini:
  AutoMaxFullScreen=true
  Windowed=false
- Users may now save all user created context in their own customer folder using
  the glest.ini setting:
  UserData_Root=mydata/
  where mydata is a folder under the current glest folder. You may use ANY path
  in this setting.
- Glest allows users to specify a special folder to read / write the ini and log
  files. To do this set an environment variable with the name:
  GLESTHOME=/mycustomerpathtoawritablefolder/
- When experiencing problems, users may enable debugging to a logfile using the 
  following glest.ini settings:
  DebugLogFile=debug.log
  DebugMode=true
  DebugNetwork=true
  This will log all debug information to a file in the glest folder called 
  debug.log
- fixed a bug with not calling <unitDied> in lua scenario, when died due 
  to running out of consumable ressources like food
- fixed the bug which lets you loose the active command if one of your unit dies. 
  This was the problem that made it very hard to build buildings when you are in 
  a battle for example.
- When a AI is disable for a faction in a scenario this faction doesn't consume
  consumable ressources like food any more. 

3.3.1
- fixed a bug with network consistency checks ( it wasn't completly active any more in 3.3.0 )
- network consistency checks are reported more user friendly (on server and client )
- all network related errors should be handled with a dialog and doesn't crash glest any more.

3.3.0
- New multiplayer version of glest! It should be much more stable now 
  and less choppy than original glest !
- New disconnect messages and chat in connect menu .....
- Now with several particles for every skill!
- black particles!
- Fire with smoke for every "burnable" unit ( even walking/flying ones )
- 8 player support
- Individual particle systems for damage indication
- More particles everywhere in the data
- New snow
- Ingame option to switch off all the new particles ( for slow machines )
- Morphing ground units to flying units
- Damage-all switch in splash definition works now
- Linux editor/g3dviewer/configurator are working and part of the linux 
  distribution too now
- New maps
- New tileset scrubland
- Editor with lots of comfort functions and 8 player (*.mgm) support.
  The editor includes things like undo/redo, brush copy , mouseover display now
  
3.2.3
- Glest forks off vanilla Glest
- Only needed factions are loaded ( thanks silnarm ). 
- a weeker CPU player was added for real beginners
- two new CPU players were added ( CPU-Mega and CPU-Easy )
- much much more data compared to glest ( factions/maps/tilesets ... )


# Glest ChangeLog

3.2.2
- Two networking bugfixes

3.2.1
- Auto tester
- Bug fixes

3.2.0
- LUA scriting
- New tileset: Dark Forest
- Tutorials
- Improved text rendering
- Added sound effect for chat messages
- Changed loading screen
- Removed IP from new game screen
- Removed Api Info screen

3.1.2
- Fixed slow building placement in ATI cards with catalyst 7.10 or newer
- Fixed fog of war smoothing
- Improved position picking when giving orders
- Fixed some glitches in shadows
- Added red colored building cursor when trying to build in an invalid location
- Fix for empty chat messages
- Reduced Drake Rider morphing time from 120 to 80 and reduced splash radius attack of a few units
- Improved battlemachine death animation
- The game now checks that all players are using the same binaries

3.1.1
- Improved shadows
- Added command line options
- Improved 'Valley Of Death' map

3.1.0
- In-game chat
- Main menu background model changed
- Server IPs are now stored in a file, and can be used later
- Network status key
- Map changes: Added 'Swamp of Sorrow' and 'Valley of Death', removed 'One on One' and 'The Ruins'
- Balance changes

3.0.0
- Network play over LAN/Internet
- New key shortcuts for giving orders and selecting special units
- Renamed some units
- Reworked tech faction upgrades
- Balance changes
- Clicking on the minimap now always moves the camera, even if the area is not explored
- Removed some dangerous glest.ini options

2.0.1
- Scenarios
- Added more info to the results screen

2.0.0
- New Magic units: 
	- Tower of Souls: Air defense building, attacks air only
	- Golem: Defensive unit, needs EP to walk, can't attack air
	- Daemon giant: Heavy melee unit
	- Drake rider: Light ranged unit 
- New Tech units:
	- Aerodrome: Building for producing air units
	- Air ballista: Air defense building, attacks air only
	- Rider: Fast medium unit
	- Ornithopter: Light air unit
	- Airship: Heavy air unit, can't attack air
- New Magic upgrades
- New Tech upgrades
- New animations for existing units
- Shared vision between allies
- New particle blending
- Felix and Matze added to the credits
- Players now start on the location indicated in the game settings menu
- Score system
- HTML documentation
- Increased projectile accuracy
- Optimized particle rendering
- Loads of balance changes
	
1.2.2
- Fixed timing problem that was causing some units to regenerate too fast on some computers
- Html tech tree documentation

1.2.1
- Fixed random crash during load

1.2.0
- New tileset objects
- AI now expands its base
- 2 new maps: "in the forest" and "island siege"
- Improved existing maps
- Pathfinding optimizations
- Fixed issues in windowed mode
- Balance changes
- Enemy projectiles are visible when being attacked outside the sight range
- Fixed crash when trying to build a unit without be_built skill

1.1.1
- Archmage energy regeneration rate back to normal
- New windowed mode with window borders
- New error message for when maps have dimensions that are not power of 2
- New contact email

1.1.0
- Glest now compiles on MinGW
- New tileset objects
- New particle systems for all units
- Shared library merged with Duelfield
- Fixed OpenGL version detection bug (now the game detects OpenGL 2 correctly)
- Auto config option added to in-game config menu
- Added configuration option to bypass OpenGL version check
- Added object variations in tilesets
- All files renamed to lowercase to ease portability
- New map: One On One
- New 3D model format (G3D v4)
- Dead skills now have a fade parameter
- Strings in XML files restricted to lowercase (game formats names) 
- ShadowMapLike shadows renamed to Projected shadows
- Shadow mapping support, producing everything over everything shadows
- 8 bit TGA support
- Changed some particle systems
- Now if video mode change fails the game tries the same mode with the default refresh rate
- Changes on about and loading screens
- Changes on language strings
- Fixed a bug affecting the orientation of the light at night

1.0.10
- Small bug fixes

1.0.5
- Balance changes
- Computer controlled units that can't attack air won't run under dragons
- New configuration tool
- Fixed a bug with building construction emplacement
- Fixed a bug related with shadows in free camera mode
- New translation strings for main menu
- Win/Lose window
- Other bug Fixes

1.0.0
- Remaining 3D models done
- Remaining 2D art done
- New magic faction music
- New main menu art and programming
- Fonts change it's size depending on screen resolution
- Bug fixes

0.8.1
- New Sumoner Guild model
- Startup crash on some systems fixed

0.8.0
- Two factions with new units and 3D models
- Cellmaps for units, now units can enter and exit buildings
- New sounds for everything
- New 2D art for icons and buttons
- New water rendering method
- Improvements on projetile damage synchonization
- New main menu system, now a 3d model is used for background
- Particle systems redesigned to allow 3D models
- Sounds synchronized to projectiles and splashes
- New menu music
- Meeting points
- Selection improvements
- HP regeneration 
- Camera can be moved with the arrow keys
- About section changed
- Bug fixes

0.7.6
- Bug fixes

0.7.3
- Selection improvements
- Air units support
- AI improvements
- Bug fixes

0.7.2
- Fixed a bug related with unit resource info

0.7.1
- New stone texture
- 3D textures disabling option added to config menu
- ShadowVolumes removed from shadow options (can be enabled via glest.ini).
- DepthBits now defaults to 32 (up from 24), and StencilBits to 0 (down from 8)
- Fixed some bugs
- Battlemage sounds are now at 22Khz
- Performance optimizations
- OGG files can also be used as static sounds

0.7.0
- New 3D models for units
- New 3D models and textures for tilesets
- 3D model format changed to add support for player colors
- New maps
- Improvements on building command
- Experience levels
- New sounds
- New command available: morph
- 3D textures (for animated water)
- Water Effects
- Focus arrows
- New XML field: anim-speed 
- MPs (Magic Points) are now called EPs (Energy Points)
- New cell sytem: maps 2x bigger, river borders more accurate
- Minimap is now filtered
- Surface detail removed (always draw on max detail)
- Animated trees on intro screen
- Pressing 'T' saves a TGA image on the screen folder
- Improved AI
- Units react instantly when given a command
- Ranged units now attack walking units
- Camera angle changed
- Sounds are synchronized to the action (new XML tag for this)
- Command and Skill format changed, now type is the first tag
- New options for particle systems
- Unit speed changed on XMLs
- Fog of war changes smoothly
- Optimizations on pathfinding
- Optimizations on particle systems
- Crash info is now saved in crash.txt

0.6.3
- New and very fast shadow mode: Shadow Mapping Like
- Some new sounds
- Camera movement stops slowly
- Fire particles changed
- Height fog removed, depth fog added
- WGL extensions added to the api info menu
- Anisotropic filtering (option added to glest.ini)
- Map format changed from text to binary
- Buildings are randomly rotated

0.6.2
- Texture tiling changed to texture splatting

0.6.1
- Units now turn arround when changing direction
- Free camera mode optimized

0.6.0
- New building models
- Music and sounds
- New particle systems: fire ball and ice nova
- Damage and armor types
- Now it is possible to attack your own units
- Now it possible to give "follow" and "attack unit" orders (using move and attack
  targeting a unit)

0.5.2
- Fixed a bug in pathfinding algorithm

0.5.1
- Fixed a bug loading GL_EXT_fogcoordf

0.5.0
- Added win/lose screen
- DX8 sound
- 3d realtime menu
- Ogg sound support
- Streaming sound support
- Game speeds and pause
- Groups and add/remove to/from selection
- New 3d models for units

0.4.3
- Fixed a bug when double clicking two towers, one under construction and the other one
  built, caused the first one to finish its construction.
- Fixed a bug that made that fire never stopped when repairing buildings
- Performance counters added for world update timing

0.4.2
- Fixed a bug that made the game crash when right clicking outside the map and a building
  was selected

0.4.1
- Fixed a bug that made the game crash when canceling unit production

0.4.0
- Shadows (infinite shadow volumes)
- New graphic engine, faster model and particle rendering
- New model format, that allows to export files from 3dsmax
- Memory leaks have been removed (using _CrtDumpMemoryLeaks())
- Air units support
- TGA texture support
- Height fog (using GL_EXT_fog_coord)
- ISO Latin 1 character set support for lang files
- Screen resolution can be changed by editing glest.ini. 
- More configuration options
- Now terrain borders are alpha-blended, and look better
- Double click selects nearby units of the same type
- Model viewer and map editor have been moved to the editor pack
- New lang file format
- New glest.ini file format

0.3.1
- Opengl 1.2 and GL_ARB_multitexture support check
- G3d model viewer improvements

0.3.0
- Keyframe animation (linear interpolation)
- Particle systems (rain, snow, fire and magic projectile)
- New pathfinding algorithm (A* with g=0)
- New tile generation system (no more problems with triple intersections)
- Teams 
- Map editor
- 3d model viewer
- More sounds
- Improved fog of war using multitexture (no more problems with water colors)
- Auto orders when clicking right mouse button
- New 3d models and animations
- New sun orbit (better lighting)
- New languaje strings added
- Selection circles reflect HP and MP status 
- Progress bars
- Bug fixes

0.2.0
- Models (such as trees) have RGBA textures.
- Better lighting model now, less ambient and more difusse and specular
- Units have new skills and magics.
- Sound support (direct sound).
- Corpses now remain in the battlefield
- Unit format has totally changed, unit and buildings are the same now.
- Speed optimizations.
- glest.ini has more options.
- Units now try to mantain it's formation when receiving move commands.
- Some models have been remade (Archmage, Town Hall..)
- Buildings adapt to the terrain
- Lots of bug fixes.

0.1.6
- Unit models have been redesigned, now they have more polygons and detailed animations.
- Minor bug fixes.