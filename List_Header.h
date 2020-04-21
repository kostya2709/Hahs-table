#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define DUMP( list ); list.List_Dump ( #list );

const int list_start_size = 5;

const int FILE_NAME_SIZE = 20;

#define PRINTF_Type "%g"

const int POISON = -666;

const int EMPTY = -27;

const int table_height = 997;

template <typename Type>
struct List
{
    Type* data;
	int* next;
	int* prev;
	int size;
	int max_size;
	int head;
	int tail;
	int free_;

/*! This function creates a list!
@param[in] Pointer to the list.
\return Returns 0 if the function behaves correctly.
*/
	List();

/*! This function cleans memory used by the list during the work with it.
Attention! Use only after the work with the list.
@param[in] Pointer to the list.
\return Returns 0 if the function behaves correctly.
*/
	~List();

/*! This function inserts a member of the list after an elements of it.
@param[in] Pointer to the list.
@param[in] Number of the list after which another is to be inserted.
@param[in] The element to insert.
\return Returns 0 if the function behaves correctly.
*/
	int Insert_After (int location, Type elem);

/*! This function prints the main information of the list.
Attention! This function is to be required only from #define DUMP(); .
@param[in] Pointer to the list.
@param[in] Name of the list.
\return Returns 0 if the function behaves correctly.
*/
	int List_Dump (const char* list_name) const;

/*! This function inserts a member of the list before an elements of it.
@param[in] Pointer to the list.
@param[in] Number of the list before which another is to be inserted.
@param[in] The element to insert.
\return Returns 0 if the function behaves correctly.
*/
	int Insert_Before (int location, Type elem);

/*! This function inserts a member of the list to the first element.
@param[in] Pointer to the list.
@param[in] The element to insert.
\return Returns 0 if the function behaves correctly.
*/
	int Insert_Front (Type elem);
/*! This function inserts a member of the list to the last element.
@param[in] Pointer to the list.
@param[in] The element to insert.
\return Returns 0 if the function behaves correctly.
*/
	int Insert_Back (Type elem);

/*!This function checks whether location is valid.
@param[in] Pointer to the list.
@param[in] Number of the list to check.
@param[in] min Minimal available number.
@param[in] max Maximal available number.
@param[in] dead Unavailable list element.
*/
	int Error_Check (int location, int min) const;

/*! This function deletes a member of the list.
@param[in] Pointer to the list.
@param[in] Number of the list to delete.
\return Returns 0 if the function behaves correctly.
*/
	int Delete (int location);

	int Delete (Type element);

/*! This function changes the allocated memory.
@param[in] Pointer to the list.
@param [in] New size of list.
\return Returns 0 if the function behaves correctly.
*/
	int List_Realloc (int new_size);

/*!This function converts the main information about the list to graph.
@param[in] Pointer to the list.
\return Returns 0 if the function behaves correctly.
*/
	int List_Dump_Graph () const;
};

unsigned int hash_default (const char* str)
{
	return 1;
}

unsigned int hash_length (const char* str)
{
	int result = 0;
    
    while (*str != 0)
    {
        str++;
        result++;
    }

	return result;
}

unsigned int hash_ascii_sum (const char* str)
{
	int result = 0;

    while (*str != 0)
    {
        result += *str;
        str++;
    }

	return result % table_height;
}

unsigned int hash_ascii_length (const char* str)
{
	int length = hash_length (str);
	int sum = hash_ascii_sum (str);
	
	return sum / length;
}

unsigned int hash_xor (const char* str)
{
	unsigned int result = 0;

	char* ptr = (char*) str;
	while (*ptr != 0)
	{
		result ^= *ptr;
		result >> 1;
		++ptr;
	}

	return result;
}


unsigned int MurmurHash2 (const char* str)
{
	int len = hash_length (str);
	unsigned int seed = 272727;

	const unsigned int m = 0x5bd1e995;
	const int r = 24;

	char* ptr = (char*) str;
	unsigned int h = seed ^ len;

	while (len >= 4)
	{
		unsigned int k = *(unsigned int *)ptr;

		k *= m; 
		k ^= k >> r; 
		k *= m; 
		
		h *= m; 
		h ^= k;

		ptr += 4;
		len -= 4;
	}

	switch(len)
	{
	case 3: h ^= ptr[2] << 16;
	case 2: h ^= ptr[1] << 8;
	case 1: h ^= ptr[0];
	        h *= m;
	};
	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h % table_height;
} 

template <typename Type>
class Hash_Table
{
public:

	enum {default_f, ascii_sum, length, ascii_length, xor_f, murmur, size};

	Hash_Table()
	{
		this->array = new List<Type>* [max_elem];
		
		func_array[0] = hash_default;
		func_array[1] = hash_ascii_sum;
		func_array[2] = hash_length;
		func_array[3] = hash_ascii_length;
		func_array[4] = hash_xor;
		func_array[5] = MurmurHash2;
	};

	explicit Hash_Table (int func_num_in) : Hash_Table() {func_num = func_num_in;}

	int func_num = 5;
	unsigned int (*func_array [size])(const char* param);

	int max_elem = table_height;

	List <Type>** array;

	void insert (Type new_elem);
	void remove (Type rem_elem);
	Type* find (Type elem);
	int hash_function (Type elem);

};



