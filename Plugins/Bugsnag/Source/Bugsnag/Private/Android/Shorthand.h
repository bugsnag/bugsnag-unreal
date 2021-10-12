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
#define ReturnNullOnFail(prop) \
	if (!prop)                 \
	{                          \
		return nullptr;        \
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

#define ReturnVoidIf(condition) \
	if (condition)              \
	{                           \
		return;                 \
	}
