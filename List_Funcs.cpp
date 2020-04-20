#include <type_traits>

using std::cout;

#ifndef iostream
#include <iostream>
#endif

#ifndef List_Header_h
#include "List_Header.h"
#endif

template <typename Type>
void Make_Poison (Type* ptr)
{ 
    *ptr = POISON;
}

template <>
void Make_Poison <std::string> (std::string* ptr)
{ 
    *ptr = "POISON";
}

template <typename Type>
void Make_Empty (Type* ptr)
{ 
    *ptr = EMPTY;
}

template <>
void Make_Empty <std::string> (std::string* ptr)
{ 
    *ptr = "EMPTY";
}


template <typename Type>
List <Type>::List()
{
    List* list1 = this;
    list1->data = new Type [list_start_size];
    Make_Poison (list1->data);

    list1->next = new int [list_start_size];
    list1->prev = new int [list_start_size];

    if (!list1->data || !list1->next || !list1->prev)
    {
        printf ("Error of allocation!\n");
        return;
    }

    list1->size = 0;
    list1->max_size = list_start_size;
    list1->head = 0;
    list1->tail = 0;
    list1->free_ = 1;

    int i = 0;
    for (i = 2; i < list1->max_size; i++)
        Make_Poison (list1->prev + i);

    for (i = 1; i < list1->max_size; i++)
        *(list1->next + i) = i + 1;

    for (i = 1; i < list1->max_size; i++)
        Make_Empty (list1->data + i);
}

template <typename Type>
int List <Type>::Insert_After (int location, Type elem)
{

    List* list1 = this;
    assert (list1);

    if (Error_Check (location, 0))
            return -1;

    list1->size += 1;
    int pos = list1->free_;
    list1->free_ = *(list1->next + pos);

    *(list1->data + pos) = elem;


    if (location == 0)
    {
        int ex_first = list1->head;
        list1->head = pos;

        *(list1->next + pos) = ex_first;
        *(list1->prev + pos) = 0;

        if (ex_first)
            *(list1->prev + ex_first) = pos;

        if (list1->tail == 0)
            list1->tail = pos;

    }

    else
    {
        if (location == list1->tail)
            list1->tail = pos;

        int next_el = *(list1->next + location);
        *(list1->next + location) = pos;

        *(list1->prev + pos) = location;
        *(list1->next + pos) = next_el;
        if (next_el)
            *(list1->prev + next_el) = pos;
    }

    if (list1->size + 1 == list1->max_size)
    {
        List_Realloc (list1->max_size * 2);
        list1->max_size *= 2;
    }

    return 0;

}

template <typename Type>
int List <Type>::Insert_Before (int location, Type elem)
{
    List* list1 = this;
    assert (list1);

    if (location == list1->head)
    {
        printf ("Error! It is impossible to insert an element before the head of the list\n");
        return -1;
    }

    Insert_After (*(list1->prev + location), elem);

    return 0;

}

template <typename Type>
int List <Type>::Insert_Front (Type elem)
{
    Insert_Before (this->head, elem);
    return 0;
}

template <typename Type>
int List <Type>::Insert_Back (Type elem)
{
    Insert_After (this->tail, elem);
    return 0;
}

template <typename Type>
int List <Type>::Delete (Type elem)
{
    for (int i = 0; i < this->size; ++i)
        if (this->data[i] == elem)
        {
            this->Delete (i);
            return 0;
        }

    return 1;
}

template <typename Type>
int List <Type>::Delete (int location)
{

    List* list1 = this;
    assert (list1);

    if (Error_Check (location, 0))
        return -1;
    if (location == 0)
    {
        printf ("Error! You cannot delete zero!\n");
        return -1;
    }

    list1->size -= 1;

    if (location == list1->head)
    {
        int next_el = *(list1->next + location);
        list1->head = next_el;
        *(list1->prev + next_el) = 0;
    }

    if (location == list1->tail)
    {
        int prev_el = *(list1->prev + location);
        list1->tail = prev_el;
        *(list1->next + prev_el) = 0;
    }

    else
    {
        int next_el = *(list1->next + location);
        int prev_el = *(list1->prev + location);

        *(list1->prev + next_el) = prev_el;
        *(list1->next + prev_el) = next_el;
    }

    Make_Empty (list1->data + location);

    *(list1->next + location) = list1->free_;
    Make_Poison (list1->prev + location);
    list1->free_ = location;

    Make_Empty (list1->data + location);

    return 0;

}

