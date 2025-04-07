#pragma once



/*-----------------------------------------------------------------------------
	if defined DEBUGMODE, Can use Debug Utils
	if DEBUGMODE not defined, code will be ignored
-----------------------------------------------------------------------------*/



#define DEBUGMODE





#ifndef DEBUGMODE
/**
* DEBUGMODE Deactivated
*/
namespace Debug
{
	namespace CallCheck
	{
		inline static void Called(const float DisplayTime_ = 15.f) {}
		inline static void Success(const float DisplayTime_ = 15.f) {}
		inline static void Error(const float DisplayTime_ = 15.f) {}
	}

	inline static void Print(const FString& Message_ = "", FColor Color_ = FColor::Cyan, const float DisplayTime_ = 15.f) {}
	template<typename T>
	inline static void NullCheck(T Object_, const FString& Message_ = "", float DisplayTime_ = 15.f) {}
	template<typename T>
	inline static void NullPrint(T Object_, const FString& Message_ = "", float DisplayTime_ = 15.f) {}

}
namespace DrawDebug
{
	inline static void DrawSphere(const UWorld* World_ = nullptr, const FVector& Location_ = FVector(0.f, 0.f, 0.f), float Thickness_ = 4.f, FColor Color_ = FColor::Black, const float Radius_ = 25.f, const int32 Segments_ = 4) {}
	inline static void DrawLine(const UWorld* World_ = nullptr, const FVector& StartLocation_ = FVector(0.f, 0.f, 0.f), const FVector& EndLocation_ = FVector(0.f, 0.f, 0.f), float Thickness_ = 3.f, FColor Color_ = FColor::Black) {}
	inline static void DrawString(const UWorld* World_ = nullptr, const FVector& Location_ = FVector(0.f, 0.f, 0.f), const FString& Message_ = "", float LiveTime_ = -1.f, FColor Color_ = FColor::Black) {}
}
namespace HardDebug
{
	inline static void HardTrace(const float DisplayTime_ = 15.f) {}
	template<typename T>
	inline static void HardCheck(T Object_, const FString& Message_ = "", float DisplayTime_ = 15.f) {}
}



#else
/**
* DEBUGMODE Activated
*/

#include <source_location>
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(DebugMode, Log, All);

enum class DebugType
{
	Error,
	Success,
	Normal,
};

namespace DebugUtils
{
	inline static void PrintDebugMessage(const FString& Message_, DebugType DebugType_, float DisplayTime_ = 15.f)
	{
		if (nullptr == GEngine)
			return;

		switch (DebugType_)
		{
		case DebugType::Error:
			UE_LOG(DebugMode, Error, TEXT("%s"), *Message_);
			GEngine->AddOnScreenDebugMessage(-1, DisplayTime_, FColor::Red, Message_);
			break;
		case DebugType::Success:
			UE_LOG(DebugMode, Warning, TEXT("%s"), *Message_);
			GEngine->AddOnScreenDebugMessage(-1, DisplayTime_, FColor::Green, Message_);
			break;
		case DebugType::Normal:
			UE_LOG(DebugMode, Warning, TEXT("%s"), *Message_);
			GEngine->AddOnScreenDebugMessage(-1, DisplayTime_, FColor::Cyan, Message_);
			break;
		default:
			check(nullptr);
			break;
		}
	}

	inline static FString ExtractFileName(const FString& FilePath_)
	{
		size_t Position = FilePath_.Find(TEXT("Private"));
		if (Position != std::string::npos)
		{
			return FilePath_.Mid(Position + FString(TEXT("Private")).Len());
		}

		Position = FilePath_.Find(TEXT("Public"));
		if (Position != std::string::npos)
		{
			return FilePath_.Mid(Position + FString(TEXT("Pulbic")).Len());
		}

		return FilePath_;
	}
}

namespace Debug
{
	/**
	* No need to give values for function
	* Always called
	*/
	namespace CallCheck
	{
		/**
		* Default Call Check
		* Result example: "Called => FunctionName"
		*/
		inline static void Called(const float DisplayTime_ = 15.f, const std::source_location Location_ = std::source_location::current())
		{
			FString Message = TEXT("Called => ");
			Message += Location_.function_name();
			DebugUtils::PrintDebugMessage(Message, DebugType::Normal, DisplayTime_);
		}

		/**
		* Use in success cases for check
		* Result example: "Success => FunctionName"
		*/
		inline static void Success(const float DisplayTime_ = 15.f, const std::source_location Location_ = std::source_location::current())
		{
			FString Message = TEXT("Success => ");
			Message += Location_.function_name();
			DebugUtils::PrintDebugMessage(Message, DebugType::Success, DisplayTime_);
		}

