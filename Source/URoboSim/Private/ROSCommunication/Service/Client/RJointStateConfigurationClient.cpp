#include "ROSCommunication/Service/Client/RJointStateConfigurationClient.h"

URJointStateConfigurationClient::URJointStateConfigurationClient()
{
  ServiceName = TEXT("rosapi/get_param");
  ServiceType = TEXT("rosapi/GetParam");
}

void URJointStateConfigurationClient::SetServiceClientParameters(URServiceClientParameter *&ServiceClientParameters)
{
  if (URJointStateConfigurationClientParameter *JointStateConfigurationClientParameters = Cast<URJointStateConfigurationClientParameter>(ServiceClientParameters))
  {
    Super::SetServiceClientParameters(ServiceClientParameters);
    JointParamTopic = JointStateConfigurationClientParameters->JointParamTopic;
  }
}

void URJointStateConfigurationClient::CreateServiceClient()
{
  if (GetOwner())
  {
    Request = MakeShareable(new rosapi::GetParam::Request(JointParamTopic, ""));
    // Create an empty response instance
    Response = MakeShareable(new rosapi::GetParam::Response());
  }
}

void URJointStateConfigurationClient::GetJointNames(TFunction<void (const TArray<FString> &JointNames)> GetJointNamesFunction)
{
  if (GetOwner())
  {
    ServiceClient = MakeShareable<FRJointStateConfigurationClient>(new FRJointStateConfigurationClient(ServiceName, ServiceType, GetJointNamesFunction));

    FTimerHandle MyTimerHandle;
    GetOwner()->GetWorldTimerManager().SetTimer(MyTimerHandle, this, &URJointStateConfigurationClient::CallService, 1.0f, false);
  }
}

void URJointStateConfigurationClient::CallService()
{
  Handler->CallService(ServiceClient, Request, Response);
}

FRJointStateConfigurationClient::FRJointStateConfigurationClient(const FString &InName, const FString &InType, TFunction<void (const TArray<FString> &JointNames)> InFunction) : FROSBridgeSrvClient(InName, InType)
{
  Function = InFunction;
}

void FRJointStateConfigurationClient::Callback(TSharedPtr<FROSBridgeSrv::SrvResponse> InResponse)
{
  TSharedPtr<rosapi::GetParam::Response> Response =
      StaticCastSharedPtr<rosapi::GetParam::Response>(InResponse);

  FString JointString = Response->GetValue();
  JointString.RemoveFromStart(TEXT("["));
  JointString.RemoveFromEnd(TEXT("]"));
//   // JointString.ParseIntoArray(*JointNames, TEXT(","), true);
//   // for (FString &JointName : *JointNames)
//   // {
//   //   JointName = JointName.TrimStartAndEnd().TrimQuotes();
//   // }
//   TArray<FString> StringArray;
//   JointString.ParseIntoArray(StringArray,TEXT(","),true);
//   for(auto& st : StringArray)
//     {
//       st = st.TrimStartAndEnd().TrimQuotes();
//       UE_LOG(LogTemp, Error, TEXT("%s"),*st);
//     }
//   JointNames->Empty();
//   JointNames->Append(StringArray);
// }

  TArray<FString> JointNames;
  JointString.ParseIntoArray(JointNames, TEXT(","), true);
  for (FString &JointName : JointNames)
  {
    JointName = JointName.TrimStartAndEnd().TrimQuotes();
  }

  Function(JointNames);
}
