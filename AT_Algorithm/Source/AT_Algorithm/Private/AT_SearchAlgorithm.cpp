// Fill out your copyright notice in the Description page of Project Settings.


#include "AT_SearchAlgorithm.h"



UAT_SearchAlgorithm::UAT_SearchAlgorithm()
{
	this->FindNamePtr = MakeShareable(new C_SearchHouseName());
}
UAT_SearchAlgorithm::~UAT_SearchAlgorithm()
{
}

void UAT_SearchAlgorithm::SBuildFindData(TArray<FString> AllName)
{
	this->FindNamePtr->ResetSearchData(AllName);
}

void UAT_SearchAlgorithm::SAddName(FString Name)
{
	this->FindNamePtr->AddWord(Name);
}

void UAT_SearchAlgorithm::SRemoveName(FString Name)
{
	this->FindNamePtr->RemoveWord(Name);
}

TArray<FString> UAT_SearchAlgorithm::SSearchName(FString SName, int Num)
{
	//return TArray<FString>();
	return this->FindNamePtr->SearchWord(SName, Num);
}


C_SearchHouseName::C_SearchHouseName() {
	//"~`!@#$%^&*_-+=;:| "
	for (char i : "~`!@#$%^&*_-+=;:| ")
		this->separateData.Add(i);
}

C_SearchHouseName::~C_SearchHouseName() {
}

void C_SearchHouseName::ResetSearchData(TArray<FString>& Words) {
	this->TrieFind = MakeShareable(new Trie());
	for (auto& word : Words) {

		TArray<FString> TempArrayStr;
		// 按照可分隔符将字符串分隔开
		this->SeparateStr(TempArrayStr, word);
		// 判断是否含中文
		bool WordIsChinese = ChineseToPinyin::IsChinese(word);
		// 再将字符串的中文转为拼音('*' + pinyin)  前缀为'*'
		TArray<FString> ChineseToPinYinArrayStr;
		if (WordIsChinese)
			for (auto& word_i : TempArrayStr) {
				//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, word_i);
				ChineseToPinYinArrayStr.Append(ChineseToPinyin::ChineseToPinyinSampleFunction(word_i));
			}

		else
			ChineseToPinYinArrayStr = TempArrayStr;

		for (auto& s : ChineseToPinYinArrayStr) {
			s = s.ToLower();
		}

		// 中文缩写 、 中文全拼
		FString Suo, Yuan;
		for (int word_i = ChineseToPinYinArrayStr.Num() - 1; word_i >= 0; --word_i) {
			if (ChineseToPinYinArrayStr[word_i][0] == '*') {
				if (WordIsChinese)
					Suo = FString::Chr(ChineseToPinYinArrayStr[word_i][1]) + Suo;
				Yuan = ChineseToPinYinArrayStr[word_i].RightChop(1) + Yuan;
			}
			else {
				if (WordIsChinese)
					Suo = ChineseToPinYinArrayStr[word_i] + Suo;
				Yuan = ChineseToPinYinArrayStr[word_i] + Yuan;
			}
			if (WordIsChinese) {
				this->TrieFind->insert(Suo, word);
				//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, Suo);
			}
			this->TrieFind->insert(Yuan, word);
			//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Orange, Yuan);
		}
		HouseTransform.Add(word);
		//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, Suo);
		//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Orange, Yuan);
		if (WordIsChinese)
			this->HouseTransform[word].Add(Suo);
		this->HouseTransform[word].Add(Yuan);
	}
}

