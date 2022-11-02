#ifndef SLIDEAREA_H
#define SLIDEAREA_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>

class SlideArea : public QWidget
{
    Q_OBJECT

public:
    SlideArea(QWidget *parent = nullptr);

    bool    openImage(const QString& fileName);
    bool    saveImage(const QString& fileName, const char* fileFormat);
    bool    isModified() const;
    int     penWidth() const;
    QColor  penColor() const;
    void    setPenColor(const QColor& newColor);
    void    setPenWidth(int newWidth);

public slots:
    void    clearImage();
    void    print();

protected:
    void    mousePressEvent(QMouseEvent* event) override;
    void    mouseMoveEvent(QMouseEvent* event) override;
    void    mouseReleaseEvent(QMouseEvent* event) override;
    void    paintEvent(QPaintEvent* event) override;
    void    resizeEvent(QResizeEvent* event) override;

private:
    void    drawLineTo(const QPoint& endPoint);
    void    resizeImage(QImage* image, const QSize& newSize);

    bool    _modified;
    bool    _painting;
    int     _penWidth;
    QColor  _penColor;
    QImage  _image;
    QPoint  _lastPoint;
};

#endif // SLIDEAREA_H
