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
    ss.erase(0, ss.find_first_not_of(" \t\n\r"));
    ss.erase(ss.find_last_not_of(" \t\n\r") + 1);
    return ss;
}

/*
 * split string to vector
 *
 * example:
 *  vector<string> cols = split("hello world@bb", " @");
 *
 * Attention: 
 *  use first_not_of
 *  the real delim is either char in a string
 */
std::vector<std::string> split(const std::string &s, const std::string &delim) {
    std::vector<std::string> cols;
    size_t last = 0;
    size_t start = s.find_first_not_of(delim, last);
    while (start != std::string::npos) {
        last = s.find_first_of(delim, start);
        if(last == std::string::npos) {
            cols.push_back(s.substr(start));
        } else {
            cols.push_back(s.substr(start, last - start));
        }
        start = s.find_first_not_of(delim, last);
    }
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
    //index = s.find_first_of(delim, last);
    keyvalue.second = s.substr(last);
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


class LineFileReader : public VirtualObject {
public:
    ~LineFileReader() {
        ::free(_buffer);
    }
    char* getline(FILE* f) {
        return this->getdelim(f, '\n');
    }
    char* getdelim(FILE* f, char delim) {
        ssize_t ret = ::getdelim(&_buffer, &_buf_size, delim, f);
        if (ret >= 0) {
            if (ret >= 1 && _buffer[ret - 1] == delim) {
                _buffer[--ret] = 0;
            }
            _length = (size_t)ret;
            return _buffer;
        } else {
            _length = 0;
            CHECK(feof(f));
            return NULL;
        }
    }
    char* get() {
        return _buffer;
    }
    size_t length() {
        return _length;
    }
private:
    char* _buffer = NULL;
    size_t _buf_size = 0;
    size_t _length = 0;
};


}; // end namespace swift_snails

#endif 
