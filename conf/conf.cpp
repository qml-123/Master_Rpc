//
// Created by ByteDance on 2022/5/22.
//

#include "conf.h"
void Backtrace(std::vector<std::string>& bt, int size, int skip) {
    void** array = (void**)malloc((sizeof(void*) * size));
    size_t s = ::backtrace(array, size);
    
    char** strings = backtrace_symbols(array, s);
    if(strings == NULL) {
        std::cout << "Backtrace_symbols error" << std::endl;
        free(strings);
        free(array);
        
        return;
    }
    
    for(size_t i = skip; i < s; ++i) {
        bt.push_back(strings[i]);
    }
    free(strings);
    free(array);
}

std::string BacktraceToString(int size, int skip, const std::string& prefix) {
    std::vector <std::string> bt;
    Backtrace(bt, size, skip);
    std::stringstream ss;
    for (size_t i = 0; i < bt.size(); ++i) {
        ss << prefix << bt[i] << std::endl;
    }
    return ss.str();
}