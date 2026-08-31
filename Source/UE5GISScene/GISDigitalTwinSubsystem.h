#pragma once

#include "CoreMinimal.h"
#include "GISCoordinateTypes.h"
#include "Subsystems/WorldSubsystem.h"
#include "GISDigitalTwinSubsystem.generated.h"

class AGISCityAssetAnchor;
class AGISPointMarker;

UCLASS()
class UE5GISScene_API UGISDigitalTwinSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GIS|Coordinate")
	FGISGeoCoordinate OriginCoordinate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GIS|Coordinate")
	float UnrealUnitsPerMeter = 100.0f;

	UFUNCTION(BlueprintCallable, Category="GIS|Coordinate")
	FVector GeoToWorld(const FGISGeoCoordinate& Coordinate) const;

	UFUNCTION(BlueprintCallable, Category="GIS|Coordinate")
	FGISGeoCoordinate WorldToGeo(const FVector& WorldLocation) const;

	UFUNCTION(BlueprintCallable, Category="GIS|Scene")
	void RegisterCityAsset(AGISCityAssetAnchor* AssetAnchor);

	UFUNCTION(BlueprintCallable, Category="GIS|Scene")
	void UnregisterCityAsset(AGISCityAssetAnchor* AssetAnchor);

	UFUNCTION(BlueprintCallable, Category="GIS|POI")
	void RegisterPointMarker(AGISPointMarker* Marker);

	UFUNCTION(BlueprintCallable, Category="GIS|POI")
	void UnregisterPointMarker(AGISPointMarker* Marker);

	UFUNCTION(BlueprintPure, Category="GIS|Scene")
	int32 GetRegisteredAssetCount() const;

	UFUNCTION(BlueprintPure, Category="GIS|POI")
	int32 GetRegisteredMarkerCount() const;

	UFUNCTION(BlueprintCallable, Category="GIS|Query")
	TArray<AGISPointMarker*> QueryMarkersByCategory(EGISAssetCategory Category) const;

private:
	UPROPERTY()
	TArray<TObjectPtr<AGISCityAssetAnchor>> RegisteredAssets;

	UPROPERTY()
	TArray<TObjectPtr<AGISPointMarker>> RegisteredMarkers;
};
