#include "FPSPlayerState.h"
#include "Net/UnrealNetwork.h"

AFPSPlayerState::AFPSPlayerState()
{
	Kills = 0;
	Deaths = 0;
}

void AFPSPlayerState::AddKill()
{
	if (HasAuthority())
	{
		++Kills;
		SetScore(Kills);
	}
}

void AFPSPlayerState::AddDeath()
{
	if (HasAuthority())
	{
		++Deaths;
	}
}

void AFPSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSPlayerState, Kills);
	DOREPLIFETIME(AFPSPlayerState, Deaths);
}
