#pragma once
#include <Windows.h>
#include <iostream>
#include <limits>
#include <random>

#undef max

using namespace std;

namespace global
{
	string randomstrings(int len);
	string gen_name();
	string get_uuid();
	string gen_plateform();
	string gen_delimiter();
}

