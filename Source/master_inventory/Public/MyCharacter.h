// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UInventoryComponent;
class UItemDatabaseComponent;
class UInventoryWidget;
class UAchievementComponent;
struct ItemSturct;
struct FInputActionValue;

UCLASS()
class MASTER_INVENTORY_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UItemDatabaseComponent* ItemDatabaseComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UInventoryComponent* InventoryComp;
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;
	UPROPERTY()
	UInventoryWidget* InventoryWidget = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAchievementComponent* AchievementComponent;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* DefaultInputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* OpenInventoryAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* UseAllItemAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* FireAction;

	bool bIsInventoryOpen = false;

	bool AddItemToInventory(int32 ItemID, int32 AMount = 1);
	bool RemoveItemFromInventory(int32 ItemID);

	float ShotgunMinSpread = 5.f;
	float ShotgunCurrentSpread = ShotgunMinSpread;
	float recoilRecoverySpeed = 25.f;
	float ShotgunMaxSpread = 30.f;
	float ShotgunRange = 2000.f;
	int32 SHotgunPelletCount = 8;



protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void OpenInventory(const FInputActionValue& value);
	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void UseAllItem(const FInputActionValue& value);
	UFUNCTION()
	void FireShotgun(const FInputActionValue& value);


	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	FVector GetCameraLookAt();

};