TArray<FString> C_SearchHouseName::SearchWord(FString word, int Num) {
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, word);

	TSet<FString> res;
	TArray<FString> resArray;
	//1.Name拆分	
	TArray<FString> TempArrayStr;
	// 按照可分隔符将字符串分隔开
	this->SeparateStr(TempArrayStr, word);
	/*for (auto& s : TempArrayStr)
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Orange, s);*/
		// 判断是否含中文
	bool WordIsChinese = ChineseToPinyin::IsChinese(word);
	// 再将字符串的中文转为拼音('*' + pinyin)  前缀为'*'
	TArray<FString> ChineseToPinYinArrayStr;
	if (WordIsChinese)
		for (auto& word_i : TempArrayStr)
			ChineseToPinYinArrayStr.Append(ChineseToPinyin::ChineseToPinyinSampleFunction(word_i));
	else
		ChineseToPinYinArrayStr = TempArrayStr;

	// 转小写
	for (auto& s : ChineseToPinYinArrayStr)
		s = s.ToLower();


	// 全包含关系查找
	TArray<TArray<int>> Nexts;
	for (auto& S : TempArrayStr)
		Nexts.Add(this->GetNext(S));
	bool NeedAppendStr, NeedAppendStrChild;

	for (auto KV : this->HouseTransform) {
		NeedAppendStr = true;

		// 判断切割后的每一个子字符串是否包含  
		for (int Sindex = 0; Sindex < TempArrayStr.Num(); ++Sindex) {
			NeedAppendStrChild = false;
			//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, KV.Key + "  " + TempArrayStr[Sindex]);
			NeedAppendStrChild |= KMP(KV.Key, TempArrayStr[Sindex], Nexts[Sindex]);
			//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Orange, NeedAppendStrChild ? TEXT("True") : TEXT("False"));
			if (!NeedAppendStrChild) {
				for (auto& Vi : KV.Value) {
					//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, Vi + "  " + TempArrayStr[Sindex]);
					NeedAppendStrChild |= KMP(Vi, TempArrayStr[Sindex], Nexts[Sindex]);
				}
				//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, NeedAppendStrChild ? TEXT("True") : TEXT("False"));
			}

			NeedAppendStr &= NeedAppendStrChild;
			if (!NeedAppendStr)
				break;
		}
		if (NeedAppendStr)
			res.Add(KV.Key);



		if (Num && res.Num() >= Num)
			return res.Array();
	}
	resArray = res.Array();

	// 前缀树查找
	FString Suo, Yuan;
	TArray<FString> tempRes;
	for (auto& word_i : ChineseToPinYinArrayStr) {
		if (word_i[0] == '*') {
			if (WordIsChinese)
				Suo += word_i[1];
			Yuan += word_i.RightChop(1);
		}
		else {
			if (WordIsChinese)
				Suo += word_i;
			Yuan += word_i;
		}
	}
	for (auto& word_i : ChineseToPinYinArrayStr) {
		tempRes = this->TrieFind->startsData(Yuan);
		if (!Num || res.Num() + tempRes.Num() <= Num) {
			for (auto& ResChild : tempRes)
				if (!res.Contains(ResChild)) {
					res.Add(ResChild);
					resArray.Add(ResChild);
				}
		}
		else
			for (auto& ResChild : tempRes) {
				if (!res.Contains(ResChild)) {
					res.Add(ResChild);
					resArray.Add(ResChild);
				}
				if (Num && resArray.Num() >= Num)
					return resArray;
			}
		if (WordIsChinese) {
			tempRes = this->TrieFind->startsData(Suo);
			if (!Num || res.Num() + tempRes.Num() <= Num) {
				for (auto& ResChild : tempRes)
					if (!res.Contains(ResChild)) {
						res.Add(ResChild);
						resArray.Add(ResChild);
					}
			}
			else
				for (auto& ResChild : tempRes) {
					if (!res.Contains(ResChild)) {
						res.Add(ResChild);
						resArray.Add(ResChild);
					}
					if (Num && resArray.Num() >= Num)
						return resArray;
				}
		}

		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Orange, FString::FromInt(res.Num()));

		if (word_i[0] == '*') {
			if (WordIsChinese)
				Suo = Suo.RightChop(1);
			Yuan = Yuan.RightChop(word_i.Len() - 1);
		}
		else {
			if (WordIsChinese)
				Suo = Suo.RightChop(word_i.Len() - 1);
			Yuan = Yuan.RightChop(word_i.Len() - 1);
		}
	}


	return resArray;
}



