//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     DAVID MIHOLA <xmihol00@stud.fit.vutbr.cz>
// $Date:       $2020-20-02
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author DAVID MIHOLA
 * 
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>

#include "gtest/gtest.h"

#include "red_black_tree.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy Red-Black Tree, testujte nasledujici:
// 1. Verejne rozhrani stromu
//    - InsertNode/DeleteNode a FindNode
//    - Chovani techto metod testuje pro prazdny i neprazdny strom.
// 2. Axiomy (tedy vzdy platne vlastnosti) Red-Black Tree:
//    - Vsechny listove uzly stromu jsou *VZDY* cerne.
//    - Kazdy cerveny uzel muze mit *POUZE* cerne potomky.
//    - Vsechny cesty od kazdeho listoveho uzlu ke koreni stromu obsahuji
//      *STEJNY* pocet cernych uzlu.
//============================================================================//

#define BLACK_LEAVES_TREE {60, 90, 70, 40, 30, 20, 10, 25}
#define RED_LEAVES_TREE {60, 90, 70, 40, 30, 20, 10, 5, 15, 25, 35, 55, 65, 85, 95}

enum random_tree_generation {NUM_OF_TREE_NODES = 800, NUM_RANGE = 100000, NUM_OF_GENERATED_TREES = 15, AXIOMS_TREE_NODES = 2500};

//for debugging purposes
#define NDEBUG
#ifndef NDEBUG
#define TREE_PRINT(ROOT) tree_print(ROOT);
class Queue
{
private:
    struct qu
    {
        BinaryTree::Node_t *item;
        qu *next;
    };

    qu *head;
    qu *tail;

public:
    Queue()
    {
        head = NULL;
        tail = NULL;
    }

    ~Queue()
    {
        qu *tmp;
        while(head)
        {
            tmp = head;
            head = head->next;
            delete tmp;
        }
    }

    bool insert_item(BinaryTree::Node_t *item)
    {
        qu *tmp = new qu;
        if (tmp == NULL)
            return true;
        tmp->item = item;
        tmp->next = NULL;

        if (head == NULL)
            head = tail = tmp;
        else
        {
            tail->next = tmp;
            tail = tail->next;
        }
        return false;
    }

    BinaryTree::Node_t *get_item()
    {
        BinaryTree::Node_t *item;
        qu *tmp;
        if (head == NULL)
            return NULL;
        else
        {
            item = head->item;
            tmp = head;
            head = head->next;
            delete tmp;
        }

        return item;
    }

    qu *get_head()
    {
        return head;
    }
};

void tree_print(BinaryTree::Node_t *root)
{
    if (root == NULL)
        return;

    BinaryTree::Node_t *node = root;
    int lenght = 0;
    while(node->pLeft || node->pRight)
    {
        if (node->pLeft)
            node = node->pLeft;
        else if (node->pRight)
            node = node->pRight;
        lenght++;
    }

    int min = root->key;
    Queue queue;
    queue.insert_item(root);

    for (int i = 0; i<(lenght+1); i++)
        printf("\t");

    while(queue.get_head())
    {
        node = queue.get_item();

        if (node->pRight || node->pLeft)
        {
            if (min > node->key && node->key)
            {
                putchar('\n');
                for (int i = 0; i<lenght; i++)
                    printf("\t");
                lenght--;
            }
                min = node->key;
            printf("%3d%s%3d", node->key, (node->color == BLACK)?"BLACK":" RED ", (node->pParent)?(node->pParent->key):0);
        }

        if (node->pLeft)
            queue.insert_item(node->pLeft);
        if (node->pRight)
            queue.insert_item(node->pRight);
    }
    putchar('\n');
}
#else
#define TREE_PRINT(ROOT)
#endif 

enum tree_errors {NO_ERRORS = 0, DOUBLE_RED, RED_ROOT, NOT_ENOUGH_BLACK_NODES, TOO_MANY_BLACK_NODES, INCORRECT_ARRANGEMENT, RED_LEAF};

class EmptyTree: public ::testing::Test
{
protected:
    BinaryTree tree;
};

class NonEmptyTree: public ::testing::Test
{
private:
    unsigned black_nodes_count;
    int randomly_generated_nums[NUM_OF_TREE_NODES] = {0,};

