// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UInventoryComponent;
class UUniformGridPanel;
class UItemSlot;

UCLASS()
class MASTER_INVENTORY_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    UUniformGridPanel* SlotGrid;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
    TSubclassOf<UItemSlot> SlotWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
    int32 ColumnCount = 4;

    UPROPERTY()
    UInventoryComponent* InventoryComponent = nullptr;

    UPROPERTY()
    TArray<UItemSlot*> SlotWidgets;

private:
    void CreateSlotWidgets();

    UFUNCTION()
    void RefreshInventory();

public:
    UFUNCTION(BlueprintCallable)
    void InitializeInventoryWidget(UInventoryComponent* InInventoryComponent);
};