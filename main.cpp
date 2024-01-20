#include "set.hpp"
#include "map.hpp"
#include "stack.hpp"
#include "vector.hpp"
#include <iostream>
#include <string>


void print_map(std::string comment, const ft::map<std::string, int>& m)
{
        std::cout << comment;
 for (ft::map<std::string, int>::const_iterator it = m.begin(); it != m.end(); it++)
     std::cout << it->first << " = " << it->second << "; ";
 
    std::cout << '\n';
}
 
int main()
{
    // Create a map of three (string, int) pairs
    ft::map<std::string, int> m;

    m.insert(ft::make_pair("CPU", 10));
    m.insert(ft::make_pair("GPU", 15));
    m.insert(ft::make_pair("RAM", 20));

    
    print_map("1) Initial map: ", m);
 
    m["CPU"] = 25; // update an existing value
    m["SSD"] = 30; // insert a new value
    print_map("2) Updated map: ", m);
 
    // using operator[] with non-existent key always performs an insert
    std::cout << "3) m[UPS] = " << m["UPS"] << '\n';
    print_map("4) Updated map: ", m);
 
    m.erase("GPU");
    print_map("5) After erase: ", m);
 
    print_map("6) After erase: ", m);
    std::cout << "7) m.size() = " << m.size() << '\n';
 
    m.clear();
    std::cout << std::boolalpha << "8) Map is empty: " << m.empty() << '\n';
}

