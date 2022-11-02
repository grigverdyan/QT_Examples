#include "mainwindow.h"
#include "slide_area.h"

#include <QApplication>
#include <QColorDialog>
#include <QFileDialog>
#include <QImageWriter>
#include <QInputDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _slideArea(new SlideArea(this))
{
    setCentralWidget(_slideArea);
    createActions();
    createMenus();
    setWindowTitle("StrongerPoint");
    resize(1000, 700);
}

MainWindow::~MainWindow() {}

void    MainWindow::open()
{
    if (maybeSave())
    {
        QString fileName = QFileDialog::getOpenFileName(
                    this, tr("Open File"), QDir::currentPath());
        if (!fileName.isEmpty())
        {
            _slideArea->openImage(fileName);
        }
    }
}

void    MainWindow::save()
{
    QAction*    action = qobject_cast<QAction *>(sender());
    QByteArray  fileFormat = action->data().toByteArray();
    saveFile(fileFormat);
}

void MainWindow::winPenColor()
{
    QColor newColor = QColorDialog::getColor(_slideArea->penColor());
    if (newColor.isValid())
    {
        _slideArea->setPenColor(newColor);
    }
}

void MainWindow::winPenWidth()
{
    bool    ok;
    int     newWidth = QInputDialog::getInt(this, tr("StrongerPoint"), tr("Select pen width:"), _slideArea->penWidth(), 1, 50, 1, &ok);
    if (ok)
    {
        _slideArea->setPenWidth(newWidth);
    }
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About StrongerPoint"),
            tr("<p>The <b>StrongerPoint</b> is a course project which is a \
               so called reimplementation of Microsoft Powerpoint.</p>"));
}

void MainWindow::createActions()
{
    _openAct = new QAction(tr("Open..."), this);
    _openAct->setShortcuts(QKeySequence::Open);
    connect(_openAct, &QAction::triggered, this, &MainWindow::open);

    const QList<QByteArray> imageFormats = QImageWriter::supportedImageFormats();
    for (const QByteArray& format : imageFormats)
    {
        QString text = tr("%1...").arg(QString::fromLatin1(format).toUpper());
        QAction *action = new QAction(text, this);
        action->setData(format);
        connect(action, &QAction::triggered, this, &MainWindow::save);
        _saveAsAct.append(action);
    }

    _printAct = new QAction(tr("Print..."), this);
    connect(_printAct, &QAction::triggered, _slideArea, &SlideArea::print);

    _exitAct = new QAction(tr("Exit"), this);
    _exitAct->setShortcuts(QKeySequence::Quit);
    connect(_exitAct, &QAction::triggered, this, &MainWindow::close);

    _penColorAct = new QAction(tr("Pen Color..."), this);
    connect(_penColorAct, &QAction::triggered, this, &MainWindow::winPenColor);

    _penWidthAct = new QAction(tr("Pen Width..."), this);
    connect(_penWidthAct, &QAction::triggered, this, &MainWindow::winPenWidth);

    _clearScreenAct = new QAction(tr("Clear Screen"), this);
    _clearScreenAct->setShortcut(tr("Ctrl+L"));
    connect(_clearScreenAct, &QAction::triggered, _slideArea, &SlideArea::clearImage);

    _aboutAct = new QAction(tr("About"), this);
    connect(_aboutAct, &QAction::triggered, this, &MainWindow::about);
}

void MainWindow::createMenus()
{
    _saveAsMenu = new QMenu(tr("Save As"), this);
    for (QAction *action : qAsConst(_saveAsAct))
    {
        _saveAsMenu->addAction(action);
    }

    _fileMenu = new QMenu(tr("File"), this);
    _fileMenu->addAction(_openAct);
    _fileMenu->addMenu(_saveAsMenu);
    _fileMenu->addAction(_printAct);
    _fileMenu->addSeparator();
    _fileMenu->addAction(_exitAct);

    _optionMenu = new QMenu(tr("Options"), this);
    _optionMenu->addAction(_penColorAct);
    _optionMenu->addAction(_penWidthAct);
    _optionMenu->addSeparator();
    _optionMenu->addAction(_clearScreenAct);

    _helpMenu = new QMenu(tr("Help"), this);
    _helpMenu->addAction(_aboutAct);

    menuBar()->addMenu(_fileMenu);
    menuBar()->addMenu(_optionMenu);
    menuBar()->addMenu(_helpMenu);
}

bool MainWindow::maybeSave()
{
    if (_slideArea->isModified())
    {
       QMessageBox::StandardButton ret =
               QMessageBox::warning(this, tr("StrongerPoint"), tr("The image has been modified.\n" "Do you want to save your changes?"),
               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
        {
            return saveFile("png");
        }
        else if (ret == QMessageBox::Cancel)
        {
            return false;
        }
    }
    return true;
}

bool MainWindow::saveFile(const QByteArray& fileFormat)
{
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                               initialPath,
                               tr("%1 Files (*.%2);;All Files (*)")
                               .arg(QString::fromLatin1(fileFormat.toUpper()))
                               .arg(QString::fromLatin1(fileFormat)));
    if (fileName.isEmpty())
    {
        return false;
    }
    return _slideArea->saveImage(fileName, fileFormat.constData());
}

// EVENTS Reimplementations

void    MainWindow::closeEvent(QCloseEvent* event)
{
    if (maybeSave())
    {
        event->accept();
    } else {
        event->ignore();
    }
}
