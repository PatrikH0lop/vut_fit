//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     JMENO PRIJMENI <xlogin00@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author JMENO PRIJMENI
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

class NonEmptyTree : public ::testing::Test
{
protected:
    virtual void SetUp() {
        int values[] = { 10, 85, 15, 70, 20, 60, 30, 50, 65, 80, 90, 40, 5, 55 };

        for(int i = 0; i < 14; ++i)
            tree.InsertNode(values[i]);
    }

    BinaryTree tree;
};

class EmptyTree : public ::testing::Test
{
protected:
    BinaryTree tree;
};

class NonEmptyTreeAxioms : public ::testing::Test
{
protected:
    BinaryTree tree;
    virtual void SetUp() {
        int values[] = { 10, 85, 15, 70, 20, 60, 30, 50, 65, 80, 90, 40, 5, 55 };

        for(int i = 0; i < 14; ++i)
            tree.InsertNode(values[i]);
    }
};

TEST_F(EmptyTree, InsertNode)
{
    tree.InsertNode(0);
    EXPECT_EQ(tree.GetRoot()->key, 0);

    tree.InsertNode(100);
    EXPECT_EQ(tree.GetRoot()->key, 0);
}


TEST_F(EmptyTree, DeleteNode)
{
    EXPECT_FALSE(tree.DeleteNode(0));
    EXPECT_TRUE(tree.GetRoot()==NULL);
    EXPECT_FALSE(tree.DeleteNode(-5));
    EXPECT_TRUE(tree.GetRoot()==NULL);
}

TEST_F(EmptyTree, FindNode)
{
    EXPECT_FALSE(tree.FindNode(0));
    EXPECT_FALSE(tree.FindNode(-5));
}

TEST_F(NonEmptyTree, DeleteNodes)
{
    EXPECT_FALSE(tree.DeleteNode(0));

    int values[] = { 5, 10, 15, 20, 30, 40, 50, 55, 60, 65, 70, 80, 85, 90 };
    for(int i = 0; i < 13; ++i)
    {
        EXPECT_TRUE(tree.DeleteNode(values[i]));
    }

    tree.DeleteNode(90);
    EXPECT_TRUE(tree.GetRoot() == NULL);
}

TEST_F(NonEmptyTree, FindNodes)
{
    int valuesOK[] = { 5, 10, 15, 20, 30, 40, 50, 55, 60, 65, 70, 80, 85, 90 };
    int valuesFALSE[] = {0,100,560,11,-2};
    for(int i = 0; i < 13; ++i)
    {
        EXPECT_TRUE(tree.FindNode(valuesOK[i]));
    }
    for(int i = 0; i < 5; i++)
    {
        EXPECT_FALSE(tree.FindNode(valuesFALSE[i]));
    }
}

TEST_F(NonEmptyTree, InsertNode)
{
    int valuesOK[] = { 31, 41, 51, 91, 101, 401};
    for(int i = 0; i < 5; i++)
    {
        std::pair<bool, void *> t = tree.InsertNode(valuesOK[i]);
        EXPECT_TRUE(t.first);
        EXPECT_EQ(tree.FindNode(valuesOK[i]), t.second); 
    }
}


TEST_F(NonEmptyTreeAxioms, AllLeafsBlack)
{
    std::vector<BinaryTree::Node_t *> ln;
    tree.GetLeafNodes(ln);
    for(int i = 0; i < ln.size(); i++)
    {
        EXPECT_EQ(ln[i]->color, 1); 
    }
}

TEST_F(NonEmptyTreeAxioms, BlackNodesAfterRed)
{
    std::vector<BinaryTree::Node_t *> ln;
    tree.GetNonLeafNodes(ln);
    for(int i = 0; i < ln.size(); i++)
    {
        if (ln[i]->color==0) {
            EXPECT_EQ(ln[i]->pLeft->color, 1); 
            EXPECT_EQ(ln[i]->pRight->color, 1); 
        }
    }
}

TEST_F(NonEmptyTreeAxioms, NumbersOfBlack)
{
    std::vector<BinaryTree::Node_t *> ln;
    tree.GetLeafNodes(ln);
    int numberOfBlack = 0;
    while(ln[0]!=tree.GetRoot()) {
        if (ln[0]->color==1) 
            numberOfBlack++;
        ln.at(0)=ln[0]->pParent;
    }
    int nob = 0;
    for(int i = 1; i < ln.size(); i++)
    {
        nob = 0;
        while(ln[i]!=tree.GetRoot()) {
            if (ln[i]->color==1) 
                nob++;
            ln.at(i)=ln[i]->pParent;
        }
        EXPECT_EQ(numberOfBlack, nob); 
    }
}


/*** Konec souboru black_box_tests.cpp ***/
