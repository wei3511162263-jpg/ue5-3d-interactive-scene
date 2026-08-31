#include "FPSGameMode.h"
#include "FPSCharacter.h"
#include "FPSPlayerController.h"
#include "FPSPlayerState.h"
#include "TimerManager.h"

AFPSGameMode::AFPSGameMode()
{
	DefaultPawnClass = AFPSCharacter::StaticClass();
	PlayerControllerClass = AFPSPlayerController::StaticClass();
	PlayerStateClass = AFPSPlayerState::StaticClass();
	RespawnDelay = 3.0f;
}

void AFPSGameMode::HandlePlayerDeath(AFPSCharacter* DeadCharacter, AController* KillerController)
{
	if (!DeadCharacter)
	{
		return;
	}

	AController* DeadController = DeadCharacter->GetController();
	if (!DeadController)
	{
		return;
	}

	DeadCharacter->DetachFromControllerPendingDestroy();

	FTimerHandle RespawnTimer;
	FTimerDelegate RespawnDelegate;
	RespawnDelegate.BindUObject(this, &AFPSGameMode::RestartDeadPlayer, DeadController);
	GetWorldTimerManager().SetTimer(RespawnTimer, RespawnDelegate, RespawnDelay, false);
}

void AFPSGameMode::RespawnPlayer(AController* ControllerToRespawn)
{
	RestartDeadPlayer(ControllerToRespawn);
}

void AFPSGameMode::RestartDeadPlayer(AController* ControllerToRespawn)
{
	if (ControllerToRespawn)
	{
		RestartPlayer(ControllerToRespawn);
	}
}
