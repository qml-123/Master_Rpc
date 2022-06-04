//
// Created by ByteDance on 2022/5/22.
//

#ifndef MASTER_RPC_CONF_H
#define MASTER_RPC_CONF_H
#include <string>
#include <iostream>
#include <assert.h>
#include <vector>
#include <string.h>
#include <sstream>
#include <execinfo.h>
#include <unistd.h>
#include "../thread/thread.h"

#define QML_ASSERT(x) \
	if(!(x)) { \
		std::cout << "ASSERTION: " #x\
			<< "\nbacktrace:\n" \
			<< BacktraceToString(100, 2, "    ") << std::endl; \
		assert(x); \
	}

#define QML_ASSERT2(x, w) \
	if(!(x)) { \
		std::cout << "ASSERTION: " #x\
			<< "\n" << w \
			<< "\nbacktrace:\n" \
			<< BacktraceToString(100, 2, "    ") << std::endl; \
		assert(x); \
	}

void Backtrace(std::vector<std::string>& bt, int size, int skip);
std::string BacktraceToString(int size, int skip, const std::string& prefix);
pid_t GetThreadId();

void print(const std::string& st);
#endif //MASTER_RPC_CONF_H
