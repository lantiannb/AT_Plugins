// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
/*
若使用了FGUI插件，则修改FGUI的CreateWindow函数名字
否则产生名字冲突CreateWindow   建议FGUI中在此函数前面加G
*/
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/PreWindowsApi.h"
#include <windows.h>
#include <winnt.h>
#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"
#include <string>
#include <vector>
#include <Engine/Engine.h>


#include "AT_SearchAlgorithm.generated.h"


using namespace std;

class C_SearchHouseName;

UCLASS(Blueprintable)
class AT_ALGORITHM_API UAT_SearchAlgorithm : public UObject
{
	GENERATED_BODY()
public:

	UAT_SearchAlgorithm();
	~UAT_SearchAlgorithm();
	// 构建或更新查找表，必须构建后才能查找
	UFUNCTION(BlueprintCallable)
		void SBuildFindData(TArray<FString> AllName);
	// 添加查找名称
	UFUNCTION(BlueprintCallable)
		void SAddName(FString Name);
	// 删除查找名称
	UFUNCTION(BlueprintCallable)
		void SRemoveName(FString Name);
	// 查找相关名称，输入查找个数，0则返回所有相关名称，按关联度降序返回
	UFUNCTION(BlueprintCallable)
		TArray<FString> SSearchName(FString SName, int Num);


private:
	TSharedPtr<C_SearchHouseName> FindNamePtr;
};



class Trie;
class  C_SearchHouseName
{
public:
	C_SearchHouseName();
	~C_SearchHouseName();

	void ResetSearchData(TArray<FString>& Words);
	TArray<FString> SearchWord(FString word, int Num);
	void AddWord(FString word);
	void RemoveWord(FString word);
	void SeparateStr(TArray<FString>& res, FString word);
	TArray<int> GetNext(FString FStr);
	bool KMP(FString FStr, FString SearchStr, TArray<int>& NextAry);
	void ToLowercase(FString& FStr);
private:
	// 遇到以下符号切割"~`!@#$%^&*_-+=;:| "
	TSet<TCHAR> separateData;
	TSharedPtr<Trie> TrieFind = nullptr;
	TMap<FString, TArray<FString>> HouseTransform;
};


/*************************************************

Author:蓝天

Date:2021-12-30

Description:
前缀树
Trie::insert				--			插入字符串元素								--		void
Trie::remove			--			删除字符串元素								--		void
Trie::search				--			查找是否存在元素							--		bool
Trie::startsWith		--			查找是否存在前缀元素					--		bool
Trie::startsData		--			查找包含前缀的元素并返回			--		vector<string>

**************************************************/

#ifndef TireNextMaxSize
#define TireNextMaxSize CHAR_MAX - CHAR_MIN + 1
#endif // TireNextMaxSize

struct TrieNode {
	bool isEnd;
	TrieNode() {};
	TrieNode(bool IisEnd) : isEnd(IisEnd) {}
	TSharedPtr<TrieNode> next[TireNextMaxSize];
	TSet<FString> WordInsts;
};

class  Trie {
private:
	bool isEnd;
	TSharedPtr<TrieNode> root = nullptr;

public:
	Trie();
	void BuildTrieTree(TArray<FString>& Words);
	void insert(FString word, FString WordInst);
	void remove(FString word, FString WordInst);
	bool search(FString word);
	bool startsWith(FString prefix);
	TArray<FString> startsData(FString prefix);
	void DFSGetLeaf(TSet<FString>& res, FString LeftStr, TSharedPtr<TrieNode>  node);
};




/*************************************************

Author:蓝天

Date:2021-12-31

Description:

摘自：https://blog.csdn.net/weixin_44052668/article/details/102588743
修改了ChineseToPinyinSampleFunction函数

ChineseToPinyinSampleFunction			--		输入字符串返回含中文列表     input: "nb中yyds国15"  output:["nb", "*zhong", "yyds", "*guo"]
																		其中'*'作为中文字的前缀以标注，由于在调用前已经将'*'作为分割符所以输入的字符串不会包含'*'
IsChinese													--		输入字符串返回是否为包含中文字符

**************************************************/

class ChineseToPinyin
{
public:
	static TArray<FString> ChineseToPinyinSampleFunction(FString fstr);
	static bool IsChinese(FString str);
	static char* ConvertLPWSTRToLPSTR(LPWSTR lpwszStrIn);
	static char* FStringToCharPtr(FString iStr);
private:
	static bool PinyinHandle(const std::string& dest_chinese, vector<string>& out_py);

};