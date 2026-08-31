#pragma once

#include "CoreMinimal.h"
#include "GISCoordinateTypes.generated.h"

UENUM(BlueprintType)
enum class EGISAssetCategory : uint8
{
	Building,
	Road,
	Water,
	GreenLand,
	Landmark,
	Utility,
	Other
};

USTRUCT(BlueprintType)
struct UE5GISScene_API FGISGeoCoordinate
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GIS")
	double Longitude = 50.5860;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GIS")
	double Latitude = 26.2285;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GIS")
	double ElevationMeters = 0.0;
};

USTRUCT(BlueprintType)
struct UE5GISScene_API FGISPointOfInterest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GIS")
	FName Id = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GIS")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GIS")
	EGISAssetCategory Category = EGISAssetCategory::Other;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GIS")
	FGISGeoCoordinate GeoCoordinate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GIS")
	FVector WorldLocation = FVector::ZeroVector;
};
