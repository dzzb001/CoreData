#pragma once
#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <thread>
#include <cstring>

//属性value 类型
enum AttrisType {
    INT_TYPE,
    STR_TYPE,
    BOOL_TYPE,
    DOUBLE_TYPE
};

//业务属性value类
class Attributes {
public:
    Attributes() {};
    ~Attributes() {
    }

    Attributes(const Attributes& other) {
        nType = other.nType;
        switch (nType) {
        case INT_TYPE:
            nValue = other.nValue;
            break;
        case STR_TYPE:
            strValue = other.strValue;
            break;
        case BOOL_TYPE:
            bValue = other.bValue;
            break;
        case DOUBLE_TYPE:
            dValue = other.dValue;
            break;
        default:
            break;
        }
    }

    bool operator==(const Attributes& other) const {
        if (nType != other.nType) {
            return false;
        }
        switch (nType) {
        case INT_TYPE:
            return nValue == other.nValue;
        case STR_TYPE: {
            {
                return !strcmp(strValue.c_str(), other.strValue.c_str());
            }
        }
        case BOOL_TYPE:
            return bValue == other.bValue;
        case DOUBLE_TYPE:
            return dValue == other.dValue;
        default:
            return false;
        }
    }

    bool operator!=(const Attributes& other) const {
        if (nType != other.nType) {
            return true;
        }
        switch (nType) {
        case INT_TYPE:
            return nValue == other.nValue ? false : true;
        case STR_TYPE: {
            {
                return strcmp(strValue.c_str(), other.strValue.c_str());
            }
        }
        case BOOL_TYPE:
            return bValue == other.bValue ? false : true;;
        case DOUBLE_TYPE:
            return dValue == other.dValue ? false : true;;
        default:
            return false;
        }
    }

    // 重载 = 运算符
    Attributes& operator=(const Attributes& other) {
        if (this != &other) {
            // 执行赋值操作
            nType = other.nType;
           
            switch (nType) {
            case INT_TYPE:
                nValue = other.nValue;
                break;
            case STR_TYPE: {
                strValue = other.strValue;
            }
              break;
            case BOOL_TYPE:
                bValue = other.bValue;
                break;
            case DOUBLE_TYPE:
                dValue = other.dValue;
                break;
            default:
                break;
            }
        }

        return *this;
    }

    // 重载 < 运算符
    bool operator<(const Attributes& other) const {
        
        // 首先比较 nType
        if (nType < other.nType) {
            return true;
        }
        else if (nType > other.nType) {
            return false;
        }

        // 如果 nType 相等，则根据具体类型的属性进行比较
        switch (nType) {
        case INT_TYPE:
            return nValue < other.nValue;
        case STR_TYPE: {
            return strcmp(strValue.c_str(), other.strValue.c_str());
        }
        case BOOL_TYPE:
            return bValue < other.bValue;
        case DOUBLE_TYPE:
            return dValue < other.dValue;
        default:
            return false;
        }
    }

    struct Hash {
        size_t operator()(const Attributes& attr) const {
            switch (attr.nType) {
            case INT_TYPE:
                return std::hash<int>()(attr.nValue);
            case STR_TYPE:
                return std::hash<std::string>()(attr.strValue);
            case BOOL_TYPE:
                return std::hash<bool>()(attr.bValue);
            case DOUBLE_TYPE:
                return std::hash<double>()(attr.dValue);
            default:
                return 0;
            }
        }
    };

    void setIntValue(int value) {
        nType = INT_TYPE;
        nValue = value;
    }

    void setStringValue(const std::string& value) {
        nType = STR_TYPE;
        strValue = value;
    }

    void setBoolValue(bool value) {
        nType = BOOL_TYPE;
        bValue = value;
    }

    void setDoubleValue(double value) {
        nType = DOUBLE_TYPE;
        dValue = value;
    }

    AttrisType getType() const {
        return nType;
    }

    int getIntValue() const {
        return nValue;
    }

    std::string getStringValue() const {
        return strValue;
    }

    bool getBoolValue() const {
        return bValue;
    }

    double getDoubleValue() const {
        return dValue;
    }

private:
    union {
        int nValue;
        bool bValue;
        double dValue;
    };
    std::string strValue;
public:
    AttrisType nType;
};

//外部属性管理类
class CBusinessAttributesManager
{
public:
	CBusinessAttributesManager();
	~CBusinessAttributesManager();

	int addBusAttr(std::string strKey, Attributes value, std::string strId);
    int findIDsByAttri(std::string strKey, Attributes value, std::map<std::string, std::string> & retMap,bool reverse = false);

    //取交集ID
    int findIDsByAttri(std::vector<std::pair<std::string, Attributes>>, std::map<std::string, std::string>& retMap);

    void deleteAttri(std::string strKey, Attributes value, std::string id);
private:
	std::unordered_map<std::string, std::unordered_map<Attributes,std::vector<std::string>, Attributes::Hash>> m_mapAttributes; //<业务属性名称， <业务属性值，元素id数组>>
};