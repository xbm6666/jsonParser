module;

#include<string>
export module leptjsonparser_test;

import Leptjson;
import test_base;

namespace leptjsonparser_test
{
export
class LeptjsonParserTest: public TestBase
{
public:
	void TestParseLiteral();
	void TestParseNumber();
	void TestParseString();
	void TestParseArray();

	void TestParseNoValue();
	void TestParseInvaildValue();
	void TestParseRootNotSingular();
	virtual void run() override;

private:
	void TestException(std:: string_view sv, std::string msg);
	void Test(std::string_view sv, const leptjson::LeptValue& value);

};

}
