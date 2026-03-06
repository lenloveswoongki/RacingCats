#pragma once
#include "RaceData.h"
#include "GameFramework/GameMode.h"
#include "RaceGameMode.generated.h"

UCLASS()
class ARaceGameMode : public AGameMode
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(BlueprintReadOnly, Category="GameMode")
	TArray<APawn*> PlayerPawns;

	UPROPERTY(BlueprintReadOnly, Category="GameMode")
	TArray<FRaceData> RacesData;

	UPROPERTY(BlueprintReadOnly, Category="GameMode")
	FRacerData CurrentRaceData;

	UPROPERTY(BlueprintReadOnly, Category="GameMode")
	TArray<FVector> SpawnPoints;
	
public:

	UFUNCTION(BlueprintCallable, Category="GameMode")
	inline void AddPlayerPawn(APawn* Pawn) { PlayerPawns.Add(Pawn); }

	
};
