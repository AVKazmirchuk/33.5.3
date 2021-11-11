#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <conio.h>



#include "Header.h"



//Вывод на печать
void print(Registry& reg)
{
    reg.print();
}

//Определение типа вводимого значения
void defineType(const std::string& tmp, std::string& generalType)
{
    //Является ли строка числом
    bool isNumber = std::all_of(tmp.begin(), tmp.end(),
        [](auto& elem)
        {
            return (elem >= '0' && elem <= '9' || elem == '.');
        });

    //Это возможно не число 
    if (!isNumber)
    {
        generalType = "std::string"; 
        return;
    }

    //Является ли число целым
    bool isInteger = std::all_of(tmp.begin(), tmp.end(),
        [](auto& elem)
        {
            return (elem >= '0' && elem <= '9');
        });

    //Это возможно целое число
    if (isInteger)
    {
        try
        {
            std::stoi(tmp);
            generalType = "int";
            return;
        }
        catch (const std::exception&) {}
    }
   
   //В строке может быть несколько символов "."
   //Поиск первого символа "."
   auto pointPosFirst = std::find_if(tmp.begin(), tmp.end(),
       [](auto& elem)
       {
           return elem == '.';
       });

   //Возможно позиция символа "." не последняя
   std::string::const_iterator pointPosNext = pointPosFirst + 1;
   if (pointPosNext != tmp.end())
   {
       //Поиск второго символа "."
       pointPosNext = std::find_if(pointPosNext, tmp.end(),
           [](auto& elem)
           {
               return elem == '.';
           });
   }

   //Возможно в строке присутствует второй символ "."
   //Возможно это дробное число
   if (pointPosNext == tmp.end())
   {
       try
       {
           std::stod(tmp);
           generalType = "double";
           return;
       }
       catch (const std::exception&) {}
   }

   generalType = "std::string";
   return;
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

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    //Определение типа ключа
    defineType(tmp, generalType);

    keyType = generalType;
    key = tmp;
}

void remove(Registry& reg)
{
    std::cout << "\nEnter the key to delete the item: ";

    std::string keyType;
    std::string key;

    input(keyType, key);

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
