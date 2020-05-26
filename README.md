# Hahs-table
This is an implementation of a hash-table using two-linked list.

# Acceleration

100 times checking words of all the text:

| Hash-function  |  -O0  | -O0 + asm__   |
| -------------- | ----- | ------------- |
| Always 1       |  43.10|         0.388 |
| Sum of ASCII   |  0.384|         0.350 |
| Length         |  6.768|         0.366 |
| ASCII-sum / len|  3.964|         0.439 |
| Rolling xor    |  0.381|         0.377 |
| Murmur Hash2   |  0.459|         0.427 |
| crc32          |  0.514|         0.524 |

10000 times checking words of all the text:

| Hash-function  |  -O3  | -O3 + asm__   |
| -------------- | ----- | ------------- |
| Always 1       |  0.714|         1.589 |
| Sum of ASCII   |  5.901|         1.595 |
| Length         |  5.355|         1.657 |
| ASCII-sum / len|  7.051|         2.113 |
| Rolling xor    |  6.165|         1.593 |
| Murmur Hash2   |  7.562|         1.799 |
| crc32          |  6.233|         1.909 |
