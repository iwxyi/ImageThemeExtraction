#ifndef COLOROCTREE_H
#define COLOROCTREE_H

#include <QColor>
#include <QList>
#include <QImage>
#include <QDebug>

typedef QList<QColor> ColorList;

class ColorOctree
{
public:
    ColorOctree();
    ColorOctree(QImage image, int maxPool = 250000);
    ~ColorOctree();

    struct RGB
    {
        int red, green, blue;
    };

    struct OctreeNode
    {
        ~OctreeNode()
        {
            for (int i = 0; i < 8; i++)
                if (children[i])
                    delete children[i];
        }

        int red = 0, green = 0, blue = 0;
        bool isLeaf = false;
        int pixelCount = 0;
        OctreeNode *children[8] = {};
    };

    struct ColorCount
    {
        int count = 0;
        char color[7] = {}; // 16进制字符串
        char red = 0, green = 0, blue = 0;
        int colorValue = 0; // 对应int值
    };

    void buildTree(QImage image);
    void colorStats(OctreeNode *node, QList<ColorCount *> *colors);
    OctreeNode *getRoot();

private:
    void addColor(OctreeNode *node, RGB* color, int level);
    void reduceTree();

private:
    OctreeNode *root;
    QList<OctreeNode *> reducible[7];
    int leafCount = 0;
    int maxCount = 20;

};

#endif // COLOROCTREE_H
