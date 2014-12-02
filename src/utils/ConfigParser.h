//
//  ConfigParser.h
//  SwiftSnails
//
//  Created by Chunwei on 12/2/14.
//  Copyright (c) 2014 Chunwei. All rights reserved.
//

#ifndef SwiftSnails_utils_ConfigParser_h_
#define SwiftSnails_utils_ConfigParser_h_
#include "common.h"

namespace swift_snails {
/*
 * 解析简单的配置文件
 *
 * 基本的语法：
 * import common.conf
 * key: value
 * # comment
 * key: value
 */
class ConfigParser : VirtualObject {
public:
    ConfigParser(const std::string &conf_path) {
    }
    void register_config(const std::string &key, const std::string &value = "") {
        CHECK(!key.empty());
        CHECK(_dic.count(key) == 0) << "multi key:\t" << key << "is registered";
        _dic.insert({std::move(key), Item(value)});
    }
    const Item& get_config(const std::string &key) {
        auto p = _dic.find(key);
        CHECK(p != _dic.end()) << "no registered key:\t" << key << endl;
        return p->second;
    }
private:
    struct Item {
        std::string value;
        Item(const std::string &value) : value(value){ }
        int to_int32() const {
            CHECK(!value.empty());
            return std::move(stoi(value));
        }
        int to_string() const {
            CHECK(!value.empty());
            return value;
        }
        bool to_bool() const {
            CHECK(!value.empty());
            CHECK(value == "true" || value == "false");
            if(value == "true") return true;
            return false;
        }
    };
    std::map<std::string, Item> _dic;
};  // end class ConfigParser


}; // end namespace swift_snails

#endif
