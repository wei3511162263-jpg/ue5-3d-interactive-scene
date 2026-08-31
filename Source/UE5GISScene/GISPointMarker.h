#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GISCoordinateTypes.h"
#include "GISPointMarker.generated.h"

class USphereComponent;
class UTextRenderComponent;

UCLASS()
class UE5GISScene_API AGISPointMarker : public AActor
{
	GENERATED_BODY()

public:
	AGISPointMarker();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<USphereComponent> Collision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UTextRenderComponent> Label;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GIS|POI")
	FName MarkerId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GIS|POI")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GIS|POI")
	EGISAssetCategory Category;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GIS|POI")
	FGISGeoCoordinate GeoCoordinate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GIS|POI")
	FLinearColor MarkerColor;

	UFUNCTION(BlueprintCallable, Category="GIS|POI")
	void ApplyGeoReference();

	UFUNCTION(BlueprintCallable, Category="GIS|POI")
	void UpdateVisualState(bool bSelected);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
