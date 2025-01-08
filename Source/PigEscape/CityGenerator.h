// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CityGenerator.generated.h"

enum class EDirection : uint8
{
	Up,
	Right,
	Left
};
UCLASS()
class PIGESCAPE_API ACityGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACityGenerator();
private:
	UPROPERTY(EditAnywhere, Category = "Grid")
		int gridResolution = 11;
	UPROPERTY(EditAnywhere, Category = "Grid")
		float cellSize = 1000.f;
	UPROPERTY(EditAnywhere, Category = "Grid")
		TSubclassOf<AActor> RoadTileClass;

	TArray<TArray<bool>> Grid; //Array storing roads on the grid

	TMap<FIntPoint, EDirection> Directions = { {{1,0}, EDirection::Up}, {{0,1}, EDirection::Right},  {{0, -1}, EDirection::Left} };


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void InitializeGrid();
	void GenerateRoadNetwork(const int X, const int Y, EDirection PreviousDirection);
	void SpawnRoadTiles();
	void SpawnRoadTile(const FVector& Location, const EDirection Direction);
};
