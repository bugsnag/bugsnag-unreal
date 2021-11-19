// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"

/**
 * Metadata allows semi-arbitrary data to be added to events sent to Bugsnag.
 *
 * Each metadata section is displayed as a tab on the Bugsnag dashboard.
 */
class BUGSNAG_API IBugsnagMetadataStore
{
public:
	/**
	 * Caches additional diagnostics to be sent with any subsequent events
	 *
	 * @param Section  The category of information
	 * @param Metadata New or updated information to append to section
	 */
	virtual void AddMetadata(const FString& Section, const TSharedRef<FJsonObject>& Metadata) = 0;

	/**
	 * Caches additional diagnostics to be sent with any subsequent events
	 *
	 * @param Section The category of information
	 * @param Key     The key in which to assign the value
	 * @param Value   New or updated information to append to section
	 */
	virtual void AddMetadata(const FString& Section, const FString& Key, const TSharedPtr<FJsonValue>& Value) = 0;

	/**
	 * Caches additional diagnostics to be sent with any subsequent events
	 *
	 * @param Section The category of information
	 * @param Key     The key in which to assign the value
	 * @param Value   New or updated information to append to section
	 */
	void AddMetadata(const FString& Section, const FString& Key, const FString& Value)
	{
		AddMetadata(Section, Key, MakeShared<FJsonValueString>(Value));
	}

	/**
	 * Caches additional diagnostics to be sent with any subsequent events
	 *
	 * @param Section The category of information
	 * @param Key     The key in which to assign the value
	 * @param Value   New or updated information to append to section
	 */
	void AddMetadata(const FString& Section, const FString& Key, const TCHAR* Value)
	{
		AddMetadata(Section, Key, MakeShareable(Value ? new FJsonValueString(Value) : nullptr));
	}

	/**
	 * Caches additional diagnostics to be sent with any subsequent events
	 *
	 * @param Section The category of information
	 * @param Key     The key in which to assign the value
	 * @param Value   New or updated information to append to section
	 */
	void AddMetadata(const FString& Section, const FString& Key, const char* Value)
	{
		AddMetadata(Section, Key, MakeShareable(Value ? new FJsonValueString(Value) : nullptr));
	}

	/**
	 * Caches additional diagnostics to be sent with any subsequent events
	 *
	 * @param Section The category of information
	 * @param Key     The key in which to assign the value
	 * @param Value   New or updated information to append to section
	 */
	void AddMetadata(const FString& Section, const FString& Key, double Value)
	{
		AddMetadata(Section, Key, MakeShared<FJsonValueNumber>(Value));
	}

	/**
	 * Caches additional diagnostics to be sent with any subsequent events
	 *
	 * @param Section The category of information
	 * @param Key     The key in which to assign the value
	 * @param Value   New or updated information to append to section
	 */
	void AddMetadata(const FString& Section, const FString& Key, float Value)
	{
		AddMetadata(Section, Key, MakeShared<FJsonValueNumber>(Value));
	}

	/**
	 * Caches additional diagnostics to be sent with any subsequent events
	 *
	 * @param Section The category of information
	 * @param Key     The key in which to assign the value
	 * @param Value   New or updated information to append to section
	 */
	void AddMetadata(const FString& Section, const FString& Key, int8 Value)
	{
		AddMetadata(Section, Key, MakeShared<FJsonValueNumber>(Value));
	}

	/**
	 * Caches additional diagnostics to be sent with any subsequent events
	 *
	 * @param Section The category of information
	 * @param Key     The key in which to assign the value
	 * @param Value   New or updated information to append to section
	 */
	void AddMetadata(const FString& Section, const FString& Key, int16 Value)
	{
		AddMetadata(Section, Key, MakeShared<FJsonValueNumber>(Value));
	}

	/**
	 * Caches additional diagnostics to be sent with any subsequent events
	 *
	 * @param Section The category of information
	 * @param Key     The key in which to assign the value
	 * @param Value   New or updated information to append to section
	 */
	void AddMetadata(const FString& Section, const FString& Key, int32 Value)
	{
		AddMetadata(Section, Key, MakeShared<FJsonValueNumber>(Value));
	}

	/**
	 * Caches additional diagnostics to be sent with any subsequent events
	 *
	 * @param Section The category of information
	 * @param Key     The key in which to assign the value
	 * @param Value   New or updated information to append to section
	 */
	void AddMetadata(const FString& Section, const FString& Key, int64 Value)
	{
		AddMetadata(Section, Key, MakeShared<FJsonValueNumber>(Value));
	}

	/**
	 * Caches additional diagnostics to be sent with any subsequent events
	 *
	 * @param Section The category of information
	 * @param Key     The key in which to assign the value
	 * @param Value   New or updated information to append to section
	 */
	void AddMetadata(const FString& Section, const FString& Key, uint8 Value)
	{
		AddMetadata(Section, Key, MakeShared<FJsonValueNumber>(Value));
	}

	/**
	 * Caches additional diagnostics to be sent with any subsequent events
	 *
	 * @param Section The category of information
	 * @param Key     The key in which to assign the value
	 * @param Value   New or updated information to append to section
	 */
	void AddMetadata(const FString& Section, const FString& Key, uint16 Value)
	{
		AddMetadata(Section, Key, MakeShared<FJsonValueNumber>(Value));
	}

	/**
	 * Caches additional diagnostics to be sent with any subsequent events
	 *
	 * @param Section The category of information
	 * @param Key     The key in which to assign the value
	 * @param Value   New or updated information to append to section
	 */
	void AddMetadata(const FString& Section, const FString& Key, uint32 Value)
	{
		AddMetadata(Section, Key, MakeShared<FJsonValueNumber>(Value));
	}

	/**
	 * Caches additional diagnostics to be sent with any subsequent events
	 *
	 * @param Section The category of information
	 * @param Key     The key in which to assign the value
	 * @param Value   New or updated information to append to section
	 */
	void AddMetadata(const FString& Section, const FString& Key, uint64 Value)
	{
		AddMetadata(Section, Key, MakeShared<FJsonValueNumber>(Value));
	}

	/**
	 * Caches additional diagnostics to be sent with any subsequent events
	 *
	 * @param Section The category of information
	 * @param Key     The key in which to assign the value
	 * @param Value   New or updated information to append to section
	 */
	void AddMetadata(const FString& Section, const FString& Key, bool Value)
	{
		AddMetadata(Section, Key, MakeShared<FJsonValueBoolean>(Value));
	}

	/**
	 * Retrieve cached diagnostics associated with a specific section
	 *
	 * @param Section The section to retrieve
	 *
	 * @return The associated data, if any
	 */
	virtual TSharedPtr<FJsonObject> GetMetadata(const FString& Section) = 0;

	/**
	 * Retrieve cached diagnostics associated with a specific section and key
	 *
	 * @param Section The section containing key
	 * @param Key     The value's identifier
	 *
	 * @return The associated data, if any
	 */
	virtual TSharedPtr<FJsonValue> GetMetadata(const FString& Section, const FString& Key) = 0;

	/**
	 * Removes the cached value from a specific section and key
	 *
	 * @param Section The section containing key
	 */
	virtual void ClearMetadata(const FString& Section) = 0;

	/**
	 * Removes the cached value from a specific section and key
	 *
	 * @param Section The section containing key
	 * @param Key     The key to remove
	 */
	virtual void ClearMetadata(const FString& Section, const FString& Key) = 0;
};
