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

        //Îïðåäåëåíèå ñîâïàäåíèå êëþ÷à ïðè óñëîâèè ðàâåíñòâà òèïîâ êëþ÷åé
        bool const find(const int& key) const override;
        

        bool const find(const double& key) const override;
        

        bool const find(const std::string& key) const override;
        

        //Âûâîä ýëåìåíòà
        void print() override
        {
            std::cout << "Key: " << key << ", Value: " << value << '\n';
        }

        //Îïðåäåëåíèå òèïà êëþ÷à
        const std::type_info& keyType() const override
        {
            return typeid(key);
        }

        //Îïðåäåëåíèå òèïà çíà÷åíèÿ
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

    //Óäàëåíèå ýëåìåíòà ïî êëþ÷ó
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

    //Ïîèñê çíà÷åíèÿ ïî êëþ÷ó è âûâîä ýëåìåíòà
    template <typename Key>
    void find(const Key& key)
    {
        for (const auto& elem : maps)
        {
            if (elem->find(key)) elem->print();
        }
    }

    //Èçìåíåíèå çíà÷åíèÿ ïî êëþ÷ó, ïðè óñëîâèè ñîâïàäåíèÿ ñòàðîãî è íîâîãî òèïà çíà÷åíèÿ
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
    
    //Âûâîä íà ïå÷àòü
    void print()
    {
        for (const auto& elem : maps)
        {
            elem->print();
        }
    }
};;
