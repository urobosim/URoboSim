#pragma once

#include "ROSBridgeMsg.h"

#include "std_msgs/Header.h"
#include "actionlib_msgs/GoalStatus.h"

namespace actionlib_msgs
{
	class GoalStatusArray : public FROSBridgeMsg
	{
		std_msgs::Header Header;
		TArray<actionlib_msgs::GoalStatus> StatusList;
	public:
		GoalStatusArray()
		{
			MsgType = "actionlib_msgs/GoalStatusArray";
		}

		GoalStatusArray
		(
			std_msgs::Header InHeader,
			const TArray<actionlib_msgs::GoalStatus>& InStatusList
		):
			Header(InHeader),
			StatusList(InStatusList)
		{
			MsgType = "actionlib_msgs/GoalStatusArray";
		}

		~GoalStatusArray() override {}

		std_msgs::Header GetHeader() const
		{
			return Header;
		}

		TArray<actionlib_msgs::GoalStatus> GetStatusList() const
		{
			return StatusList;
		}

		void SetHeader(std_msgs::Header InHeader)
		{
			Header = InHeader;
		}

		void SetStatusList(TArray<actionlib_msgs::GoalStatus>& InStatusList)
		{
			StatusList = InStatusList;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Header = std_msgs::Header::GetFromJson(JsonObject->GetObjectField(TEXT("header")));

			TArray<TSharedPtr<FJsonValue>> ValuesPtrArr;

			StatusList.Empty();
			ValuesPtrArr = JsonObject->GetArrayField(TEXT("status_list"));
			for (auto &ptr : ValuesPtrArr)
				StatusList.Add(actionlib_msgs::GoalStatus::GetFromJson(ptr->AsObject()));

		}

		static GoalStatusArray GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			GoalStatusArray Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());

			Object->SetObjectField(TEXT("header"), Header.ToJsonObject());
			TArray<TSharedPtr<FJsonValue>> StatusListArray;
			for (auto &val : StatusList)
				StatusListArray.Add(MakeShareable(new FJsonValueObject(val.ToJsonObject())));
			Object->SetArrayField(TEXT("status_list"), StatusListArray);
			return Object;
		}
		virtual FString ToYamlString() const override
		{
			FString OutputString;
			TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
			FJsonSerializer::Serialize(ToJsonObject().ToSharedRef(), Writer);
			return OutputString;
		}
	};
}