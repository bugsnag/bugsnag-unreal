#pragma once

#include "JNIUtilities.h"
#include "Shorthand.h"
#include "Templates/SharedPointer.h"
#include <jni.h>

class FJavaObjectWrapper
{
public:
	FJavaObjectWrapper(JNIEnv* Env, const JNIReferenceCache* Cache, jobject JavaObject)
		: JavaObject(JavaObject)
		, Cache(Cache)
		, Env(Env)
	{
	}

	jobject GetJavaObject() { return JavaObject; }

	template <typename Interface, typename InstanceType>
	TArray<TSharedRef<Interface>> GetListItems(jmethodID Method) const
	{
		TArray<TSharedRef<Interface>> Items;
		jobject jItems = (*Env).CallObjectMethod(JavaObject, Method);
		ReturnValueOnException(Env, Items);
		jint Len = (*Env).CallIntMethod(jItems, Cache->ListSize);
		ReturnValueOnException(Env, Items);
		for (jint Index = 0; Index < Len; Index++)
		{
			jobject jItem = (*Env).CallObjectMethod(jItems, Cache->ListGet, Index);
			ContinueOnException(Env);
			Items.Add(MakeShared<InstanceType>(Env, Cache, jItem));
		}
		return Items;
	}

	template <typename T>
	void SetPrimitiveObjectField(jclass PrimitiveClass, jmethodID PrimitiveConstructor, jmethodID Method, bool IsNullable, const TSharedPtr<T> Value) const
	{
		if (Value.IsValid())
		{
			jobject jValue = (*Env).NewObject(PrimitiveClass, PrimitiveConstructor, *Value);
			ReturnVoidOnException(Env);
			(*Env).CallVoidMethod(JavaObject, Method, jValue);
		}
		else if (IsNullable)
		{
			(*Env).CallVoidMethod(JavaObject, Method, nullptr);
		}
		FAndroidPlatformJNI::CheckAndClearException(Env);
	}

	template <typename T>
	TSharedPtr<T> GetLongObjectField(jmethodID Method) const
	{
		jobject jValue = (*Env).CallObjectMethod(JavaObject, Method);
		ReturnNullOnException(Env);
		ReturnNullOnFail(jValue); // might be nullable
		jlong Value = (*Env).CallLongMethod(jValue, Cache->NumberLongValue);
		ReturnNullOnException(Env);
		return MakeShareable(new T(Value));
	}

	template <typename T>
	void SetLongObjectField(jmethodID Method, bool IsNullable, const TSharedPtr<T> Value) const
	{
		SetPrimitiveObjectField(Cache->LongClass, Cache->LongConstructor, Method, IsNullable, Value);
	}

	TSharedPtr<bool> GetBoolField(jmethodID Method)
	{
		jboolean jValue = (*Env).CallBooleanMethod(JavaObject, Method);
		return FAndroidPlatformJNI::CheckAndClearException(Env)
				   ? nullptr
				   : MakeShareable(new bool(jValue == JNI_TRUE));
	}

	TSharedPtr<bool> GetBoolObjectField(jmethodID Method) const
	{
		jobject jValue = (*Env).CallObjectMethod(JavaObject, Method);
		ReturnNullOnException(Env);
		ReturnNullOnFail(jValue); // boolean objects are nullable
		jboolean Value = (*Env).CallBooleanMethod(jValue, Cache->BooleanBooleanValue);
		ReturnNullOnException(Env);
		return MakeShareable(new bool(Value == JNI_TRUE));
	}

	void SetBoolObjectField(jmethodID Method, bool IsNullable, const TSharedPtr<bool> Value) const
	{
		TSharedPtr<jboolean> jValue = Value.IsValid() ? MakeShareable(new jboolean(*Value ? JNI_TRUE : JNI_FALSE)) : nullptr;
		SetPrimitiveObjectField(Cache->BooleanClass, Cache->BooleanConstructor, Method, IsNullable, jValue);
	}