void C_SearchHouseName::AddWord(FString word) {
	TArray<FString> TempArrayStr;
	// 按照可分隔符将字符串分隔开
	this->SeparateStr(TempArrayStr, word);
	// 判断是否含中文
	bool WordIsChinese = ChineseToPinyin::IsChinese(word);
	// 再将字符串的中文转为拼音('*' + pinyin)  前缀为'*'
	TArray<FString> ChineseToPinYinArrayStr;
	if (WordIsChinese)
		for (auto& word_i : TempArrayStr) {
			//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, word_i);
			ChineseToPinYinArrayStr.Append(ChineseToPinyin::ChineseToPinyinSampleFunction(word_i));
		}

	else
		ChineseToPinYinArrayStr = TempArrayStr;

	for (auto& s : ChineseToPinYinArrayStr) {
		s = s.ToLower();
	}


	// 中文缩写 、 中文全拼
	FString Suo, Yuan;
	for (int word_i = ChineseToPinYinArrayStr.Num() - 1; word_i >= 0; --word_i) {
		if (ChineseToPinYinArrayStr[word_i][0] == '*') {
			if (WordIsChinese)
				Suo = FString::Chr(ChineseToPinYinArrayStr[word_i][1]) + Suo;
			Yuan = ChineseToPinYinArrayStr[word_i].RightChop(1) + Yuan;
		}
		else {
			if (WordIsChinese)
				Suo = ChineseToPinYinArrayStr[word_i] + Suo;
			Yuan = ChineseToPinYinArrayStr[word_i] + Yuan;
		}
		if (WordIsChinese) {
			this->TrieFind->insert(Suo, word);
			//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, Suo);
		}
		this->TrieFind->insert(Yuan, word);
		//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Orange, Yuan);
	}
	if (!HouseTransform.Contains(word))
		HouseTransform.Add(word);
	if (WordIsChinese)
		this->HouseTransform[word].Add(Suo);
	this->HouseTransform[word].Add(Yuan);
}

void C_SearchHouseName::RemoveWord(FString word) {
	TArray<FString> TempArrayStr;
	// 按照可分隔符将字符串分隔开
	this->SeparateStr(TempArrayStr, word);
	// 判断是否含中文
	bool WordIsChinese = ChineseToPinyin::IsChinese(word);
	// 再将字符串的中文转为拼音('*' + pinyin)  前缀为'*'
	TArray<FString> ChineseToPinYinArrayStr;
	if (WordIsChinese)
		for (auto& word_i : TempArrayStr)
			ChineseToPinYinArrayStr.Append(ChineseToPinyin::ChineseToPinyinSampleFunction(word_i));
	else
		ChineseToPinYinArrayStr = TempArrayStr;

	for (auto& s : ChineseToPinYinArrayStr) {
		s = s.ToLower();
	}


	// 中文缩写 、 中文全拼
	FString Suo, Yuan;
	for (int word_i = ChineseToPinYinArrayStr.Num() - 1; word_i > 0; --word_i) {
		if (ChineseToPinYinArrayStr[word_i][0] == '*') {
			if (WordIsChinese)
				Suo = Suo + ChineseToPinYinArrayStr[word_i][1];
			Yuan = Yuan + ChineseToPinYinArrayStr[word_i].RightChop(1);
		}
		else {
			if (WordIsChinese)
				Suo = Suo + ChineseToPinYinArrayStr[word_i];
			Yuan = Yuan + ChineseToPinYinArrayStr[word_i];
		}
		if (WordIsChinese)
			this->TrieFind->remove(Suo, word);
		this->TrieFind->remove(Yuan, word);
	}
	if (this->HouseTransform.Contains(word))
		this->HouseTransform.Remove(word);

}

void C_SearchHouseName::SeparateStr(TArray<FString>& res, FString word) {
	FString TempStr = "";
	for (auto& i : word) {
		if (!separateData.Contains(i))
			TempStr += i;
		else {
			if (TempStr.Len())
				res.Add(TempStr);
			TempStr = "";
		}
	}
	if (TempStr.Len())
		res.Add(TempStr);

}

