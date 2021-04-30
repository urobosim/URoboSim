#include "ROSCommunication/Publisher/2DLidarPublisher.h"
#include "sensor_msgs/LaserScan.h"

DEFINE_LOG_CATEGORY_STATIC(LogRLidar2DPublisher, Log, All);

UR2DLidarPublisher::UR2DLidarPublisher()
{
  Topic = TEXT("/base_scan");
  MessageType = TEXT("sensor_msgs/LaserScan");
  ROSLinkName = TEXT("base_laser_link");
}

void UR2DLidarPublisher::SetPublishParameters(URPublisherParameter *&PublisherParameters)
{
  if (UR2DLidarPublisherParameter *LidarPublisherParameters = Cast<UR2DLidarPublisherParameter>(PublisherParameters))
  {
    Super::SetPublishParameters(PublisherParameters);
    LidarName = LidarPublisherParameters->LidarName;
    ROSLinkName = LidarPublisherParameters->ROSLinkName;
  }
}

void UR2DLidarPublisher::Init()
{
  Super::Init();
  if (GetOwner())
  {
    TArray<UR2DLidar *> Lidars;
    GetOwner()->GetComponents(Lidars, true);
    for (UR2DLidar *&MyLidar : Lidars)
    {
      if (MyLidar->GetName().Equals(LidarName))
      {
        Lidar = MyLidar;
      }
    }
    if (!Lidar)
    {
      UE_LOG(LogRLidar2DPublisher, Error, TEXT("%s not found in %s"), *LidarName, *GetOwner()->GetName())
    }
  }
}

void UR2DLidarPublisher::Publish()
{
  if (GetOwner() && Lidar && Lidar->bPublishResult)
  {
    static int Seq = 0;
    TSharedPtr<sensor_msgs::LaserScan> ScanData = MakeShareable(new sensor_msgs::LaserScan());
    ScanData->SetHeader(std_msgs::Header(Seq, FROSTime(), ROSLinkName));
    ScanData->SetAngleMin(Lidar->ScanAngleMin);
    ScanData->SetAngleMax(Lidar->ScanAngleMax);
    ScanData->SetAngleIncrement(Lidar->AngularIncrement);
    ScanData->SetRangeMin(Lidar->MinimumDistance / 100.);
    ScanData->SetRangeMax(Lidar->MaximumDistance / 100.);
    ScanData->SetTimeIncrement(Lidar->TimeIncrement);
    ScanData->SetScanTime(Lidar->ScanTime);

    TArray<float> Measurments = Lidar->GetMeasurementData();
    // UE_LOG(LogTemp, Error, TEXT("Message Length %d"), Measurments.Num());
    ScanData->SetRanges(Measurments);
    Handler->PublishMsg(Topic, ScanData);
    Handler->Process();

    Seq++;
    Lidar->bPublishResult = false;
  }
}
