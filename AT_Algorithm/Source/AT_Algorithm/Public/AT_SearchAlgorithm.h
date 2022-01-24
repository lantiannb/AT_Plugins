// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
/*
��ʹ����FGUI��������޸�FGUI��CreateWindow��������
����������ֳ�ͻCreateWindow   ����FGUI���ڴ˺���ǰ���G
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
	// ��������²��ұ����빹������ܲ���
	UFUNCTION(BlueprintCallable)
		void SBuildFindData(TArray<FString> AllName);
	// ��Ӳ�������
	UFUNCTION(BlueprintCallable)
		void SAddName(FString Name);
	// ɾ����������
	UFUNCTION(BlueprintCallable)
		void SRemoveName(FString Name);
	// ����������ƣ�������Ҹ�����0�򷵻�����������ƣ��������Ƚ��򷵻�
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
	// �������·����и�"~`!@#$%^&*_-+=;:| "
	TSet<TCHAR> separateData;
	TSharedPtr<Trie> TrieFind = nullptr;
	TMap<FString, TArray<FString>> HouseTransform;
};


/*************************************************

Author:����

Date:2021-12-30

Description:
ǰ׺��
Trie::insert				--			�����ַ���Ԫ��								--		void
Trie::remove			--			ɾ���ַ���Ԫ��								--		void
Trie::search				--			�����Ƿ����Ԫ��							--		bool
Trie::startsWith		--			�����Ƿ����ǰ׺Ԫ��					--		bool
Trie::startsData		--			���Ұ���ǰ׺��Ԫ�ز�����			--		vector<string>

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

Author:����

Date:2021-12-31

Description:

ժ�ԣ�https://blog.csdn.net/weixin_44052668/article/details/102588743
�޸���ChineseToPinyinSampleFunction����

ChineseToPinyinSampleFunction			--		�����ַ������غ������б�     input: "nb��yyds��15"  output:["nb", "*zhong", "yyds", "*guo"]
																		����'*'��Ϊ�����ֵ�ǰ׺�Ա�ע�������ڵ���ǰ�Ѿ���'*'��Ϊ�ָ������������ַ����������'*'
IsChinese													--		�����ַ��������Ƿ�Ϊ���������ַ�

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