TArray<int> C_SearchHouseName::GetNext(FString FStr) {
	string sFStr = ChineseToPinyin::ConvertLPWSTRToLPSTR(FStr.GetCharArray().GetData());
	//int len = FStr.Len();///先求字符串的长度，便于循环赋值
	int len = sFStr.size();
	int i = 0, j = -1;
	TArray<int> next = { -1 };
	while (i < len) {
		if (j == -1 || sFStr[i] == sFStr[j]) {
			next.Add(++j);
			++i;
		}
		else j = next[j];
	}
	return next;

}

bool C_SearchHouseName::KMP(FString FStr, FString SearchStr, TArray<int>& NextAry) {
	string sFStr = ChineseToPinyin::ConvertLPWSTRToLPSTR(FStr.GetCharArray().GetData());
	string sSearchStr = ChineseToPinyin::ConvertLPWSTRToLPSTR(SearchStr.GetCharArray().GetData());
	/*int lenStr = FStr.Len();
	int lenSStr = SearchStr.Len();*/
	int lenStr = sFStr.size();
	int lenSStr = sSearchStr.size();
	int i = 0, j = 0;
	while (i < lenStr && j < lenSStr)
	{
		if (j == -1 || sFStr[i] == sSearchStr[j])
		{
			j++;
			i++;
		}
		else
			j = NextAry[j];
	}
	if (j == lenSStr)
		return true;
	else
		return false;

}

void C_SearchHouseName::ToLowercase(FString& FStr) {

	for (TCHAR& c : FStr) {

		c = isupper(c) ? c + 32 : c;
	}
}

/***********************************************Trie***********************************************/


Trie::Trie() {
	isEnd = false;
	root = MakeShareable(new(TrieNode));
	//memset(next, 0, sizeof(next));
}
void Trie::BuildTrieTree(TArray<FString>& Words) {

	for (auto& word : Words)
		this->insert(word, word);
	return;
}

void Trie::insert(FString word, FString WordInst) {

	auto node = this->root;
	for (char c : word) {
		if (!node->next[c - CHAR_MIN]) {
			node->next[c - CHAR_MIN] = MakeShareable(new TrieNode());
		}
		node = node->next[c - CHAR_MIN];
		//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, FString("a"));
	}
	node->WordInsts.Add(WordInst);
	node->isEnd = true;
}

void Trie::remove(FString word, FString WordInst) {
	auto node = this->root;
	for (char c : word) {
		if (!node->next[c - CHAR_MIN])
			return;
		node = node->next[c - CHAR_MIN];
	}
	if (node->WordInsts.Contains(WordInst))
		node->WordInsts.Remove(WordInst);
}

bool Trie::search(FString word) {
	auto node = this->root;
	for (char c : word) {
		node = node->next[c - CHAR_MIN];
		if (!node) {
			return false;
		}
	}
	return node->isEnd;
}

bool Trie::startsWith(FString prefix) {
	auto node = this->root;
	for (char c : prefix) {
		node = node->next[c - CHAR_MIN];
		if (!node) {
			return false;
		}
	}
	return true;
}
TArray<FString> Trie::startsData(FString prefix) {
	auto node = this->root;
	TSet<FString> res;

	for (char c : prefix) {
		node = node->next[c - CHAR_MIN];
		if (!node)
			return res.Array();
	}
	DFSGetLeaf(res, prefix, node);
	return res.Array();
}
void Trie::DFSGetLeaf(TSet<FString>& res, FString LeftStr, TSharedPtr<TrieNode>  node) {
	if (node->isEnd)
		res.Append(node->WordInsts);
	for (int i = 0; i < TireNextMaxSize; ++i)
		if (node->next[i])
			DFSGetLeaf(res, LeftStr + (char)(i + CHAR_MIN), node->next[i]);
}



/***********************************************ChineseToPinyin***********************************************/


