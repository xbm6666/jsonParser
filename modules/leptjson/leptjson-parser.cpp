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

unique_ptr <LeptValue> LeptParseString(LeptContent& content)
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
			return unique_ptr<LeptValue>( new LeptValue{ .type=LeptType::LEPT_STRING ,.str=res});
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
			throw Exceptions::parse_miss_comma_or_square_bracket_error(GetInvalidValueErrorMsg(msg));;
		}
	}
	


}

unique_ptr <LeptValue> LeptParseLiteral(LeptContent& content, LeptType type,string_view value)
{
	string_view& json = content.json;
	if (!json.starts_with(value)) throw Exceptions::invaild_value_error(GetInvalidValueErrorMsg(value));
	json.remove_prefix(value.size());
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

}