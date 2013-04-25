#ifndef SAMAELMAINWINDOW_H
#define SAMAELMAINWINDOW_H

class TerminalWidget;
class SamaelItemModel;

class SamaelMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    SamaelMainWindow(QWidget *parent = nullptr);
    ~SamaelMainWindow();

    void initialize(TerminalWidget* terminal);

private:
    void createActions();
    void createMenus();
    void createWidgets();
    void createViews();
    void createLayouts();
    void createStatusBar();

    // QActions
    QAction *m_OpenAction;
    QAction *m_ExitAction;
    QAction *m_AboutAction;

    // QMenus
    QMenu *m_FileMenu;
    QMenu *m_EditMenu;
    QMenu *m_ViewMenu;
    QMenu *m_HelpMenu;

    // QWidgets 
    QDockWidget *m_DockAlpha;
    QWidget *m_DockAlphaContent;
    QDockWidget *m_DockBeta;
    QWidget *m_DockBetaContent;
    QLabel *m_StatusBarLabel;
    TerminalWidget *m_TerminalWidget;

    // QViews & Models
    QTreeView *m_TreeView;
    SamaelItemModel *m_SamaelItemModel;

    // QLayouts
    QVBoxLayout *m_DockAlphaVBoxLayout;
    QVBoxLayout *m_DockBetaVBoxLayout;

public slots:
    void open();
    void exit();
    void about();
};

#endif // SAMAELMAINWINDOW_H
