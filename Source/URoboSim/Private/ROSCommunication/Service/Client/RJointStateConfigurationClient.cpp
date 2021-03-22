#include "ROSCommunication/Service/Client/RJointStateConfigurationClient.h"
#include "XmlFile.h"

FRJointStateConfigurationClient::FRJointStateConfigurationClient(TArray<FString> *OutJointNames, const FString &InName, const FString &InType) : FROSBridgeSrvClient(InName, InType)
{
  JointNames = OutJointNames;
}

void FRJointStateConfigurationClient::Callback(TSharedPtr<FROSBridgeSrv::SrvResponse> InResponse)
{
  TSharedPtr<rosapi::GetParam::Response> Response =
      StaticCastSharedPtr<rosapi::GetParam::Response>(InResponse);

  FString JointString = Response->GetValue();
  UE_LOG(LogTemp, Warning, TEXT("JointString: %s"), *JointString)
  // TArray<FString> StringArray;
  // JointString.RemoveFromStart(TEXT("["));
  // JointString.RemoveFromEnd(TEXT("]"));
  // JointString.ParseIntoArray(StringArray, TEXT(","), true);
  // for (FString &String : StringArray)
  // {
  //   String = String.TrimStartAndEnd().TrimQuotes();
  // }
  // JointNames->Empty();
  // JointNames->Append(StringArray);
}

void URJointStateConfigurationClient::Init(UObject *InModel, TArray<FString> *OutArray)
{
  FString JointParamTopic = TEXT("rosapi/get_param");

  // Create the service client
  ServiceClient = MakeShareable<FRJointStateConfigurationClient>(new FRJointStateConfigurationClient(OutArray, JointParamTopic, TEXT("rosapi/GetParam")));
  
  // Create a request instance with request parameters
  Request = MakeShareable(new rosapi::GetParam::Request(JointParamTopic, JointPath));
  
  // Create an empty response instance
  Response = MakeShareable(new rosapi::GetParam::Response());

  FTimerHandle MyTimerHandle;
  Cast<ARModel>(InModel)->GetWorldTimerManager().SetTimer(MyTimerHandle, this, &URJointStateConfigurationClient::CallService, 1.0f, false);
}

void URJointStateConfigurationClient::CallService()
{
  Handler->CallService(ServiceClient, Request, Response);
}

// void FROSJointControllerConfigurationClient::Callback(TSharedPtr<FROSBridgeSrv::SrvResponse> InResponse)
// {
//   TSharedPtr<rosapi::GetParam::Response> Response =
//       StaticCastSharedPtr<rosapi::GetParam::Response>(InResponse);

//   FString JointString = Response->GetValue();
//   TArray<FString> StringArray;
//   JointString.RemoveFromStart(TEXT("["));
//   JointString.RemoveFromEnd(TEXT("]"));
//   JointString.ParseIntoArray(StringArray, TEXT(","), true);
//   for (auto &st : StringArray)
//   {
//     st = st.TrimStartAndEnd().TrimQuotes();
//     float &JointState = JointNames->FindOrAdd(st);
//     JointState = 0.f;
//   }
// }

// FROSJointControllerConfigurationClient::FROSJointControllerConfigurationClient(TMap<FString, float> *OutJointNames, const FString &InName, const FString &InType) : FROSBridgeSrvClient(InName, InType)
// {
//   JointNames = OutJointNames;
// }

// void FROSJointLimitControllerConfigurationClient::Callback(TSharedPtr<FROSBridgeSrv::SrvResponse> InResponse)
// {
//   TSharedPtr<rosapi::GetParam::Response> Response =
//       StaticCastSharedPtr<rosapi::GetParam::Response>(InResponse);

//   TArray<TCHAR> Chars;
//   Chars.Add('\n');
//   Chars.Add('\"');
//   FString JointString = Response->GetValue();
//   JointString = JointString.ReplaceEscapedCharWithChar(&Chars);

//   JointString.RemoveFromStart(TEXT("\""));
//   JointString.RemoveFromEnd(TEXT("\""));

//   FXmlFile XmlFile(JointString, EConstructMethod::ConstructFromBuffer);

//   if (XmlFile.IsValid())
//   {
//     for (auto &Node : XmlFile.GetRootNode()->GetChildrenNodes())
//     {
//       if (Node->GetTag().Equals(TEXT("joint")))
//       {
//         const FString MyName = Node->GetAttribute(TEXT("name"));
//         for (const auto &ChildNode : Node->GetChildrenNodes())
//         {
//           if (ChildNode->GetTag().Equals(TEXT("safety_controller")))
//           {
//             float SoftLowerLimit = FCString::Atof(*ChildNode->GetAttribute("soft_lower_limit"));
//             float SoftUpperLimit = FCString::Atof(*ChildNode->GetAttribute("soft_upper_limit"));

//             UE_LOG(LogTemp, Warning, TEXT("%s: Lower %s Upper: %s"), *MyName, *ChildNode->GetAttribute("soft_lower_limit"), *ChildNode->GetAttribute("soft_upper_limit"));

//             if (Model)
//             {
//               if (URJoint *Joint = Model->Joints[MyName])
//               {
//                 Joint->Constraint->SoftUpper = SoftUpperLimit;
//                 Joint->Constraint->SoftLower = SoftLowerLimit;
//               }

//               if (0.0f < SoftLowerLimit)
//               {
//                 float &JointState = JointNames->FindOrAdd(MyName);
//                 JointState = SoftLowerLimit;
//               }
//               else if (0.0f > SoftUpperLimit)
//               {
//                 float &JointState = JointNames->FindOrAdd(MyName);
//                 JointState = SoftUpperLimit;
//               }
//             }
//           }
//         }
//       }
//     }
//   }
// }

// FROSJointLimitControllerConfigurationClient::FROSJointLimitControllerConfigurationClient(TMap<FString, float> *OutJointNames, AActor *InModel, const FString &InName, const FString &InType) : FROSBridgeSrvClient(InName, InType)
// {
//   Model = Cast<ARModel>(InModel);
//   JointNames = OutJointNames;
// }