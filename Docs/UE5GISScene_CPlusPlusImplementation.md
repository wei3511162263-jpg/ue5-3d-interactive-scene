# UE5GISScene C++ Implementation Notes

This project was converted from a Blueprint-only FPS prototype into a hybrid C++ and Blueprint Unreal project.

## Main C++ Classes

- `AFPSCharacter`: player movement, camera control, health, death, weapon ownership, input binding, and replicated character state.
- `AFPSWeapon`: firing, automatic fire timer, ammo count, reload logic, hit-scan shooting, projectile shooting, and replicated ammo.
- `AFPSProjectile`: projectile collision and damage application.
- `AFPSGameMode`: default class registration and respawn flow.
- `AFPSPlayerController`: local game input mode and server respawn request.
- `AFPSPlayerState`: replicated kills and deaths for multiplayer scoreboard use.

## Blueprint Integration

The existing Blueprint assets can continue to hold visual assets such as character meshes, first-person arms, weapon meshes, animation montages, UI widgets, sounds, and particle effects.

Recommended Blueprint parent classes:

- Player Blueprint parent: `FPSCharacter`
- Weapon Blueprint parent: `FPSWeapon`
- GameMode Blueprint parent: `FPSGameMode`
- Projectile Blueprint parent: `FPSProjectile`

## Implemented Gameplay Flow

1. The game starts with `FPSGameMode`.
2. `FPSGameMode` spawns `FPSCharacter` as the default player pawn.
3. `FPSCharacter` registers Enhanced Input actions, creates the default weapon on the server, and owns health/death state.
4. `FPSWeapon` handles fire, reload, ammo, muzzle effects, and damage.
5. Damage is applied through Unreal's normal damage system.
6. When health reaches zero, the character dies, PlayerState statistics update, and GameMode respawns the player after a short delay.

## Packaging Note

Before packaging, regenerate project files and compile the project once in Unreal Editor or Visual Studio. Blueprint assets should then be reparented to the new C++ classes where needed.
