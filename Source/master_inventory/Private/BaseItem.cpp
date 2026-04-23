// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItem.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include <MyCharacter.h>

// Sets default values
ABaseItem::ABaseItem()
{
    PrimaryActorTick.bCanEverTick = true;
    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    SetRootComponent(Scene);

    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    Collision->SetupAttachment(Scene);
    Collision->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnItemOverlap);

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMesh->SetupAttachment(Collision);
    StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));

    //ItemIcon = CreateDefaultSubobject<UWidgetComponent>(TEXT("ItemIcon"));
    //ItemIcon->SetupAttachment(Collision);
    //ItemIcon->SetWidgetSpace(EWidgetSpace::Screen);
    //ItemIcon->SetVisibility(false);



}

void ABaseItem::OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->ActorHasTag("Player")) {
        AMyCharacter* character = Cast<AMyCharacter>(OtherActor);
        if (character) {
            character->AddItemToInventory(itemInfo.ItemID);
            Destroy();
        }
    }
}

// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();

    if (ItemRowHandle.DataTable) {
        FItemData* row = ItemRowHandle.GetRow<FItemData>(TEXT("Item"));
        if (row){
            itemInfo.Amount = 1;
            itemInfo.ItemID = row->ItemID;

        }
    }
}

// Called every frame
void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    RunningTime += DeltaTime;
    float sine = FMath::Sin(RunningTime*2);
    FVector newLocation{ 0, 0, GetActorLocation().Z+(sine*50) };
    StaticMesh->SetRelativeLocation(newLocation);

    FRotator newRotate = { 0, StaticMesh->GetRelativeRotation().Yaw+(DeltaTime*90), 0};
    StaticMesh->SetRelativeRotation(newRotate);
    //사인파 그리면서 위아래로 움직이고,
    //rotate계속 회전하도록
}