TArray<FString> ChineseToPinyin::ChineseToPinyinSampleFunction(FString fstr) {
	//char* str_name = new char(1);;
	char* str_name = ConvertLPWSTRToLPSTR(fstr.GetCharArray().GetData());
	string in_str(str_name);
	vector<string> out_py;
	PinyinHandle(in_str, out_py);
	TArray<FString> result_py;
	for (auto& i : out_py)
		result_py.Add(i.c_str());
	return result_py;

}

bool ChineseToPinyin::IsChinese(FString fstrName)
{
	//char* charName = new char(1);
	char* charName = ConvertLPWSTRToLPSTR(fstrName.GetCharArray().GetData());
	string strName(charName);
	for (int i = 0, chrasc = 0; i < strName.length(); i++)
	{
		if (i + 1 < strName.length()) {
			chrasc = strName.at(i) * 256 + strName.at(i + 1) + 256;
			if ((strName.at(i) < 0 || strName.at(i) >= 128) && (chrasc <= 0 || chrasc >= 160))
			{
				return true;
			}
			continue;
		}
		else {
			return false;
		}
	}
	return false;
}

char* ChineseToPinyin::ConvertLPWSTRToLPSTR(LPWSTR lpwszStrIn)
{
	LPSTR pszOut = NULL;
	if (lpwszStrIn != NULL)
	{
		int nInputStrLen = wcslen(lpwszStrIn);

		// Double NULL Termination
		int nOutputStrLen = WideCharToMultiByte(CP_ACP, 0, lpwszStrIn, nInputStrLen, NULL, 0, 0, 0) + 2;
		pszOut = new char[nOutputStrLen];
		if (pszOut != NULL)
		{
			memset(pszOut, 0x00, nOutputStrLen);
			WideCharToMultiByte(CP_ACP, 0, lpwszStrIn, nInputStrLen, pszOut, nOutputStrLen, 0, 0);
		}
	}
	return pszOut;
}
char* ChineseToPinyin::FStringToCharPtr(FString iStr)
{
	return ConvertLPWSTRToLPSTR(iStr.GetCharArray().GetData());
}