    tree_errors axiom_tree_test(BinaryTree::Node_t* tmp, bool color, unsigned count, bool position)
    {
        static tree_errors err = NO_ERRORS;

        if (tree.GetRoot() == NULL)
            return NO_ERRORS;

        if (tree.GetRoot()->color == RED)
            return RED_ROOT;

        if (tmp->pParent && tmp->pLeft && tmp->pRight)
        {
            if (position)
            {
                if (tmp->key > tmp->pParent->key)
                    return INCORRECT_ARRANGEMENT;
            }
            else
            {
                if (tmp->key < tmp->pParent->key)
                    return INCORRECT_ARRANGEMENT;
            }
        }

        if (color == RED && tmp->color == RED)
        {
            return DOUBLE_RED;
        }
        if (tmp->color == BLACK)
            count++;

        if (tmp->pRight == NULL && tmp->pLeft == NULL)
        {
            if (!black_nodes_count)
                black_nodes_count = count;
            else if (black_nodes_count > count)
                return NOT_ENOUGH_BLACK_NODES;
            else if (black_nodes_count < count)
                return TOO_MANY_BLACK_NODES;
            else if (tmp->color == RED)
                return RED_LEAF;

            return NO_ERRORS;
        }

        if (tmp->pLeft)
            if (err = axiom_tree_test(tmp->pLeft, tmp->color, count, true))
                return err;
        if (tmp->pRight)
            if (err = axiom_tree_test(tmp->pRight, tmp->color, count, false))
            return err;

        return NO_ERRORS;
    }

protected:
    BinaryTree tree;

    void insert_values(std::vector<int> values)
    {
        for (unsigned i = 0; i < values.size(); i++)
            tree.InsertNode(values[i]);
    }


    tree_errors axioms_tree_test()
    {
        black_nodes_count = 0;
        return axiom_tree_test(tree.GetRoot(), BLACK, 0, 0);
    }

    bool generate_tree()
    {
        for(unsigned i = 0; i < NUM_OF_TREE_NODES; i++)
        {
            tree.InsertNode(randomly_generated_nums[i]=((rand() % (2 * NUM_RANGE)) - NUM_RANGE));
            if (axioms_tree_test())
                return true;
        }

        return false;
    }

    bool delete_generated_tree_by_one()
    {
        for(unsigned i = 0; i < NUM_OF_TREE_NODES; i++)
        {
            tree.DeleteNode(randomly_generated_nums[i]);
            if (axioms_tree_test())
                return true;
        }

        return false;
    }

    bool find_nodes()
    {
        BinaryTree::Node_t *tmp;
        for (unsigned i = 0; i < NUM_OF_TREE_NODES; i++)
        {
            if (!(tmp = tree.FindNode(randomly_generated_nums[i])))
               return true;
            if (tmp->key != randomly_generated_nums[i])
                return true;
        }
        return false;
    }
};

class TreeAxioms: public ::testing::Test
{
private:
    unsigned black_nodes_count;
    bool test_black_nodes_path_count(BinaryTree::Node_t *tmp, unsigned count)
    {
        if (tmp == NULL)
            return false;
        
        if (tmp->color == BLACK)
            count++;
        
        if (tmp->pLeft == NULL && tmp->pRight == NULL)
        {
            if (!black_nodes_count)
                black_nodes_count = count;
            else if (black_nodes_count > count)
                return true;
            else if (black_nodes_count < count) 
                return true;
            
            return false;
        }

        if (tmp->pLeft && test_black_nodes_path_count(tmp->pLeft, count)) //lazy evaluation
            return true;
        if (tmp->pRight && test_black_nodes_path_count(tmp->pRight, count)) //lazy evaluation
            return true;

        return false;
    }

protected:
    BinaryTree tree;
    std::vector<BinaryTree::Node_t *> all_nodes;
    std::vector<BinaryTree::Node_t *> leaf_nodes;
    
    bool test_black_nodes_path_count()
    {
        black_nodes_count = 0;
        return test_black_nodes_path_count(tree.GetRoot(), 0);
    }
};

TEST_F(EmptyTree, DeleteNode)
{
    EXPECT_FALSE(tree.DeleteNode(0));
    EXPECT_FALSE(tree.GetRoot());
}

TEST_F(EmptyTree, FindNode)
{
    EXPECT_FALSE(tree.FindNode(0));
    EXPECT_FALSE(tree.GetRoot());
}

TEST_F(EmptyTree, InsertNode)
{
    //parameters of EXPECT_EQ are evaluated backwards
    EXPECT_EQ(std::make_pair(true, tree.GetRoot()), tree.InsertNode(0));

    EXPECT_EQ(tree.GetRoot()->key, 0);
    EXPECT_EQ(tree.GetRoot()->color, BLACK);
    EXPECT_FALSE(tree.GetRoot()->pParent);
    EXPECT_FALSE(tree.GetRoot()->pLeft->key);
    EXPECT_FALSE(tree.GetRoot()->pRight->key);
}

