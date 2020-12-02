#pragma once
#include "UObject/Object.h"
#include "SpawnableObjectBase.h"
#include "ObjectSpawnerConfig.generated.h"

UCLASS(config = Game, defaultconfig)
class OBJECTSPAWNER_API UObjectSpawnerConfig : public UObject
{
	GENERATED_BODY()

public:
	UObjectSpawnerConfig(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer) {};

	UPROPERTY(config, EditAnywhere, Category=Settings)
	TArray<TSoftClassPtr<ASpawnableObjectBase>> ObjectsToSpawn;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0.f, ClampMax = 1.f, UIMin = 0.f, UIMax = 1.f))
	float SpawnersPerRoom;
};