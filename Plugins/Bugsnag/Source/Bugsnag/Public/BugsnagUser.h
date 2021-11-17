// Copyright 2021 Bugsnag. All Rights Reserved.

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

	FBugsnagUser(const TOptional<FString>& Id, const TOptional<FString>& Email, const TOptional<FString>& Name)
		: Id(Id)
		, Email(Email)
		, Name(Name)
	{
	}

	const TOptional<FString>& GetId() const { return Id; }
	const TOptional<FString>& GetEmail() const { return Email; }
	const TOptional<FString>& GetName() const { return Name; }

	bool IsEmpty() const
	{
		return (!Id.IsSet() || Id->IsEmpty()) &&
			   (!Email.IsSet() || Email->IsEmpty()) &&
			   (!Name.IsSet() || Name->IsEmpty());
	}

private:
	TOptional<FString> Id;
	TOptional<FString> Email;
	TOptional<FString> Name;
};
