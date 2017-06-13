#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    pExcel(Q_NULLPTR)
{
    ui->setupUi(this);

    ui->versionLabel->setText(tr("Version: ") + QCoreApplication::applicationVersion());

    // settings
    if (!settings.contains("lastExcelPath"))
        settings.setValue("lastExcelPath", QDir::currentPath());
    if (!settings.contains("lastSaveFilePath"))
        settings.setValue("lastSaveFilePath", QDir::currentPath());
    if (!settings.contains("lastSaveFolderPath"))
        settings.setValue("lastSaveFolderPath", QDir::currentPath());
}

Widget::~Widget()
{
    delete ui;

    if(pExcel)
    {
        pExcel->dynamicCall("Quit()");
        delete pExcel;
    }

}

// 生成二维码
QImage encodeQRCode(QString imei)
{
    QJsonObject json;
    json.insert("IMEI", imei);

    QRcode *qrCode;
    QString encodeStr = QJsonDocument(json).toJson();
    qrCode = QRcode_encodeString(encodeStr.toStdString().c_str(), 0, QR_ECLEVEL_Q, QR_MODE_8, 1);
    qreal ratio = QRCode_Width / qrCode->width;

    QImage image = QImage(QRCode_Width, QRCode_Width, QImage::Format_RGB888);
    QPainter painter(&image);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::white));
    painter.drawRect(0, 0, QRCode_Width, QRCode_Width);
    painter.setBrush(QBrush(Qt::black));

    for (qint32 i = 0; i < qrCode->width; i++)
        for (qint32 j = 0; j < qrCode->width; j++)
            if (qrCode->data[i*qrCode->width+j] & 0x01)
            {
                QRectF rect(i*ratio, j*ratio, ratio, ratio);
                painter.drawRect(rect);
            }
    free(qrCode->data);
    painter.end();

    return image;
}

// 生成条形码
QImage encodeBarCode(QString data)
{
    struct zint_symbol *my_symbol = ZBarcode_Create();
    my_symbol->symbology = BARCODE_UPCA;
    ZBarcode_Encode_and_Buffer(my_symbol, (unsigned char*)data.toLocal8Bit().toStdString().c_str(), 11, 0);

    QImage image(my_symbol->bitmap_width, my_symbol->bitmap_height, QImage::Format_RGB32);
    int row, col, i = 0;
    uint red, blue, green;
    for (row = 0; row < my_symbol->bitmap_height; row++) {
        for (col = 0; col < my_symbol->bitmap_width; col++) {
            red = my_symbol->bitmap[i];
            green = my_symbol->bitmap[i + 1];
            blue = my_symbol->bitmap[i + 2];
            QRgb value = qRgb(red, green, blue);
            image.setPixel(col, row, value);
            i += 3;
        }
    }
    ZBarcode_Delete(my_symbol);

    return image.copy(image.rect().marginsRemoved(QMargins(15, 0, 15, 68)));
}


// 生成带设备号的图片
QImage generateImage1(DataItem dataItem)
{
    QImage outputImage(":/res/images/image.jpg");

    QPainter painter(&outputImage);
    QPen pen(QBrush(Qt::black), 2);
    painter.setPen(pen);
    QRect rect(outputImage.rect());
    rect = rect.marginsRemoved(QMargins(1, 1, 1, 1));
    painter.drawRect(rect);

    outputImage.setDotsPerMeterX(3780);
    outputImage.setDotsPerMeterY(3780);

    painter.drawImage(QRect(9, 11, 160, 160), dataItem.qrCode);
    painter.setFont(QFont("黑体", 16, QFont::Normal, false));
    painter.drawText(QPoint(236, 30), dataItem.IMEI);
    painter.setFont(QFont("黑体", 12, QFont::Normal, false));
    painter.drawText(QPoint(240, 168), QString("SN:") + dataItem.SN);

    outputImage.setDotsPerMeterX(11816);
    outputImage.setDotsPerMeterY(11816);

    return outputImage;
}


// 生成带条形码的图片
QImage generateImage2(DataItem dataItem)
{
    QImage outputImage(":/res/images/image.jpg");

    QPainter painter1(&outputImage);
    QPen pen(QBrush(Qt::black), 2);
    painter1.setPen(pen);
    QRect rect(outputImage.rect());
    rect = rect.marginsRemoved(QMargins(1, 1, 1, 1));
    painter1.drawRect(rect);

    outputImage.setDotsPerMeterX(3780);
    outputImage.setDotsPerMeterY(3780);

    painter1.drawImage(QRect(9, 11, 160, 160), dataItem.qrCode);
    painter1.setFont(QFont("黑体", 12, QFont::Normal, false));
    painter1.drawText(QPoint(240, 168), QString("SN:") + dataItem.SN);


    painter1.fillRect(QRect(175, 10, 75, 25), Qt::white);
    painter1.drawImage(QRect(180, 8, 260, 30), dataItem.barCode);


    outputImage.setDotsPerMeterX(11816);
    outputImage.setDotsPerMeterY(11816);

    return outputImage;
}





void Widget::on_excelButton_clicked()
{
    QString path = settings.value("lastExcelPath").toString();

    QString filePath = QFileDialog::getOpenFileName(this, tr("Get excel file"), path, tr("Excel File (*.xlsx *.xls)"));
    ui->excelEdit->setText(filePath);
}


//void Widget::on_smallImageButton_clicked()
//{
//    QString filePath = QFileDialog::getOpenFileName(this, tr("Get image"), QDir::homePath(), tr("Image file (*.jpg *.png)"));
//    ui->smallImageEdit->setText(filePath);
//}


