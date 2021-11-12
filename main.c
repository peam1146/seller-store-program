#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "lib/json-parse/json.c"
#include "controllers/products_controller.c"

// get object from json
void get_object_from_json(json_value *root, struct Product *product)
{
    product->name = root->u.object.values[0].value->u.string.ptr;
    product->price = root->u.object.values[1].value->u.integer;
    product->quantity = root->u.object.values[2].value->u.integer;
    product->id = root->u.object.values[3].value->u.integer;
}

int get_index_from_key(json_value *root, char *key)
{
    int i;
    for (i = 0; i < root->u.object.length; i++)
    {
        if (strcmp(root->u.object.values[i].name, key) == 0)
        {
            return i;
        }
    }
    return -1;
}

// print menu
void print_menu()
{
    printf("\n");
    printf("1. Add product\n");
    printf("2. Remove product\n");
    printf("3. Update product\n");
    printf("4. Buy product\n");
    printf("5. Exit\n");
    printf("\n");
}

// Buy product
void buy_product(uint64_t product_id, int quantity, struct Product *product, int size)
{
    // check if product is exist
    int i;
    for (i = 0; i < size; i++)
    {
        if (product[i].id == product_id)
        {
            break;
        }
    }
    if (i == size)
    {
        printf("Product not found!\n");
        return;
    }
    // check if quantity is enough
    if (product[i].quantity < quantity)
    {
        printf("Not enough quantity!\n");
        return;
    }
    // update quantity
    product[i].quantity -= quantity;
}

void main()
{
    struct stat filestatus;
    char *filename = "database.json";
    char *file_contents;
    // bool to check if program is running
    int running = 1;
    int product_id, quantity;
    int choice;

    // read json file
    if (stat(filename, &filestatus) == 0)
    {
        //Add a byte to the buffer size to hold the nul.
        // Use calloc to set the entire allocation to zero.
        file_contents = calloc(filestatus.st_size + 1, 1);
        FILE *file = fopen(filename, "rt");
        fread(file_contents, filestatus.st_size, 1, file);
        fclose(file);
    }
    else
    {
        printf("File not found\n");
        return;
    }

    // parers json file
    json_value *json = json_parse(file_contents, filestatus.st_size);
    if (json == NULL)
    {
        printf("Error parsing json\n");
        return;
    }

    // get name value from json
    int index = get_index_from_key(json, "stocks");
    int stocks_length = json->u.object.values[index].value->u.array.length;
    // create products array
    init_products(stocks_length);
    for (int i = 0; i < stocks_length; i++)
    {
        get_object_from_json(json->u.object.values[index].value->u.array.values[i], &products[i]);
    }

    while (running == 1)
    {
        // sort products by id
        sort_products(products, stocks_length);

        print_products(products, stocks_length);
        // print menu
        print_menu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            // add_product();
            break;
        case 2:
            // remove_product();
            break;
        case 3:
            // update_product();
            break;
        case 4:
            // show products
            printf("Enter product id and quantity: ");
            scanf("%d %d", &product_id, &quantity);
            buy_product(product_id, quantity, products, stocks_length);
            break;
        case 5:
            running = 0;
            break;
        default:
            printf("Invalid choice!\n");
            break;
        }
        printf("%d", stocks_length);
    }

    // write json file
    FILE *file = fopen(filename, "wt");
    fprintf(file, "{\"stocks\":[");
    for (int i = 0; i < stocks_length; i++)
    {
        fprintf(file, "{\"name\":\"%s\",\"price\":%llu,\"quantity\":%llu,\"id\":%llu}", products[i].name, products[i].price, products[i].quantity, products[i].id);
        if (i != stocks_length - 1)
        {
            fprintf(file, ",");
        }
    }
    fprintf(file, "]}");
    fclose(file);
};