TEST_F(NonEmptyTree, InsertNode_BlackLeavesTree)
{
    insert_values(BLACK_LEAVES_TREE);

    EXPECT_EQ(axioms_tree_test(), NO_ERRORS);
}

TEST_F(NonEmptyTree, InsertNode_RedLeavesTree)
{
    insert_values(RED_LEAVES_TREE);

    EXPECT_EQ(axioms_tree_test(), NO_ERRORS);
}

TEST_F(NonEmptyTree, InsertNode_ToBlackNode)
{
    insert_values(BLACK_LEAVES_TREE);

    //left insertion
    ASSERT_EQ(std::make_pair(true, tree.FindNode(5)), tree.InsertNode(5));
    EXPECT_EQ(tree.FindNode(5)->color, RED);
    EXPECT_EQ(tree.FindNode(5)->pParent->color, BLACK);

    EXPECT_EQ(axioms_tree_test(), NO_ERRORS);

    //right insertion
    ASSERT_EQ(std::make_pair(true, tree.FindNode(65)), tree.InsertNode(65));
    EXPECT_EQ(tree.FindNode(65)->color, RED);
    EXPECT_EQ(tree.FindNode(65)->pParent->color, BLACK);

    EXPECT_EQ(axioms_tree_test(), NO_ERRORS);
}

TEST_F(NonEmptyTree, InsertNode_ColorFlip)
{
    insert_values(RED_LEAVES_TREE);

    //left insertion
    ASSERT_EQ(tree.FindNode(55)->color, RED);
    EXPECT_FALSE(tree.FindNode(55)->pLeft->key);
    EXPECT_FALSE(tree.FindNode(55)->pRight->key);
    ASSERT_EQ(std::make_pair(true, tree.FindNode(45)), tree.InsertNode(45));
    EXPECT_EQ(tree.FindNode(60)->color, RED);
    EXPECT_EQ(tree.FindNode(60)->pParent->color, BLACK);
    EXPECT_EQ(tree.FindNode(45)->color, RED);
    EXPECT_EQ(tree.FindNode(55)->color, BLACK);
    EXPECT_EQ(tree.FindNode(65)->color, BLACK);

    EXPECT_EQ(axioms_tree_test(), NO_ERRORS);

    //right insertion
    ASSERT_EQ(tree.FindNode(95)->color, RED);
    EXPECT_FALSE(tree.FindNode(95)->pLeft->key);
    EXPECT_FALSE(tree.FindNode(95)->pRight->key);
    ASSERT_EQ(std::make_pair(true, tree.FindNode(100)), tree.InsertNode(100));
    EXPECT_EQ(tree.FindNode(90)->color, RED);
    EXPECT_EQ(tree.FindNode(90)->pParent->color, BLACK);
    EXPECT_EQ(tree.FindNode(100)->color, RED);
    EXPECT_EQ(tree.FindNode(85)->color, BLACK);
    EXPECT_EQ(tree.FindNode(95)->color, BLACK);

    EXPECT_EQ(axioms_tree_test(), NO_ERRORS);
}

TEST_F(NonEmptyTree, InsertNode_LLrotation) 
{                                                
    insert_values(BLACK_LEAVES_TREE);        
    tree.DeleteNode(25); // for initial set up

    ASSERT_EQ(std::make_pair(true, tree.FindNode(28)), tree.InsertNode(28));
    ASSERT_EQ(std::make_pair(true, tree.FindNode(26)), tree.InsertNode(26));
    EXPECT_EQ(tree.FindNode(28)->color, BLACK);
    EXPECT_EQ(tree.FindNode(30)->color, RED);
    EXPECT_EQ(tree.FindNode(26)->color, RED);

    EXPECT_EQ(axioms_tree_test(), NO_ERRORS);
}

TEST_F(NonEmptyTree, InsertNode_RRrotation)
{
    insert_values(BLACK_LEAVES_TREE);

    ASSERT_EQ(std::make_pair(true, tree.FindNode(62)), tree.InsertNode(62));
    ASSERT_EQ(std::make_pair(true, tree.FindNode(64)), tree.InsertNode(64));
    EXPECT_EQ(tree.FindNode(62)->color, BLACK);
    EXPECT_EQ(tree.FindNode(60)->color, RED);
    EXPECT_EQ(tree.FindNode(64)->color, RED);

    EXPECT_EQ(axioms_tree_test(), NO_ERRORS);
}

