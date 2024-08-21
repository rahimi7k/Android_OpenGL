#include "test.h"



Test::Test() {
	Log::e("Teeeeeeeeeeeeeeeeeeeeest");
}


Test::~Test() {
	delete matrix; // Clean up dynamically allocated memory
	matrix = nullptr; // Optional: Set to nullptr for safety
}


void Test::SSSSSSSS(std::string texts) {
	text = texts;
	Log::e("Teeeeeeeeeeeeeeeeeeeeest " + text);

}
void Test::dddddddddd() {
	Log::e("ddddd " + text);

}

