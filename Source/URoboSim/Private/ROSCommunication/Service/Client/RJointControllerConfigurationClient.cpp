#include "ROSCommunication/Service/Client/RJointControllerConfigurationClient.h"
#include "XmlFile.h"

DEFINE_LOG_CATEGORY_STATIC(LogRJointControllerConfigurationClient, Log, All)

URJointControllerConfigurationClient::URJointControllerConfigurationClient()
{
  ServiceName = TEXT("rosapi/get_param");
  ServiceType = TEXT("rosapi/GetParam");
  JointParamTopic = TEXT("/whole_body_controller/joints");
  LimitParamTopic = TEXT("/robot_description");
  JointControllerName = TEXT("JointController");
}

void URJointControllerConfigurationClient::SetServiceClientParameters(URServiceClientParameter *&ServiceClientParameters)
{
  URJointControllerConfigurationClientParameter *JointControllerConfigurationClientParameters = Cast<URJointControllerConfigurationClientParameter>(ServiceClientParameters);
  if (JointControllerConfigurationClientParameters)
  {
    Super::SetServiceClientParameters(ServiceClientParameters);
    JointParamTopic = JointControllerConfigurationClientParameters->JointParamTopic;
    LimitParamTopic = JointControllerConfigurationClientParameters->LimitParamTopic;
    JointControllerName = JointControllerConfigurationClientParameters->JointControllerName;
  }
}

void URJointControllerConfigurationClient::CreateServiceClient()
{
  if (GetOwner())
  {
    if (URJointController *JointController = Cast<URJointController>(GetOwner()->GetController(JointControllerName)))
    {
      TMap<FString, FJointState> *JointNames = &JointController->DesiredJointStates;
      JointRequest = MakeShareable(new rosapi::GetParam::Request(JointParamTopic, TEXT("")));
      JointResponse = MakeShareable(new rosapi::GetParam::Response());
      JointServiceClient = MakeShareable<FRJointControllerConfigurationClient>(new FRJointControllerConfigurationClient(JointController, JointNames, ServiceName, ServiceType));

      LimitRequest = MakeShareable(new rosapi::GetParam::Request(LimitParamTopic, TEXT("")));
      LimitResponse = MakeShareable(new rosapi::GetParam::Response());
      JointLimitServiceClient = MakeShareable<FRJointLimitControllerConfigurationClient>(new FRJointLimitControllerConfigurationClient(JointNames, GetOwner(), ServiceName, ServiceType));

      FTimerHandle MyTimerHandle;
      GetOwner()->GetWorldTimerManager().SetTimer(MyTimerHandle, this, &URJointControllerConfigurationClient::CallService, 1.0f, false);
      // ServiceClient = MakeShareable<FROSJointStateConfigurationClient>(new FROSJointStateConfigurationClient(OutArray,TEXT("rosapi/get_param"), TEXT("rosapi/GetParam")));
    }
  }
}

void URJointControllerConfigurationClient::CallService()
{
  Handler->CallService(JointServiceClient, JointRequest, JointResponse);
  Handler->CallService(JointLimitServiceClient, LimitRequest, LimitResponse);
}

FRJointControllerConfigurationClient::FRJointControllerConfigurationClient(URJointController *InJointController, TMap<FString, FJointState> *OutJointNames, const FString &InName, const FString &InType) : FROSBridgeSrvClient(InName, InType)
{
  JointNames = OutJointNames;
  JointController = InJointController;
}

void FRJointControllerConfigurationClient::Callback(TSharedPtr<FROSBridgeSrv::SrvResponse> InResponse)
{
  TSharedPtr<rosapi::GetParam::Response> Response =
      StaticCastSharedPtr<rosapi::GetParam::Response>(InResponse);

  FString JointString = Response->GetValue();
  TArray<FString> StringArray;
  JointString.RemoveFromStart(TEXT("["));
  JointString.RemoveFromEnd(TEXT("]"));
  JointString.ParseIntoArray(StringArray, TEXT(","), true);


  TArray<FString> JointNamesTemp;
  for (auto &st : StringArray)
  {
    st = st.TrimStartAndEnd().TrimQuotes();
    // float &JointState = JointNames->FindOrAdd(st).JointPosition;
    JointNamesTemp.Add(st);
    // JointState = 0.f;
  }
  JointController->SetJointNames(JointNamesTemp);
}

