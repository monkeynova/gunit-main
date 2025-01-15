#include "fuzztest/fuzztest.h"
#include "gtest/gtest.h"

TEST(Meta, Meta) {
    // The purpose of this test is to verify that main_lib works with it, so
    // we don't actually assert anything interesting.
    EXPECT_EQ(1, 1);
}

void Fuzzable(int arg) {
    EXPECT_NE(arg, 0);
}

FUZZ_TEST(Fuzzable, Fuzzable)
    .WithDomains(/*i:*/fuzztest::NonZero<int>());