template <typename Type>
int List <Type>::List_Dump (const char* list_name) const
{
    const List* list1 = this;

    printf ("\n\n\tList_name: %s\n", list_name);
    printf ("\tList max_size: %d\n", list1->max_size);
    printf ("\tList size: %d\n", list1->size);
    printf ("\tList elements:\n");

    int i = 0;
    for (i = 0; i < list1->max_size; i++)
        cout << "\t\tdata [" << i << "] = " << *(list1->data + i) << '\n';

    printf ("\n\tArray of next elements:\n");
    for (i = 0; i < list1->max_size; i++)
        printf ("\t\tnext [%d] = %d\n", i, *(list1->next + i));

    printf ("\n\tArray of previous elements:\n");
    for (i = 0; i < list1->max_size; i++)
        printf ("\t\tprev [%d] = %d\n", i, *(list1->prev + i));

    printf ("\n\nList head: %d\n", list1->head);
    printf ("List tail: %d\n", list1->tail);
    printf ("List free_: %d\n\n\n", list1->free_);

    return 0;
}

template <typename Type>
int List <Type>::Error_Check (int location, int min) const
{
    const List* list1 = this;

    if (location < min)
    {
        printf ("Error! Invalid index of list!\n");
        printf ("Expected a number above %d\n", min);
        printf ("Received: %d\n", location);

        return -1;
    }

    if ((*(list1->prev + location) == POISON) && (location))
    {
        printf ("Error! Invalid index of list!\n");
        printf ("Expected an existing element from zero to %d\n", list1->max_size);
        printf ("Received: %d\n", location);
        printf ("znachenie = %d\n", *(list1->prev + location));

        return -1;
    }

    return 0;
}

template <typename Type>
int List <Type>::List_Realloc (int new_size)
{
    List* list1 = this;
    assert (list1);

    int prev_size = list1->max_size;

    list1->data = (Type*)realloc (list1->data, new_size * sizeof (Type));
    list1->next = (int*)realloc (list1->next, new_size * sizeof (int));
    list1->prev = (int*)realloc (list1->prev, new_size * sizeof (int));

    if (!list1->data || !list1->next || !list1->prev)
    {
        printf ("Error of reallocation!\n");
        return -1;
    }

    if (prev_size < new_size)
    {
        int i = 0;

        for (i = prev_size; i < new_size; i++)
            Make_Empty (list1->data + i);

        for (i = prev_size; i < new_size; i++)
            Make_Poison (list1->prev + i);

        for (i = prev_size; i < new_size; i++)
            *(list1->next + i) = i + 1;
    }

    return 0;
}

template <typename Type>
List <Type>::~List()
{
    List* list1 = this;
    assert (list1);

    if (!list1->data || !list1->next || !list1->prev)
        return;

    delete [] list1->data;
    delete [] list1->next;
    delete [] list1->prev;

    return;
}