FRJointLimitControllerConfigurationClient::FRJointLimitControllerConfigurationClient(TMap<FString, FJointState> *OutJointNames, ARModel *InModel, const FString &InName, const FString &InType) : FROSBridgeSrvClient(InName, InType)
{
  Model = InModel;
  JointNames = OutJointNames;
}

void FRJointLimitControllerConfigurationClient::Callback(TSharedPtr<FROSBridgeSrv::SrvResponse> InResponse)
{
  TSharedPtr<rosapi::GetParam::Response> Response =
      StaticCastSharedPtr<rosapi::GetParam::Response>(InResponse);

  TArray<TCHAR> Chars;
  Chars.Add('\n');
  Chars.Add('\"');
  FString JointString = Response->GetValue();
  JointString = JointString.ReplaceEscapedCharWithChar(&Chars);

  JointString.RemoveFromStart(TEXT("\""));
  JointString.RemoveFromEnd(TEXT("\""));

  FXmlFile XmlFile(JointString, EConstructMethod::ConstructFromBuffer);

  if (XmlFile.IsValid())
  {
    for (auto &Node : XmlFile.GetRootNode()->GetChildrenNodes())
    {
      if (Node->GetTag().Equals(TEXT("joint")))
      {
        const FString MyName = Node->GetAttribute(TEXT("name"));
        for (const auto &ChildNode : Node->GetChildrenNodes())
        {
          if (ChildNode->GetTag().Equals(TEXT("safety_controller")))
          {
            float SoftLowerLimit = FCString::Atof(*ChildNode->GetAttribute("soft_lower_limit"));
            float SoftUpperLimit = FCString::Atof(*ChildNode->GetAttribute("soft_upper_limit"));

            UE_LOG(LogRJointControllerConfigurationClient, Log, TEXT("%s: Lower %s Upper: %s"), *MyName, *ChildNode->GetAttribute("soft_lower_limit"), *ChildNode->GetAttribute("soft_upper_limit"));

            if (Model)
            {
              if (URJoint *Joint = Model->GetJoint(MyName))
              {
                Joint->Constraint->SoftUpper = SoftUpperLimit;
                Joint->Constraint->SoftLower = SoftLowerLimit;
              }

              if (0.0f < SoftLowerLimit)
              {
                float &JointState = JointNames->FindOrAdd(MyName).JointPosition;
                JointState = SoftLowerLimit;
              }
              else if (0.0f > SoftUpperLimit)
              {
                float &JointState = JointNames->FindOrAdd(MyName).JointPosition;
                JointState = SoftUpperLimit;
              }
            }
            // for(auto& Attributs : ChildNode->GetAttributes())
            //   {
            //     UE_LOG(LogTemp, Error, TEXT("safety %s"), *Attributs.GetTag());
            //   }
          }
        }
      }
    }
  }

  // UE_LOG(LogTemp, Warning, TEXT("rootnode: %s"), *JointString);
  // UE_LOG(LogTemp, Warning, TEXT("rootnode: %s"), *JointString);
  // TArray<FString> StringArray;
  // JointString.RemoveFromStart(TEXT("["));
  // JointString.RemoveFromEnd(TEXT("]"));
  // JointString.ParseIntoArray(StringArray,TEXT(","),true);
  // for(auto& st : StringArray)
  // {
  // 	st = st.Trim().TrimQuotes();
  //   float& JointState = JointNames->FindOrAdd(st);
  //   JointState = 0.f;
  // }
}
