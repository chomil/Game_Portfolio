#pragma once
#include "Base.h"
#include <string>
#include <codecvt>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem; 
using namespace std;

namespace stringHelper
{
	inline std::wstring s2ws(const std::string& str)
	{
		using convert_typeX = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.from_bytes(str);
	}

	inline std::string ws2s(const std::wstring& wstr)
	{
		using convert_typeX = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.to_bytes(wstr);
	}

	inline string ReplaceAll(const string& message, const string& pattern, const string& replace)
	{
		if (pattern.empty() == true) return message;
		string result = message;
		string::size_type pos = 0;
		string::size_type offset = 0;

		while ((pos = result.find(pattern, offset)) != string::npos)
		{
			result.replace(result.begin() + pos, result.begin() + pos + pattern.size(), replace);
			offset = pos + replace.size();
		}
		return result;
	}

	inline list<string> Split(const string& str, char delimiter)
	{
		list<string> result;
		std::stringstream ss(str);
		string tmp;

		while (std::getline(ss, tmp, delimiter)) {
			result.push_back(tmp);
		}
		return result;
	}
}


namespace PathHelper
{
	inline string CombinePath(const string& dest, const string& src)
	{
		string result = "";
		unsigned long long lastIndex = dest.length() - 1;
		if (src[0] != '/' && dest[lastIndex] != '/')
		{
			result = dest + '/' + src;
		}
		else if (src[0] == '/' && dest[lastIndex] == '/')
		{
			result = dest.substr(0, lastIndex) + src;
		}
		else
		{
			result = dest + src;
		}
		return result;
	}
	inline string GetUniqueFileName(const string& filePath)
	{
		auto p = fs::path(filePath);
		string extension = p.extension().string();
		string originFileName = stringHelper::ReplaceAll(p.filename().string(), extension, "");
		string fileName = originFileName;

		string targetDir = p.parent_path().string();



		auto path = CombinePath(targetDir, originFileName + extension);
		int  cnt = 0;
		bool is_exist = fs::exists(path);
		while (is_exist == true)
		{
			++cnt;
			fileName = originFileName + "_" + std::to_string(cnt);
			path = CombinePath(targetDir, fileName + extension);
			is_exist = fs::exists(path);
		}

		return path;
	}
}




namespace FileHelper
{
	inline bool ReadAllText(const string& path, string* out_str)
	{
		if (out_str == nullptr)
		{
			return false;
		}
		std::ifstream fin;
		fin.open(path);
		if (fin.is_open() == true)
		{
			std::stringstream ss;
			ss << fin.rdbuf();
			*out_str = ss.str();

			fin.close();
			return true;
		}
		else
		{
			fin.close();
			return false;
		}
	}

	inline bool WriteAllText(const string& path, const string& txt)
	{
		std::ofstream fout;
		fout.open(path);

		if (fout.is_open() == true)
		{
			fout << txt;
			fout.close();
			return true;
		}
		else
		{
			fout.close();
			return false;
		}
	}


	inline bool ReadAllByte(const string& path, void* data, unsigned long long dataSize)
	{
		std::ifstream fin;
		fin.open(path, std::ios::binary);
		if (fin.is_open() == true)
		{
			fin.read(reinterpret_cast<char*>(data), dataSize);
			fin.close();
			return true;
		}
		else
		{
			fin.close();
			return false;
		}
	}
	inline bool WriteAllByte(const string& path, void* data, unsigned long long dataSize)
	{
		std::ofstream fout;
		fout.open(path, std::ios::binary);

		if (fout.is_open() == true)
		{
			fout.write(reinterpret_cast<const char*>(data), dataSize);
			fout.close();
			return true;
		}
		else
		{
			fout.close();
			return false;
		}
	}


}

