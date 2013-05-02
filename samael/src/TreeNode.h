#ifndef TREENODE_H
#define TREENODE_H

class TreeNode
{
public:
    explicit TreeNode(TreeNode* parent = nullptr);
    explicit TreeNode(const QVariant& data, TreeNode* parent = nullptr);
    explicit TreeNode(const QVector<QVariant>& data, TreeNode* parent = nullptr);
    ~TreeNode();

    void insertChild(int row, TreeNode* child);
    void appendChild(TreeNode* child);
    void removeChild(int row);
    TreeNode* getChild(int row);
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const;
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
