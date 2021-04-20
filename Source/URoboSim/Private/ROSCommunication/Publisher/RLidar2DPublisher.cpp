#include "ROSCommunication/Publisher/RLidar2DPublisher.h"
#include "sensor_msgs/LaserScan.h"

DEFINE_LOG_CATEGORY_STATIC(LogRLidar2DPublisher, Log, All);

URLidar2DPublisher::URLidar2DPublisher()
{
  Topic = TEXT("/base_scan");
  MessageType = TEXT("sensor_msgs/LaserScan");
  LidarReferenceROSLinkName = TEXT("base_laser_link");
  LidarName = TEXT("Laser");
}

void URLidar2DPublisher::Init()
{
  Super::Init();
  if (!Lidar)
  {
    SetLidar();
  }
}

void URLidar2DPublisher::SetPublishParameters(URPublisherParameter *&PublisherParameters)
{
  URLidar2DPublisherParameter *LidarPublisherParameters = Cast<URLidar2DPublisherParameter>(PublisherParameters);
  if (LidarPublisherParameters)
  {
    Super::SetPublishParameters(PublisherParameters);
    LidarReferenceROSLinkName = LidarPublisherParameters->LidarReferenceROSLinkName;
    LidarName = LidarPublisherParameters->LidarName;
    SetLidar();
  }
}

void URLidar2DPublisher::Publish()
{
  if (Lidar && Lidar->bPublishResult)
  {
    static int Seq = 0;
    static TSharedPtr<sensor_msgs::LaserScan> ScanData = MakeShareable(new sensor_msgs::LaserScan());

    ScanData->SetHeader(std_msgs::Header(Seq++, FROSTime(), LidarReferenceROSLinkName));
    ScanData->SetAngleMin(Lidar->ScanAngleMin);
    ScanData->SetAngleMax(Lidar->ScanAngleMax);
    ScanData->SetAngleIncrement(Lidar->AngularIncrement);
    ScanData->SetRangeMin(Lidar->MinimumDistance / 100.);
    ScanData->SetRangeMax(Lidar->MaximumDistance / 100.);
    ScanData->SetTimeIncrement(Lidar->TimeIncrement);
    ScanData->SetScanTime(Lidar->ScanTime);

    TArray<float> Measurements = Lidar->GetMeasuredData();
    ScanData->SetRanges(Measurements);

    Handler->PublishMsg(Topic, ScanData);

    Handler->Process();

    Lidar->bPublishResult = false;
  }
}

void URLidar2DPublisher::SetLidar()
{
  if (GetOwner())
  {
    Lidar = Cast<URLidar2D>(GetOwner()->GetSensor(LidarName));
    if (!Lidar)
    {
      UE_LOG(LogRLidar2DPublisher, Error, TEXT("%s not found in %s"), *LidarName, *GetOwner()->GetName())
    }
  }
}