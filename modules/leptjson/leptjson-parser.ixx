module;

#include<memory>
#include<string>
#include<iostream>
#include<vector>
#include <unordered_map>

export module Leptjson:Parser;

using namespace std;

namespace leptjson
{
export
enum class LeptType
{
	LEPT_NULL,
	LEPT_FALSE,
	LEPT_TRUE,
	LEPT_NUMBER,
	LEPT_STRING,
	LEPT_ARRAY,
	LEPT_OBJECT,
};


export
struct LeptValue
{
	LeptType type;
	union {
		double number;
		string str;
		vector<unique_ptr <LeptValue>>ary;
		unordered_multimap<string, unique_ptr <LeptValue>>obj;
	};


	bool operator ==(const LeptValue&lhs)const
	{
		if (type != lhs.type)return false;

		switch (type)
		{
		case leptjson::LeptType::LEPT_NULL:
			break;
		case leptjson::LeptType::LEPT_FALSE:
			break;
		case leptjson::LeptType::LEPT_TRUE:
			break;
		case leptjson::LeptType::LEPT_NUMBER:
			return number == lhs.number;
		case leptjson::LeptType::LEPT_STRING:
			return str == lhs.str;
		case leptjson::LeptType::LEPT_ARRAY:
			if (ary.size() != lhs.ary.size()) return false;
			for (int i = 0; i < ary.size(); ++i) if (*ary[i] != *lhs.ary[i]) return false;
			break;
		case leptjson::LeptType::LEPT_OBJECT:
			if (obj.size() != lhs.obj.size()) return false;
			for (auto&[key,value]:obj) if (!lhs.obj.contains(key)||*value!=*lhs.obj.find(key)->second) return false;
			break;
		default:
			break;
		}
		return true;
	}

	~LeptValue()
	{
		switch (type)
		{
		case leptjson::LeptType::LEPT_NULL:
			break;
		case leptjson::LeptType::LEPT_FALSE:
			break;
		case leptjson::LeptType::LEPT_TRUE:
			break;
		case leptjson::LeptType::LEPT_NUMBER:
			break;
		case leptjson::LeptType::LEPT_STRING:
			str.~string();
			break;
		case leptjson::LeptType::LEPT_ARRAY:
			ary.~vector();
			break;
		case leptjson::LeptType::LEPT_OBJECT:
			obj.~unordered_multimap();
			break;
		default:
			break;
		}
	}
};

export
ostream& operator<<(ostream& os, const LeptValue&lv)
{
	switch (lv.type)
	{
	case leptjson::LeptType::LEPT_NULL:
		os << "null";
		break;
	case leptjson::LeptType::LEPT_FALSE:
		os << "false";
		break;
	case leptjson::LeptType::LEPT_TRUE:
		os << "true";
		break;
	case leptjson::LeptType::LEPT_NUMBER:
		os << lv.number;
		break;
	case leptjson::LeptType::LEPT_STRING:
		os << lv.str;
		break;
	case leptjson::LeptType::LEPT_ARRAY:
		os << "ary is ";
		for (auto& a : lv.ary) os << *a<<"   ";
		break;
	case leptjson::LeptType::LEPT_OBJECT:
		os << "obj is ";
		for (auto& [key,value] : lv.obj) os << key<<"    "<<*value<<endl;
		break;
	default:
		break;
	}
	return os;
}


export unique_ptr <LeptValue> ParseFile(const string&filename);
export unique_ptr <LeptValue> Parse(string_view json);


	
}