#pragma once

#include <QColor>
#include <QVector>
#include <QRect>
#include <QChar>
#include <QPoint>
#include <QString>
#include <QObject>

#include "TextAttributes.h"

struct TextElement
{
    QChar character;
    TextAttributes attributes;
};

class TextBlock
{
    Q_GADGET
public:
    TextBlock(int position = 0);

    int position() const;
    void setPosition(int position);

    int length() const;

    const QVector<TextElement> &elements() const;
    QVector<TextElement> &elements();

private:
    int m_position = 0;
    QVector<TextElement> m_elements;
};

class TextLine
{
    Q_GADGET
public:
    void addBlock(const TextBlock &block);
    void clear();

    int firstIndex() const;
    int lastIndex() const;

    const TextElement *elementAt(int index) const;

    const QVector<TextBlock> &blocks() const;
    QVector<TextBlock> &blocks();

private:
    QVector<TextBlock> m_blocks;
    int m_firstIndex = 0;
    int m_lastIndex = -1;
};

struct RenderCell
{
    QChar character;
    QPoint position;
    TextAttributes attributes;
};

class TextFramebuffer : public QObject
{
    Q_OBJECT
public:
    explicit TextFramebuffer(QObject *parent = nullptr);

    void putText(int row, int column, const QString &text, const TextAttributes &attr);
    void writeLn(const QString &text, const TextAttributes &attr);
    void clear(const QRect &rect);
    void fill(const QRect &rect, QChar ch, const TextAttributes &attr);

    QVector<RenderCell> collect(const QRect &region) const;

    const QVector<TextLine> &lines() const;
    QVector<TextLine> &lines();

signals:
    void changed(int pos, int count);
    void linesAdded(int pos, int count);
    void linesRemoved(int pos, int count);

private:
    QVector<TextLine> m_lines;
};

