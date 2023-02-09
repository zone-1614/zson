#pragma once

#include <string>
#include <variant>
#include <map>
#include <vector>
#include <memory>
#include <sstream>
#include <fstream>

namespace zson {

enum class DataType {
    NUMBER, STRING, BOOLEAN, ARRAY, OBJECT, NIL
};

class json;
using json_object = std::map<std::string, std::shared_ptr<json>>;
using json_array = std::vector<std::shared_ptr<json>>;
using var = std::variant<std::monostate, double, std::string, bool, std::shared_ptr<json_array>, std::shared_ptr<json_object>>;

class json {
public:
    json() : type_(DataType::NIL) { }

    void set_var(double d) {
        var_ = d;
        type_ = DataType::NUMBER;
    }

    void set_var(const std::string& s) {
        var_ = s;
        type_ = DataType::STRING;
    }

    void set_var(bool b) {
        var_ = b;
        type_ = DataType::BOOLEAN;
    }

    void set_var(std::shared_ptr<json_array> arr) {
        var_ = arr;
        type_ = DataType::ARRAY;
    }

    void set_var(std::shared_ptr<json_object> obj) {
        var_ = obj;
        type_ = DataType::OBJECT;
    }

    json& operator=(double d) {
        var_ = d;
        type_ = DataType::NUMBER;
        return *this;
    }

    json& operator=(const std::string& s) {
        var_ = s;
        type_ = DataType::STRING;
        return *this;
    }

    std::shared_ptr<json> operator[](int i) {
        return std::get<4>(var_)->at(i);
    }

    std::shared_ptr<json> operator[](const std::string& key) {
        return std::get<5>(var_)->at(key);
    }
    
    std::string toString(int tabsize = 2) const {
        return toString(1, tabsize);
    }

    friend std::ostream& operator<<(std::ostream& os, const json& j) {
        os << j.toString();
        return os;
    }

    void save(const std::string& path, int tabsize = 2) {
        std::ofstream ofs(path);
        auto str = toString(tabsize);
        std::stringstream ss(str);
        ofs << ss.rdbuf();
        ofs.close();
    }
    
private:
    var var_;
    DataType type_;

    std::string toString(int depth, int tabsize) const {
        std::string tab = "";
        for (int i = 0; i < tabsize; i++) {
            tab += " ";
        }
        std::string prefix = "";
        std::string subprefix = "";
        for (int i = 0; i < depth; i++) {
            prefix += tab;
            if (i != depth - 1)
                subprefix += tab;
        }
        std::stringstream ss;
        switch (type_) {
            case DataType::NUMBER: {
                ss << std::to_string(std::get<1>(var_));
                break;
            }
            case DataType::STRING: {
                ss << "\"" << std::get<2>(var_) << "\"";
                break;
            }
            case DataType::BOOLEAN: {
                if (std::get<3>(var_)) {
                    ss << "true";
                } else {
                    ss << "false";
                }
                break;
            }
            case DataType::ARRAY: {
                ss << "[\n" << prefix;
                auto arr = *std::get<4>(var_);
                for (int i = 0; i < arr.size(); i++) {
                    ss << arr[i]->toString(depth + 1, tabsize);
                    if (i != arr.size() - 1) {
                        ss << ", \n" << prefix;
                    }
                }
                ss << "\n" << subprefix << "]";
                break;
            }
            case DataType::OBJECT: {
                ss << "{\n";
                auto obj = *std::get<5>(var_);
                for (auto it = obj.begin(); it != obj.end(); it++) {
                    ss << prefix << '"' << it->first << "\": " << it->second->toString(depth + 1, tabsize);
                    if (it != --obj.end()) {
                        ss << ", \n";
                    }
                }
                ss << "\n" << subprefix << "}";
                break;
            }
            case DataType::NIL: {
                ss << "null";
            }
        }
        return ss.str();
    }
};

}
