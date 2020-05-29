#include <iostream>
#include <vector>
#include <sstream> 
#include <algorithm>
#include "argdict.h"


using namespace std;

bool argdict::add_int_arg(string argname, int argval){
    if (int_args.find(argname) != int_args.end()) return false;
    int_args[argname] = argval;
    return true;
}

bool argdict::add_dbl_arg(string argname, double argval){
    if (double_args.find(argname) != double_args.end()) return false;
    double_args[argname] = argval;
    return true;
}

bool argdict::add_str_arg(string argname, string argval){
    if (string_args.find(argname) != string_args.end()) return false;
    string_args[argname] = argval;
    return true;
}

int argdict::get_int_arg(string argname){
    if (int_args.find(argname) == int_args.end())
        throw logic_error("argdict::get_int_arg: argument: \""+argname+"\" of type int is not present in the argdict\n");
    
    return int_args[argname];
}

double argdict::get_dbl_arg(string argname){
    if (double_args.find(argname) == double_args.end())
        throw logic_error("argdict::get_dbl_arg: argument: \""+argname+"\" of type double is not present in the argdict\n");
    return double_args[argname];
}

string argdict::get_str_arg(string argname){
    if (string_args.find(argname) == string_args.end())
        throw logic_error("argdict::get_str_arg: argument: \""+argname+"\" of type string is not present in the argdict\n");
    return string_args[argname];
}

bool argdict::contains_int(string argname){
    return (int_args.find(argname) != int_args.end());
}

bool argdict::contains_dbl(string argname){
    return (double_args.find(argname) != double_args.end());
}

bool argdict::contains_str(string argname){
    return (string_args.find(argname) != string_args.end());
}

string argdict::print(){
    std::stringstream buffer;
    int ncols = 2;
    buffer << "printing int args" << endl;
    vector<string> int_keys;
    for (auto child: int_args){
        int_keys.push_back(child.first);
    }
    sort(int_keys.begin(), int_keys.end());
    for (auto key: int_keys){
        buffer << key << "\t" << int_args[key] << endl;
    }
    
    buffer << "printing dbl args" << endl;
    vector<string> dbl_keys;
    for (auto child: double_args){
        dbl_keys.push_back(child.first);
    }
    sort(dbl_keys.begin(), dbl_keys.end());
    for (auto key: dbl_keys){
        buffer << key << "\t" << double_args[key] << endl;
    }

    buffer << "printing string args" << endl;
    vector<string> str_keys;
    for (auto child: string_args){
        str_keys.push_back(child.first);
    }
    sort(str_keys.begin(), str_keys.end());
    for (auto key: str_keys){
        buffer << key << "\t" << string_args[key] << endl;
    }
    return buffer.str();
}