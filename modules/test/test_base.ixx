module;


#include<iostream>
#include<format>


export module test_base;

using namespace std;

namespace leptjsonparser_test
{
export 
class TestBase
{
public:
	virtual void run() = 0;
	void print() ;
	
	void clear(){ test_pass_ = 0, test_count_ = 0; };

protected:
	bool equal(const auto& res, const auto& value)
	{
		return res == value;
	}
	bool equal(double res,double value)
	{
		return abs(res - value)<1e-5;
	}
	void expect(const auto& res, const auto& value)
	{
		++test_count_;
		if (equal(res ,value))
		{
			++test_pass_;

			std::cout << std::format("test {} successed!", test_count_) << std::endl;

		}
		else
		{
			std::cout << std::format("test {} failed", test_count_) << std::endl;
			std::cout << "input value is " << res << std::endl;
			std::cout <<"parsed value is " << value << std::endl;
			std::cout << std::endl;
		}
	};

	
private:
	int test_pass_ = 0;
	int test_count_ = 0;

	
};
}