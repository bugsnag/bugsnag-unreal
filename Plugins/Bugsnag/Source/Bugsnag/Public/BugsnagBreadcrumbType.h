#pragma once

UENUM()
enum class EBugsnagBreadcrumbType : uint8
{
	Manual,
	Error,
	Log,
	Navigation,
	Process,
	Request,
	State,
	User
};
