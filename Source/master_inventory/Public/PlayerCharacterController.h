// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class MASTER_INVENTORY_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|UI")
	TSubclassOf<UUserWidget> InventoryWidgetClass;

	UUserWidget* InventoryWidget;

	void ShowInventory();

protected:
	void BeginPlay() override;

private:
	bool bIsInventoryOpen = false;
};
