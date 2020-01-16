
#include "ROSCommunication/RROSClientImpl.h"
#include "ROSCommunication/RRosComunication.h"
// #include "ROSCommunication/RPublisher.h"
#include "ROSCommunication/RROSClient.h"
// #include "Controller/RController.h"
// #include "ROSCommunication/RRosCommunicationComponent.h"
#include "Physics/RModel.h"
#include "XmlFile.h"


void FROSJointStateConfigurationClient::Callback( TSharedPtr<FROSBridgeSrv::SrvResponse> InResponse)
{

	TSharedPtr<rosapi::GetParam::Response> Response =
		StaticCastSharedPtr<rosapi::GetParam::Response>(InResponse);

	FString JointString = Response->GetValue();
	TArray<FString> StringArray;
	JointString.RemoveFromStart(TEXT("["));
	JointString.RemoveFromEnd(TEXT("]"));
	JointString.ParseIntoArray(StringArray,TEXT(","),true);
	for(auto& st : StringArray)
	{
		st = st.Trim().TrimQuotes();
	}
	JointNames->Empty();
	JointNames->Append(StringArray);
}

FROSJointStateConfigurationClient::FROSJointStateConfigurationClient(TArray<FString>* OutJointNames, const FString& InName, const FString& InType):
	FROSBridgeSrvClient(InName, InType)
{
	JointNames = OutJointNames;
}

void FROSJointControllerConfigurationClient::Callback( TSharedPtr<FROSBridgeSrv::SrvResponse> InResponse)
{
	TSharedPtr<rosapi::GetParam::Response> Response =
		StaticCastSharedPtr<rosapi::GetParam::Response>(InResponse);

	FString JointString = Response->GetValue();
	TArray<FString> StringArray;
	JointString.RemoveFromStart(TEXT("["));
	JointString.RemoveFromEnd(TEXT("]"));
	JointString.ParseIntoArray(StringArray,TEXT(","),true);
	for(auto& st : StringArray)
	{
		st = st.Trim().TrimQuotes();
          float& JointState = JointNames->FindOrAdd(st);
          JointState = 0.f;
	}
}

FROSJointControllerConfigurationClient::FROSJointControllerConfigurationClient(TMap<FString, float>* OutJointNames, const FString& InName, const FString& InType):
	FROSBridgeSrvClient(InName, InType)
{
	JointNames = OutJointNames;
}

void FROSJointLimitControllerConfigurationClient::Callback( TSharedPtr<FROSBridgeSrv::SrvResponse> InResponse)
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

  FXmlFile XmlFile(JointString,  EConstructMethod::ConstructFromBuffer);

  if(XmlFile.IsValid())
    {
      for(auto & Node : XmlFile.GetRootNode()->GetChildrenNodes())
        {
          if (Node->GetTag().Equals(TEXT("joint")))
            {
              const FString Name = Node->GetAttribute(TEXT("name"));
              for (const auto& ChildNode : Node->GetChildrenNodes())
                {
                  if (ChildNode->GetTag().Equals(TEXT("safety_controller")))
                    {
                      float SoftLowerLimit = FCString::Atof(*ChildNode->GetAttribute("soft_lower_limit"));
                      float SoftUpperLimit = FCString::Atof(*ChildNode->GetAttribute("soft_upper_limit"));

                      UE_LOG(LogTemp, Warning, TEXT("%s: Lower %s Upper: %s"), *Name, *ChildNode->GetAttribute("soft_lower_limit"), *ChildNode->GetAttribute("soft_upper_limit"));
                      if(0.0f < SoftLowerLimit)
                        {
                          float& JointState = JointNames->FindOrAdd(Name);
                          JointState = SoftLowerLimit;
                        }
                      else if(0.0f > SoftUpperLimit)
                        {
                          float& JointState = JointNames->FindOrAdd(Name);
                          JointState = SoftUpperLimit;
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

FROSJointLimitControllerConfigurationClient::FROSJointLimitControllerConfigurationClient(TMap<FString, float>* OutJointNames, const FString& InName, const FString& InType):
	FROSBridgeSrvClient(InName, InType)
{
	JointNames = OutJointNames;
}
