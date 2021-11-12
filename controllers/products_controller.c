#include <stdio.h>

struct Product
{
    char *name;
    uint64_t id;
    uint64_t price;
    uint64_t quantity;
};

struct Product *products;

// init_products
// Description: Initializes the products array
void init_products(int size)
{
    products = malloc(sizeof(struct Product) * size);
}

// sort products by id
void sort_products(struct Product *products, int size)
{
    int i, j;
    struct Product temp;
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size - 1; j++)
        {
            if (products[j].id > products[j + 1].id)
            {
                temp = products[j];
                products[j] = products[j + 1];
                products[j + 1] = temp;
            }
        }
    }
}

// print all products
void print_products(struct Product *products, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%3llu). Name: %-20s Price: %-3llu Quantity: %-3llu\n", products[i].id, products[i].name, products[i].price, products[i].quantity);
    }
}