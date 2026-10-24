#include <gtest/gtest.h>

TEST(AddTests, ADD_Immediate)
{
	int x = 0;
	int y = 2;

	int result = x + y;

	ASSERT_EQ(result, 2);
}
