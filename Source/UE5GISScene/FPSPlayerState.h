#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "FPSPlayerState.generated.h"

UCLASS()
class UE5GISScene_API AFPSPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AFPSPlayerState();

	UPROPERTY(Replicated, BlueprintReadOnly, Category="Score")
	int32 Kills;

	UPROPERTY(Replicated, BlueprintReadOnly, Category="Score")
	int32 Deaths;

	UFUNCTION(BlueprintCallable, Category="Score")
	void AddKill();

	UFUNCTION(BlueprintCallable, Category="Score")
	void AddDeath();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
