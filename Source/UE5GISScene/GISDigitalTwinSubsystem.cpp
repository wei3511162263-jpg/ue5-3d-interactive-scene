#include "GISDigitalTwinSubsystem.h"
#include "GISCityAssetAnchor.h"
#include "GISPointMarker.h"

void UGISDigitalTwinSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	OriginCoordinate.Longitude = 50.5860;
	OriginCoordinate.Latitude = 26.2285;
	OriginCoordinate.ElevationMeters = 0.0;
}

void UGISDigitalTwinSubsystem::Deinitialize()
{
	RegisteredAssets.Reset();
	RegisteredMarkers.Reset();
	Super::Deinitialize();
}

FVector UGISDigitalTwinSubsystem::GeoToWorld(const FGISGeoCoordinate& Coordinate) const
{
	constexpr double MetersPerDegreeLatitude = 111320.0;
	const double LatitudeRadians = FMath::DegreesToRadians(OriginCoordinate.Latitude);
	const double MetersPerDegreeLongitude = MetersPerDegreeLatitude * FMath::Cos(LatitudeRadians);

	const double EastMeters = (Coordinate.Longitude - OriginCoordinate.Longitude) * MetersPerDegreeLongitude;
	const double NorthMeters = (Coordinate.Latitude - OriginCoordinate.Latitude) * MetersPerDegreeLatitude;
	const double UpMeters = Coordinate.ElevationMeters - OriginCoordinate.ElevationMeters;

	return FVector(EastMeters * UnrealUnitsPerMeter, NorthMeters * UnrealUnitsPerMeter, UpMeters * UnrealUnitsPerMeter);
}

FGISGeoCoordinate UGISDigitalTwinSubsystem::WorldToGeo(const FVector& WorldLocation) const
{
	constexpr double MetersPerDegreeLatitude = 111320.0;
	const double LatitudeRadians = FMath::DegreesToRadians(OriginCoordinate.Latitude);
	const double MetersPerDegreeLongitude = MetersPerDegreeLatitude * FMath::Cos(LatitudeRadians);

	FGISGeoCoordinate Result;
	Result.Longitude = OriginCoordinate.Longitude + (WorldLocation.X / UnrealUnitsPerMeter) / MetersPerDegreeLongitude;
	Result.Latitude = OriginCoordinate.Latitude + (WorldLocation.Y / UnrealUnitsPerMeter) / MetersPerDegreeLatitude;
	Result.ElevationMeters = OriginCoordinate.ElevationMeters + WorldLocation.Z / UnrealUnitsPerMeter;
	return Result;
}

void UGISDigitalTwinSubsystem::RegisterCityAsset(AGISCityAssetAnchor* AssetAnchor)
{
	if (AssetAnchor)
	{
		RegisteredAssets.AddUnique(AssetAnchor);
	}
}

void UGISDigitalTwinSubsystem::UnregisterCityAsset(AGISCityAssetAnchor* AssetAnchor)
{
	RegisteredAssets.Remove(AssetAnchor);
}

void UGISDigitalTwinSubsystem::RegisterPointMarker(AGISPointMarker* Marker)
{
	if (Marker)
	{
		RegisteredMarkers.AddUnique(Marker);
	}
}

void UGISDigitalTwinSubsystem::UnregisterPointMarker(AGISPointMarker* Marker)
{
	RegisteredMarkers.Remove(Marker);
}

int32 UGISDigitalTwinSubsystem::GetRegisteredAssetCount() const
{
	return RegisteredAssets.Num();
}

int32 UGISDigitalTwinSubsystem::GetRegisteredMarkerCount() const
{
	return RegisteredMarkers.Num();
}

TArray<AGISPointMarker*> UGISDigitalTwinSubsystem::QueryMarkersByCategory(EGISAssetCategory Category) const
{
	TArray<AGISPointMarker*> Results;
	for (AGISPointMarker* Marker : RegisteredMarkers)
	{
		if (Marker && Marker->Category == Category)
		{
			Results.Add(Marker);
		}
	}
	return Results;
}
