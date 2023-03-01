#pragma once
#include "Base.h"
#include "Utility.h"


using namespace std;

template<class T>
class StorableVar
{
private:
	string Key;
	T      Value;
	bool   mIsInit = false;

public:
	StorableVar() = default;
	StorableVar(const string& key, const T& initData)
	{
		if (mIsInit == false)
		{
			Key = key;
			Init(key, initData);
			mIsInit = true;
		}
	}
	~StorableVar()
	{
		Save(Value);
	}
public:
	void SetValue(const T& value)
	{
		Save(value);
	}

	const T& GetValue() const {
		return Value;
	}
private:
	string GetDataPath() const {
		unsigned long long    hashCode = std::hash<string>()(Key);
		string cachePath = "../bin/SaveData/";
		return PathHelper::CombinePath(cachePath, std::to_string(hashCode));
	}
	void Init(const string& key, const T& initData)
	{
		string dataPath = GetDataPath();
		// 존재하면 읽기, 없으면 초기화 데이터
		if (fs::exists(dataPath) == true)
		{
			FileHelper::ReadAllByte(dataPath, (void*)&Value, sizeof(T));
		}
		else
		{
			FileHelper::WriteAllByte(dataPath, (void*)&initData, sizeof(T));
			Value = initData;
		}
	}
	void Save(const T& value)
	{
		if (mIsInit == true)
		{
			Value = value;
			FileHelper::WriteAllByte(GetDataPath(), (void*)&Value, sizeof(T));
		}
	}

};

template<>
class StorableVar<string>
{
private:
	string Key;
	string Value;
	bool mIsInit = false;


public:
	StorableVar() = default;
	StorableVar(const string& key, const string& initData)
	{
		if (mIsInit == false)
		{
			Init(key, initData);
			mIsInit = true;
		}
	}
	~StorableVar()
	{
		Save(Value);
	}
public:
	void SetValue(const string& value)
	{
		Save(value);
	}

	const string& GetValue() const {
		return Value;
	}
private:
	string GetDataPath() const {
		unsigned long long    hashCode = std::hash<string>()(Key);
		string cachePath = "../bin/SaveData/";
		return PathHelper::CombinePath(cachePath, std::to_string(hashCode));
	}
	void Init(const string& key, const string& initData)
	{
		string dataPath = GetDataPath();
		// 존재하면 읽기, 없으면 초기화 데이터
		if (fs::exists(dataPath) == true)
		{
			FileHelper::ReadAllText(dataPath, &Value);
		}
		else
		{
			FileHelper::WriteAllText(dataPath, initData);
			Value = initData;
		}
	}
	void Save(const string& value)
	{
		if (mIsInit == true)
		{
			Value = value;
			FileHelper::WriteAllText(GetDataPath(), Value);
		}
	}
};


using Storablestring = StorableVar<string>;
using StorableBool = StorableVar<bool>;


using StorableFloat = StorableVar<float>;
using StorableFloat2 = StorableVar<_float2>;
using StorableFloat3 = StorableVar<_float3>;
using StorableFloat4 = StorableVar<_float4>;


using StorableInt = StorableVar<int>;

using StorableUint = StorableVar<unsigned long long>;