bool ChineseToPinyin::PinyinHandle(const std::string& dest_chinese, vector<string>& out_py) {
	const int spell_value[] = { -20319, -20317, -20304, -20295, -20292, -20283, -20265, -20257, -20242, -20230, -20051, -20036, -20032, -20026,
		-20002, -19990, -19986, -19982, -19976, -19805, -19784, -19775, -19774, -19763, -19756, -19751, -19746, -19741, -19739, -19728,
		-19725, -19715, -19540, -19531, -19525, -19515, -19500, -19484, -19479, -19467, -19289, -19288, -19281, -19275, -19270, -19263,
		-19261, -19249, -19243, -19242, -19238, -19235, -19227, -19224, -19218, -19212, -19038, -19023, -19018, -19006, -19003, -18996,
		-18977, -18961, -18952, -18783, -18774, -18773, -18763, -18756, -18741, -18735, -18731, -18722, -18710, -18697, -18696, -18526,
		-18518, -18501, -18490, -18478, -18463, -18448, -18447, -18446, -18239, -18237, -18231, -18220, -18211, -18201, -18184, -18183,
		-18181, -18012, -17997, -17988, -17970, -17964, -17961, -17950, -17947, -17931, -17928, -17922, -17759, -17752, -17733, -17730,
		-17721, -17703, -17701, -17697, -17692, -17683, -17676, -17496, -17487, -17482, -17468, -17454, -17433, -17427, -17417, -17202,
		-17185, -16983, -16970, -16942, -16915, -16733, -16708, -16706, -16689, -16664, -16657, -16647, -16474, -16470, -16465, -16459,
		-16452, -16448, -16433, -16429, -16427, -16423, -16419, -16412, -16407, -16403, -16401, -16393, -16220, -16216, -16212, -16205,
		-16202, -16187, -16180, -16171, -16169, -16158, -16155, -15959, -15958, -15944, -15933, -15920, -15915, -15903, -15889, -15878,
		-15707, -15701, -15681, -15667, -15661, -15659, -15652, -15640, -15631, -15625, -15454, -15448, -15436, -15435, -15419, -15416,
		-15408, -15394, -15385, -15377, -15375, -15369, -15363, -15362, -15183, -15180, -15165, -15158, -15153, -15150, -15149, -15144,
		-15143, -15141, -15140, -15139, -15128, -15121, -15119, -15117, -15110, -15109, -14941, -14937, -14933, -14930, -14929, -14928,
		-14926, -14922, -14921, -14914, -14908, -14902, -14894, -14889, -14882, -14873, -14871, -14857, -14678, -14674, -14670, -14668,
		-14663, -14654, -14645, -14630, -14594, -14429, -14407, -14399, -14384, -14379, -14368, -14355, -14353, -14345, -14170, -14159,
		-14151, -14149, -14145, -14140, -14137, -14135, -14125, -14123, -14122, -14112, -14109, -14099, -14097, -14094, -14092, -14090,
		-14087, -14083, -13917, -13914, -13910, -13907, -13906, -13905, -13896, -13894, -13878, -13870, -13859, -13847, -13831, -13658,
		-13611, -13601, -13406, -13404, -13400, -13398, -13395, -13391, -13387, -13383, -13367, -13359, -13356, -13343, -13340, -13329,
		-13326, -13318, -13147, -13138, -13120, -13107, -13096, -13095, -13091, -13076, -13068, -13063, -13060, -12888, -12875, -12871,
		-12860, -12858, -12852, -12849, -12838, -12831, -12829, -12812, -12802, -12607, -12597, -12594, -12585, -12556, -12359, -12346,
		-12320, -12300, -12120, -12099, -12089, -12074, -12067, -12058, -12039, -11867, -11861, -11847, -11831, -11798, -11781, -11604,
		-11589, -11536, -11358, -11340, -11339, -11324, -11303, -11097, -11077, -11067, -11055, -11052, -11045, -11041, -11038, -11024,
		-11020, -11019, -11018, -11014, -10838, -10832, -10815, -10800, -10790, -10780, -10764, -10587, -10544, -10533, -10519, -10331,
		-10329, -10328, -10322, -10315, -10309, -10307, -10296, -10281, -10274, -10270, -10262, -10260, -10256, -10254 };

	// 395个字符串，每个字符串长度不超过6
	const char spell_dict[396][7] = { "a", "ai", "an", "ang", "ao", "ba", "bai", "ban", "bang", "bao", "bei", "ben", "beng", "bi", "bian", "biao",
		"bie", "bin", "bing", "bo", "bu", "ca", "cai", "can", "cang", "cao", "ce", "ceng", "cha", "chai", "chan", "chang", "chao", "che", "chen",
		"cheng", "chi", "chong", "chou", "chu", "chuai", "chuan", "chuang", "chui", "chun", "chuo", "ci", "cong", "cou", "cu", "cuan", "cui",
		"cun", "cuo", "da", "dai", "dan", "dang", "dao", "de", "deng", "di", "dian", "diao", "die", "ding", "diu", "dong", "dou", "du", "duan",
		"dui", "dun", "duo", "e", "en", "er", "fa", "fan", "fang", "fei", "fen", "feng", "fo", "fou", "fu", "ga", "gai", "gan", "gang", "gao",
		"ge", "gei", "gen", "geng", "gong", "gou", "gu", "gua", "guai", "guan", "guang", "gui", "gun", "guo", "ha", "hai", "han", "hang",
		"hao", "he", "hei", "hen", "heng", "hong", "hou", "hu", "hua", "huai", "huan", "huang", "hui", "hun", "huo", "ji", "jia", "jian",
		"jiang", "jiao", "jie", "jin", "jing", "jiong", "jiu", "ju", "juan", "jue", "jun", "ka", "kai", "kan", "kang", "kao", "ke", "ken",
		"keng", "kong", "kou", "ku", "kua", "kuai", "kuan", "kuang", "kui", "kun", "kuo", "la", "lai", "lan", "lang", "lao", "le", "lei",
		"leng", "li", "lia", "lian", "liang", "liao", "lie", "lin", "ling", "liu", "long", "lou", "lu", "lv", "luan", "lue", "lun", "luo",
		"ma", "mai", "man", "mang", "mao", "me", "mei", "men", "meng", "mi", "mian", "miao", "mie", "min", "ming", "miu", "mo", "mou", "mu",
		"na", "nai", "nan", "nang", "nao", "ne", "nei", "nen", "neng", "ni", "nian", "niang", "niao", "nie", "nin", "ning", "niu", "nong",
		"nu", "nv", "nuan", "nue", "nuo", "o", "ou", "pa", "pai", "pan", "pang", "pao", "pei", "pen", "peng", "pi", "pian", "piao", "pie",
		"pin", "ping", "po", "pu", "qi", "qia", "qian", "qiang", "qiao", "qie", "qin", "qing", "qiong", "qiu", "qu", "quan", "que", "qun",
		"ran", "rang", "rao", "re", "ren", "reng", "ri", "rong", "rou", "ru", "ruan", "rui", "run", "ruo", "sa", "sai", "san", "sang",
		"sao", "se", "sen", "seng", "sha", "shai", "shan", "shang", "shao", "she", "shen", "sheng", "shi", "shou", "shu", "shua",
		"shuai", "shuan", "shuang", "shui", "shun", "shuo", "si", "song", "sou", "su", "suan", "sui", "sun", "suo", "ta", "tai",
		"tan", "tang", "tao", "te", "teng", "ti", "tian", "tiao", "tie", "ting", "tong", "tou", "tu", "tuan", "tui", "tun", "tuo",
		"wa", "wai", "wan", "wang", "wei", "wen", "weng", "wo", "wu", "xi", "xia", "xian", "xiang", "xiao", "xie", "xin", "xing",
		"xiong", "xiu", "xu", "xuan", "xue", "xun", "ya", "yan", "yang", "yao", "ye", "yi", "yin", "ying", "yo", "yong", "you",
		"yu", "yuan", "yue", "yun", "za", "zai", "zan", "zang", "zao", "ze", "zei", "zen", "zeng", "zha", "zhai", "zhan", "zhang",
		"zhao", "zhe", "zhen", "zheng", "zhi", "zhong", "zhou", "zhu", "zhua", "zhuai", "zhuan", "zhuang", "zhui", "zhun", "zhuo",
		"zi", "zong", "zou", "zu", "zuan", "zui", "zun", "zuo" };
	const string pinyinprefix = "*";

	// 循环处理字节数组
	const int length = dest_chinese.length();
	string TempStr = "";
	for (int j = 0, chrasc = 0; j < length;) {
		// 非汉字处理
		if (dest_chinese.at(j) >= 0 && dest_chinese.at(j) < 128) {
			TempStr += dest_chinese.at(j);
			// 偏移下标
			j++;
			continue;
		}

		// 汉字处理
		if (j + 1 < length) {
			chrasc = dest_chinese.at(j) * 256 + dest_chinese.at(j + 1) + 256;
			if (chrasc > 0 && chrasc < 160) {
				// 非汉字
				TempStr += dest_chinese.at(j);
				// 偏移下标
				j++;
			}
			else {
				if (TempStr.size()) {
					out_py.push_back(TempStr);
					TempStr = "";
				}
				// 汉字
				for (int i = (sizeof(spell_value) / sizeof(spell_value[0]) - 1); i >= 0; --i) {
					// 查找字典
					if (i < 396) {
						if (spell_value[i] <= chrasc) {
							out_py.push_back(pinyinprefix + spell_dict[i]);
							break;
						}
					}
					else
						return 0;
				}
				// 偏移下标 （汉字双字节）
				j += 2;
			}
		}
		else {
			TempStr += dest_chinese.at(j);
			++j;
		}
	} // for end
	if (TempStr.size())
		out_py.push_back(TempStr);

	return true;
}

