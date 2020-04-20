//!@file
#include <iostream>
#include "List_Funcs.cpp"
#include <string>

int main()
{
    //std::ios::sync_with_stdio (false);
    
    List <std::string> list;

    list.Insert_After (0, "hello");

    DUMP (list);

    Hash_Table<std::string> hash_table;
    hash_table.insert ("hello\n");

    cout << hash_table.find ("hello\n") << "\n";

    hash_table.remove ("hello\n");

   cout << hash_table.find ("hello\n") << "\n";
    

    return 0;
}
