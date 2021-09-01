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

	FBugsnagUser(const FString& Id, const FString& Email, const FString& Name)
		: Id(Id)
		, Email(Email)
		, Name(Name)
	{
	}

	const FString& GetId() const { return Id; }
	const FString& GetEmail() const { return Email; }
	const FString& GetName() const { return Name; }

private:
	FString Id;
	FString Email;
	FString Name;
};
