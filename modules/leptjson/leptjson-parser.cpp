module;

#include<memory>
#include<string>
#include<fstream>
#include<format>
#include<vector>
#include<iostream>

module Leptjson:Parser;

import :Exceptions;

using namespace std;

namespace leptjson
{

struct LeptContent
{
	string_view json;
};

void LeptParseWhitespace(LeptContent& content)
{
	string_view& json = content.json;
	while (!json.empty()&&isspace(json.front()))json.remove_prefix(1);
}

unique_ptr <LeptValue> LeptParseNumber(LeptContent& content)
{
	string_view& json = content.json;
	size_t n;
	double num = stod(static_cast<string>(json), &n);
	json.remove_prefix(n);

	//TODO vaild number
	auto ret = make_unique<LeptValue>(LeptType::LEPT_NUMBER);
	(*ret).number = num;
	return ret;
	
}

inline string GetInvalidValueErrorMsg(string_view str)
{
	return format("invaild value! parse '{}' failed", str);
}


string LeptParseStringRaw(LeptContent& content)
{
	string_view& json = content.json;
	if(json.front()!='"') throw Exceptions::invaild_value_error(GetInvalidValueErrorMsg(json.substr(0,10)));
	int n = 1;
	string res;
	while (json.size()>n)
	{
		switch (json[n])
		{
		case '"':
		{
			json.remove_prefix(n+1);
			return res;
		}
		case '\\':
		{
			++n;
			if(n==json.size()) throw Exceptions::invaild_value_error(GetInvalidValueErrorMsg(json.substr(0, n)));
			switch (json[n]) {
				case '\"':res+='\"'; break;
				case '\\':res+='\\'; break;
				case '/': res+='/' ; break;
				case 'b': res+='\b'; break;
				case 'f': res+='\f'; break;
				case 'n': res+='\n'; break;
				case 'r': res+='\r'; break;
				case 't': res += '\t'; break;
				default:  
					if (json[n] < 0x20)
						throw Exceptions::invaild_value_error(GetInvalidValueErrorMsg(json.substr(0, n+1)));
					res += json[n];
					break;
			}
			break;
		}
		default:
			res += json[n];
			break;
		}

		++n;
	}
	throw Exceptions::invaild_value_error(GetInvalidValueErrorMsg(json.substr(0, n)));

}
unique_ptr <LeptValue> LeptParseString(LeptContent& content)
{
	return unique_ptr<LeptValue>(new LeptValue{ .type = LeptType::LEPT_STRING ,.str = LeptParseStringRaw(content) });
}

unique_ptr <LeptValue> LeptParseValue(LeptContent& content);
unique_ptr <LeptValue> LeptParseArray(LeptContent& content)
{
	string_view&json = content.json;
	auto msg = json;
	json.remove_prefix(1);// remove '['

	unique_ptr <LeptValue> p_ary(new LeptValue{ .type = LeptType::LEPT_ARRAY,.ary = {} });
	LeptParseWhitespace(content);
	if (json.empty()) throw Exceptions::invaild_value_error(GetInvalidValueErrorMsg(msg));
	if (json.front() == ']')
	{
		json.remove_prefix(1);
		return p_ary;
	}
	while (true)
	{
		LeptParseWhitespace(content);
		if (json.empty()) throw Exceptions::invaild_value_error(GetInvalidValueErrorMsg(msg));
		p_ary->ary.push_back(LeptParseValue(content));
		LeptParseWhitespace(content);
		switch (json.front())
		{
		case ',':
			json.remove_prefix(1);
			break;
		case ']':
			json.remove_prefix(1);
			return p_ary;
		default:
			throw Exceptions::parse_miss_comma_or_square_bracket_error(GetInvalidValueErrorMsg(msg));
		}
	}
}

unique_ptr <LeptValue> LeptParseObject(LeptContent& content)
{
	string_view& json = content.json;
	auto msg = json;
	json.remove_prefix(1);// remove '{'

	unique_ptr <LeptValue> obj(new LeptValue{ .type = LeptType::LEPT_OBJECT,.obj = {} });
	LeptParseWhitespace(content);
	if (json.empty()) throw Exceptions::invaild_value_error(GetInvalidValueErrorMsg(msg));
	if (json.front() == '}')
	{
		json.remove_prefix(1);
		LeptParseWhitespace(content);

		return obj;
	}
	while (true)
	{
		LeptParseWhitespace(content);
		if (json.empty()) throw Exceptions::invaild_value_error(GetInvalidValueErrorMsg(msg));
		string key = LeptParseStringRaw(content);
		LeptParseWhitespace(content);
		if(json.front()!=':')throw Exceptions::invaild_value_error(GetInvalidValueErrorMsg(msg));
		json.remove_prefix(1);
		LeptParseWhitespace(content);
		if (json.empty())throw Exceptions::invaild_value_error(GetInvalidValueErrorMsg(msg));
		obj->obj.insert({ std::move(key), LeptParseValue(content) });

		LeptParseWhitespace(content);
		if (json.empty())throw Exceptions::invaild_value_error(GetInvalidValueErrorMsg(msg));
		switch (json.front())
		{
		case ',':
			json.remove_prefix(1);
			break;
		case '}':
			json.remove_prefix(1);
			LeptParseWhitespace(content);
			return obj;
		default:
			throw Exceptions::parse_miss_comma_or_square_bracket_error(GetInvalidValueErrorMsg(msg));
		}
	}
}

unique_ptr <LeptValue> LeptParseLiteral(LeptContent& content, LeptType type,string_view value)
{
	string_view& json = content.json;
	if (!json.starts_with(value)) throw Exceptions::invaild_value_error(GetInvalidValueErrorMsg(value));
	json.remove_prefix(value.size());
	LeptParseWhitespace(content);
	return make_unique<LeptValue>(type);
}

unique_ptr <LeptValue> LeptParseValue(LeptContent& content)
{
	string_view& json = content.json;
	if(json.empty()) throw Exceptions::novalue_error(R"(novalue)");
	switch (json.front())
	{
	case 'n':return LeptParseLiteral(content, LeptType::LEPT_NULL, "null");
	case 't':return LeptParseLiteral(content, LeptType::LEPT_TRUE, "true");
	case 'f':return LeptParseLiteral(content, LeptType::LEPT_FALSE, "false");
	case '"':return LeptParseString(content);
	case '[':return LeptParseArray(content);
	case '{':return LeptParseObject(content);

	default:return LeptParseNumber(content);
	}
	std::unreachable();
}


unique_ptr <LeptValue> ParseFile(const string& filename)
{
	ifstream in(filename, std::ios::binary);
	string content((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
	
	return Parse(content);

}

unique_ptr <LeptValue> Parse(string_view json)
{
	LeptContent content{ .json = json };
	LeptParseWhitespace(content);
	auto ret= LeptParseValue(content);
	if (!content.json.empty())
	{
		throw Exceptions::root_not_singular_error(R"(root not singular)");
	}
	return ret;
}

string LeptStringifyString(const string& value)
{
	string ret;
	ret.push_back('"');
	for (char ch : value)
	{
		switch (ch)
		{
		case '\"':ret += ("\\\""); break;
		case '\\':ret += ("\\\\"); break;
		case '\b':ret += ("\\b"); break;
		case '\f':ret += ("\\f"); break;
		case '\n':ret += ("\\n"); break;
		case '\r':ret += ("\\r"); break;
		case '\t':ret += ("\\t"); break;
		case 'u':ret += ("\\u"); break;
		default:
			ret += ch;
		}
	}
	ret.push_back('"');

	return ret;
}

string LeptStringifyValue(const LeptValue& value)
{

	switch (value.type)
	{
		using enum LeptType;
	case LEPT_NULL:
		return "null";
	case LEPT_FALSE:
		return "false";
	case LEPT_TRUE:
		return "true";
	case LEPT_NUMBER:
		return to_string(value.number);
	case LEPT_STRING:
		return LeptStringifyString(value.str);
	case LEPT_ARRAY:
	{
		string ret;
		ret.push_back('[');
		for (int i = 0; i < value.ary.size(); ++i)
		{
			if (i > 0) ret += ",";
			ret += LeptStringifyValue(*value.ary[i]);
		}

		ret.push_back(']');
		return ret;

	}
	case LEPT_OBJECT:
	{
		string ret;
		ret.push_back('{');
		for (auto&[k,v]:value.obj)
		{
			ret += LeptStringifyString(k);
			ret += ":";
			ret += LeptStringifyValue(*v);
			ret += ',';
		}
		if (value.obj.empty()) ret.push_back('}');
		else ret.back() = '}';
		return ret;
	}
	default:
		std::unreachable();
		break;
	}
	return "";
}

string LeptStringify(const LeptValue& value)
{
	return LeptStringifyValue(value);
	
}


}