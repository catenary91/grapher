#ifndef __OPTIONS__
#define __OPTIONS__

#include <vector>
#include <string>
#include <stdexcept>
using namespace std;

template <typename T>
T get_option(const vector<string>& args, const string& option) {
	return T();
}

template <>
string get_option<string>(const vector<string>& args, const string& option) {
    int len = args.size();
	for (int i=0; i<len; i++) {
		if (option.compare(args[i])==0) {
			if (i == len-1) break; // fail
			return args[i+1];
		}
	}
	throw "invalid argument: \"" + option + "\" option is necessary";
}

template<>
int get_option<int>(const vector<string>& args, const string& option) {
	string s = get_option<string>(args, option);
	try {
		return stoi(s);
	}
	catch (const invalid_argument&) {
		throw "invalid argument: \"" + s + "\" is not an integer";
	}
}

template <>
double get_option<double>(const vector<string>& args, const string& option) {
	string s = get_option<string>(args, option);
	try {
		return stod(s);
	}
	catch (const invalid_argument&) {
		throw "invalid argument: \"" + s + "\" is not a double";
	}
}

bool option_exist(const vector<string>& args, const string& option) {
    int len = args.size();
    for (int i=0; i<len; i++) {
        if (option.compare(args[i])==0) {
            if (i == len-1) break;
            return true; 
        }
    }
    return false;
}

#endif