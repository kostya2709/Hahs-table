//!@file
#include <iostream>
#include "List_Funcs.cpp"
#include <string>

int main()
{
    //std::ios::sync_with_stdio (false);
    
    List <std::string> list;

    DUMP (list);

    list.Insert_After (0, "hello");

    DUMP (list);

    return 0;
}
