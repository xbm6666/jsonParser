module;

#include<memory>
#include<string>
#include<iostream>

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
			break;
		case leptjson::LeptType::LEPT_OBJECT:
			break;
		default:
			break;
		}
		return true;
	}

	void SetString(const string& str)
	{
		this->type = LeptType::LEPT_STRING;
		new(&this->str) string(str);
	}

	~LeptValue()
	{
		if (type == LeptType::LEPT_STRING) str.~str();
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
		break;
	case leptjson::LeptType::LEPT_OBJECT:
		break;
	default:
		break;
	}
	return os;
}


export unique_ptr <LeptValue> ParseFile(const string&filename);
export unique_ptr <LeptValue> Parse(string_view json);


	
}