#include <string>



#include "Header.h"



//����������� ���������� ����� ��� ������� ��������� ����� ������
template <typename Key, typename Value>
bool const Registry::Map<Key, Value>::find(const int& key) const
{
    if constexpr (std::is_same_v<Key, int>)
    {
        if (this->key == key) return true;
        else return false;
    }
    else return false;
}

template <typename Key, typename Value>
bool const Registry::Map<Key, Value>::find(const double& key) const
{
    if constexpr (std::is_same_v<Key, double>)
    {
        if (this->key == key) return true;
        else return false;
    }
    else return false;
}

template <typename Key, typename Value>
bool const Registry::Map<Key, Value>::find(const std::string& key) const
{
    if constexpr (std::is_same_v<Key, std::string>)
    {
        if (this->key == key) return true;
        else return false;
    }
    else return false;
}

