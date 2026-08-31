#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GISCoordinateTypes.h"
#include "GISCityAssetAnchor.generated.h"

class UBillboardComponent;
class USceneComponent;
class UTextRenderComponent;

UCLASS()
class UE5GISScene_API AGISCityAssetAnchor : public AActor
{
	GENERATED_BODY()

public:
	AGISCityAssetAnchor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UBillboardComponent> Icon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UTextRenderComponent> Label;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GIS|Asset")
	FText AssetDisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GIS|Asset")
	FString SourceAssetName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GIS|Asset")
	FString SourceAssetUrl;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GIS|Asset")
	FGISGeoCoordinate AnchorCoordinate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GIS|Asset")
	FVector2D ApproximateCoverageMeters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GIS|Optimization")
	bool bEnableHLOD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GIS|Optimization")
	bool bUseNaniteForStaticMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GIS|Optimization")
	int32 ExpectedBuildingCount;

	UFUNCTION(BlueprintCallable, Category="GIS|Asset")
	void ApplyGeoReference();

	UFUNCTION(BlueprintCallable, Category="GIS|Asset")
	void UpdateLabel();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
