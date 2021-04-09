#include "ROSCommunication/Publisher/ROdomPublisher.h"
#include "Conversions.h"
#include "sensor_msgs/JointState.h"

void UROdomPublisher::Init()
{
  if (!PublisherParameters)
  {
    PublisherParameters = CreateDefaultSubobject<UROdomPublisherParameter>(TEXT("OdomPublisherParameters"));
  }
  
  const UROdomPublisherParameter *OdomPublisherParameters = GetOdomPublisherParameters();
  if (OdomPublisherParameters)
  {
    OdomPosition.Init(0., OdomPublisherParameters->FrameNames.Num());
    OdomVelocity.Init(0., OdomPublisherParameters->FrameNames.Num());
  }
}

void UROdomPublisher::Publish()
{
  static int Seq = 0;
  const UROdomPublisherParameter *OdomPublisherParameters = GetOdomPublisherParameters();
  if (OdomPublisherParameters)
  {
    static TSharedPtr<sensor_msgs::JointState> JointStateMsg =
        MakeShareable(new sensor_msgs::JointState());

    JointStateMsg->SetHeader(std_msgs::Header(Seq++, FROSTime(), OdomPublisherParameters->FrameId));
    JointStateMsg->SetName(OdomPublisherParameters->FrameNames);
    CalculateOdomStates();
    JointStateMsg->SetPosition(OdomPosition);
    JointStateMsg->SetVelocity(OdomVelocity);

    Handler->PublishMsg(OdomPublisherParameters->Topic, JointStateMsg);

    Handler->Process();
  }
}

void UROdomPublisher::CalculateOdomStates()
{
  if (GetOwner() && OdomPosition.Num() == 3 && OdomVelocity.Num() == 3)
  {
    FTransform BasePose = GetOwner()->GetBaseLink()->GetCollisionMeshes()[0]->GetComponentTransform();
    FVector BasePosition = FConversions::UToROS(BasePose.GetLocation());
    FQuat BaseQuaternion = FConversions::UToROS(BasePose.GetRotation());
    OdomPosition[0] = BasePosition.X;
    OdomPosition[1] = BasePosition.Y;
    OdomPosition[2] = FMath::DegreesToRadians(BaseQuaternion.Rotator().Yaw);

    FVector BaseLinearVelocity = FConversions::UToROS(GetOwner()->GetBaseLink()->GetCollisionMeshes()[0]->GetComponentVelocity());
    FVector BaseAngularVelocity = GetOwner()->GetBaseLink()->GetCollisionMeshes()[0]->GetPhysicsAngularVelocityInDegrees();
    OdomVelocity[0] = BaseLinearVelocity.X;
    OdomVelocity[1] = BaseLinearVelocity.Y;
    OdomVelocity[2] = -FMath::DegreesToRadians(BaseAngularVelocity.Z);
  }
}