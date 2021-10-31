#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <typeindex>



class MapBase
{
public:
    virtual bool find(const std::string&) = 0;
    virtual void print() = 0;
        
    virtual ~MapBase() {}
};



template <typename Key, typename Value>
class Map : public MapBase
{
private:
    Key key;
    Value value;
public:
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
   
    bool find(const std::string& key) override
    {
        if constexpr (std::is_same_v<decltype(this->key), std::string>) return this->key == key;
        else return std::to_string(this->key) == key;
    }

    void print()
    {
        std::cout << "Key: " << key << ", Value: " << value << '\n';
    }
    
    ~Map() {};
};

class Registry
{
private:

    std::vector<MapBase*> maps;

public:
    template <typename Key, typename Value>
    void add(Key&& key, Value&& value)
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
        std::string tmp;
        
        if (typeid(Key) != typeid(std::string)) tmp = std::to_string(key);
        else tmp = key;

        for (const auto& elem : maps)
        {
            if (elem->find(tmp)) elem->print();
        }
    }
    
    void print()
    {
        for (const auto& elem : maps)
        {
            elem->print();
        }
    }
};

void inputAndConvert(std::string& keyType, std::string& key, std::string& valueType, std::string& value)
{
    std::cout << "Key and value, (qwerty 123456): ";

    std::string generalType{};
    std::string tmp;

    
        for (int i{ 1 }; i <= 2; ++i)
        {
            std::cin >> tmp;

            const std::string::size_type isPoint = tmp.find('.');

            if (isPoint == std::string::npos)
            {
                try
                {
                    std::stoi(tmp);
                    generalType = "int";
                    
                }
                catch (const std::exception&) {}
            }
            else if (isPoint != std::string::npos)
            {
                try
                {
                    std::stod(tmp);
                    generalType = "double";
                    
                }
                catch (const std::exception&) {}
            }

            if (generalType == "") generalType = "std::string";

            if (i == 1)
            {
                keyType = generalType;
                key = tmp;
            }
            else
            {
                valueType = generalType;
                value = tmp;
            }

            generalType = "";
        }
    
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main()
{
    std::string keyType, key, valueType, value;

    Registry reg;

    for (int k{}; k < 3; ++k)
    {
        inputAndConvert(keyType, key, valueType, value);

        std::cout << keyType << key << valueType << value;
        
        std::cout << "qqq";
        

        if (keyType == "int")
        {
            if (valueType == "int") reg.add(std::stoi(key), std::stoi(value));
            else if (valueType == "double") reg.add(std::stoi(key), std::stod(value));
            else reg.add(std::stoi(key), value);
        }
        else if (keyType == "double")
        {
            if (valueType == "int") reg.add(std::stod(key), std::stoi(value));
            else if (valueType == "double") reg.add(std::stod(key), std::stod(value));
            else reg.add(std::stod(key), value);
        }
        else if (keyType == "std::string")
        {
            if (valueType == "int") reg.add(key, std::stoi(value));
            else if (valueType == "double") reg.add(key, std::stod(value));
            else reg.add(key, value);
        }

    }

    reg.print();
    reg.remove(2);
    reg.print();
    reg.find(3);
    
    
    





       



    /*reg.add(1, "qqq");
    reg.add(2, 'w');
    reg.add(3, "e");
    reg.print();
    reg.remove(2);
    reg.print();
    reg.find(3);*/


}
