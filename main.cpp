//!@file
#include <iostream>
#include "List_Funcs.cpp"
#include <string>

const int hash_num = 6;

char* Download_Text (Hash_Table<char*> hash_table [hash_num]);
void Write_Statistics (const Hash_Table<char*> hash_table [hash_num]);

int main()
{
    Hash_Table<char*> hash_array [hash_num];
    
    hash_array[0] = Hash_Table <char*> (Hash_Table <char*>::default_f);
    hash_array[1] = Hash_Table <char*> (Hash_Table <char*>::ascii_sum);
    hash_array[2] = Hash_Table <char*> (Hash_Table <char*>::length);
    hash_array[3] = Hash_Table <char*> (Hash_Table <char*>::ascii_length);
    hash_array[4] = Hash_Table <char*> (Hash_Table <char*>::xor_f);
    hash_array[5] = Hash_Table <char*> (Hash_Table <char*>::murmur);
    
    char* file = Download_Text (hash_array);

    Write_Statistics (hash_array);

    free (file);

    return 0;
}

char* Download_Text (Hash_Table<char*> hash_table [hash_num])
{
    FILE* f = fopen ("text", "r");

    fseek (f, 0, SEEK_END);
    unsigned long long file_size = ftell (f);
    
    fseek (f, 0, SEEK_SET);

    char* str = (char*) calloc (1, file_size);
    fread (str, 1, file_size, f);

    char* cur_ptr = str;
    char* end_ptr = str + file_size;

    for (int i = file_size; i > 0 && !isalpha (*cur_ptr); --i)
        cur_ptr++;


    if (!*cur_ptr)
    {
        printf ("No readable information\n");
        fclose (f);
        return str;;
    }

    char* cur_start = cur_ptr;

    while (cur_ptr < end_ptr)
    {
        cur_start = cur_ptr;
        while (isalpha (*cur_ptr))
            cur_ptr++;

        *cur_ptr = '\0';

        for (int i = 0; i < 6; ++i)
            hash_table[i].insert (cur_start);

        while (!isalpha (*cur_ptr))
            cur_ptr++;
    }

    fclose (f);

    return str;
}

void Write_Statistics (const Hash_Table<char*> hash_table [hash_num])
{
    FILE* f = fopen ("table_statistic.csv", "w");

    for (int i = 0; i < hash_table[0].max_elem; ++i)
    {
        for (int j = 0; j < hash_num; ++j)
            if (hash_table[j].array[i])
                fprintf (f, "%d, %d, , ", i, hash_table[j].array[i]->size);
            else 
                fprintf (f, "%d, 0 , , ", i);  
                
        fprintf (f, "\n");

    }

    fclose (f);
}