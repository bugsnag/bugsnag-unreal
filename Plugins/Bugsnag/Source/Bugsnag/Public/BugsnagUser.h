#pragma once

#include "CoreMinimal.h"

class BUGSNAG_API FBugsnagUser
{
public:
	FBugsnagUser() {}

	FBugsnagUser(const FBugsnagUser& User)
		: Id(User.Id)
		, Email(User.Email)
		, Name(User.Name)
	{
	}

	FBugsnagUser(const TSharedPtr<FString>& Id, const TSharedPtr<FString>& Email, const TSharedPtr<FString>& Name)
		: Id(Id)
		, Email(Email)
		, Name(Name)
	{
	}

	const TSharedPtr<FString>& GetId() const { return Id; }
	const TSharedPtr<FString>& GetEmail() const { return Email; }
	const TSharedPtr<FString>& GetName() const { return Name; }

	bool IsEmpty() const
	{
		return (!Id.IsValid() || Id->IsEmpty()) &&
			   (!Email.IsValid() || Email->IsEmpty()) &&
			   (!Name.IsValid() || Name->IsEmpty());
	}

private:
	TSharedPtr<FString> Id;
	TSharedPtr<FString> Email;
	TSharedPtr<FString> Name;
};
