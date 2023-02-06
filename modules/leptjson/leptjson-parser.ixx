module;

#include<memory>
#include<string>

export module Leptjson:Parser;

using namespace std;

namespace leptjson
{
export
enum class LeptType
{
	LEPT_NULL,
	LEPT_FALSE,
	LEPT_TRUE,
	LEPT_NUMBER,
	LEPT_STRING,
	LEPT_ARRAY,
	LEPT_OBJECT,
};
export
struct LeptValue
{
	LeptType type;
	double number;
	bool operator ==(const LeptValue&)const = default;
};

export unique_ptr <LeptValue> ParseFile(const string&filename);
export unique_ptr <LeptValue> Parse(string_view json);


	
}