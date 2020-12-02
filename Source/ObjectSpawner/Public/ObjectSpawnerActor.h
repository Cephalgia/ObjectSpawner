#pragma once
#include "GameFramework/Actor.h"
#include "SpawnableObjectBase.h"
#include "Components/BoxComponent.h"
#include "ObjectSpawnerActor.generated.h"

UCLASS(config = Game, defaultconfig)
class OBJECTSPAWNER_API AObjectSpawnerActor : public AActor
{
	GENERATED_BODY()

public:
	AObjectSpawnerActor(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	bool CanSpawnObject(TSubclassOf<ASpawnableObjectBase> ObjectToSpawn) const;

	ASpawnableObjectBase* SpawnObject(TSubclassOf<ASpawnableObjectBase> ObjectToSpawn);

	UPROPERTY()
	ASpawnableObjectBase * SpawnedObject = nullptr;

	UPROPERTY(EditAnywhere, Category=Settings)
	UBoxComponent * SpawnVolume = nullptr;
};
