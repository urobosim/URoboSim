#include "Sensor/REncoder.h"

UREncoder::UREncoder()
{
  AccumulatedValue = 0.0f;
  OldValue = 0.0f;
}

void UREncoder::ResetValue()
{
  AccumulatedValue = 0.0f;
}

void UREncoder::UpdateValue(float InNewValue)
{
  NewValue = InNewValue;
  UpdateValue();
}

float UREncoder::GetValue(bool bNormalize)
{
  //Write function for normalized values
  return GetAccumulatedValue();
}

float UREncoder::GetAccumulatedValue()
{
  return AccumulatedValue;
}

void UREncoder::UpdateValue()
{
  AccumulatedValue = NewValue;
}

void URAngularEncoder::UpdateValue()
{
  float TempNewValue = 0;

  if(NewValue * OldValue < 0 && FMath::Abs(NewValue) > PI/2.)
    {
      if(OldValue > 0)
        {
          TempNewValue = 2 * PI + NewValue;
        }
      else
        {
          TempNewValue = -2 * PI + NewValue;
        }
    }
  else
    {
      TempNewValue = NewValue;
    }

  AccumulatedValue += TempNewValue - OldValue;
  OldValue = NewValue;
}

void UREncoder::Test()
{
  UpdateValue(PI/4);
  UE_LOG(LogTemp, Log, TEXT("Value should %f Vale: %f"), PI/4., GetValue());
  UpdateValue(PI/2.);
  UE_LOG(LogTemp, Log, TEXT("Value should %f Vale: %f"), PI/2., GetValue());
  UpdateValue(-PI/4.);
  UE_LOG(LogTemp, Log, TEXT("Value should %f Vale: %f"), -PI/4., GetValue());
  UpdateValue(-PI/2.);
  UE_LOG(LogTemp, Log, TEXT("Value should %f Vale: %f"), -PI/2., GetValue());
  UpdateValue(-PI*3./4.);
  UE_LOG(LogTemp, Log, TEXT("Value should %f Vale: %f"), -PI*3./4., GetValue());
  UpdateValue(-PI);
  UE_LOG(LogTemp, Log, TEXT("Value should %f Vale: %f"), -PI, GetValue());
  UpdateValue(PI * 0.8);
  UE_LOG(LogTemp, Log, TEXT("Value should %f Vale: %f"), -PI*1.2, GetValue());



}
