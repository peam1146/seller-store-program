#include <stdio.h>

struct Bucket
{
    int product_id;
    int quantity;
};

struct Bucket *buckets;
unsigned int bucket_count = 0;

// init_buckets
// Initializes the buckets array
void init_buckets(int size)
{
    buckets = malloc(sizeof(struct Bucket) * size);
}

// is_product_in_bucket
// Returns the index of the bucket that contains the product
// Returns -1 if the product is not in any bucket
int is_product_in_bucket(struct Bucket *buckets, int product_id)
{
    for (int i = 0; i < bucket_count; i++)
    {
        if (buckets[i].product_id == product_id)
        {
            return i;
        }
    }
    return -1;
}

// add_product_to_bucket
// Adds a product to a bucket
// Returns the index of the bucket that was added to
int add_product_to_bucket(struct Bucket *buckets, int product_id, int quantity)
{
    int index = is_product_in_bucket(buckets, product_id);
    if (index == -1)
    {
        buckets[bucket_count].product_id = product_id;
        buckets[bucket_count].quantity = quantity;
        bucket_count++;
    }
    else
    {
        buckets[index].quantity += quantity;
    }
    return index;
}

// remove_product_from_bucket
// Removes a product from a bucket
// Returns the index of the bucket that was removed from
int remove_product_from_bucket(struct Bucket *buckets, int product_id, int quantity)
{
    int index = is_product_in_bucket(buckets, product_id);
    if (index == -1)
    {
        return -1;
    }
    else
    {
        buckets[index].quantity -= quantity;
        if (buckets[index].quantity <= 0)
        {
            buckets[index].product_id = 0;
            buckets[index].quantity = 0;
            bucket_count--;
        }
    }
    return index;
}

// get_total_products_quantity
// Returns the total quantity of all products in the buckets
int get_total_products_quantity(struct Bucket *buckets)
{
    int total = 0;
    for (int i = 0; i < bucket_count; i++)
    {
        total += buckets[i].quantity;
    }
    return total;
}
