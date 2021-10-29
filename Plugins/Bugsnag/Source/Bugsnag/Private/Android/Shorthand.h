#pragma once
#define ReturnFalseIfNullAndClearExceptions(env, var) \
	if (var == NULL)                                  \
	{                                                 \
		if (env->ExceptionCheck())                    \
		{                                             \
			env->ExceptionClear();                    \
		}                                             \
		return false;                                 \
	}

// Quick exit when a call which should have returned something does not
#define ReturnNullOnFail(condition) \
	if (!(condition))               \
	{                               \
		return nullptr;             \
	}

// Quick exit when a call which should have returned something does not
#define ReturnValueOnFail(condition, value) \
	if (!(condition))                       \
	{                                       \
		return value;                       \
	}

// Quick exit when a JNI call fails
#define ReturnNullOnException(env) \
	if (env->ExceptionCheck())     \
	{                              \
		env->ExceptionClear();     \
		return nullptr;            \
	}

// Quick exit when a JNI call fails
#define ReturnFalseOnException(env) \
	if (env->ExceptionCheck())      \
	{                               \
		env->ExceptionClear();      \
		return false;               \
	}

#define ReturnVoidOnException(env) \
	if (env->ExceptionCheck())     \
	{                              \
		env->ExceptionClear();     \
		return;                    \
	}

#define ContinueOnException(env) \
	if (env->ExceptionCheck())   \
	{                            \
		env->ExceptionClear();   \
		continue;                \
	}

#define ReturnValueOnException(env, value) \
	if (env->ExceptionCheck())             \
	{                                      \
		env->ExceptionClear();             \
		return value;                      \
	}

#define ReturnVoidIf(condition) \
	if (condition)              \
	{                           \
		return;                 \
	}

#define ReturnStringFieldOptional(target, method)              \
	jobject jString = (*Env).CallObjectMethod(target, method); \
	return FAndroidPlatformJNI::CheckAndClearException(Env)    \
			   ? TOptional<FString>()                          \
			   : TOptional<FString>(FAndroidPlatformJNI::ParseJavaString(Env, Cache, jString));
