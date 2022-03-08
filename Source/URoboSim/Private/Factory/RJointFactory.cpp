#include "Factory/RJointFactory.h"

URJoint* URJointFactory::Load(UObject* InOuter, USDFJoint* InJointDescription)
{
  if(!InOuter && !InJointDescription)
    {
      return nullptr;
    }

  JointBuilder = CreateBuilder(InJointDescription);
  if(!JointBuilder)
  {
    return nullptr;
  }

  JointBuilder->Init(InOuter, InJointDescription);
  return JointBuilder->NewJoint();

}

URJointBuilder* URJointFactory::CreateBuilder(USDFJoint* InJointDescription)
{
  if(InJointDescription->Type.Equals("revolute"))
    {
      if((InJointDescription->Axis->Upper > 2 * PI) ||
         (InJointDescription->Axis->Lower < -2 * PI))
        {
          return NewObject<URContiniousJointBuilder>(this);
        }
      else
        {
          return NewObject<URRevoluteJointBuilder>(this);
        }
    }
  else if(InJointDescription->Type.Equals("prismatic"))
    {
      return NewObject<URPrismaticJointBuilder>(this);
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("%s Constraint Type not supported."), *InJointDescription->Type);
      return nullptr;
    }
}

void URJointBuilder::Init(UObject* InOuter, USDFJoint* InJointDescription)
{
  Outer = InOuter;
  JointDescription = InJointDescription;
}

URJoint* URJointBuilder::NewJoint()
{
  Joint = NewObject<URJoint>(Outer, FName((JointDescription->Name).GetCharArray().GetData()));
  SetJointParameters();
  CreateConstraint();
  if(Joint->Constraint)
    {
      SetAxis();
      SetJointLimit();
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("Constraint for %s not created"), *JointDescription->Name);
    }
  return Joint;
}
void URJointBuilder::SetJointParameters()
{
  Joint->ParentName = JointDescription->Parent;
  Joint->ChildName = JointDescription->Child;
  Joint->bUseParentModelFrame = JointDescription->Axis->bUseParentModelFrame;
  Joint->Pose = JointDescription->Pose;
}

void URJointBuilder::CreateConstraint()
{
  FString Name = FString(*JointDescription->Name) + TEXT("_constraint");
  Joint->Constraint = NewObject<URFixedConstraintComponent>(Joint, FName(*Name));
}

void URJointBuilder::SetAxis()
{
  Joint->Constraint->RefAxis = JointDescription->Axis->Xyz;
  Joint->Constraint->ConstraintInstance.SetDisableCollision(true);
  Joint->Constraint->ConstraintInstance.SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0);
  Joint->Constraint->ConstraintInstance.SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0);
  Joint->Constraint->ConstraintInstance.SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0);
  Joint->Constraint->ConstraintInstance.SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
  Joint->Constraint->ConstraintInstance.SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
  Joint->Constraint->ConstraintInstance.SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
  Joint->Constraint->ConstraintInstance.AngularRotationOffset = FRotator(0, 0, 0);
  Joint->Constraint->ConstraintInstance.ProfileInstance.TwistLimit.bSoftConstraint = false;
  Joint->Constraint->ConstraintInstance.ProfileInstance.ConeLimit.bSoftConstraint = false;
}

void URJointBuilder::SetJointLimit()
{
  Joint->Constraint->Upper = JointDescription->Axis->Upper;
  Joint->Constraint->Lower = JointDescription->Axis->Lower;
}

void URJointBuilder::RotateConstraintToRefAxis()
{
  FVector CurrentRefAxis;
  FVector RefAxisInJointFrame;
  FQuat Rotation;

  if(JointDescription->Axis->bUseParentModelFrame)
    {
      Rotation = Joint->Constraint->GetComponentQuat();
      RefAxisInJointFrame = Rotation.Inverse().RotateVector(Joint->Constraint->RefAxis);
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("model frame not used"));
      RefAxisInJointFrame = Joint->Constraint->RefAxis;
    }

  RefAxisInJointFrame /= RefAxisInJointFrame.Size();

  if(RefAxisInJointFrame[0] == 1 || RefAxisInJointFrame[1] == 1 || RefAxisInJointFrame[2]==1)
    {
      Joint->Constraint->RefAxis = RefAxisInJointFrame;
    }
  else
    {
      if(JointDescription->Axis->bUseParentModelFrame)
        {
          CurrentRefAxis = Joint->Constraint->GetComponentQuat().GetAxisZ();
          FQuat BetweenQuat = FQuat::FindBetweenVectors(CurrentRefAxis, Joint->Constraint->RefAxis);
          Joint->Constraint->AddLocalRotation(BetweenQuat);
        }
      else
        {
          UE_LOG(LogTemp, Error, TEXT("Usage of JointFrame for axis not implemented"));
        }

      Joint->Constraint->RefAxis = FVector(0.0f, 0.0f, 1.0f);
    }
}

float URJointBuilder::CalculateRotationOffset()
{
  Joint->Constraint->RotationOffset = 0.5 * (JointDescription->Axis->Upper + JointDescription->Axis->Lower);
  if (JointDescription->Axis->Xyz[0] == 1 || JointDescription->Axis->Xyz[0] == -1)
    {
      Joint->Constraint->RotationOffset *= -1;
    }
  return Joint->Constraint->RotationOffset;
}

