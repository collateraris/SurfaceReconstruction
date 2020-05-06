#pragma once

#include "SafePtr.h"

#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <vector>
#include <utility>

namespace Experiments::Threads 
{
	class SafeMapTest
	{
	public:
		SafePtr<std::map<std::string, std::pair<std::string, int> >> mSafeMapStrings;

		void func(decltype(mSafeMapStrings) safeMapStrings);

		void testRun();
	};

	void SafeMapTest::func(decltype(mSafeMapStrings) safeMapStrings)
	{
		(*safeMapStrings)["apple"].first = "fruit";
		(*safeMapStrings)["potato"].first = "vegetable";

		for (size_t i = 0; i < 10000; ++i)
		{
			safeMapStrings->at("apple").second++;
			safeMapStrings->find("potato")->second.second++;
		}

		auto const readonlySafeMapString = safeMapStrings;

		std::cout << "potato is " << readonlySafeMapString->at("potato").first <<
			" " << readonlySafeMapString->at("potato").second <<
			", apple is " << readonlySafeMapString->at("apple").first <<
			" " << readonlySafeMapString->at("apple").second << std::endl;
	}

	void SafeMapTest::testRun()
	{
		std::vector<std::thread> vecThread(10);
		for (auto& i : vecThread)
		{
			i = std::move(std::thread(&SafeMapTest::func, this, mSafeMapStrings));
		}

		for (auto& i : vecThread)
		{
			i.join();
		}
	}
}