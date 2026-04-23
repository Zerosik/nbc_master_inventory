// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemInterface.h"
#include "ItemData.h"
#include "BaseItem.generated.h"

class USphereComponent;
class UWidgetComponent;
struct FItemSlot;
UCLASS()
class MASTER_INVENTORY_API ABaseItem : public AActor, public IItemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseItem();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	USphereComponent* Collision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	UStaticMeshComponent* StaticMesh;
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Component")
	//UWidgetComponent* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FDataTableRowHandle ItemRowHandle;



	FItemSlotData itemInfo{};

	void SetItem(FItemSlotData item);
	void SetItem(int32 itemID, int32 Amount);


protected:
	void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);


	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
private:
	float RunningTime;

};
