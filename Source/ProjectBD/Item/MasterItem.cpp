// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterItem.h"
#include "ItemComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StreamableManager.h"
#include "ItemDataTable.h"
#include "Net/UnrealNetwork.h"
#include "../Basic/BasicPlayer.h"

// Sets default values
AMasterItem::AMasterItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemComponent = CreateDefaultSubobject<UItemComponent>(TEXT("Item"));

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = Sphere;
	Sphere->SetSphereRadius(150.0f);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);

	//��Ʈ��ũ�� ����ȭ�ϱ�
	bReplicates = true;
}

// Called when the game starts or when spawned
void AMasterItem::BeginPlay()
{
	Super::BeginPlay();
	
	if (ItemComponent->ItemDataTable)
	{
		//ȣ��Ʈ������ ����ǵ��� �Ѵ�.
		//Ŭ���̾�Ʈ�� �������� �������� �ʰ� ������ ������ �������� ����ȭ�ϵ��� �Ѵ�.
		if (HasAuthority())
		{
			//������ ���� ����
			ItemIndex = FMath::RandRange(1, 6) * 10;
		}
		//���õ� ������ ������ Ŭ���̾�Ʈ �Ѵ� ����
		ItemData = ItemComponent->GetItemData(ItemIndex);
		if (ItemIndex != 0)
		{
			//
			FStreamableManager Loader;
			StaticMesh->SetStaticMesh(Loader.LoadSynchronous<UStaticMesh>(ItemData.ItemMesh));
		}
	}

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AMasterItem::ProcessBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AMasterItem::ProcessEndOverlap);
}

// Called every frame
void AMasterItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMasterItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMasterItem, ItemIndex);
}

void AMasterItem::ProcessBeginOverlap(UPrimitiveComponent * OverlappedComponent, 
	AActor * OtherActor,
	UPrimitiveComponent * OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Player")))
	{
		ABasicPlayer* Pawn = Cast<ABasicPlayer>(OtherActor);

		//�ݸ����� ���ƴٴ� �� �ٸ� ������ Ŭ���̾�Ʈ�鵵 ���ƴٴ� ���� �ȴ�.
		//UI�� ��µ��� ������ �Ʒ� �ڵ带 ȣ���� �ϰ� �ȴ�.
		//IsLocallyControlled()�� �������� �˻�����
		if (Pawn && Pawn->IsLocallyControlled())
		{
			//UE_LOG(LogClass, Warning, TEXT("ProcessBeginOverlap"));
			//�ڱ��ڽ��� �߰�
			Pawn->AddPickItem(this);
		}
	}
}

void AMasterItem::ProcessEndOverlap(UPrimitiveComponent * OverlappedComponent,
	AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag(TEXT("Player")))
	{
		ABasicPlayer* Pawn = Cast<ABasicPlayer>(OtherActor);
		if (Pawn && Pawn->IsLocallyControlled())
		{
			//UE_LOG(LogClass, Warning, TEXT("ProcessEndOverlap"));
			//�ڱ��ڽ��� ����
			Pawn->RemovePickItem(this);
		}
	}
}
