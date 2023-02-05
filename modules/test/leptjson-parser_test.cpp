module;

#include<exception>
#include<string>
#include<format>

module leptjsonparser_test;

import Leptjson;

using namespace std;

using namespace leptjson;
using namespace leptjson::Exceptions;




namespace leptjsonparser_test
{

void LeptjsonParserTest::run() 
{
	TestParseLiteral();
	TestParseNoValue();
	TestParseInvaildValue();
	TestParseRootNotSingular();
}
void LeptjsonParserTest::TestParseLiteral()
{
	expect(Parse("null")->type, LeptType::LEPT_NULL);
	expect(Parse("true")->type, LeptType::LEPT_TRUE);
	expect(Parse("false")->type, LeptType::LEPT_FALSE);
}

void LeptjsonParserTest::TestParseNoValue()
{
	TestException("", "novalue");
	TestException(" ", "novalue");
}

inline string GetInvalidValueErrorMsg(const string& str)
{
	return format("invaild value! parse '{}' failed", str);
}

void LeptjsonParserTest::TestParseInvaildValue()
{
	TestException("nul", GetInvalidValueErrorMsg("null"));
	TestException("?", "invaild value");

}

void LeptjsonParserTest::TestParseRootNotSingular()
{
	TestException("null x", "root not singular");

}

void LeptjsonParserTest::TestException(string_view sv, string msg)
{
	try
	{
		Parse(sv);

		expect(true, true);
	}
	catch (exception& err)
	{
		expect(err.what(), msg);
	}
}

}