template <typename Type>
int List <Type>::List_Dump_Graph () const
{

    const char* file_name = (char*) calloc (FILE_NAME_SIZE, sizeof (char));
    const char* file_png = (char*) calloc (FILE_NAME_SIZE, sizeof (char));


    file_name = "graph_dump.gv";
    file_png = "list_dump.png";


    FILE* f = fopen (file_name, "w");


    fprintf (f, "digraph First{\n");
    fprintf (f, "node [shape=\"record\", style=\"filled\", fillcolor=\"pink\"];\n");
    fprintf (f, "rankdir=\"LR\";\n");
    fprintf (f, "dpi=\"300\";\n");

    fprintf (f, "\"head\" [label = \"head\"];\n");
    fprintf (f, "\"tail\" [label = \"tail\"];\n");

    fprintf (f, "\"head1\" [label = \"head\"];\n");
    fprintf (f, "\"tail1\" [label = \"tail\"];\n");

    fprintf (f, "node [fillcolor=\"lightblue\"];\n");

    int i = 0;

    const List* list1 = this;
    int number = list1->head;
        while (true)
        {
            fprintf (f, "\"box%d\"  [label = \"adress = %d|value = " PRINTF_Type "|next = %d|prev = %d\"];\n\n",
                     i, number, *(list1->data + number), *(list1->next + number), *(list1->prev + number));
            i++;
            if (number == list1->tail)
                break;
            number = *(list1->next + number);
        }

        int temp = i;

        for (i = 0; i < list1->max_size - 1; i++)
        {
            if (*(list1->data + i + 1) == "EMPTY")
                    fprintf (f, "node [fillcolor=\"green\"];\n");

            fprintf (f, "\"box%d\"  [label = \"adress = %d|value = " PRINTF_Type "|next = %d|prev = %d\"];\n\n",
                i + temp, i + 1, *(list1->data + i + 1), *(list1->next + i + 1), *(list1->prev + i + 1));

            if (*(list1->data + i + 1) == "EMPTY")
                    fprintf (f, "node [fillcolor=\"lightblue\"];\n");

        }

    fprintf (f, "\"head\"->\"box0\";\n");

    int j = 0;

    for (j = 0; j < temp - 1; j++)
            fprintf (f, "\"box%d\" -> \"box%d\";\n", j, j + 1);

    fprintf (f, "\"box%d\" -> \"tail\";\n\n", j);


    fprintf (f, "\"head1\"->\"box%d\";\n", temp);

    fprintf (f, "edge[color=\"white\"];\n");

    for (j = 0; j < i - 1; j++)
            fprintf (f, "\"box%d\" -> \"box%d\";\n", j + temp, j + 1 + temp);

    fprintf (f, "edge[color=\"black\"];\n");


    j = list1->head;
    while (true)
    {
        i = *(list1->next + j);
        fprintf (f, "\"box%d\" -> \"box%d\";\n", j + temp - 1, i + temp - 1);

        j = i;

        if (i == list1->tail)
            break;
    }
    j = list1->free_;
    while (*(list1->next + j) < list1->max_size)
    {
        i = *(list1->next + j);
        fprintf (f, "\"box%d\" -> \"box%d\";\n", j + temp - 1, i + temp - 1);
        j = i;
    }

    fprintf (f, "\"box%d\"->\"tail1\";\n", list1->tail + temp - 1);

    fprintf (f, "node [fillcolor=\"green\"];");

    fprintf (f, "\"info\" [label = \"Info|head = %d|tail = %d|free_ = %d|size = %d|max_size = %d\"]\n", list1->head, list1->tail, list1->free_, list1->size, list1->max_size);


    fprintf (f, "}");

    fclose (f);

    system ("dot -Tpng graph_dump.gv -o list_dump.png");

    //system ("pause");

    return 0;
}





template <typename Type>
void Hash_Table <Type>::insert (Type new_elem)
{
    int value = this->hash_function (new_elem);

    if (value > this->max_elem)
    {
        printf ("Hash-value is too big for the reserved array.\n");
        return;
    }

    if (this->array[value] == NULL)
    {
        List <Type>* list = new List <Type>;
        this->array[value] = list;
    }

    this->array[value]->Insert_After (0, new_elem);
}

template <typename Type>
int compare_elem (Type elem1, Type elem2)
{
    return elem1 == elem2;
}

template <>
int compare_elem (std::string elem1, std::string elem2)
{
    return elem1 == elem2;
}
template <typename Type>
void Hash_Table <Type>::remove (Type new_elem)
{
    int value = this->hash_function (new_elem);

    if (value < 0)
    {
        printf ("Inappropriate value of the element to remove.\n");
        return;
    }

    List<Type>* list1 = this->array[value];
    int size = list1->size;

    for (int i = 1; i <= size; ++i)
    if (compare_elem (list1->data[i], new_elem))
        list1->Delete (i);
}


template <typename Type>
Type* Hash_Table<Type>::find (Type elem)
{
    int index = this->hash_function (elem);

    List <Type>* list1 = this->array[index];
    int size = list1->size;

    for (int i = 1; i <= size; ++i)
        if (compare_elem (list1->data[i], elem))
            return &(list1->data[i]);

    return NULL;
}

template <typename Type>
int Hash_Table <Type>::hash_function (Type elem)
{
    return 0;
}

template <>
int Hash_Table <std::string>::hash_function (std::string elem)
{
    int size = elem.size();
    int hash = 0;

    for (int i = 0; i < size; ++i)
        hash += elem.data()[i];

    return hash;
}


