#include "ROSCommunication/Publisher/RNavOdometryPublisher.h"
#include "Conversions.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Quaternion.h"
#include "nav_msgs/Odometry.h"
#include "Controller/ControllerType/BaseController/RBaseController.h"

URNavOdometryPublisher::URNavOdometryPublisher()
{
	Topic = TEXT("/base_odometry/odom");
	MessageType = TEXT("nav_msgs/Odometry");
	OdomFrameId = TEXT("odom_combined");
	BaseFrameId = TEXT("base_footprint");
}

void URNavOdometryPublisher::SetPublishParameters(URPublisherParameter*& PublisherParameters)
{
	if (URNavOdometryPublisherParameter* NavOdometryPublisherParameters = Cast<URNavOdometryPublisherParameter>(
		PublisherParameters))
	{
		Super::SetPublishParameters(PublisherParameters);
		OdomFrameId = NavOdometryPublisherParameters->OdomFrameId;
		BaseFrameId = NavOdometryPublisherParameters->BaseFrameId;
	}
}

void URNavOdometryPublisher::Publish()
{
	if (GetOwner())
	{
		static int Seq = 0;
		FVector BasePose = GetOwner()->GetActorLocation();
		FQuat BaseQuat = GetOwner()->GetActorQuat();

		TArray<double> Covariance;
		Covariance.SetNumZeroed(36);
		FConversions::UToROS(BasePose);
		FConversions::UToROS(BaseQuat);

		geometry_msgs::PoseWithCovariance Pose(
			geometry_msgs::Pose(
				geometry_msgs::Point(BasePose),
				geometry_msgs::Quaternion(BaseQuat)),
			Covariance);

		TSharedPtr<nav_msgs::Odometry> Msg =
			MakeShareable(new nav_msgs::Odometry());
		
		Msg->SetHeader(std_msgs::Header(Seq, FROSTime(), OdomFrameId));
		Msg->SetString(BaseFrameId);
		Msg->SetPose(Pose);
		
		URBaseController* Controller = Cast<URBaseController>(GetOwner()->GetController(TEXT("BaseController")));
		if (Controller)
		{
			TArray<double> CovarianceTwist;
			CovarianceTwist.SetNumZeroed(36);
			geometry_msgs::TwistWithCovariance Twist(
				geometry_msgs::Twist(
					geometry_msgs::Vector3(Controller->GetLinearVel()),
					geometry_msgs::Vector3(Controller->GetAngularVel())),
				CovarianceTwist);
			Msg->SetTwist(Twist);
		}


		Handler->PublishMsg(Topic, Msg);
		Handler->Process();

		Seq++;
	}
}
