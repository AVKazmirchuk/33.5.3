#include <iostream>
#include <vector>
#include <sstream>

class MapBase
{
public:
    virtual MapBase* create() = 0;
    virtual ~MapBase() = 0;
};

template <typename Key, typename Value>
class Map : public MapBase
{
private:
    Key key;
    Value value;

    
public:
    Map() {}
    
    explicit Map(const Key& _key, const Value& _value) : key{ _key }, value{ _value } {}

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

    MapBase* create() 
    {
        
            std::cout << "Key Value: ";
            std::string keyValue, stub;
            std::cin >> keyValue;

            std::stringstream ss;
            ss >> key >> stub >> value;
            ss.clear();

            return { key, value };
        
    }

    friend class Registry;

    ~Map() {};
};

class Registry
{
private:
    std::vector<MapBase*> maps;
public:
    void add()
    {
        while (true)
        {
            
            Map* map{ 1, 1.0 };
            
            maps.push_back(map->create());

            std::cout << "\nExit - 0: ";
            int ex;
            std::cin >> ex;

            if (!ex) break;
        }
    }
};


int main()
{
    Registry reg;
    reg.add();



}