void Widget::on_outputButton_clicked()
{
    QString path = settings.value("lastSaveFolderPath").toString();

    QString folderPath = QFileDialog::getExistingDirectory(this, tr("Get output folder"), path);
    ui->outputEdit->setText(folderPath);
}



void Widget::on_generateButton_clicked()
{   
    // Excel COM
    pExcel = new QAxObject("Excel.Application");
    if (!pExcel)
        qDebug() << "This Computer doesn't installed Excel, please install one.";
    else
        qDebug() << "Excel initializes successfully, version" + pExcel->property("Version").toString() + ".";
    pWorkbooks = pExcel->querySubObject("WorkBooks");


    ui->progressBar->setValue(0);

    // excel
    if (ui->excelEdit->text().isEmpty())
        return;
    else if (!QFile(ui->excelEdit->text()).exists())
        return;

    // settings
    settings.setValue("lastExcelPath", QFileInfo(ui->excelEdit->text()).dir().path());
    settings.setValue("lastSaveFolderPath", ui->outputEdit->text());

    // small image
//    if (ui->smallImageEdit->text().isEmpty())
//        return;
//    else if (!QFile(ui->smallImageEdit->text()).exists())
//        return;
    QImage smallImage(":/res/images/image.jpg");
    QPainter painter(&smallImage);
    QPen pen(QBrush(Qt::black), 2);
    painter.setPen(pen);
    QRect rect(smallImage.rect());
    rect = rect.marginsRemoved(QMargins(1, 1, 1, 1));
    painter.drawRect(rect);


    // 输出文件夹
    QDir dir(ui->outputEdit->text());
    if (ui->outputEdit->text().isEmpty())
        return;
    else if (!dir.exists())
        dir.mkpath(dir.path());

    // 分为有设备号和有条形码的文件夹
    dir.mkdir(dir.filePath(tr("Device number")));
    dir.mkdir(dir.filePath(tr("Barcode")));

    if (!pExcel)
        return;
    pWorkbook = pWorkbooks->querySubObject("Open(QString, QVariant)", ui->excelEdit->text(), QVariant(0));
    pWorksheet = pWorkbook->querySubObject("WorkSheets(int)", 1);

    QAxObject *userRange = pWorksheet->querySubObject("UsedRange");
    QVariant var = userRange->dynamicCall("Value");

    pWorkbook->dynamicCall("Close(bool)", true);

    QVariantList varRows = var.toList();
    if (varRows.isEmpty())
        return;

    ui->progressBar->setMaximum(varRows.count() - 2);
    ui->generateButton->setEnabled(false);

    for (qint32 i = 2; i < varRows.count(); i++)
    {
        DataItem dataItem;
        QStringList dataList = varRows.at(i).toStringList();
        dataItem.order = dataList.at(0).toInt();
        dataItem.IMEI = dataList.at(1);
        dataItem.SN = dataList.at(2);
        dataItem.qrCode = encodeQRCode(dataItem.IMEI);
        dataItem.barCode = encodeBarCode(dataItem.SN);

        generateImage1(dataItem).save(QDir(dir.filePath(tr("Device number"))).filePath(dataItem.SN + "-" + dataItem.IMEI + ".jpg"), "JPG");
        generateImage2(dataItem).save(QDir(dir.filePath(tr("Barcode"))).filePath(dataItem.SN + "-" + dataItem.IMEI + ".jpg"), "JPG");

        ui->progressBar->setValue(i-1);
    }


    QMessageBox messageBox(QMessageBox::Information, tr("Success"), tr("Genetate successfully!"), QMessageBox::Ok, this);
    messageBox.setButtonText(QMessageBox::Ok, tr("Ok"));
    messageBox.exec();

    ui->generateButton->setEnabled(true);
}



void Widget::on_singleGenerateButton_clicked()
{
    if (ui->IMEIEdit->text().isEmpty() || ui->SNEdit->text().isEmpty())
        return;

    DataItem dataItem;
    dataItem.order = 0;
    dataItem.IMEI = ui->IMEIEdit->text();
    dataItem.SN = ui->SNEdit->text();
    dataItem.qrCode = encodeQRCode(dataItem.IMEI);
    dataItem.barCode = encodeBarCode(dataItem.SN);

    ui->image1Label->setPixmap(QPixmap::fromImage(generateImage1(dataItem)));
    ui->image2Label->setPixmap(QPixmap::fromImage(generateImage2(dataItem)));
    ui->saveButton->setEnabled(true);
}

void Widget::on_saveButton_clicked()
{
    QString path = settings.value("lastSaveFilePath").toString();

    QImage image = ui->image1Label->pixmap()->toImage();
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save image"),
                                                    QDir(path).filePath(ui->SNEdit->text()+"-"+ui->IMEIEdit->text()),
                                                    tr("Images files (*.jpg)"));
    if (filePath.isEmpty())
        return;

    settings.setValue("lastSaveFilePath", QFileInfo(filePath).dir().path());


    if (!image.save(filePath, "JPG"))
        return;

    ui->IMEIEdit->clear();
    ui->SNEdit->clear();
    ui->image1Label->clear();
    ui->saveButton->setEnabled(false);
}

void Widget::on_switchButton_clicked()
{
    if (ui->stackedWidget->currentIndex() == 0)
        ui->stackedWidget->setCurrentIndex(1);
    else
        ui->stackedWidget->setCurrentIndex(0);
}
