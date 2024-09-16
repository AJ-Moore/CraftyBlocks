#include "pch.h"
#include <Core/ResourceManager/ResourceManager.h>

TEST(TestCaseName, TestName)
{
	CraftyBlocks::ResourceManager rm; 
	rm.GetMaterial("assets/materials/default.material");
	EXPECT_EQ(1, 1);
	EXPECT_TRUE(true);
}