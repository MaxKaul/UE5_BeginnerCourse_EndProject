#include "TrapBase.h"

#include "Components/BoxComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "UE5_BeginnerCourse/UE5_BeginnerCourseCharacter.h"

ATrapBase::ATrapBase()
{
	PrimaryActorTick.bCanEverTick = true;

	knockbackForce = 500.f;
	damage = 10.f;

	objectMesh = CreateDefaultSubobject<UStaticMeshComponent>("ObjectMesh");
	RootComponent = objectMesh;

	hitBox = CreateDefaultSubobject<UBoxComponent>("HitBox");
	hitBox->SetupAttachment(GetRootComponent());

	rotator = CreateDefaultSubobject<URotatingMovementComponent>("Rotator");
}

void ATrapBase::BeginPlay()
{
	Super::BeginPlay();

	hitBox->OnComponentBeginOverlap.AddDynamic(this, &ATrapBase::PlayerHit_OnOverlapBegin);
	hitBox->OnComponentEndOverlap.AddDynamic(this, &ATrapBase::PlayerHit_OnOverlapEnd);
}

void ATrapBase::PlayerHit_OnOverlapBegin(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult)
{
	if (!_otherComp->ComponentHasTag("Interactor"))
	{
		if (AUE5_BeginnerCourseCharacter* player = Cast<AUE5_BeginnerCourseCharacter>(_otherActor))
			MakeDamage(player);
	}
}

void ATrapBase::PlayerHit_OnOverlapEnd(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex)
{
}

void ATrapBase::MakeDamage(AUE5_BeginnerCourseCharacter* _player)
{
	_player->TakeDamage(damage);
	ApplyKockback(_player);
}

void ATrapBase::ApplyKockback(AUE5_BeginnerCourseCharacter* _player)
{
	if(_player->PlayerDied())
		_player->GetMesh()->AddImpulse(-_player->GetActorForwardVector() * knockbackForce * 50 + GetActorForwardVector() * knockbackForce * 50);
	else
		_player->LaunchCharacter(-_player->GetActorForwardVector() * knockbackForce + GetActorForwardVector() * knockbackForce, true, true);
}

void ATrapBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

