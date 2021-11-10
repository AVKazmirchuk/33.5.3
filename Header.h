#pragma once



#include <iostream>
#include <typeinfo>
#include <vector>



class Registry
{
private:
    class MapBase
    {
    public:
        virtual const bool find(const int&) const = 0;
        virtual const bool find(const double&) const = 0;
        virtual const bool find(const std::string&) const = 0;

        virtual void print() = 0;

        virtual const std::type_info& keyType() const = 0;
        virtual const std::type_info& valueType() const = 0;

        virtual ~MapBase() {}
    };

    template <typename Key, typename Value>
    class Map : public MapBase
    {
    public:
        const Key key;
        Value value;
    public:
        Map(const Key& _key, const Value& _value) : key{ _key }, value{ _value } {}

        //Определение совпадение ключа при условии равенства типов ключей
        bool const find(const int& key) const override;
        

        bool const find(const double& key) const override;
        

        bool const find(const std::string& key) const override;
        

        //Вывод элемента
        void print() override
        {
            std::cout << "Key: " << key << ", Value: " << value << '\n';
        }

        //Определение типа ключа
        const std::type_info& keyType() const override
        {
            return typeid(key);
        }

        //Определение типа значения
        const std::type_info& valueType() const override
        {
            return typeid(value);
        }
    };

    std::vector<MapBase*> maps;

public:
    template <typename Key, typename Value>
    void add(const Key& key, const Value& value)
    {
        maps.push_back(new Map(key, value));
    }

    template <typename Key, typename Value>
    void add(Key&& key, Value&& value)
    {
        maps.push_back(new Map(key, value));
    }

    ~Registry()
    {
        for (auto& elem : maps)
        {
            delete elem;
        }
    }

    //Удаление элемента по ключу
    template <typename Key>
    void remove(const Key& key)
    {
        maps.erase(std::remove_if(maps.begin(), maps.end(),
            [&](auto& elem)
            {
                return elem->find(key);
            }),
            maps.end());
    }

    //Поиск значения по ключу и вывод элемента
    template <typename Key>
    void find(const Key& key)
    {
        for (const auto& elem : maps)
        {
            if (elem->find(key)) elem->print();
        }
    }

    //Изменение значения по ключу, при условии совпадения старого и нового типа значения
    template <typename Key, typename Value>
    void changeValue(const Key& key, const Value& value)
    {
        for (auto& elem : maps)
        {
            if (elem->valueType() == typeid(Value) && elem->find(key))
            {
                dynamic_cast<Map<Key, Value>*>(elem)->value = value;
            }
        }
    }
    
    //Вывод на печать
    void print()
    {
        for (const auto& elem : maps)
        {
            elem->print();
        }
    }
};