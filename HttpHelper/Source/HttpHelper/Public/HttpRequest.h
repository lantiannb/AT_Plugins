// Copyright 2021 RLoris

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Http.h"
#include "Interfaces/IHttpRequest.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "HttpRequest.generated.h"

UENUM(BlueprintType)
enum class EHttpContentType : uint8 {
	Custom																					UMETA(DisplayName = "Custom"),
	Application_Java__Archive																UMETA(DisplayName = "application/java-archive"),
	Application_EDI__X12																	UMETA(DisplayName = "application/EDI-X12"),
	Application_EDIFACT																		UMETA(DisplayName = "application/EDIFACT"),
	Application_Javascript																	UMETA(DisplayName = "application/javascript"),
	Application_Octet__Stream																UMETA(DisplayName = "application/octet-stream"),
	Application_Ogg																			UMETA(DisplayName = "application/ogg"),
	Application_Pdf																			UMETA(DisplayName = "application/pdf"),
	Application_Xhtml___Xml																	UMETA(DisplayName = "application/xhtml+xml"),
	Application_X__Shockwave__Flash															UMETA(DisplayName = "application/x-shockwave-flash"),
	Application_Json																		UMETA(DisplayName = "application/json"),
	Application_Ld___Json																	UMETA(DisplayName = "application/ld+json"),
	Application_Xml																			UMETA(DisplayName = "application/xml"),
	Application_Zip																			UMETA(DisplayName = "application/zip"),
	Application_X__Www__Form__Urlencoded													UMETA(DisplayName = "application/x-www-form-urlencoded"),
	Audio_Mpeg																				UMETA(DisplayName = "audio/mpeg"),
	Audio_X__Ms__Wma																		UMETA(DisplayName = "audio/x-ms-wma"),
	Audio_Vnd____Rn__Realaudio																UMETA(DisplayName = "audio/vnd.rn-realaudio"),
	Audio_X__Wav																			UMETA(DisplayName = "audio/x-wav"),
	Image_Gif																				UMETA(DisplayName = "image/gif"),
	Image_Jpeg																				UMETA(DisplayName = "image/jpeg"),
	Image_Png																				UMETA(DisplayName = "image/png"),
	Image_Tiff																				UMETA(DisplayName = "image/tiff"),
	Image_Vnd____Microsoft____Icon															UMETA(DisplayName = "image/vnd.microsoft.icon"),
	Image_X__Icon																			UMETA(DisplayName = "image/x-icon"),
	Image_Vnd____Djvu																		UMETA(DisplayName = "image/vnd.djvu"),
	Image_Svg___Xml																			UMETA(DisplayName = "image/svg+xml"),
	Multipart_Mixed																			UMETA(DisplayName = "multipart/mixed"),
	Multipart_Alternative																	UMETA(DisplayName = "multipart/alternative"),
	Multipart_Related																		UMETA(DisplayName = "multipart/related"),
	Multipart_Form__Data																	UMETA(DisplayName = "multipart/form-data"),
	Text_Css																				UMETA(DisplayName = "text/css"),
	Text_Csv																				UMETA(DisplayName = "text/csv"),
	Text_Html																				UMETA(DisplayName = "text/html"),
	Text_Javascript																			UMETA(DisplayName = "text/javascript"),
	Text_Plain																				UMETA(DisplayName = "text/plain"),
	Text_Xml																				UMETA(DisplayName = "text/xml"),
	Video_Mpeg																				UMETA(DisplayName = "video/mpeg"),
	Video_Mp4																				UMETA(DisplayName = "video/mp4"),
	Video_Quicktime																			UMETA(DisplayName = "video/quicktime"),
	Video_X__Ms__Wmv																		UMETA(DisplayName = "video/x-ms-wmv"),
	Video_X__Msvideo																		UMETA(DisplayName = "video/x-msvideo"),
	Video_X__Flv																			UMETA(DisplayName = "video/x-flv"),
	Video_Webm																				UMETA(DisplayName = "video/webm"),
	Application_Vnd____Android____Package__Archive											UMETA(DisplayName = "application/vnd.android.package-archive"),
	Application_Vnd____Oasis____Opendocument____Text										UMETA(DisplayName = "application/vnd.oasis.opendocument.text"),
	Application_Vnd____Oasis____Opendocument____Spreadsheet									UMETA(DisplayName = "application/vnd.oasis.opendocument.spreadsheet"),
	Application_Vnd____Oasis____Opendocument____Presentation								UMETA(DisplayName = "application/vnd.oasis.opendocument.presentation"),
	Application_Vnd____Oasis____Opendocument____Graphics									UMETA(DisplayName = "application/vnd.oasis.opendocument.graphics"),
	Application_Vnd____Ms__Excel															UMETA(DisplayName = "application/vnd.ms-excel"),
	Application_Vnd____Openxmlformats__Officedocument____Spreadsheetml____Sheet				UMETA(DisplayName = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"),
	Application_Vnd____Ms__Powerpoint														UMETA(DisplayName = "application/vnd.ms-powerpoint"),
	Application_Vnd____Openxmlformats__Officedocument____Presentationml____Presentation		UMETA(DisplayName = "application/vnd.openxmlformats-officedocument.presentationml.presentation"),
	Application_Msword																		UMETA(DisplayName = "application/msword"),
	Application_Vnd____Openxmlformats__Officedocument____Wordprocessingml____Document		UMETA(DisplayName = "application/vnd.openxmlformats-officedocument.wordprocessingml.document"),
	Application_Vnd____Mozilla____Xul___Xml													UMETA(DisplayName = "application/vnd.mozilla.xul+xml"),
};

UENUM(BlueprintType)
enum class EHttpRequestError : uint8 {
	None			UMETA(DisplayName = "None"),
	Activation		UMETA(DisplayName = "Activation"),
	InvalidBody		UMETA(DisplayName = "InvalidBody"),
	InvalidResponse UMETA(DisplayName = "InvalidResponse"),
	FailedRequest	UMETA(DisplayName = "FailedRequest"),
	InvalidRequest	UMETA(DisplayName = "InvalidRequest"),
	TimedOut		UMETA(DisplayName = "TimedOut")
};

UENUM(BlueprintType)
enum class EBodyType : uint8 {
	None   UMETA(DisplayName = "None"),
	Bytes  UMETA(DisplayName = "Bytes"),
	Text   UMETA(DisplayName = "Text"),
	File   UMETA(DisplayName = "File")
};

UENUM(BlueprintType)
enum class EHttpMethod : uint8 {
	Get       UMETA(DisplayName = "GET"),
	Post      UMETA(DisplayName = "POST"),
	Put       UMETA(DisplayName = "PUT"),
	Delete    UMETA(DisplayName = "DELETE"),
	Head      UMETA(DisplayName = "HEAD"),
	Patch	  UMETA(DisplayName = "PATCH")
};

USTRUCT(BlueprintType)
struct FHttpRequestOptions {
	GENERATED_BODY();
public:
	UPROPERTY(BlueprintReadWrite, Category = "Http")
	EHttpMethod Method = EHttpMethod::Get;
	UPROPERTY(BlueprintReadWrite, Category = "Http")
	FString Url;
	UPROPERTY(BlueprintReadWrite, Category = "Http")
	TMap<FString, FString> Headers;
	UPROPERTY(BlueprintReadWrite, Category = "Http")
	int32 Timeout = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Http")
	EHttpContentType ContentType = EHttpContentType::Custom;
	UPROPERTY(BlueprintReadWrite, Category = "Http")
	EBodyType BodyType = EBodyType::None;
	UPROPERTY(BlueprintReadWrite, Category = "Http")
	FString	TextBody;
	UPROPERTY(BlueprintReadWrite, Category = "Http")
	TArray<uint8> BytesBody;
	UPROPERTY(BlueprintReadWrite, Category = "Http")
	FString	FilepathBody;
};

USTRUCT(BlueprintType)
struct FHttpResponse {
	GENERATED_BODY();
public:
	UPROPERTY(BlueprintReadOnly, Category = "Http")
	bool IsFinished = false;
	UPROPERTY(BlueprintReadOnly, Category = "Http")
	TMap<FString, FString> Headers;
	UPROPERTY(BlueprintReadOnly, Category = "Http")
	int32 Code = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Http")
	bool IsResponseCodeOk = false;
	UPROPERTY(BlueprintReadOnly, Category = "Http")
	FString ContentType;
	UPROPERTY(BlueprintReadOnly, Category = "Http")
	FString Content;
	UPROPERTY(BlueprintReadOnly, Category = "Http")
	TArray<uint8> RawContent;
	UPROPERTY(BlueprintReadOnly, Category = "Http")
	float ElapsedTime = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Http")
	FString Status;
	UPROPERTY(BlueprintReadOnly, Category = "Http")
	int32 BytesSent = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Http")
	int32 BytesReceived = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Http")
	float DownloadPercentage = -1;
	UPROPERTY(BlueprintReadOnly, Category = "Http")
	float UploadPercentage = -1;
	UPROPERTY(BlueprintReadOnly, Category = "Http")
	EHttpRequestError ErrorReason = EHttpRequestError::None;
	UPROPERTY(BlueprintReadOnly, Category = "Http")
	FString Error;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOutputPin, const FHttpResponse&, Result);

/**
 * 
 */
UCLASS()
class HTTPHELPER_API UHttpRequest : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOutputPin Progress;
	UPROPERTY(BlueprintAssignable)
	FOutputPin Completed;
	UPROPERTY(BlueprintAssignable)
	FOutputPin Failed;
private:
	const UObject* WorldContextObject = nullptr;
	bool Active = false;
	FHttpRequestOptions Options;
	TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = nullptr;
	FTimerHandle TimeoutHandle;
	FHttpResponse HttpResponse;
public:
	UFUNCTION(BlueprintCallable, Category = "Http", meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"))
	static UHttpRequest* AsyncHttpRequest(UObject* WorldContextObject, const FHttpRequestOptions& Options);
	virtual void Activate() override;
private:
	void _Completed();
	void _Failed(EHttpRequestError Reason = EHttpRequestError::FailedRequest, FString Error = "");
	void _Progress();
};
