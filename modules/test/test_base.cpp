module;

#include<iostream>
#include<string>
#include<format>

module test_base;

using namespace std;

namespace leptjsonparser_test
{
void TestBase::print()
{
	cout<<std::format("{} passed, {} all, pass ratio is {}%", test_pass_, test_count_, 100.0 * test_pass_ / test_count_)<<endl;
};

}