#include "slide_area.h"
#include <QMouseEvent>
#include <QPainter>

#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>

#endif
#endif

SlideArea::SlideArea(QWidget *parent)
    : QWidget(parent)
{
    _modified = false;
    _painting = false;
    _penWidth = 1;
    _penColor = Qt::black;

    setAttribute(Qt::WA_StaticContents);
}

bool SlideArea::isModified() const
{
    return _modified;
}

int SlideArea::penWidth() const
{
    return _penWidth;
}

QColor  SlideArea::penColor() const
{
    return _penColor;
}

void       SlideArea::setPenColor(const QColor& newColor)
{
    _penColor = newColor;
}

void    SlideArea::setPenWidth(int newWidth)
{
    _penWidth = newWidth;
}

void    SlideArea::clearImage()
{
    _image.fill(qRgb(255, 255, 255));
    _modified = true;
    update();
}

bool    SlideArea::openImage(const QString& fileName)
{
    QImage  loadedImage;
    if (!loadedImage.load(fileName))
    {
        return false;
    }

    QSize   newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    _image = loadedImage;
    _modified = false;
    update();
    return true;
}

bool    SlideArea::saveImage(const QString& fileName, const char* fileFormat)
{
    QImage  visibleImage = _image;
    resizeImage(&visibleImage, size());

    if (visibleImage.save(fileName, fileFormat))
    {
        _modified = false;
        return true;
    }
    return false;
}

void    SlideArea::drawLineTo(const QPoint& endPoint)
{
    QPainter    painter(&_image);
    painter.setPen(QPen(_penColor, _penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(_lastPoint, endPoint);
    _modified = true;
    int rad = (_penWidth / 2) + 2;
    update(QRect(_lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
    _lastPoint = endPoint;
}

void    SlideArea::resizeImage(QImage* image, const QSize& newSize)
{
    if (image->size() == newSize)
    {
        return ;
    }
    QImage  newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter    painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

void    SlideArea::print()
{
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printdialog)

    QPrinter    printer(QPrinter::HighResolution);

    QPrintDialog    printDialog(&printer, this);
    if (printDialog.exec() == QDialog::ACCepted)
    {
        QPainter    painter(&printer);
        QRect   rect = painter.viewport();
        QSize   size = _image.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(_image.rect());
        painter.drawImage(0, 0, _image);
    }
#endif  // QT_CONFIG(printdialog)
}

//EVENTS Overrided Implementations


void    SlideArea::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        _lastPoint = event->pos();
        _painting = true;
    }
}

void    SlideArea::mouseMoveEvent(QMouseEvent* event)
{
    if ((event->buttons() & Qt::LeftButton) && _painting)
    {
        drawLineTo(event->pos());
    }
}

void    SlideArea::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && _painting)
    {
        drawLineTo(event->pos());
        _painting = false;
    }
}

void    SlideArea::paintEvent(QPaintEvent* event)
{
    QPainter    painter(this);
    QRect       dirtyRect = event->rect();
    painter.drawImage(dirtyRect, _image, dirtyRect);
}

void    SlideArea::resizeEvent(QResizeEvent* event)
{
    if (width() > _image.width() || height() > _image.height())
    {
        int newWidth = qMax(width() + 128, _image.width());
        int newHeight = qMax(height() + 128, _image.height());
        resizeImage(&_image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}
