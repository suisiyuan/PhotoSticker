#pragma once

#include <QWidget>
#include <QDir>
#include <QFileDialog>
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QMessageBox>
#include <QSettings>

#include <QAxObject>
#include <QJsonObject>
#include <QJsonDocument>
#include "qrencode.h"

#include "app.h"

#include "zint.h"

#define QRCode_Width    300

typedef struct
{
    qint32	order;
    QString IMEI, SN;
    QImage qrCode, barCode;
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
    void on_excelButton_clicked();
//    void on_smallImageButton_clicked();
    void on_outputButton_clicked();
    void on_generateButton_clicked();

    void on_singleGenerateButton_clicked();
    void on_saveButton_clicked();

    void on_switchButton_clicked();

private:
    Ui::Widget *ui;

    QAxObject *pExcel, *pWorkbooks, *pWorkbook, *pWorksheet;
    QSettings settings;

    void processSmallImage(const QImage &orgImage, DataItem dataItem, QDir outputDir);

};

