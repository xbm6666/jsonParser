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
	TestParseNumber();
	TestParseNoValue();
	TestParseString();
	TestParseInvaildValue();
	TestParseRootNotSingular();
}
void LeptjsonParserTest::TestParseLiteral()
{
	expect(*Parse("null"), LeptValue{ LeptType::LEPT_NULL });
	expect(*Parse("true"), LeptValue{ LeptType::LEPT_TRUE });
	expect(*Parse("false"), LeptValue{ LeptType::LEPT_FALSE });
}

void LeptjsonParserTest::TestParseNumber()
{
	auto test = [this](double num, const string& str) {expect(*Parse(str), LeptValue{ LeptType::LEPT_NUMBER,num }); };
	test(0.0, "0");
	test(0.0, "-0");
	test(0.0, "-0.0");
	test(1.0, "1");
	test(-1.0, "-1");
	test(1.5, "1.5");
	test(-1.5, "-1.5");
	test(3.1416, "3.1416");
	test(1E10, "1E10");
	test(1e10, "1e10");
	test(1E+10, "1E+10");
	test(1E-10, "1E-10");
	test(-1E10, "-1E10");
	test(-1e10, "-1e10");
	test(-1E+10, "-1E+10");
	test(-1E-10, "-1E-10");
	test(1.234E+10, "1.234E+10");
	test(1.234E-10, "1.234E-10");
	//test(0.0, "1e-10000"); /* must underflow */

}

void LeptjsonParserTest::TestParseString()
{
	Test("\"\"", LeptValue{ .type = LeptType::LEPT_STRING,.str = "" });
	Test("\"Hello\"", LeptValue{ .type = LeptType::LEPT_STRING,.str = "Hello" });
	Test("\"Hello\\nWorld\"", LeptValue{ .type = LeptType::LEPT_STRING,.str ="Hello\nWorld" });
	Test("\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"", LeptValue{.type=LeptType::LEPT_STRING,.str= "\" \\ / \b \f \n \r \t" });
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
	TestException("?", "invalid stod argument");

}

void LeptjsonParserTest::TestParseRootNotSingular()
{
	TestException("null x", "root not singular");

}
void LeptjsonParserTest::Test(std::string_view sv,const LeptValue& value)
{
	expect(*Parse(sv), value);
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