#include "BugsnagUtils.h"

#include "Misc/EngineVersion.h"

FString BugsnagUtils::GetUnrealEngineVersion()
{
	return FEngineVersion::Current().ToString(EVersionComponent::Changelist);
}
