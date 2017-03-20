#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDir>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug>
#include <QImage>
#include <QPainter>


typedef struct
{
    quint16	order;
    QString IMEI, SN;
}DataItem;


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();


private slots:
    void on_dataButton_clicked();

    void on_codeButton_clicked();

    void on_smallImageButton_clicked();

    void on_bigImageButton_clicked();

    void on_outputButton_clicked();

    void on_generateButton_clicked();

private:
    Ui::Widget *ui;

    QVector<DataItem> dataItems;

    void processSmallImage(const QImage &orgImage, QDir outputDir, QFileInfo fileInfo);
    void processBigImage(const QImage &orgImage, QDir outputDir, QFileInfo fileInfo);

};

#endif // WIDGET_H