TEST_F(NonEmptyTree, InsertNode_LRrotation)
{
    insert_values(BLACK_LEAVES_TREE);
    ASSERT_EQ(std::make_pair(true, tree.FindNode(5)), tree.InsertNode(5));
    ASSERT_EQ(std::make_pair(true, tree.FindNode(8)), tree.InsertNode(8));
    EXPECT_EQ(tree.FindNode(8)->color, BLACK);
    EXPECT_EQ(tree.FindNode(5)->color, RED);
    EXPECT_EQ(tree.FindNode(10)->color, RED);

    EXPECT_EQ(axioms_tree_test(), NO_ERRORS);
}

TEST_F(NonEmptyTree, InsertNode_RLrotation)
{
    insert_values(BLACK_LEAVES_TREE);
    ASSERT_EQ(std::make_pair(true, tree.FindNode(100)), tree.InsertNode(100));
    ASSERT_EQ(std::make_pair(true, tree.FindNode(95)), tree.InsertNode(95));
    EXPECT_EQ(tree.FindNode(95)->color, BLACK);
    EXPECT_EQ(tree.FindNode(100)->color, RED);
    EXPECT_EQ(tree.FindNode(90)->color, RED);

    EXPECT_EQ(axioms_tree_test(), NO_ERRORS);
}

TEST_F(NonEmptyTree, FindNode_test1)
{
    insert_values(BLACK_LEAVES_TREE);
    std::vector<int> nodes = BLACK_LEAVES_TREE;

    for (unsigned i = 0; i < nodes.size(); i++)
    {
        ASSERT_TRUE(tree.FindNode(nodes[i]));
        EXPECT_EQ(tree.FindNode(nodes[i])->key, nodes[i]);
    }

    EXPECT_EQ(axioms_tree_test(), NO_ERRORS);

    nodes = {-10, -5, 4, 88, 12, -87, -25, 36, 78, 41, -41, -25, 32, 51, 53, 48};

    for(unsigned i = 0; i < nodes.size(); i++)
        EXPECT_FALSE(tree.FindNode(nodes[i]));

    EXPECT_EQ(axioms_tree_test(), NO_ERRORS);
}

TEST_F(NonEmptyTree, FindNode_test2)
{
    insert_values(RED_LEAVES_TREE);
    std::vector<int> nodes = RED_LEAVES_TREE;

    for (unsigned i = 0; i < nodes.size(); i++)
    {
        ASSERT_TRUE(tree.FindNode(nodes[i]));
        EXPECT_EQ(tree.FindNode(nodes[i])->key, nodes[i]);
    }

    EXPECT_EQ(axioms_tree_test(), NO_ERRORS);

    nodes = {2, 8, 13, 18, 23, 29, 34, 41, 51, 57, 63, 81, 97, 103, 111, -5, -8, -32, -14, -56, -27, -74};

    for(unsigned i = 0; i < nodes.size(); i++)
        EXPECT_FALSE(tree.FindNode(nodes[i]));
    
    EXPECT_EQ(axioms_tree_test(), NO_ERRORS);
}

//deletion cases which I hade to solve in my own implementation of RB_tree
TEST_F(NonEmptyTree, DeleteNode_RedLeaf)
{
    insert_values({2, 4, 6, 7, 8});
    EXPECT_TRUE(tree.DeleteNode(6));
    EXPECT_FALSE(axioms_tree_test());
}

TEST_F(NonEmptyTree, DeleteNode_Root) //there is only the root
{
    insert_values({25});
    EXPECT_TRUE(tree.DeleteNode(25));
    EXPECT_FALSE(axioms_tree_test());
}

TEST_F(NonEmptyTree, DeleteNode_BlackWithRedLeafChild)
{
    insert_values({10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 250, 
                   240, 260, 180, 190, 200, 125, 135, 270});
    //both red leaves
    EXPECT_TRUE(tree.DeleteNode(130));
    //left red leav
    EXPECT_TRUE(tree.DeleteNode(240));
    //right red leav
    EXPECT_TRUE(tree.DeleteNode(260));

    EXPECT_FALSE(axioms_tree_test());
}

TEST_F(NonEmptyTree, DeleteNode_BlackLeafWithRedNieces) //Parent color is irelevant
{
    insert_values({10, 20, 30, 40, 50, 60, 70, 8, 5, 3, 6});
    //black left leaf
    EXPECT_TRUE(tree.DeleteNode(30));
    //black right leaf
    EXPECT_TRUE(tree.DeleteNode(10));
    EXPECT_FALSE(axioms_tree_test());
}

TEST_F(NonEmptyTree, DeleteNode_BlackLeafWithoutNieces) //Parent color is irelevant
{
    insert_values({10, 20, 30, 40, 50, 60, 70, 80});
    tree.DeleteNode(70);

    //black left leaf
    EXPECT_TRUE(tree.DeleteNode(10));
    //black right leaf 
    EXPECT_TRUE(tree.DeleteNode(80));
    EXPECT_FALSE(axioms_tree_test());
}

