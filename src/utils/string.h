//
//  string.h
//  SwiftSnails
//
//  Created by Chunwei on 12/2/14.
//  Copyright (c) 2014 Chunwei. All rights reserved.
//
//
#ifndef SwiftSnails_utils_string_h_
#define SwiftSnails_utils_string_h_
#include "common.h"
namespace swift_snails {

std::string& trim(std::string &ss) {
    if (ss.empty()) return ss;
    ss.erase(0, ss.find_first_not_of(" \t\n"));
    ss.erase(ss.find_last_not_of(" \t\n") + 1);
    return ss;
}

std::vector<std::string> split(const std::string &s, const std::string &delim) {
    std::vector<std::string> cols;
    size_t last = 0;
    size_t index = s.find_first_of(delim, last);
    while (index != std::string::npos) {
        cols.push_back(s.substr(last, index - last));
        last = index + 1;
        index = s.find_first_of(delim, last);
    }
    if (index - last > 0) cols.push_back(s.substr(last, index - last));
    return std::move(cols);
}

std::pair<std::string, std::string> key_value_split(const std::string &s, const std::string &delim) {
    std::pair<std::string, std::string> keyvalue;
    size_t last = 0;
    // get key
    size_t index = s.find_first_of(delim, last);
    CHECK(index != std::string::npos);
    keyvalue.first = s.substr(last, index - last);
    last = index + 1;
    // get value
    index = s.find_first_of(delim, last);
    keyvalue.second = s.substr(last, index - last);
    return std::move(keyvalue);
}

bool headswith(const std::string &s, const std::string &head) {
    return s.substr(0, head.size()) == head;
}

template<typename... ARGS>
void format_string(std::string& s, const char* format, ARGS... args) {
    // get target size
    int len = snprintf(NULL, 0, format, args...);
    CHECK(len >= 0);
    size_t oldlen = s.length();
    s.resize(oldlen + len + 1);
    CHECK(snprintf(&s[oldlen], (size_t)len+1, format, args...) == len);
    s.resize(oldlen + len);
}

template<typename... ARGS>
std::string format_string(const char* format, ARGS... args) {
    std::string tmp_s;
    int len = snprintf(NULL, 0, format, args...);
    CHECK(len >= 0);
    tmp_s.resize(len + 1);
    CHECK(snprintf(&tmp_s[0], (size_t)len+1, format, args...) == len);
    tmp_s.resize(len);
    return std::move(tmp_s);
}

}; // end namespace swift_snails

#endif 
