#pragma once
#include "ManagerBase.h"
#include "SpawnableObjectBase.h"
#include "ObjectSpawnerActor.h"
#include "ObjectSpawnerManager.generated.h"

USTRUCT()
struct OBJECTSPAWNER_API FObjectSpawnerInfo
{
	GENERATED_BODY();

	FObjectSpawnerInfo() {}
	FObjectSpawnerInfo(FName InLevelName) {LevelName = InLevelName;}

	FName LevelName;

	UPROPERTY()
	TArray<AObjectSpawnerActor*> ObjectSpawners;
};

USTRUCT()
struct OBJECTSPAWNER_API FObjectToSpawnInfo
{
	GENERATED_BODY();

	FObjectToSpawnInfo() {}
	FObjectToSpawnInfo(TSoftClassPtr<ASpawnableObjectBase> InPointer)
	{
		ObjectToSpawn = InPointer.LoadSynchronous();
	}
	UPROPERTY()
	TSubclassOf<ASpawnableObjectBase> ObjectToSpawn = nullptr;

	UPROPERTY()
	ASpawnableObjectBase* SpawnedObject = nullptr;
};

UCLASS(config = Game, defaultconfig)
class OBJECTSPAWNER_API UObjectSpawnerManager : public UManagerBase
{
	GENERATED_BODY()

public:
	UObjectSpawnerManager(const FObjectInitializer& ObjectInitializer);

	virtual void Initialize() override;

	virtual void OnStartPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	void RegisterSpawner(AObjectSpawnerActor * InSpawner);
	void UnRegisterSpawner(AObjectSpawnerActor * InSpawner);

protected:
	UPROPERTY()
	TArray<FObjectToSpawnInfo> ObjectsToSpawn;
	UPROPERTY()
	TArray<FObjectSpawnerInfo> ObjectSpawners;
};