TEST_F(NonEmptyTree, DeleteNode_BlackLeafWithOneNiece) //Parent color is irelevant 
{
    insert_values({500, 250, 750, 400, 600, 200, 800, 900, 300, 700, 850, 150, 450, 550, 350, 650, 10, 5, 970});
    //right black leaf, left niece
    EXPECT_TRUE(tree.DeleteNode(350));
    //right black leaf, right niece
    EXPECT_TRUE(tree.DeleteNode(450));
    //left black leaf, left niece
    EXPECT_TRUE(tree.DeleteNode(550));
    //left black leaf, right niece
    EXPECT_TRUE(tree.DeleteNode(800));
    EXPECT_FALSE(axioms_tree_test());
}

TEST_F(NonEmptyTree, DeleteNode_BlackLeafWithBlackParentAndRedSibling)
{
    insert_values({10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 250, 240, 260, 180, 190, 200,
                   210, 220, 230, 240, 250, 260, 185, 175, 178});
    //left black leaf
    EXPECT_TRUE(tree.DeleteNode(130));
    //right black leaf
    EXPECT_TRUE(tree.DeleteNode(260));

    EXPECT_FALSE(axioms_tree_test());
}

TEST_F(NonEmptyTree, DeleteNode_DoublelBackNode) //Black leaf with black parent and black sibling without nieces
{                                                //Double black node is created
    insert_values({10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 250, 240, 260, 180, 190});

    //left black leaf
    EXPECT_TRUE(tree.DeleteNode(10));
    
    //right black leaf
    EXPECT_TRUE(tree.DeleteNode(110));

    EXPECT_FALSE(axioms_tree_test());
}

TEST_F(NonEmptyTree, DeleteNode_InsideTree)
{
    insert_values({10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 250, 240, 260, 180, 190});

    //red node
    EXPECT_TRUE(tree.DeleteNode(40));

    //black node
    EXPECT_TRUE(tree.DeleteNode(80));

    EXPECT_FALSE(axioms_tree_test());
}


TEST_F(TreeAxioms, Axiom1)
{
    for (unsigned i = 0; i < AXIOMS_TREE_NODES; i++)
    {
        tree.InsertNode(((rand() % (2 * NUM_RANGE)) - NUM_RANGE));

        tree.GetLeafNodes(leaf_nodes);

        for (unsigned i = 0; i < leaf_nodes.size(); i++)
            ASSERT_EQ(leaf_nodes[i]->color, BLACK);
    }
}

TEST_F(TreeAxioms, Axiom2)
{
    for (unsigned i = 0; i < AXIOMS_TREE_NODES; i++)
    {
        tree.InsertNode(((rand() % (2 * NUM_RANGE)) - NUM_RANGE));

        tree.GetAllNodes(all_nodes);

        for (unsigned i = 0; i < all_nodes.size(); i++)
        {
            if (all_nodes[i]->color == RED)
            {
                ASSERT_EQ(all_nodes[i]->pRight->color, BLACK);
                ASSERT_EQ(all_nodes[i]->pLeft->color, BLACK);
            }
        }
    }
}

TEST_F(TreeAxioms, Axiom3)
{
    for (unsigned i = 0; i < AXIOMS_TREE_NODES; i++)
    {
        tree.InsertNode(((rand() % (2 * NUM_RANGE)) - NUM_RANGE));

        ASSERT_FALSE(test_black_nodes_path_count());
    }
}

//testing on randomly generated trees
TEST_F(NonEmptyTree, DeleteNode_RandomGeneration)
{
    for (unsigned i = 0; i < NUM_OF_GENERATED_TREES; i++)
    {
        generate_tree();
        EXPECT_TRUE(tree.GetRoot());
        ASSERT_FALSE(delete_generated_tree_by_one());
        EXPECT_FALSE(tree.GetRoot());
    }
}

TEST_F(NonEmptyTree, FindNode_RandomGeneration)
{
    generate_tree();
    EXPECT_FALSE(find_nodes());
    EXPECT_EQ(axioms_tree_test(), NO_ERRORS);
}

TEST_F(NonEmptyTree, InsertNode_RandomGeneration)
{
    for (unsigned i = 0; i < NUM_OF_GENERATED_TREES; i++)
    {
        ASSERT_FALSE(generate_tree());
        EXPECT_TRUE(tree.GetRoot());
        tree.~BinaryTree();
        EXPECT_FALSE(tree.GetRoot());
    }
}

/*** Konec souboru black_box_tests.cpp ***/
