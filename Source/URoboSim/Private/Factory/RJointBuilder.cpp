#include "Factory/RJointBuilder.h"
#include "Conversions.h"
#include "Physics/RLink.h"

DEFINE_LOG_CATEGORY_STATIC(LogRJointBuilder, Log, All);

/* Begin helper functions */
static const float CalculateRotationOffset(float &JointLimit, USDFJointAxis *&SDFJointAxis)
{
  if (JointLimit > 2 * PI)
  {
    JointLimit = PI;
    return 0;
  }
  else
  {
    return 0.5 * (SDFJointAxis->Upper + SDFJointAxis->Lower);
  }
}

static void SetupPrismaticConstraint(UPhysicsConstraintComponent *&Constraint, USDFJointAxis *&SDFJointAxis)
{
  float JointLimit = 0;
  ELinearConstraintMotion LinearConstraintMotion = ELinearConstraintMotion::LCM_Free;
  float Upper = FConversions::MToCm((float)SDFJointAxis->Upper);
  float Lower = FConversions::MToCm((float)SDFJointAxis->Lower);

  if (FMath::Abs(Lower) < 10000 && FMath::Abs(Upper) < 10000)
  {
    JointLimit = FMath::Abs(Upper) > FMath::Abs(Lower) ? FMath::Abs(Upper) : FMath::Abs(Lower);
    LinearConstraintMotion = ELinearConstraintMotion::LCM_Limited;
  }
  if (SDFJointAxis->Xyz.Equals(FVector::ForwardVector))
  {
    Constraint->ConstraintInstance.SetLinearXLimit(LinearConstraintMotion, JointLimit);
  }
  else if (SDFJointAxis->Xyz.Equals(FVector::RightVector))
  {
    Constraint->ConstraintInstance.SetLinearYLimit(LinearConstraintMotion, JointLimit);
  }
  else
  {
    Constraint->ConstraintInstance.SetLinearZLimit(LinearConstraintMotion, JointLimit);
  }
}

static void SetupRevoluteConstraint(UPhysicsConstraintComponent *&Constraint, USDFJointAxis *&SDFJointAxis)
{
  float JointLimit = 0.5 * FMath::Abs(SDFJointAxis->Upper - SDFJointAxis->Lower);
  EAngularConstraintMotion AngularConstraintMotion = EAngularConstraintMotion::ACM_Limited;

  //Because the limit is symetrical the Rotation center has to be offseted so that upper and lower limit corespond to the sdf values
  float RotationOffset = CalculateRotationOffset(JointLimit, SDFJointAxis);

  if (SDFJointAxis->Xyz.Equals(FVector::ForwardVector))
  {
    Constraint->ConstraintInstance.SetAngularTwistLimit(AngularConstraintMotion, FMath::RadiansToDegrees(JointLimit));
    Constraint->ConstraintInstance.AngularRotationOffset.Roll = FMath::RadiansToDegrees(RotationOffset);
  }
  else if (SDFJointAxis->Xyz.Equals(FVector::RightVector))
  {
    Constraint->ConstraintInstance.SetAngularSwing2Limit(AngularConstraintMotion, FMath::RadiansToDegrees(JointLimit));
    Constraint->ConstraintInstance.AngularRotationOffset.Pitch = FMath::RadiansToDegrees(RotationOffset);
  }
  else
  {
    Constraint->ConstraintInstance.SetAngularSwing1Limit(AngularConstraintMotion, FMath::RadiansToDegrees(JointLimit));
    Constraint->ConstraintInstance.AngularRotationOffset.Yaw = FMath::RadiansToDegrees(RotationOffset);
  }
}

static void SetupScrewConstraint(UPhysicsConstraintComponent *&Constraint, USDFJointAxis *&SDFJointAxis)
{
  SetupRevoluteConstraint(Constraint, SDFJointAxis);
}

static void SetupConstraint(UPhysicsConstraintComponent *&Constraint, USDFJoint *&SDFJoint)
{
  if (SDFJoint->Type.Equals("revolute"))
  {
    SetupRevoluteConstraint(Constraint, SDFJoint->Axis);
  }
  else if (SDFJoint->Type.Equals("prismatic"))
  {
    SetupPrismaticConstraint(Constraint, SDFJoint->Axis);
  }
  else if (SDFJoint->Type.Equals("screw"))
  {
    SetupScrewConstraint(Constraint, SDFJoint->Axis);
  }
  else
  {
    UE_LOG(LogRJointBuilder, Error, TEXT("JointType %s not supported"), *SDFJoint->Type)
  }
}
/* End helper functions */

bool URJointBuilder::LoadSDF(USDFJoint *&SDFJoint)
{
  if (!SDFJoint)
  {
    return false;
  }
  else
  {
    SetNewJoint(SDFJoint);
    return true;
  }
}

