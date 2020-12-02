#include "ObjectSpawnerManager.h"

#include "Engine/LevelStreaming.h"

#include "ObjectSpawnerConfig.h"

#include "DreamGameMode.h"

#include "LevelManager.h"

UObjectSpawnerManager::UObjectSpawnerManager(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer) 
{
}

void UObjectSpawnerManager::Initialize()
{
	const UObjectSpawnerConfig * Config = GetDefault<UObjectSpawnerConfig>();
	for (auto ObjectToSpawn : Config->ObjectsToSpawn)
	{
		ObjectsToSpawn.Add(FObjectToSpawnInfo(ObjectToSpawn));
	}
}

void UObjectSpawnerManager::OnStartPlay()
{

}

void UObjectSpawnerManager::Tick(float DeltaSeconds)
{
	if (ADreamGameMode * GameMode = Cast<ADreamGameMode>(GetWorld()->GetAuthGameMode()))
	{
		if (ULevelManager * LevelManager = GameMode->GetManager<ULevelManager>())
		{
			ULevel * CurrentLevel = LevelManager->GetCurrentLevel();
			FName CurrentLevelName = CurrentLevel->GetOuter()->GetFName();
			// put current level spawners first to give them higher priority
			for (FObjectSpawnerInfo& SpawnerInfo : ObjectSpawners)
			{
				if (SpawnerInfo.LevelName == CurrentLevelName)
				{
					FObjectSpawnerInfo FirstInfo = ObjectSpawners[0];
					ObjectSpawners[0] = SpawnerInfo;
					SpawnerInfo = FirstInfo;
					break;
				}
			}

			TArray<AObjectSpawnerActor*> AllSpawners;
			for (auto SpawnerInfo : ObjectSpawners)
			{
				for (auto Spawner : SpawnerInfo.ObjectSpawners)
				{
					AllSpawners.Add(Spawner);
				}
			}

			for (FObjectToSpawnInfo& ObjectToSpawn : ObjectsToSpawn)
			{
				if (ObjectToSpawn.SpawnedObject == nullptr)
				{
					for (auto Spawner : AllSpawners)
					{
						if (Spawner->CanSpawnObject(ObjectToSpawn.ObjectToSpawn))
						{
							if (ASpawnableObjectBase * SpawnedObject = Spawner->SpawnObject(ObjectToSpawn.ObjectToSpawn))
							{
								ObjectToSpawn.SpawnedObject = SpawnedObject;
								Spawner->SpawnedObject = SpawnedObject;
							}
							break;
						}
					}
				}
			}
		}
	}
}

void UObjectSpawnerManager::RegisterSpawner(AObjectSpawnerActor * InSpawner)
{
	if (ULevel * DoorLevel = Cast<ULevel>(InSpawner->GetOuter()))
	{
		FName LevelName = DoorLevel->GetOuter()->GetFName();

		bool bFoundSpawnerInfo = false;
		for (auto SpawnerInfo : ObjectSpawners)
		{
			if (SpawnerInfo.LevelName == LevelName)
			{
				SpawnerInfo.ObjectSpawners.AddUnique(InSpawner);
				bFoundSpawnerInfo = true;
			}
		}

		if (!bFoundSpawnerInfo)
		{
			FObjectSpawnerInfo SpawnerInfo(LevelName);
			SpawnerInfo.ObjectSpawners.Add(InSpawner);
			ObjectSpawners.Add(SpawnerInfo);
		}
	}
}

void UObjectSpawnerManager::UnRegisterSpawner(AObjectSpawnerActor * InSpawner)
{
	if (ULevel * DoorLevel = Cast<ULevel>(InSpawner->GetOuter()))
	{
		FName LevelName = DoorLevel->GetOuter()->GetFName();

		for (FObjectSpawnerInfo& SpawnerInfo : ObjectSpawners)
		{
			if (SpawnerInfo.LevelName == LevelName)
			{
				SpawnerInfo.ObjectSpawners.Remove(InSpawner);
			}
		}

		ObjectSpawners.RemoveAll([](FObjectSpawnerInfo& SpawnerInfo) { return SpawnerInfo.ObjectSpawners.Num() == 0; });
	}
}