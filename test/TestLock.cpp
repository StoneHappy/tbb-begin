#include <gtest/gtest.h>

#include <Core/test.h>

TEST(TBBTest, SumWithnoLock)
{
	EXPECT_EQ(SumWithnoLock(), 100);
}

TEST(TBBTest, SumWithLock)
{
	EXPECT_EQ(SumWithLock(), 5050);
}