#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

void UYourClass::PingGoogle()
{
    FHttpModule* Http = &FHttpModule::Get();
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();

    Request->OnProcessRequestComplete().BindLambda([](FHttpRequestPtr Req, FHttpResponsePtr Resp, bool bSucceeded)
    {
        if (bSucceeded && Resp.IsValid())
        {
            UE_LOG(LogTemp, Log, TEXT("Ping success! Code: %d"), Resp->GetResponseCode());
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Ping failed."));
        }
    });

    Request->SetURL("https://www.google.com");
    Request->SetVerb("GET");
    Request->ProcessRequest();
}
