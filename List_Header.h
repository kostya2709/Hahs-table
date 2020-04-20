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

int hash_default (char* str)
{
	return 1;
}

int hash_length (char* str)
{
	int result = 0;
    
    while (*str != 0)
    {
        str++;
        result++;
    }

	return result;
}

int hash_ascii_sum (char* str)
{
	int result = 0;

    while (*str != 0)
    {
        result += *str;
        str++;
    }

	return result % table_height;
}

int hash_ascii_length (char* str)
{
	int length = hash_length (str);
	int sum = hash_ascii_sum (str);
	
	return sum / length;
}


template <typename Type>
class Hash_Table
{
public:

	enum {default_f, ascii_sum, length, ascii_length, size};

	Hash_Table()
	{
		this->array = new List<Type>* [max_elem];
		
		func_array[0] = hash_default;
		func_array[1] = hash_ascii_sum;
		func_array[2] = hash_length;
		func_array[3] = hash_ascii_length;
	};

	Hash_Table (int func_num_in) : Hash_Table() {func_num = func_num_in;}

	int func_num = 0;
	int (*func_array [size])(char* param);

	int max_elem = table_height;

	List <Type>** array;

	void insert (Type new_elem);
	void remove (Type rem_elem);
	Type* find (Type elem);
	int hash_function (Type elem);

};



