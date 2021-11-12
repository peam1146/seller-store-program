#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "lib/json-parse/json.c"
#include "controllers/products_controller.c"
#include "controllers/bucket_controller.c"

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

// print order
void print_order(char *name, int price_per_item, int quantity)
{
    printf("    Name: %s Quantity: %d Price: %d\n", name, quantity, quantity * price_per_item);
}

// print menu
void print_menu()
{
    printf("\n");
    printf("1. Add product\n");
    printf("2. Remove product\n");
    printf("3. Check out\n");
    printf("4. Exit\n");
    printf("\n");
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
    init_buckets(stocks_length);
    for (int i = 0; i < stocks_length; i++)
    {
        get_object_from_json(json->u.object.values[index].value->u.array.values[i], &products[i]);
    }

    // print products
    print_products(products, stocks_length);
    // chose product
    printf("\n");
    printf("Enter product id and quantity : ");
    scanf("%d %d", &product_id, &quantity);
    // show selected product
    printf("\n");
    printf("Selected product : \n");
    printf("    Name: %s Quantity: %d", products[product_id - 1].name, quantity);
    if (quantity > products[product_id - 1].quantity)
    {
        printf("\n");
        printf("Not enough product in stock");
        return;
    }
    add_product_to_bucket(buckets, product_id, quantity);
    products[product_id - 1].quantity -= quantity;
    // show bucket
    printf("\n");
    printf("Your order: \n");
    for (int i = 0; i < bucket_count; i++)
    {
        print_order(products[buckets[i].product_id - 1].name, products[buckets[i].product_id - 1].price, buckets[i].quantity);
    }
    while (running == 1)
    {
        // print menu
        print_menu();
        // get choice
        printf("\n");
        printf("Enter your choice : ");
        scanf("%d", &choice);
        // check choice
        switch (choice)
        {
        case 1:
            // add product
            // chose new product
            printf("\n");
            print_products(products, stocks_length);
            printf("\n");
            printf("Enter product id and quantity : ");
            scanf("%d %d", &product_id, &quantity);
            // show selected product
            printf("\n");
            printf("Selected product : \n");
            printf("    Name: %s Quantity: %d", products[product_id].name, quantity);
            if (products[product_id - 1].quantity >= quantity)
            {
                add_product_to_bucket(buckets, product_id, quantity);
                products[product_id - 1].quantity -= quantity;
            }
            else
            {
                printf("\n");
                printf("Not enough product in stock\n");
            }
            // show bucket
            printf("\n");
            printf("Your orders : \n");
            for (int i = 0; i < bucket_count; i++)
            {
                print_order(products[buckets[i].product_id - 1].name, products[buckets[i].product_id - 1].price, buckets[i].quantity);
            }
            break;
        case 2:
            // remove product
            // show bucket
            printf("\n");
            printf("Your orders : \n");
            for (int i = 0; i < bucket_count; i++)
            {
                printf("    Id: %d Name: %s Quantity: %d\n", buckets[i].product_id, products[buckets[i].product_id - 1].name, buckets[i].quantity);
            }
            // chose product
            printf("\n");
            printf("Enter product id and quantity to remove : ");
            scanf("%d %d", &product_id, &quantity);
            // remove product
            remove_product_from_bucket(buckets, product_id, quantity);
            products[product_id - 1].quantity += quantity;
            // show bucket
            printf("\n");
            printf("Your orders : \n");
            for (int i = 0; i < bucket_count; i++)
            {
                print_order(products[buckets[i].product_id - 1].name, products[buckets[i].product_id - 1].price, buckets[i].quantity);
            }
            break;
        case 3:
            // checkout
            // show bucket
            printf("\n");
            printf("Your orders : \n");
            for (int i = 0; i < bucket_count; i++)
            {
                print_order(products[buckets[i].product_id - 1].name, products[buckets[i].product_id - 1].price, buckets[i].quantity);
            }
            // total price
            printf("\n");
            int total_price = 0;
            for (int i = 0; i < bucket_count; i++)
            {
                total_price += products[buckets[i].product_id - 1].price * buckets[i].quantity;
            }
            printf("Total price : %d\n", total_price);
            printf("\n");
            int discount = 0;
            // if total price reach target price
            if (total_price >= 1000)
            {
                // choose discount option
                int discount_option;
                printf("Choose discount option : \n");
                printf("    1. 10%%\n");
                printf("    2. 20%%\n");
                printf("    3. 30%%\n");
                printf("Enter option : ");
                scanf("%d", &discount_option);

                while (discount_option != 1 && discount_option != 2 && discount_option != 3)
                {
                    printf("Invalid option\n");
                    printf("Choose discount option : \n");
                    printf("    1. 10%%\n");
                    printf("    2. 20%%\n");
                    printf("    3. 30%%\n");
                    scanf("%d", &discount_option);
                }

                switch (discount_option)
                {
                case 1:
                    discount = total_price * 10 / 100;
                    break;
                case 2:
                    discount = total_price * 20 / 100;
                    break;
                case 3:
                    discount = total_price * 30 / 100;
                    break;
                default:

                    break;
                }
                // print discount
                printf("\n");
                printf("Discount : %d\n", discount);
                // ask if user don't want to use discount
                printf("\n");
                printf("Do you want to use discount? (Yes 1/No 2) : ");
                scanf("%d", &choice);
                if (choice == 1)
                {
                    total_price -= discount;
                }
            }
            else if (total_price >= 500)
            {
                // choose discount option
                int discount_option;
                printf("Choose discount option : \n");
                printf("    1. 10%%\n");
                printf("    2. 20%%\n");
                printf("Enter option : ");
                scanf("%d", &discount_option);

                while (discount_option != 1 && discount_option != 2)
                {
                    printf("Invalid option\n");
                    printf("Choose discount option : \n");
                    printf("    1. 10%%\n");
                    printf("    2. 20%%\n");
                    scanf("%d", &discount_option);
                }

                switch (discount_option)
                {
                case 1:
                    discount = total_price * 10 / 100;
                    break;
                case 2:
                    discount = total_price * 20 / 100;
                    break;
                default:
                    break;
                }
                // print discount
                printf("\n");
                printf("Discount : %d\n", discount);
                // ask if user don't want to use discount
                printf("\n");
                printf("\n");
                printf("Do you want to use discount? (Yes 1/No 2) : ");
                scanf("%d", &choice);
                if (choice == 1)
                {
                    total_price -= discount;
                }
            }
            else if (total_price >= 100)
            {
                // choose discount option
                int discount_option;
                printf("Choose discount option : \n");
                printf("    1. 10%%\n");
                scanf("%d", &discount_option);

                while (discount_option != 1)
                {
                    printf("Invalid option\n");
                    printf("Choose discount option : \n");
                    printf("    1. 10%%\n");
                    printf("Enter option : ");
                    scanf("%d", &discount_option);
                }

                switch (discount_option)
                {
                case 1:
                    discount = total_price * 10 / 100;
                    break;
                default:
                    break;
                }
                // print discount
                printf("\n");
                printf("Discount : %d\n", discount);
                // ask if user don't want to use discount
                printf("\n");
                printf("\n");
                printf("Do you want to use discount? (Yes 1/No 2) : ");
                scanf("%d", &choice);
                if (choice == 1)
                {
                    total_price -= discount;
                }
            }
            printf("Total price after discount : %d\n", total_price);
            // payment
            int payment;
            printf("Enter payment : ");
            scanf("%d", &payment);
            while (payment < total_price)
            {
                // failed payment
                printf("\n");
                printf("Payment failed\n");
                printf("\n");
                printf("Enter payment : ");
                scanf("%d", &payment);
            }
            // change
            int change = payment - total_price;
            printf("Change : %d\n", change);
            running = 0;
            break;
        case 4:
            // exit
            return;
            break;
        default:
            printf("Invalid choice!\n");
            break;
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
    }
}