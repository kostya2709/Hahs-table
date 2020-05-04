//!@file
#include <iostream>
#include "List_Funcs.cpp"
#include <string>
#include <time.h>

const int hash_num = 7;

char* Download_Text (Hash_Table<char*> hash_table [hash_num], char*** words_array, unsigned long long& words);
void Write_Statistics (const Hash_Table<char*> hash_table [hash_num]);
void time_testing (char** text, Hash_Table<char*> hash_table [hash_num], unsigned long long words);

extern "C" bool _fast_strcmp (const char*, const char*);

int asm_test (void)
{
    int input = 0;
    int output = 5;
/*
    __asm__ (   ".intel_syntax noprefix\n\t"
                "push rax\n\t"
                "mov eax, ebx\n\t"
                "mov ebx, %1\n\t"
                //"mov %1, eax\n\t"
                "pop rax\n"
                ".att_syntax prefix\n\t"
                : "=r" (output), "=b" (input)
                : "b" (input), "r" (output)
                : "eax"
    );
*/

}

int main()
{
    asm_test();

    Hash_Table<char*> hash_array [hash_num];
    
    hash_array[0] = Hash_Table <char*> (Hash_Table <char*>::default_f);
    hash_array[1] = Hash_Table <char*> (Hash_Table <char*>::ascii_sum);
    hash_array[2] = Hash_Table <char*> (Hash_Table <char*>::length);
    hash_array[3] = Hash_Table <char*> (Hash_Table <char*>::ascii_length);
    hash_array[4] = Hash_Table <char*> (Hash_Table <char*>::xor_f);
    hash_array[5] = Hash_Table <char*> (Hash_Table <char*>::murmur);
    hash_array[6] = Hash_Table <char*> (Hash_Table <char*>::crc32);
    
    char** words_array = NULL;
    unsigned long long words = 0;
    char* file = Download_Text (hash_array, &words_array, words);

    time_testing (words_array, hash_array, words);

    Write_Statistics (hash_array);

    free (file);

    return 0;
}

void time_testing (char** text, Hash_Table<char*> hash_table [hash_num], unsigned long long words)
{
    clock_t start = {};
    clock_t end = {};
    double seconds = 0;
    //printf ("words = %llu, %s\n", words, text[0]);

    for (int i = 0; i < hash_num; ++i)
    {
        start = clock();
        for (int torture = 0; torture < 100; ++torture)
            for (int j = 0; j < words; ++j)
            {
                hash_table[i].find (text[j]);
            }

        end = clock();
        seconds = (double) (end - start) / CLOCKS_PER_SEC;
        printf ("Hash %d worked %g seconds.\n", i, seconds);
    }
}

char* Download_Text (Hash_Table<char*> hash_table [hash_num], char*** words_array, unsigned long long& words)
{
    FILE* f = fopen ("text", "r");

    fseek (f, 0, SEEK_END);
    unsigned long long file_size = ftell (f);
    
    fseek (f, 0, SEEK_SET);

    char* str = (char*) calloc (1, file_size);
    fread (str, 1, file_size, f);

    char* cur_ptr = str;
    char* end_ptr = str + file_size;

    *words_array = new char* [file_size];

    for (int i = file_size; i > 0 && !isalpha (*cur_ptr); --i)
        cur_ptr++;


    if (!*cur_ptr)
    {
        printf ("No readable information\n");
        fclose (f);
        return str;;
    }

    char* cur_start = cur_ptr;

    unsigned long long words_count = 0;

    while (cur_ptr < end_ptr)
    {
        cur_start = cur_ptr;
        while (isalpha (*cur_ptr))
            cur_ptr++;

        *cur_ptr = '\0';

        for (int i = 0; i < hash_num; ++i)
            hash_table[i].insert (cur_start);

        (*words_array)[words_count] = cur_start;
        ++words_count;

        while (!isalpha (*cur_ptr))
            cur_ptr++;
    }

    words = words_count;

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