		/**
		* Use in error cases for check
		* Result example: "Error => FileName = LineNumber = FunctionName"
		*/
		inline static void Error(const float DisplayTime_ = 15.f, const std::source_location Location_ = std::source_location::current())
		{
			FString Message = TEXT("Error => ");
			Message += DebugUtils::ExtractFileName(Location_.file_name());
			Message += TEXT(" = ");
			Message += FString::FromInt(Location_.line());
			Message += TEXT(" = ");
			Message += Location_.function_name();
			DebugUtils::PrintDebugMessage(Message, DebugType::Error, DisplayTime_);
		}
	}

	/**
	* need values for function
	*/
	inline static void Print(const FString& Message_, FColor Color_ = FColor::Cyan, const float DisplayTime_ = 15.f)
	{
		UE_LOG(DebugMode, Warning, TEXT("%s"), *Message_);
		GEngine->AddOnScreenDebugMessage(-1, DisplayTime_, Color_, Message_);
	}

	/**
	* Print both, null and not null cases
	* Result example: "Not Null => ObjectName = FunctionName"
	*/
	template<typename T>
	inline static void NullCheck(T Object_, const FString& Message_, float DisplayTime_ = 15.f, const std::source_location Location_ = std::source_location::current())
	{
		if (nullptr != Object_)
		{
			FString Message = TEXT("Not Null => ");
			Message += Message_;
			Message += TEXT(" = ");
			Message += Location_.function_name();
			DebugUtils::PrintDebugMessage(Message, DebugType::Success, DisplayTime_);

			return true;
		}

		FString Message = TEXT("Null => ");
		Message += Message_;
		Message += TEXT(" = ");
		Message += DebugUtils::ExtractFileName(Location_.file_name());
		Message += TEXT(" = ");
		Message += FString::FromInt(Location_.line());
		Message += TEXT(" = ");
		Message += Location_.function_name();
		DebugUtils::PrintDebugMessage(Message, DebugType::Error, DisplayTime_);

		return false;
	}

	/**
	* Print only null cases
	* Result example: "Null => ObjectName = FileName = LineNumber = FunctionName"
	*/
	template<typename T>
	inline static bool NullPrint(T Object_, const FString& Message_, float DisplayTime_ = 15.f, const std::source_location Location_ = std::source_location::current())
	{
		if (nullptr != Object_)
			return true;

		FString Message = TEXT("Null => ");
		Message += Message_;
		Message += TEXT(" = ");
		Message += DebugUtils::ExtractFileName(Location_.file_name());
		Message += TEXT(" = ");
		Message += FString::FromInt(Location_.line());
		Message += TEXT(" = ");
		Message += Location_.function_name();
		DebugUtils::PrintDebugMessage(Message, DebugType::Error, DisplayTime_);

		return false;
	}
}

namespace DrawDebug
{
	inline static void DrawSphere(const UWorld* World_, const FVector& Location_, float Thickness_ = 4.f, FColor Color_ = FColor::Black, const float Radius_ = 25.f, const int32 Segments_ = 4)
	{
		if (nullptr == World_)
			return;

		DrawDebugSphere(World_, Location_, Radius_, Segments_, Color_, false, -1.f, 0U, Thickness_);
	}

	inline static void DrawLine(const UWorld* World_, const FVector& StartLocation_, const FVector& EndLocation_, float Thickness_ = 3.f, FColor Color_ = FColor::Black)
	{
		if (nullptr == World_)
			return;

		DrawDebugLine(World_, StartLocation_, EndLocation_, Color_, false, -1.f, 0U, Thickness_);
	}

	inline static void DrawString(const UWorld* World_, const FVector& Location_, const FString& Message_, float LiveTime_ = -1.f, FColor Color_ = FColor::Black)
	{
		if (nullptr == World_)
			return;

		DrawDebugString(World_, Location_, Message_, 0, Color_, LiveTime_, false);
	}
}

/**
* Must use it with 'Windows Debugger'
* HardDebug creates breakpoint
*/
namespace HardDebug
{
	inline static void HardTrace(const float DisplayTime_ = 15.f, const std::source_location Location_ = std::source_location::current())
	{
		FString Message = TEXT("HardTrace Called => ");
		Message += Location_.function_name();
		DebugUtils::PrintDebugMessage(Message, DebugType::Normal, DisplayTime_);
		__debugbreak();
	}

	template<typename T>
	inline static void HardCheck(T Object_, const FString& Message_ = "", float DisplayTime_ = 15.f, const std::source_location Location_ = std::source_location::current())
	{
		if (nullptr != Object_)
			return true;

		FString Message = TEXT("Null => ");
		Message += Message_;
		Message += TEXT(" = ");
		Message += DebugUtils::ExtractFileName(Location_.file_name());
		Message += TEXT(" = ");
		Message += FString::FromInt(Location_.line());
		Message += TEXT(" = ");
		Message += Location_.function_name();
		DebugUtils::PrintDebugMessage(Message, DebugType::Error, DisplayTime_);
		__debugbreak();
	}
}

#endif // !DEBUGMODE

using namespace Debug;
using namespace Debug::CallCheck;
using namespace DrawDebug;
using namespace HardDebug;