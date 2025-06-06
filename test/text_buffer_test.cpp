#include <gtest/gtest.h>
#include "TextFrameBuffer.h"

TEST(TextLineTest, UpdatesBounds) {
    TextLine line;
    TextAttributes attr(QColor(), QColor(), QColor(), 0.0f, nullptr);
    TextBlock b1(5);
    b1.elements().append({QChar('a'), attr});
    b1.elements().append({QChar('b'), attr});
    b1.elements().append({QChar('c'), attr});
    line.addBlock(b1);

    EXPECT_EQ(line.firstIndex(), 5);
    EXPECT_EQ(line.lastIndex(), 7);

    TextBlock b2(2);
    b2.elements().append({QChar('x'), attr});
    b2.elements().append({QChar('y'), attr});
    line.addBlock(b2);

    EXPECT_EQ(line.firstIndex(), 2);
    EXPECT_EQ(line.lastIndex(), 7);

    ASSERT_NE(line.elementAt(2), nullptr);
    EXPECT_EQ(line.elementAt(2)->character, QChar('x'));
    EXPECT_EQ(line.elementAt(3)->character, QChar('y'));
    EXPECT_EQ(line.elementAt(5)->character, QChar('a'));
    EXPECT_EQ(line.elementAt(7)->character, QChar('c'));
    EXPECT_EQ(line.elementAt(4), nullptr);

    line.clear();
    EXPECT_EQ(line.firstIndex(), 0);
    EXPECT_EQ(line.lastIndex(), -1);
    EXPECT_TRUE(line.blocks().isEmpty());
}

TEST(TextFrameBufferTest, FillClearPutTextCollect) {
    TextFrameBuffer fb;
    TextAttributes attr(QColor(), QColor(), QColor(), 0.0f, nullptr);

    fb.fill(QRect(0, 0, 3, 1), QChar('x'), attr);
    ASSERT_EQ(fb.lines().size(), 1);
    EXPECT_EQ(fb.lines()[0].firstIndex(), 0);
    EXPECT_EQ(fb.lines()[0].lastIndex(), 2);

    fb.clear(QRect(1, 0, 2, 1));
    EXPECT_EQ(fb.lines()[0].firstIndex(), 0);
    EXPECT_EQ(fb.lines()[0].lastIndex(), 0);
    EXPECT_EQ(fb.lines()[0].elementAt(0)->character, QChar('x'));
    EXPECT_EQ(fb.lines()[0].elementAt(1), nullptr);

    fb.putText(1, 0, QStringLiteral("hello"), attr);
    fb.writeLn(QStringLiteral("end"), attr);

    auto cells = fb.collect(QRect(0, 0, 5, 3));
    std::vector<QPoint> positions;
    for (const auto &c : cells)
        positions.push_back(c.position);

    EXPECT_TRUE(std::find(positions.begin(), positions.end(), QPoint(0, 0)) != positions.end());
    EXPECT_TRUE(std::find(positions.begin(), positions.end(), QPoint(0, 1)) != positions.end());
    EXPECT_TRUE(std::find(positions.begin(), positions.end(), QPoint(4, 1)) != positions.end());
    EXPECT_TRUE(std::find(positions.begin(), positions.end(), QPoint(0, 2)) != positions.end());
}

