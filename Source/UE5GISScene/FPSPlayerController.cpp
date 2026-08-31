#include "FPSPlayerController.h"
#include "FPSGameMode.h"

AFPSPlayerController::AFPSPlayerController()
{
	bShowMouseCursor = false;
}

void AFPSPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetInputMode(FInputModeGameOnly());
}

void AFPSPlayerController::RequestRespawn()
{
	if (HasAuthority())
	{
		if (AFPSGameMode* GameMode = GetWorld()->GetAuthGameMode<AFPSGameMode>())
		{
			GameMode->RespawnPlayer(this);
		}
	}
	else
	{
		ServerRequestRespawn();
	}
}

void AFPSPlayerController::ServerRequestRespawn_Implementation()
{
	RequestRespawn();
}
