#include "TextFramebuffer.h"

#include <algorithm>

TextBlock::TextBlock(int position)
    : m_position(position)
{
}

int TextBlock::position() const
{
    return m_position;
}

void TextBlock::setPosition(int position)
{
    m_position = position;
}

int TextBlock::length() const
{
    return m_elements.size();
}

const QVector<TextElement> &TextBlock::elements() const
{
    return m_elements;
}

QVector<TextElement> &TextBlock::elements()
{
    return m_elements;
}

void TextLine::addBlock(const TextBlock &block)
{
    if (m_blocks.isEmpty()) {
        m_firstIndex = block.position();
        m_lastIndex = block.position() + block.length() - 1;
    } else {
        m_firstIndex = std::min(m_firstIndex, block.position());
        m_lastIndex = std::max(m_lastIndex, block.position() + block.length() - 1);
    }
    m_blocks.push_back(block);
}

void TextLine::clear()
{
    m_blocks.clear();
    m_firstIndex = 0;
    m_lastIndex = -1;
}

int TextLine::firstIndex() const
{
    return m_firstIndex;
}

int TextLine::lastIndex() const
{
    return m_lastIndex;
}

const TextElement *TextLine::elementAt(int index) const
{
    for (const TextBlock &block : m_blocks) {
        if (index >= block.position() && index < block.position() + block.length()) {
            return &block.elements().at(index - block.position());
        }
    }
    return nullptr;
}

const QVector<TextBlock> &TextLine::blocks() const
{
    return m_blocks;
}

QVector<TextBlock> &TextLine::blocks()
{
    return m_blocks;
}


void TextFramebuffer::putText(int row, int column, const QString &text, const TextAttributes &attr)
{
    if (row < 0 || column < 0)
        return;
    int oldSize = m_lines.size();
    if (m_lines.size() <= row) {
        m_lines.resize(row + 1);
    }
    TextBlock block(column);
    block.elements().reserve(text.size());
    for (QChar ch : text) {
        block.elements().append({ch, attr});
    }
    m_lines[row].addBlock(block);
}

void TextFramebuffer::writeLn(const QString &text, const TextAttributes &attr)
{
    putText(m_lines.size(), 0, text, attr);
}

void TextFramebuffer::clear(const QRect &rect)
{
    for (int y = rect.top(); y <= rect.bottom() && y < m_lines.size(); ++y) {
        if (y < 0)
            continue;
        if (rect.left() == 0 && rect.right() >= m_lines[y].lastIndex()) {
            m_lines[y].clear();
            continue;
        }
        // partial clear
        QVector<TextBlock> newBlocks;
        for (const TextBlock &block : m_lines[y].blocks()) {
            int start = block.position();
            int end = block.position() + block.length() - 1;
            if (end < rect.left() || start > rect.right()) {
                newBlocks.push_back(block);
                continue;
            }
            // add left part
            if (start < rect.left()) {
                TextBlock left(start);
                int count = rect.left() - start;
                left.elements().reserve(count);
                for (int i = 0; i < count; ++i)
                    left.elements().append(block.elements().at(i));
                newBlocks.push_back(left);
            }
            // add right part
            if (end > rect.right()) {
                TextBlock right(rect.right() + 1);
                int offset = rect.right() + 1 - start;
                int count = end - rect.right();
                right.elements().reserve(count);
                for (int i = 0; i < count; ++i)
                    right.elements().append(block.elements().at(offset + i));
                newBlocks.push_back(right);
            }
        }
        m_lines[y].clear();
        for (const TextBlock &b : newBlocks)
            m_lines[y].addBlock(b);
    }
}

void TextFramebuffer::fill(const QRect &rect, QChar ch, const TextAttributes &attr)
{
    for (int y = rect.top(); y <= rect.bottom(); ++y) {
        if (y < 0)
            continue;
        QString str(rect.width(), ch);
        putText(y, rect.left(), str, attr);
    }
}

QVector<RenderCell> TextFramebuffer::collect(const QRect &region) const
{
    QVector<RenderCell> cells;
    for (int y = region.top(); y <= region.bottom() && y < m_lines.size(); ++y) {
        if (y < 0)
            continue;
        for (const TextBlock &block : m_lines[y].blocks()) {
            int start = std::max(block.position(), region.left());
            int end = std::min(block.position() + block.length() - 1, region.right());
            for (int i = start; i <= end; ++i) {
                const TextElement *el = m_lines[y].elementAt(i);
                if (!el)
                    continue;
                if (el->attributes.isValid())
                    cells.append({el->character, QPoint(i, y), el->attributes});
            }
        }
    }
    return cells;
}

const QVector<TextLine> &TextFramebuffer::lines() const
{
    return m_lines;
}

QVector<TextLine> &TextFramebuffer::lines()
{
    return m_lines;
}

