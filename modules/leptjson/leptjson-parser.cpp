module;

#include<memory>
#include<string>
#include<fstream>
#include<format>
#include<vector>


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
	return make_unique<LeptValue>(LeptType::LEPT_NUMBER,num);
	
}

inline string GetInvalidValueErrorMsg(string_view str)
{
	return format("invaild value! parse '{}' failed", str);
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
	if(!content.json.empty()) throw Exceptions::root_not_singular_error(R"(root not singular)");
	return ret;
}

}