#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GISMeasurementTool.generated.h"

UCLASS()
class UE5GISScene_API AGISMeasurementTool : public AActor
{
	GENERATED_BODY()

public:
	AGISMeasurementTool();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GIS|Measure")
	float UnrealUnitsPerMeter;

	UPROPERTY(BlueprintReadOnly, Category="GIS|Measure")
	FVector StartLocation;

	UPROPERTY(BlueprintReadOnly, Category="GIS|Measure")
	FVector EndLocation;

	UPROPERTY(BlueprintReadOnly, Category="GIS|Measure")
	bool bHasMeasurement;

	UFUNCTION(BlueprintCallable, Category="GIS|Measure")
	void BeginMeasurement(const FVector& InStartLocation);

	UFUNCTION(BlueprintCallable, Category="GIS|Measure")
	float FinishMeasurement(const FVector& InEndLocation);

	UFUNCTION(BlueprintPure, Category="GIS|Measure")
	float GetDistanceMeters() const;

	UFUNCTION(BlueprintPure, Category="GIS|Measure")
	float GetHorizontalDistanceMeters() const;
};
