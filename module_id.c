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
    struct identity *node = NULL;

    list_for_each_entry(node, &data_base, list)
    {
        if (node->id == id)
        {
            pr_info("Identify with id:%d already exist\n", id);
            return -EEXIST;
        }
    }

    node = kmalloc(sizeof(*node), GFP_KERNEL);
    if (!node) {
        pr_info("Cannot create identify: %s with id %d. Memory error\n", name, id);
        return -ENOMEM;
    }

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
    struct identity *identity = NULL;

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
    struct identity *identity = NULL;

    list_for_each_entry(identity, &data_base, list)
    {
        if (identity->id == id)
        {
            list_del(&identity->list);
            kfree(identity);
            pr_info("Id:%d was deleted successfully!\n", id);
            return;
        }
    }
    pr_info("Id:%d was NOT founded!\n", id);

}

int identity_hire(int id)
{
    struct identity *identity = NULL;
    list_for_each_entry(identity, &data_base, list)
    {
        if (identity->id == id)
        {
            identity->hired = true;
            pr_info("Id:%d was hired successfully!\n", id);
            return 0;
        }
    }
    return -1;
}

static int __init identity_manager_init(void)
{
    pr_info("Hello, KernelCare!\n");

    if(identity_create("Andrey", 1))
        pr_info("Module couldn't create identity\n");
    if(identity_create("Ivan", 2))
        pr_info("Module couldn't create identity\n");
    if(identity_create("Aleksey", 3))
        pr_info("Module couldn't create identity\n");
    if(identity_create("Boris", 3))
        pr_info("Module couldn't create identity\n");

    struct identity * temp = identity_find(1);
    if(temp)
        pr_info("id 1 = %s\n", temp->name);
    if(identity_hire(1))
        pr_info("Module cannot hire identity\n");
    temp = identity_find(10);
    if (!temp)
        pr_info("id 10 not found\n");
    identity_destroy(2);
    identity_destroy(1);
    return 0;
}

static void __exit identity_manager_exit(void)
{
    struct identity *identity, *tmp;
    list_for_each_entry_safe(identity, tmp, &data_base, list) {
        pr_info("Removing identity: %s with id %d\n", identity->name, identity->id);
        list_del(&identity->list);
        kfree(identity);
    }
    pr_info("We have deleted all of garbage. Goodbye, KernelCare!\n");
}

module_init(identity_manager_init);
module_exit(identity_manager_exit);
