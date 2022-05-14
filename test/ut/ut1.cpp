//
// Created by Szymon Jędras on 5/12/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//

#include <gtest/gtest.h>

class test_test : public ::testing::Test
{
};


TEST_F(test_test, test1) {
    ASSERT_TRUE(true);
}