void URJointBuilder::SetNewJoint(USDFJoint *&SDFJoint)
{
  Joint = NewObject<URJoint>(Model, *SDFJoint->GetName());
  SetJointParameters(SDFJoint);
  URLink *Parent = *Model->GetLinks().FindByPredicate([&](URLink *Link) { return Link->GetName().Equals(SDFJoint->Parent); });
  URLink *Child = *Model->GetLinks().FindByPredicate([&](URLink *Link) { return Link->GetName().Equals(SDFJoint->Child); });
  if (!Parent)
  {
    UE_LOG(LogRJointBuilder, Error, TEXT("ParentLink %s of Joint %s not found"), *Joint->GetParent()->GetName(), *Joint->GetName())
  }
  else if (!Child)
  {
    UE_LOG(LogRJointBuilder, Error, TEXT("ChildLink %s of Joint %s not found"), *Joint->GetChild()->GetName(), *Joint->GetName())
  }
  else
  {
    Child->AttachToComponent(Parent->GetCollisionMeshes()[0]);
    Joint->SetParentChild(Parent, Child);
  }
  CreateConstraint(SDFJoint);
  Model->AddJoint(Joint);
}

void URJointBuilder::CreateConstraint(USDFJoint *&SDFJoint)
{
  UPhysicsConstraintComponent *Constraint;
  Constraint = NewObject<UPhysicsConstraintComponent>(Joint, *(SDFJoint->GetName() + TEXT("_constraint")));
  Constraint->ConstraintInstance.SetDisableCollision(true);
  Constraint->ConstraintInstance.SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.f);
  Constraint->ConstraintInstance.SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.f);
  Constraint->ConstraintInstance.SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0.f);
  Constraint->ConstraintInstance.SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
  Constraint->ConstraintInstance.SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
  Constraint->ConstraintInstance.SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0.f);
  Constraint->ConstraintInstance.AngularRotationOffset = FRotator(0.f);
  Constraint->ConstraintInstance.ProfileInstance.TwistLimit.bSoftConstraint = false;
  Constraint->ConstraintInstance.ProfileInstance.ConeLimit.bSoftConstraint = false;
  
  SetupConstraint(Constraint, SDFJoint);
  ConnectToComponents(Constraint, SDFJoint);
  Joint->SetConstraint(Constraint);
}

void URJointBuilder::SetJointParameters(USDFJoint *&SDFJoint)
{
  Joint->SetJointType(SDFJoint);
}

void URJointBuilder::RotateConstraintToRefAxis(UPhysicsConstraintComponent *&Constraint, USDFJointAxis *&SDFJointAxis)
{
  if (Constraint)
  {
    if (SDFJointAxis->Xyz.Equals(FVector::ForwardVector) || SDFJointAxis->Xyz.Equals(FVector::RightVector) || SDFJointAxis->Xyz.Equals(FVector::UpVector))
    {
      Joint->SetJointAxis(SDFJointAxis->Xyz);
    }
    else
    {
      Joint->SetJointAxis(FVector::UpVector);
      FQuat BetweenQuat = FQuat::FindBetweenVectors(FVector::UpVector, SDFJointAxis->Xyz);
      Constraint->SetRelativeRotationExact(Constraint->GetRelativeRotationFromWorld(BetweenQuat).Rotator());
    }
  }
  else
  {
    UE_LOG(LogRJointBuilder, Error, TEXT("Joint %s doesn't have constraint, RotateConstraintToRefAxis failed"), *Joint->GetName());
  }
}

void URJointBuilder::ConnectToComponents(UPhysicsConstraintComponent *&Constraint, USDFJoint *&SDFJoint)
{
  if (Constraint)
  {
    Constraint->RegisterComponent();
    Constraint->AttachToComponent(Joint->GetParent()->GetCollisionMeshes()[0], FAttachmentTransformRules::KeepWorldTransform);
    if (SDFJoint->Axis->bUseParentModelFrame)
    {
      Constraint->SetWorldLocation(Joint->GetChild()->GetPose().GetLocation());
      Constraint->AddLocalOffset(SDFJoint->Pose.GetLocation());
    }
    else
    {
      Constraint->SetWorldLocation(SDFJoint->Pose.GetLocation());
    }
    RotateConstraintToRefAxis(Constraint, SDFJoint->Axis);
    Constraint->ConstraintActor1 = Joint->GetParent()->GetCollisionMeshes()[0]->GetOwner();
    Constraint->ConstraintActor2 = Joint->GetChild()->GetCollisionMeshes()[0]->GetOwner();
    Constraint->SetConstrainedComponents(Cast<UPrimitiveComponent>(Joint->GetParent()->GetCollisionMeshes()[0]), NAME_None, Cast<UPrimitiveComponent>(Joint->GetChild()->GetCollisionMeshes()[0]), NAME_None);
  }
  else
  {
    UE_LOG(LogRJointBuilder, Error, TEXT("Joint %s doesn't have constraint, ConnectToComponents failed"), *Joint->GetName())
  }
}