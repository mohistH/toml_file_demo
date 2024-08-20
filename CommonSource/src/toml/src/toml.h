/// ----------------------------------------------------
/// @file toml.h
/// @author oct (oct@tengden.com)
/// @brief toml中常用的一些函数的转换
/// @version 0.1
/// @date 2024-08-12
/// 
/// @copyright Copyright (c) 2024
/// 
/// ----------------------------------------------------

#ifndef TOML_H_
#define TOML_H_
#include <string>
#include <toml.hpp>

namespace oct 
{
    /// @brief 读取节点中的string
    /// @param node - 哪个节点
    /// @param key - 键值对中的键值是什么
    /// @return 
    static std::string toStdString(toml::node_view<toml::node>& node, const std::string& key)
    {
        bool isStr = node[key].is_string();
        if (false == isStr)
        {
            return std::string{};
        }

        return node[key].value<std::string>().value();
    }


    /// @brief  整形数据转换，内部支持 int64_t
    /// @tparam TValue 
    /// @param node 
    /// @param key 
    /// @return 
    template<typename TValue>
    static TValue toInt(toml::node_view<toml::node>& node, const std::string& key)
    {
        bool isInteger = node[key].is_integer();
        if (false == isInteger)
        {
            TValue ret = 0;
            return ret;
        }

        TValue ret = node[key].value<toml::inserter>.value().value();
        return ret;
    }

    /// @brief 浮点数转换
    /// @tparam TValue 可以是 float, double
    /// @param node 
    /// @param key 
    /// @return 
    template<typename TValue>
    static TValue toDouble(toml::node_view<toml::node>& node, const std::string& key)
    {
        bool isDB = node[key].is_floating_point();
        if (false == isDB)
        {
            TValue ret = 0.0;
            return ret;
        }

        TValue ret = node[key].value<double>.value().value();
        return ret;
    }


    /// @brief bool转换
    /// @param node 
    /// @param key 
    /// @return 
    static bool toBoolean(toml::node_view<toml::node>& node, const std::string& key)
    {
        bool isBool = node[key].is_boolean();
        if (false == isBool)
        {
            return false;
        }

        bool ret = node[key].value<bool>().value();
        return ret;
    }

    /// @brief 插入非string节点
    /// @param whichTable -哪个表
    /// @param key - 插入的数据值
    /// @param value -值
    template<typename T>
    static void tableNodeInsert(toml::table& whichTable, const std::string& key, const T& value)
    {
        whichTable.insert_or_assign(key, toml::value<T>(value));
    }

    /// @brief 数组插入值
    /// @tparam T - 值的类型
    /// @param arr 数据
    /// @param value -待插入值
    template<typename T>
    static void arrayInsert(toml::array& arr, const T& value)
    {
        arr.insert(arr.begin(), toml::value<T>(value));
    }

    /// @brief 数据插入数据偏特化-std::string
    /// @param arr -数据
    /// @param value -待插入的值
    template<>
    static void arrayInsert<std::string>(toml::array& arr, const std::string& value)
    {
        arr.insert(arr.begin(), toml::value<std::string>(value));
    }

    /// @brief 数据插入数据偏特化-bool
    /// @param arr -数据
    /// @param value -待插入的值
    template<>
    static void arrayInsert<bool>(toml::array& arr, const bool& value)
    {
        arr.insert(arr.begin(), toml::value<bool>(value));
    }

    /// @brief 数据插入数据偏特化-int64_t
    /// @param arr -数据
    /// @param value -待插入的值
    template<>
    static void arrayInsert<int64_t>(toml::array& arr, const int64_t& value)
    {
        arr.insert(arr.begin(), toml::value<int64_t>(value));
    }
    

}

#endif ///! TOML_H_