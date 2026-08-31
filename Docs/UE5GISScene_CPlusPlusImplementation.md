# UE5GISScene C++ Implementation Notes

This project started as a UE5 FPS client-side gameplay demo and was later extended with
city-scale rendering, GIS, and digital-twin-oriented support classes.

This project was converted from a Blueprint-only FPS prototype into a hybrid C++ and Blueprint Unreal project.

## Main C++ Classes

- `AFPSCharacter`: player movement, camera control, health, death, weapon ownership, input binding, and replicated character state.
- `AFPSWeapon`: firing, automatic fire timer, ammo count, reload logic, hit-scan shooting, projectile shooting, and replicated ammo.
- `AFPSProjectile`: projectile collision and damage application.
- `AFPSGameMode`: default class registration and respawn flow.
- `AFPSPlayerController`: local game input mode and server respawn request.
- `AFPSPlayerState`: replicated kills and deaths for multiplayer scoreboard use.
- `UGISDigitalTwinSubsystem`: geographic origin management, lightweight longitude/latitude to Unreal world coordinate conversion, city asset registration, and POI query support.
- `AGISCityAssetAnchor`: metadata anchor for imported city-scale assets, including source URL, coverage size, approximate geo-reference, HLOD, and Nanite-oriented optimization flags.
- `AGISPointMarker`: point-of-interest marker for landmarks, roads, water areas, green land, utilities, and other GIS scene objects.
- `AGISMeasurementTool`: distance and horizontal distance measurement helper for GIS-style spatial analysis.

## Blueprint Integration

The existing Blueprint assets can continue to hold visual assets such as character meshes, first-person arms, weapon meshes, animation montages, UI widgets, sounds, and particle effects.

Recommended Blueprint parent classes:

- Player Blueprint parent: `FPSCharacter`
- Weapon Blueprint parent: `FPSWeapon`
- GameMode Blueprint parent: `FPSGameMode`
- Projectile Blueprint parent: `FPSProjectile`
- City asset anchor Blueprint parent: `GISCityAssetAnchor`
- POI marker Blueprint parent: `GISPointMarker`
- Measurement tool Blueprint parent: `GISMeasurementTool`

## Implemented Gameplay Flow

1. The game starts with `FPSGameMode`.
2. `FPSGameMode` spawns `FPSCharacter` as the default player pawn.
3. `FPSCharacter` registers Enhanced Input actions, creates the default weapon on the server, and owns health/death state.
4. `FPSWeapon` handles fire, reload, ammo, muzzle effects, and damage.
5. Damage is applied through Unreal's normal damage system.
6. When health reaches zero, the character dies, PlayerState statistics update, and GameMode respawns the player after a short delay.

## GIS / Digital Twin Flow

1. The city asset is represented by `GISCityAssetAnchor`, using the Manama Bahrain 3D City Map Model listing as the test data source.
2. `GISDigitalTwinSubsystem` keeps a geographic origin and converts longitude/latitude/elevation into UE world positions.
3. `GISPointMarker` actors can be placed as landmarks, road nodes, water areas, green-land entries, or facility markers.
4. `GISMeasurementTool` provides point-to-point distance and horizontal distance calculations for simple spatial analysis.
5. The existing first-person character is used as an urban roaming and inspection camera inside the city-scale scene.

## Packaging Note

Before packaging, regenerate project files and compile the project once in Unreal Editor or Visual Studio. Blueprint assets should then be reparented to the new C++ classes where needed.
