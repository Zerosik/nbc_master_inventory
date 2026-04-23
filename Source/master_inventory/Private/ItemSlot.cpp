// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSlot.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Styling/SlateBrush.h"

void UItemSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	//QuantityText = Cast<UTextBlock>(GetWidgetFromName(TEXT("QuantityText")));
	//ItemIcon = Cast<UImage>(GetWidgetFromName(TEXT("ItemImage")));
}

void UItemSlot::SetItem(FItemData data, int32 Quantity)
{
	QuantityText->SetText(FText::AsNumber(Quantity));
	FSlateBrush Brush;
	Brush.SetResourceObject(data.Icon);
	ItemImage->SetBrush(Brush);
}

void UItemSlot::SetEmpty()
{
	QuantityText->SetText(FText::GetEmpty());
	ItemImage->SetBrush(FSlateBrush());
}

bool UItemSlot::bIsEmpty()
{
	return false;
}