void URContiniousJointBuilder::SetAxis()
{
  Super::SetAxis();

  RotateConstraintToRefAxis();

  // Joint->Constraint->SetAngularDriveMode(EAngularDriveMode::TwistAndSwing);
  // Joint->Constraint->SetAngularDriveParams(190000.0f, 100.0f, 900000.0f);

  if (Joint->Constraint->RefAxis[0]== 1)
    {
      Joint->Constraint->ConstraintInstance.SetAngularTwistLimit(EAngularConstraintMotion::ACM_Free, 0);
    }
  else if (Joint->Constraint->RefAxis[1]== 1)
    {
      Joint->Constraint->ConstraintInstance.SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 0);
    }
  else if (Joint->Constraint->RefAxis[2]==1)
    {
      Joint->Constraint->ConstraintInstance.SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Free, 0);
    }

}

void URContiniousJointBuilder::CreateConstraint()
{
  FString Name = FString(*JointDescription->Name) + TEXT("_constraint");
  Joint->Constraint = NewObject<URContinuousConstraintComponent>(Joint, FName(*Name));
}

void URRevoluteJointBuilder::SetAxis()
{
  // //TODO rotate joint 180° if  xyz is negative?
  Super::SetAxis();

  Joint->Constraint->Limit  =  0.5*FMath::Abs(JointDescription->Axis->Upper - JointDescription->Axis->Lower) ;
  EAngularConstraintMotion AngularConstraintMotion = EAngularConstraintMotion::ACM_Limited;

  //Because the limit is symetrical the Rotation center has to be offseted so that upper and lower limit corespond to the sdf values
  CalculateRotationOffset();

  // RefAxis = RefAxis;
  if (Joint->Constraint->RefAxis[0] == 1)
    {
      Joint->Constraint->ConstraintInstance.SetAngularTwistLimit(AngularConstraintMotion, FMath::RadiansToDegrees(Joint->Constraint->Limit));
      Joint->Constraint->ConstraintInstance.AngularRotationOffset.Roll =  FMath::RadiansToDegrees(Joint->Constraint->RotationOffset);
    }
  else if (Joint->Constraint->RefAxis[1] == 1)
    {
      Joint->Constraint->ConstraintInstance.SetAngularSwing2Limit(AngularConstraintMotion, FMath::RadiansToDegrees(Joint->Constraint->Limit));
      Joint->Constraint->ConstraintInstance.AngularRotationOffset.Pitch = FMath::RadiansToDegrees(Joint->Constraint->RotationOffset);
    }
  // else if (JointDescription->Axis->Xyz[2] == 1)
  // {
  //     ConstraintInstance.SetAngularSwing1Limit(AngularConstraintMotion, Limit);
  // 	ConstraintInstance.AngularRotationOffset.Yaw =  RotationOffset;
  // }
  else
    {
      // FVector ZAxis = GetComponentQuat().GetAxisZ();
      // FQuat BetweenQuat = FQuat::FindBetweenVectors(RefAxis, ZAxis);
      // AddLocalRotation(BetweenQuat);
      // RefAxis = FVector(0.0f, 0.0f, 1.0f);
      Joint->Constraint->ConstraintInstance.SetAngularSwing1Limit(AngularConstraintMotion, FMath::RadiansToDegrees(Joint->Constraint->Limit));
      Joint->Constraint->ConstraintInstance.AngularRotationOffset.Yaw =  FMath::RadiansToDegrees(Joint->Constraint->RotationOffset);
    }

}

void URRevoluteJointBuilder::CreateConstraint()
{
  FString Name = FString(*JointDescription->Name) + TEXT("_constraint");
  Joint->Constraint = NewObject<URRevoluteConstraintComponent>(Joint, FName(*Name));
}

void URPrismaticJointBuilder::SetAxis()
{
  Super::SetAxis();
  Joint->Constraint->Limit  =  0;
  ELinearConstraintMotion LinearConstraintMotion = ELinearConstraintMotion::LCM_Free;
  float UpperUUnits = JointDescription->Axis->Upper * 100;
  float LowerUUnits = JointDescription->Axis->Lower * 100;

  if(FMath::Abs(UpperUUnits) < 10000 && FMath::Abs(LowerUUnits) < 10000)
    {
      Joint->Constraint->Limit  =  0.5*FMath::Abs(UpperUUnits - LowerUUnits) ;
      LinearConstraintMotion = ELinearConstraintMotion::LCM_Limited;

      Joint->Constraint->Offset = 0.5*(UpperUUnits + LowerUUnits) * JointDescription->Axis->Xyz;
    }

  if (FMath::Abs(JointDescription->Axis->Xyz[0])== 1)
    {
      Joint->Constraint->ConstraintInstance.SetLinearXLimit(LinearConstraintMotion, Joint->Constraint->Limit);
    }

  else if (FMath::Abs(JointDescription->Axis->Xyz[1])== 1)
    {
      Joint->Constraint->ConstraintInstance.SetLinearYLimit(LinearConstraintMotion, Joint->Constraint->Limit);
    }

  else if (FMath::Abs(JointDescription->Axis->Xyz[2])== 1)
    {
      Joint->Constraint->ConstraintInstance.SetLinearZLimit(LinearConstraintMotion, Joint->Constraint->Limit);
    }
}

void URPrismaticJointBuilder::CreateConstraint()
{
  FString Name = FString(*JointDescription->Name) + TEXT("_constraint");
  Joint->Constraint = NewObject<URPrismaticConstraintComponent>(Joint, FName(*Name));
}
