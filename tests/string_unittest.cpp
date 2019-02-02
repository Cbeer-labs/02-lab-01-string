// Copyright 2018 Your Name <your_email>

#include <gtest/gtest.h>

#include <sstream>
#include <string.hpp>

TEST(String, ConstructAssign) {
  const char* data = "some string";

  String s0(data);
  s0[0] = 'S';
  String s1(s0);
  s1[5] = 'S';

  String s2;
  s2 = s1;
  s2[0] = 's';

  EXPECT_EQ("Some string", s0);
  EXPECT_EQ("Some String", s1);
  EXPECT_EQ("some String", s2);
  EXPECT_EQ(String("some string"), String(data));
}

TEST(String, GetByIndex) {
  const char* data = "some string";

  const String s1(data);

  EXPECT_EQ('s', s[0]);
  EXPECT_EQ('m', s[2]);
  EXPECT_EQ('s', s[5]);

  String s(data);
  s[2] = 'M';
  s[5] = s[0] = 'S';

  EXPECT_EQ('S', s[0]);
  EXPECT_EQ('M', s[2]);
  EXPECT_EQ('S', s[5]);
}

TEST(String, Append) {
  String s1("some");
  s1 += " ";
  s1 += String("string");

  String s2 = String("some ") + String("string");

  EXPECT_EQ(s1, s2);
}

TEST(String, Replace) {
  String s1("some string");

  s1.Replace('s', 'S');

  EXPECT_EQ(String("Some String"), s1);
}

TEST(String, Size) {
  String s1("some");

  EXPECT_EQ(s1.Size(), 4);
  EXPECT_TRUE(String{}.Empty());

  EXPECT_FALSE(s1.Empty());
}

TEST(String, Mult) {
  String s1("ABC");
  s1 *= 2;

  EXPECT_EQ(s1, String("ABCABCABC"));

  String s2 = s1 * 3;
  EXPECT_EQ(s2, String("ABCABCABCABCABCABCABCABCABC"));
}

TEST(String, Trim) {
  String s("  abc   ____");

  s.RTrim('_');
  EXPECT_EQ(s, String("  abc   "));

  s.LTrim(' ');
  EXPECT_EQ(s, String("abc   "));

  s.RTrim(' ');
  EXPECT_EQ(s, String("abc"));
}

TEST(String, Find) {
  String s1("some string");

  EXPECT_EQ(4, s1.Find(" "));
  EXPECT_EQ(5, s1.Find("str"));
  EXPECT_EQ(0, s1.Find("some"));

  EXPECT_EQ(static_cast<size_t>(-1), s1.Find("not found"));
}

TEST(String, Stream) {
  std::stringstream stream;
  String s1("some string");
  stream << s1 << " and " << String("stream");

  EXPECT_EQ("some string and stream", ss.str());
}

TEST(String, ConstructAssign) {
  String s0("some string");
  String s1("other string");
  

  EXPECT_EQ("some string", s0);
  EXPECT_EQ("other string", s1);

  s0.swap(s1);

  EXPECT_EQ("other string", s0);
  EXPECT_EQ("some string", s1);
}