	void SetStringField(jmethodID Method, bool IsNullable, const TSharedPtr<FString>& Value) const
	{
		if (Value.IsValid())
		{
			jstring jString = FAndroidPlatformJNI::ParseFString(Env, *Value);
			ReturnVoidIf(!jString);
			(*Env).CallVoidMethod(JavaObject, Method, jString);
			FAndroidPlatformJNI::CheckAndClearException(Env);
		}
		else if (IsNullable)
		{
			(*Env).CallVoidMethod(JavaObject, Method, nullptr);
			FAndroidPlatformJNI::CheckAndClearException(Env);
		}
	}

	const FString GetStringField(jmethodID Method) const
	{
		jobject jValue = (*Env).CallObjectMethod(JavaObject, Method);
		FAndroidPlatformJNI::CheckAndClearException(Env);
		return FAndroidPlatformJNI::ParseJavaString(Env, Cache, jValue);
	}

	const FDateTime GetDateField(jmethodID Method) const
	{
		jobject jValue = (*Env).CallObjectMethod(JavaObject, Method);
		FAndroidPlatformJNI::CheckAndClearException(Env);
		return FAndroidPlatformJNI::ParseDateTime(Env, Cache, jValue);
	}

	void SetDateField(jmethodID Method, bool IsNullable, TSharedPtr<FDateTime> Value) const
	{
		if (Value.IsValid())
		{
			jobject jValue = FAndroidPlatformJNI::ParseJavaDate(Env, Cache, *Value);
			ReturnVoidOnException(Env);
			(*Env).CallVoidMethod(JavaObject, Method, jValue);
		}
		else if (IsNullable)
		{
			(*Env).CallVoidMethod(JavaObject, Method, nullptr);
		}
		FAndroidPlatformJNI::CheckAndClearException(Env);
	}

	void SetField(jmethodID Method, bool IsNullable, jobject Value) const
	{
		ReturnVoidIf(!Value && !IsNullable);
		(*Env).CallVoidMethod(JavaObject, Method, Value);
		FAndroidPlatformJNI::CheckAndClearException(Env);
	}

	TSharedPtr<FJsonObject> GetJsonObjectField(jmethodID Method) const
	{
		jobject jValue = (*Env).CallObjectMethod(JavaObject, Method);
		ReturnValueOnException(Env, MakeShareable(new FJsonObject));
		return FAndroidPlatformJNI::ConvertJavaMapToJson(Env, Cache, jValue);
	}

	const TSharedPtr<TArray<FString>> GetStringArrayField(jmethodID Method) const
	{
		TSharedPtr<TArray<FString>> Array = MakeShareable(new TArray<FString>);
		jobjectArray jArray = (jobjectArray)(*Env).CallObjectMethod(JavaObject, Method);
		ReturnValueOnException(Env, Array);
		jsize Len = (*Env).GetArrayLength(jArray);
		ReturnValueOnException(Env, Array);
		for (jsize Index = 0; Index < Len; Index++)
		{
			jobject jItem = (*Env).GetObjectArrayElement(jArray, Index);
			ReturnValueOnException(Env, Array);
			Array->Add(FAndroidPlatformJNI::ParseJavaString(Env, Cache, jItem));
		}
		return Array;
	}

	void SetStringArrayField(jmethodID Method, const TSharedPtr<TArray<FString>>& Value) const
	{
		jobjectArray jArray = nullptr;
		if (Value.IsValid())
		{
			int32 Length = (*Value).Num();
			jArray = (*Env).NewObjectArray(Length, Cache->StringClass, NULL);
			ReturnVoidOnException(Env);
			for (int32 Index = 0; Index < Length; Index++)
			{
				jstring jItem = FAndroidPlatformJNI::ParseFString(Env, (*Value)[Index]);
				if (jItem)
				{
					(*Env).SetObjectArrayElement(jArray, Index, jItem);
					FAndroidPlatformJNI::CheckAndClearException(Env);
				}
			}
		}
		(*Env).CallVoidMethod(JavaObject, Method, jArray);
		FAndroidPlatformJNI::CheckAndClearException(Env);
	}

protected:
	JNIEnv* Env;
	const JNIReferenceCache* Cache;
	const jobject JavaObject;
};
