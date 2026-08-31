#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameMode.generated.h"

class AFPSCharacter;

UCLASS()
class UE5GISScene_API AFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFPSGameMode();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Rules")
	float RespawnDelay;

	UFUNCTION(BlueprintCallable, Category="Rules")
	void HandlePlayerDeath(AFPSCharacter* DeadCharacter, AController* KillerController);

	UFUNCTION(BlueprintCallable, Category="Rules")
	void RespawnPlayer(AController* ControllerToRespawn);

protected:
	void RestartDeadPlayer(AController* ControllerToRespawn);
};
