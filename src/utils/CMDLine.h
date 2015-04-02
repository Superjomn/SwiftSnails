/*
    Command line parser
    Author:   Steffen Rendle, http://www.libfm.org/
    modified: 2012-01-04
    Copyright 2010-2012 Steffen Rendle, see license.txt for more information
*/

#ifndef CMDLINE_H_
#define CMDLINE_H_

#include <map>
#include <vector>
namespace fms {

std::vector<std::string> tokenize(const std::string& str, const std::string& delimiter) {
    std::vector<std::string> result;
    std::string::size_type lastPos = str.find_first_not_of(delimiter, 0);

    std::string::size_type pos = str.find_first_of(delimiter, lastPos);
    while (std::string::npos != pos || std::string::npos != lastPos) {
        result.push_back(str.substr(lastPos, pos - lastPos));
            lastPos = str.find_first_not_of(delimiter, pos);
            pos = str.find_first_of(delimiter, lastPos);
    }
    return result;
}

class CMDLine {
    protected:
        std::map< std::string, std::string > help;
        std::map< std::string, std::string > value;
        bool parse_name(std::string& s) {
            if ((s.length() > 0) && (s[0] == '-')) {
                if ((s.length() > 1) && (s[1] == '-')) {
                    s = s.substr(2);
                } else {
                    s = s.substr(1);
                }
                return true;
            } else {
                return false;
            }       
        }

    public:
        std::string delimiter;

        CMDLine(int argc, char **argv) {
            delimiter = ";,";
            int i = 1;
            while (i < argc) {
                std::string s(argv[i]);
                if (parse_name(s)) {
                    if (value.find(s) != value.end()) {
                        throw "the parameter " + s + " is already specified";                           
                    }
                    if ((i+1) < argc) {
                        std::string s_next(argv[i+1]);
                        if (! parse_name(s_next)) {
                            value[s] = s_next;
                            i++;
                        } else {
                            value[s] = "";
                        }
                    } else {
                        value[s] = "";
                    }
                } else {
                    throw "cannot parse " + s;
                }
                i++;
            }
        }

        void setValue(std::string parameter, std::string value) {
            this->value[parameter] = value;
        }

        bool hasParameter(std::string parameter) {
            return (value.find(parameter) != value.end());
        }

        
        void print_help() {
            for (std::map< std::string, std::string >::const_iterator pv = help.begin(); pv != help.end(); ++pv) {
                std::cout << "-" << pv->first;
                for (int i=pv->first.size()+1; i < 16; i++) { std::cout << " "; } 
                std::string s_out = pv->second;
                while (s_out.size() > 0) {
                    if (s_out.size() > (72-16)) {
                        size_t p = s_out.substr(0, 72-16).find_last_of(" \t");
                        if (p == 0) {
                            p = 72-16;
                        }
                        std::cout << s_out.substr(0, p) << std::endl;
                        s_out = s_out.substr(p+1, s_out.length()-p);            
                    } else {
                        std::cout << s_out << std::endl;
                        s_out = "";  
                    }
                    if (s_out.size() > 0) {
                        for (int i=0; i < 16; i++) { std::cout << " "; }
                    }
                }
            }
        }
        const std::string& registerParameter(const std::string& parameter, const std::string& help) {
            this->help[parameter] = help;
            return parameter;
        }

        void checkParameters() {
            // make sure there is no parameter specified on the cmdline that is not registered:
            for (std::map< std::string, std::string >::const_iterator pv = value.begin(); pv != value.end(); ++pv) {
                if (help.find(pv->first) == help.end()) {
                    throw "the parameter " + pv->first + " does not exist";
                }
            }
        }

        const std::string& getValue(const std::string& parameter) {
            return value[parameter];
        }

        const std::string& getValue(const std::string& parameter, const std::string& default_value) {
            if (hasParameter(parameter)) {
                return value[parameter];
            } else {
                return default_value;
            }
        }

        double getValue(const std::string& parameter, const double& default_value) {
            if (hasParameter(parameter)) {
                return atof(value[parameter].c_str());
            } else {
                return default_value;
            }
        }

        int getValue(const std::string& parameter, const int& default_value) {
            if (hasParameter(parameter)) {
                return atoi(value[parameter].c_str());
            } else {
                return default_value;
            }
        }

        uint getValue(const std::string& parameter, const uint& default_value) {
            if (hasParameter(parameter)) {
                return atoi(value[parameter].c_str());
            } else {
                return default_value;
            }
        }

        std::vector<std::string> getStrValues(const std::string& parameter) {
            std::vector<std::string> result = tokenize(value[parameter], delimiter);
            return result;
        }
        std::vector<int> getIntValues(const std::string& parameter) {
            std::vector<int> result; 
            std::vector<std::string> result_str = getStrValues(parameter);
            result.resize(result_str.size());
            for (uint i = 0; i < result.size(); i++) {
                result[i] = atoi(result_str[i].c_str());
            }
            return result;
        }
        std::vector<double> getDblValues(const std::string& parameter) {
            std::vector<double> result; 
            std::vector<std::string> result_str = getStrValues(parameter);
            result.resize(result_str.size());
            for (uint i = 0; i < result.size(); i++) {
                result[i] = atof(result_str[i].c_str());
            }
            return result;
        }
        std::vector<uint> getUIntValues(const std::string& parameter) {
            std::vector<uint> result; 
            std::vector<std::string> result_str = getStrValues(parameter);
            result.resize(result_str.size());
            for (uint i = 0; i < result.size(); i++) {
                result[i] = atoi(result_str[i].c_str());
            }
            return result;
        }
};

}; // end namespace fms


#endif /*CMDLINE_H_*/
