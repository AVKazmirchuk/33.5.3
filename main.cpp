#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <conio.h>



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
        bool const find(const int& key) const override
        {
            if constexpr (std::is_same_v<Key, int>)
            {
                if (this->key == key) return true;
                else return false;
            }
            else return false;
        }

        bool const find(const double& key) const override
        {
            if constexpr (std::is_same_v<Key, double>)
            {
                if (this->key == key) return true;
                else return false;
            }
            else return false;
        }
        
        bool const find(const std::string& key) const override
        {
            if constexpr (std::is_same_v<Key, std::string>)
            {
                if (this->key == key) return true;
                else return false;
            }
            else return false;
        }

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



//Вывод на печать
void print(Registry& reg)
{
    reg.print();
}

//Определение типа вводимого значения
void defineType(const std::string& tmp, std::string& generalType)
{
    //Поиск символов в строке отличных от 0 до 9
    auto pos = std::find_if_not(tmp.begin(), tmp.end(),
        [](auto elem)
        {
            return elem >= 0 && elem <= 9;
        });

    //Это целое число
    if (pos == tmp.end())
    {
        try
        {
            std::stoi(tmp);
            generalType = "int";
        }
        catch (const std::exception&) {}
    }
    else
    {
        //Поиск символов в строке отличных от 0 до 9 и .
        auto pos = std::find_if_not(tmp.begin(), tmp.end(),
            [](auto elem)
            {
                return elem >= 0 && elem <= 9 && elem == '.';
            });

        //Это дробное число
        if (pos == tmp.end())
        {
            try
            {
                std::stod(tmp);
                generalType = "double";
            }
            catch (const std::exception&) {}
        }
    }

    //Это строка
    if (generalType == "") generalType = "std::string";

    std::cout << generalType;
}

void input(std::string& keyType, std::string& key, std::string& valueType, std::string& value)
{
    std::cout << "\nKey and value, (qwerty 123456): ";

    std::string tmp;

    for (int i{ 1 }; i <= 2; ++i)
    {
        std::string generalType{};

        std::cin >> tmp;

        defineType(tmp, generalType);

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
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void add(Registry& reg)
{
    std::string keyType, key, valueType, value;

    for (;;)
    {
        input(keyType, key, valueType, value);

        //Конвертация в типы на основании определёных типов при вводе
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

        std::cout << "esc - to complete: ";

        int ch = _getch();
        if (ch == 27) break;
    }
}

void changeValue(Registry& reg)
{
    std::cout << "\nEnter a key and a new value to change the old value\n";

    std::string keyType, key, valueType, value;

    input(keyType, key, valueType, value);

    //Конвертация в типы на основании определёных типов при вводе
    if (keyType == "int")
    {
        if (valueType == "int") reg.changeValue(std::stoi(key), std::stoi(value));
        else if (valueType == "double") reg.changeValue(std::stoi(key), std::stod(value));
        else reg.changeValue(std::stoi(key), value);
    }
    else if (keyType == "double")
    {
        if (valueType == "int") reg.changeValue(std::stod(key), std::stoi(value));
        else if (valueType == "double") reg.changeValue(std::stod(key), std::stod(value));
        else reg.changeValue(std::stod(key), value);
    }
    else if (keyType == "std::string")
    {
        if (valueType == "int") reg.changeValue(key, std::stoi(value));
        else if (valueType == "double") reg.changeValue(key, std::stod(value));
        else reg.changeValue(key, value);
    }
}

void input(std::string& keyType, std::string& key)
{
    std::string tmp;

    std::string generalType{};

    std::cin >> tmp;

    //Определение типа ключа
    defineType(tmp, generalType);

    keyType = generalType;
    key = tmp;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void remove(Registry& reg)
{
    std::cout << "\nEnter the key to delete the item: ";

    std::string keyType;
    std::string key;

    input(keyType, key);

    //Определение типа ключа
    defineType(keyType, key);

    //Конвертация в типы на основании определёных типов при вводе
    if (keyType == "int")
        reg.remove(std::stoi(key));
    
    else if (keyType == "double")
             reg.remove(std::stod(key));
    
    else if (keyType == "std::string")
             reg.remove(key);
}

void find(Registry& reg)
{
    std::cout << "\nEnter the key to search for the item: ";

    std::string keyType;
    std::string key;

    input(keyType, key);

    //Определение типа ключа
    defineType(keyType, key);

    //Конвертация в типы на основании определёных типов при вводе
    if (keyType == "int")
        reg.find(std::stoi(key));

    else if (keyType == "double")
        reg.find(std::stod(key));

    else if (keyType == "std::string")
        reg.find(key);
}

void message()
{
    std::cout << "\n\nMenu:\n\n";

    std::cout << "add (1) - add an element with a key\n";
    std::cout << "remove (2) - delete an item by key\n";
    std::cout << "print (3) - print all the elements with their keys on the screen\n";
    std::cout << "find (4) - find an element by key\n";
    std::cout << "change (5) - change values by key\n";
    std::cout << "exit (0) - exit the program\n\n";
}

int main()
{
    Registry reg;

    while (true)
    {
        message();

        std::string command;
        std::cin >> command;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (command == "add" || command == "1") add(reg);
        else if (command == "remove" || command == "2") remove(reg);
        else if (command == "print" || command == "3") print(reg);
        else if (command == "find" || command == "4") find(reg);
        else if (command == "change" || command == "5") changeValue(reg);
        else if (command == "exit" || command == "0") break;
    }
}
