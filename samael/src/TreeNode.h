#ifndef TREENODE_H
#define TREENODE_H

class TreeNode
{
public:
    explicit TreeNode(const QVector<QVariant>& data, TreeNode* parent = nullptr);
    ~TreeNode();

    void appendChild(TreeNode* child);
    TreeNode* getChild(int row);
    QVector<TreeNode*> getChildren();

    int childCount() const;
    int columnCount() const;
    
    int row() const;
    QVariant data(int column) const;
    TreeNode* parent();

private:
    QVector<TreeNode*> m_Children;
    QVector<QVariant> m_Data;
    TreeNode* m_Parent;
};

#endif // TREENODE_H
