#include "ROSCommunication/2DLidarPublisher.h"
#include "sensor_msgs/LaserScan.h"

UR2DLidarPublisher::UR2DLidarPublisher()
{
  Topic = TEXT("/base_scan");
}

void UR2DLidarPublisher::SetMessageType()
{
  MessageType = TEXT("sensor_msgs/LaserScan");
}

void UR2DLidarPublisher::SetOwner(UObject* InOwner)
{
  TArray<UR2DLidar*> LidarList;
  Cast<ARModel>(InOwner)->GetComponents(LidarList, true);
  for(auto & Sensor : LidarList)
    {
      if(Sensor->GetName().Equals(SensorName))
        {
          Owner = Sensor;
        }
    }
  if(!Owner)
    {
      UE_LOG(LogTemp, Error, TEXT("No Sensor with Name %s found"), *SensorName);
    }
}

void UR2DLidarPublisher::Publish()
{
  if(Owner->bPublishResult)
    {
      TSharedPtr<sensor_msgs::LaserScan> ScanData = MakeShareable(new sensor_msgs::LaserScan());
      ScanData->SetHeader(std_msgs::Header(Seq, FROSTime(), "base_laser_link"));
      ScanData->SetAngleMin(Owner->ScanAngleMin);
      ScanData->SetAngleMax(Owner->ScanAngleMax);
      ScanData->SetAngleIncrement(Owner->AngularIncrement);
      ScanData->SetRangeMin(Owner->MinimumDistance /100.);
      ScanData->SetRangeMax(Owner->MaximumDistance /100.);
      ScanData->SetTimeIncrement(Owner->TimeIncrement);
      ScanData->SetScanTime(Owner->ScanTime);

      TArray<float> Measurments = Owner->GetMeasurementData();
      // UE_LOG(LogTemp, Error, TEXT("Message Length %d"), Measurments.Num());
      ScanData->SetRanges(Measurments);
      Handler->PublishMsg(Topic, ScanData);
      Handler->Process();


      Seq++ ;
      Owner->bPublishResult = false;
    }
}
