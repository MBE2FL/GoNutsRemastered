// Fill out your copyright notice in the Description page of Project Settings.


#include "Egg.h"
#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"
//#include "Particles/ParticleEmitter.h"
#include "Particles/ParticleSystemComponent.h"
#include "EggEffect.h"

// Sets default values
AEgg::AEgg()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	_eggMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Test"));
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> meshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> meshAsset(TEXT("StaticMesh'/Game/Birdman/Eggs/Meshes/egg.egg'"));
	_eggMeshComp->SetStaticMesh(meshAsset.Object);
	//static ConstructorHelpers::FObjectFinder<UMaterial> matAsset(TEXT("Material'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> matAsset(TEXT("Material'/Game/Birdman/Eggs/Meshes/Egg_Texture.Egg_Texture'"));
	_eggMeshComp->SetMaterial(0, matAsset.Object);
	_eggMeshComp->SetCollisionProfileName(TEXT("Custom"));
	_eggMeshComp->SetCollisionObjectType(ECC_PhysicsBody);
	_eggMeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	_eggMeshComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	_eggMeshComp->SetCollisionResponseToChannel(ECC_TileChannel, ECR_Overlap);
	_eggMeshComp->SetCollisionResponseToChannel(ECC_PlayerChannel, ECR_Overlap);
	_eggMeshComp->SetupAttachment(nullptr);
	RootComponent = _eggMeshComp;


	_impactParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Impact Particles"));
	_impactParticleComponent->SetAutoActivate(false);
	_impactParticleComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AEgg::BeginPlay()
{
	Super::BeginPlay();

	
	_playerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();


	//if (!_eggMeshComp)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("Failed to get Egg sphere component!"));
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Got Egg sphere component!"));
	//	_eggMeshComp->OnComponentBeginOverlap.AddDynamic(this, &AEgg::OnOverlapBegin);
	//	_eggMeshComp->OnComponentEndOverlap.AddDynamic(this, &AEgg::OnOverlapEnd);
	//}

}

void AEgg::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Egg Collided!"));
	_debugColour = FColor::Red;




	_eggMeshComp->SetVisibility(false);

	const FVector pos = GetActorLocation();
	const FRotator rot = GetActorRotation();
	AEggEffect* eggEffect = Cast<AEggEffect>(GetWorld()->SpawnActor(_eggEffectType.Get(), &pos, &rot));

	_impactParticleComponent->ActivateSystem();
	onEggImpact(_eggType, _eggEffectType, eggEffect);
}

void AEgg::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Egg Stopped Colliding!"));
	_debugColour = FColor::Purple;
}

// Called every frame
void AEgg::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//DrawDebugSphere(GetWorld(), GetActorLocation(), 80.0f, 20, _debugColour, false, -1.0f, 0, 1.0f);


	// Check if this egg has moved far enough behind the player to be destroyed.
	if ((GetActorLocation().X + 500.0f) < _playerPawn->GetActorLocation().X)
	{
		Destroy();
	}
}

//void AEgg::eggEffect_Implementation()
//{
//}
