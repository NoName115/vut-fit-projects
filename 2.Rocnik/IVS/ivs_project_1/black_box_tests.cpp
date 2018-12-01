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

class NonEmptyBinaryTree : public ::testing::Test
{
protected:
    virtual void SetUp() {
        int values[] = { 10, 85, 15, 70, 20, 60, 30, 50, 65, 80, 90, 40, 5, 55 };

        for(int i = 0; i < 14; ++i)
            binaryTree.InsertNode(values[i]);
    }

    BinaryTree binaryTree;
};

class EmptyBinaryTree : public ::testing::Test
{
protected:
    BinaryTree binaryTree;
};


// NonEmpty tree tests
TEST_F(NonEmptyBinaryTree, InsertNode)
{
	EXPECT_FALSE(binaryTree.InsertNode(10).first);
	EXPECT_TRUE(binaryTree.InsertNode(100).first);
}

TEST_F(NonEmptyBinaryTree, DeleteNode)
{
	EXPECT_TRUE(binaryTree.DeleteNode(60));
	EXPECT_FALSE(binaryTree.DeleteNode(120));
}

TEST_F(NonEmptyBinaryTree, FindNode)
{
	EXPECT_TRUE(binaryTree.FindNode(10) != NULL);
	EXPECT_TRUE(binaryTree.FindNode(120) == NULL);
}

TEST_F(NonEmptyBinaryTree, LeafNodesBlack)
{
	std::vector<BinaryTree::Node_t *> outLeafNodes;
	binaryTree.GetLeafNodes(outLeafNodes);

	for(std::vector<BinaryTree::Node_t *>::iterator i = outLeafNodes.begin(); i != outLeafNodes.end(); ++i)
	{
    	EXPECT_TRUE((*i)->color == BinaryTree::Color_t::BLACK);
	}
}

TEST_F(NonEmptyBinaryTree, ParentRedChildrenBlack)
{
	std::vector<BinaryTree::Node_t *> allNodes;
	binaryTree.GetAllNodes(allNodes);

	for (std::vector<BinaryTree::Node_t *>::iterator i = allNodes.begin(); i != allNodes.end(); ++i)
	{
		if ((*i)->color == BinaryTree::Color_t::RED)
		{
			if ((*i)->pLeft != NULL)
			{
				EXPECT_TRUE((*i)->pLeft->color == BinaryTree::Color_t::BLACK);
			}

			if ((*i)->pRight != NULL)
			{
				EXPECT_TRUE((*i)->pRight->color == BinaryTree::Color_t::BLACK);
			}
		}
	}
}

TEST_F(NonEmptyBinaryTree, SameNumberOfBlackNodes)
{
	std::vector<BinaryTree::Node_t *> leafNodes;
	binaryTree.GetLeafNodes(leafNodes);
	std::vector<int> arrCounter;

	for (std::vector<BinaryTree::Node_t *>::iterator i = leafNodes.begin(); i != leafNodes.end(); ++i)
	{
		int counter = 0;
		BinaryTree::Node_t *tempNode = (*i)->pParent;
		while(tempNode != NULL)
		{
			if (tempNode->color == BinaryTree::Color_t::BLACK)
			{
				counter++;
			}
			tempNode = tempNode->pParent;
		}
		arrCounter.push_back(counter);
	}

	bool sameElements = false;
	if ( std::adjacent_find( arrCounter.begin(), arrCounter.end(), std::not_equal_to<int>() ) == arrCounter.end() )
	{
    	sameElements = true;
	}

	EXPECT_TRUE(sameElements);
}

// Empty tree tests
TEST_F(EmptyBinaryTree, DeleteNode)
{
	EXPECT_FALSE(binaryTree.DeleteNode(2));
}

TEST_F(EmptyBinaryTree, FindNode)
{
	EXPECT_TRUE(binaryTree.FindNode(2) == NULL);
}

TEST_F(EmptyBinaryTree, InsertNode)
{
	EXPECT_TRUE(binaryTree.InsertNode(5).first);
	EXPECT_TRUE(binaryTree.InsertNode(6).second != NULL);
}

/*** Konec souboru black_box_tests.cpp ***/
