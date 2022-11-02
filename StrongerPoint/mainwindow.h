#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>

class SlideArea;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void    closeEvent(QCloseEvent* event) override;

private slots:
    void    open();
    void    save();
    void    winPenColor();
    void    winPenWidth();
    void    about();

private:
    void    createActions();
    void    createMenus();
    bool    maybeSave();
    bool    saveFile(const QByteArray& fileFormat);

    SlideArea*  _slideArea;

    QMenu*      _saveAsMenu;
    QMenu*      _fileMenu;
    QMenu*      _optionMenu;
    QMenu*      _helpMenu;

    QAction*            _openAct;
    QList<QAction *>    _saveAsAct;
    QAction*            _exitAct;
    QAction*            _penColorAct;
    QAction*            _penWidthAct;
    QAction*            _printAct;
    QAction*            _clearScreenAct;
    QAction*            _aboutAct;
};

#endif // MAINWINDOW_H
