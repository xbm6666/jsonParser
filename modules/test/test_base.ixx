module;


#include<iostream>
#include<format>


export module test_base;


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
	void expect(const auto& res, const auto& value)
	{
		++test_count_;

		if (res == value)
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