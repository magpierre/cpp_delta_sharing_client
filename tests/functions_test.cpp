#include <gtest/gtest.h>
#include <string>
#include "../include/functions.h"

TEST(DeltaSharingTest, ParseURL_Test1) {
    std::string s("magnus#a.b.c");
    std::string m[] = {"magnus","a", "b", "c"};
    auto r = DeltaSharing::ParseURL(s);
    
    EXPECT_EQ(r.size(), 4);
    EXPECT_EQ(r.at(0),m[0] );
    EXPECT_EQ(r.at(1),m[1] );
    EXPECT_EQ(r.at(2),m[2] );
    EXPECT_EQ(r.at(3),m[3] );
}