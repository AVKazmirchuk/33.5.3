#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <typeindex>

template <typename Key, typename Value>
class Map;


class MapBase
{
private:

    virtual ~MapBase() {}
public:
    virtual bool find(const std::string&) = 0;
    virtual void print() = 0;

    virtual const std::type_index& typeKey() const = 0;
    virtual const std::type_index& typeValue() const = 0;
};






template <typename Key, typename Value>
class Map : public MapBase
{
private:

    Key key;
    Value value;


public:
    //Map() {}

    Map(const Key& _key, const Value& _value) : key{ _key }, value{ _value } {}

    Map(const Map& _map)
    {
        key = _map.key;
        value = _map.value;
    }

    Map& operator=(const Map& _map)
    {
        if (this == &_map) return *this;

        key = _map.key;
        value = _map.value;

        return *this;
    }

    MapBase* get() override
    {
        return this;
    }

    const std::type_index& typeKey() const override
    {
        return typeid(key);
    }
    const std::type_index& typeValue() const override
    {
        return typeid(value);
    }

    Key& first()
    {
        return key;
    }

    Value& second()
    {
        return value;
    }

    bool find(const std::string& key) override
    {
        return std::to_string(this->key) == key;
    }

    void print()
    {
        std::cout << "Key: " << key << ", Value: " << value << '\n';
    }
    //std::ostringstream& out(std::ostringstream& os) { os << key << value; std::return os; }

    virtual ~Map() {};
};

class Registry
{
private:
    
    std::vector<MapBase*> maps;

public:
    template <typename Key, typename Value>
    void add(const Key key, const Value value)
    {

        maps.push_back(new Map(key, value));
    }

    template <typename Key>
    void remove(const Key& key)
    {

        maps.erase(std::remove_if(maps.begin(), maps.end(),
            [=](auto elem)
            {
                return elem->find(std::to_string(key));
            }),
            maps.end());


    }

    template <typename Key>
    void find(const Key& key)
    {
        for (const auto& elem : maps)
        {
            bool found = elem->find(std::to_string(key));
            if (found) elem->print();

        }
    }

    void print()
    {
        for (const auto& elem : maps)
        {
            elem->print();
            const std::type_index Key = elem->typeKey();
            const std::type_index Value = elem->typeValue();
            std::cout << dynamic_cast<Map<((elem)->typeKey())::  , Value>*>(elem)->first();
                
        }
    }

    void test()
    {

    }
};

/*auto inputAndConvert()
{
    std::cout << "Key and value, (qwerty 123456): ";

    std::string generalType{}, keyType, valueType;
    std::string key, value;
    std::string tmp;

    for (int k{}; k < 3; ++k)
    {
        for (int i{ 1 }; i <= 2; ++i)
        {
            std::cin >> tmp;

            const std::string::size_type isPoint = tmp.find('.');

            if (isPoint == std::string::npos)
            {
                try
                {
                    return std::stoi(tmp);
                    generalType = "int";
                }
                catch (const std::exception&) {}
            }
            else if (isPoint != std::string::npos)
            {
                try
                {
                    return std::stod(tmp);
                    generalType = "double";
                }
                catch (const std::exception&) {}
            }

            if (generalType == "") generalType = "std::string";

            if (i == 1)
            {
                keyType = generalType;
                return tmp;
            }
            else
            {
                valueType = generalType;
                value = tmp;
            }

            generalType = "";
        }
}*/

int main()
{
    Registry reg;
    //reg.add();





        /*std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


        std::cout << keyType << ' ' << valueType;



        if (keyType == "int")
        {
            if (valueType == "int") reg.add(new Map(std::stoi(key), std::stoi(value)));
            else if (valueType == "double") reg.add(new Map(std::stoi(key), std::stod(value)));
            else reg.add(new Map(std::stoi(key), value));
        }*/



    reg.add(1, "qqq");
    reg.add(2, 'w');
    reg.add(3, "e");
    reg.print();
    reg.remove(2);
    reg.print();
    reg.find(3);


}
