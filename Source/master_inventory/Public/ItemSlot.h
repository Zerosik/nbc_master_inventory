// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemData.h"
#include "ItemSlot.generated.h"

class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class MASTER_INVENTORY_API UItemSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintCallable)
	void SetItem(FItemData data, int32 Quantity);

	UFUNCTION(BlueprintCallable)
	void SetEmpty();

	UFUNCTION(BlueprintCallable)
	bool bIsEmpty();

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* QuantityText;
	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage;

};
