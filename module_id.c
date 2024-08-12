// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Andrey Misyurov");
MODULE_DESCRIPTION("Manage identity structs in a kernel-linked list");

struct identity {
    char name[20];
    int id;
    bool hired;
    struct list_head list;
};

static LIST_HEAD(data_base);

int identity_create(char *name, int id)
{
    struct identity *node = kmalloc(sizeof(*node), GFP_KERNEL);
    if (!node)
        return -ENOMEM;

    strncpy(node->name, name, sizeof(node->name) - 1);
    node->name[sizeof(node->name) - 1] = '\0';
    node->id = id;
    node->hired = false;

    INIT_LIST_HEAD(&node->list);
    list_add_tail(&node->list, &data_base);

    pr_info("Identity created: %s with id %d\n", node->name, node->id);

    return 0;
}

struct identity *identity_find(int id)
{
    struct identity *identity;

    list_for_each_entry(identity, &data_base, list)
    {
        if (identity->id == id)
        {
            return identity;
        }
    }
    return NULL;
}

void identity_destroy(int id)
{
    struct identity *identity;

    list_for_each_entry(identity, &data_base, list)
    {
        if (identity->id == id)
        {
            list_del(&identity->list);
            kfree(identity);
            break;
        }
    }
}

int identity_hire(int id)
{
    struct identity *identity;

    list_for_each_entry(identity, &data_base, list)
    {
        if (identity->id == id)
        {
            identity->hired = true;
            return 0;
        }
    }
    return -1;
}

static int __init hello_init(void)
{
    pr_info("Hello, KernelCare!\n");
    struct identity * temp;
    identity_create("Andrey", 1);
    identity_create("Ivan", 2);
    identity_create("Aleksey", 3);
    temp = identity_find(1);
    pr_info("id 1 = %s\n", temp->name);
    identity_hire(1);
    temp = identity_find(10);
    if (temp == NULL)
        pr_info("id 10 not found\n");
    identity_destroy(2);
    identity_destroy(1);
    return 0;
}

static void __exit hello_exit(void)
{
    struct identity *identity, *tmp;
    list_for_each_entry_safe(identity, tmp, &data_base, list) {
        pr_info("Removing identity: %s with id %d\n", identity->name, identity->id);
        list_del(&identity->list);
        kfree(identity);
    }
    pr_info("Goodbye, KernelCare!\n");
}

module_init(hello_init);
module_exit(hello_exit);
