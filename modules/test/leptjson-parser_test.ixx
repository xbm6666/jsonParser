module;

#include<string>
export module leptjsonparser_test;

import test_base;

namespace leptjsonparser_test
{
export
class LeptjsonParserTest: public TestBase
{
public:
	void TestParseLiteral();
	void TestParseNoValue();
	void TestParseInvaildValue();
	void TestParseRootNotSingular();
	virtual void run() override;

private:
	void TestException(std:: string_view sv, std::string msg);

};

}
