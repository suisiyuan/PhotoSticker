#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    // Excel COM
    pExcel = new QAxObject("Excel.Application");
    if (!pExcel)
        qDebug() << "This Computer doesn't installed Excel, please install one.";
    else
        qDebug() << "Excel initializes successfully, version" + pExcel->property("Version").toString() + ".";
    pWorkbooks = pExcel->querySubObject("WorkBooks");
}

Widget::~Widget()
{
    delete ui;

    pExcel->dynamicCall("Quit()");
    delete pExcel;
}


void Widget::on_excelButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Get excel file"), QDir::homePath(), tr("Excel File (*.xlsx *.xls)"));
    ui->excelEdit->setText(filePath);
}


void Widget::on_smallImageButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Get image"), QDir::homePath(), tr("Image file (*.jpg *.png)"));
    ui->smallImageEdit->setText(filePath);
}


void Widget::on_outputButton_clicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, tr("Get output folder"), QDir::homePath());
    ui->outputEdit->setText(folderPath);
}



void Widget::on_generateButton_clicked()
{
    ui->progressBar->setValue(0);

    // excel
    if (ui->excelEdit->text().isEmpty())
        return;
    else if (!QFile(ui->excelEdit->text()).exists())
        return;


    // small image
    if (ui->smallImageEdit->text().isEmpty())
        return;
    else if (!QFile(ui->smallImageEdit->text()).exists())
        return;
    QImage smallImage(ui->smallImageEdit->text());
    QPainter painter(&smallImage);
    QPen pen(QBrush(Qt::black), 2);
    painter.setPen(pen);
    QRect rect(smallImage.rect());
    rect = rect.marginsRemoved(QMargins(1, 1, 1, 1));
    painter.drawRect(rect);


    // output directory
    QDir dir(ui->outputEdit->text());
    if (ui->outputEdit->text().isEmpty())
        return;
    else if (!dir.exists())
        dir.mkpath(dir.path());



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

        QJsonObject json;
        json.insert("IMEI", dataItem.IMEI);

        QRcode *qrCode;
        QString encodeStr = QJsonDocument(json).toJson();
        qrCode = QRcode_encodeString(encodeStr.toStdString().c_str(), 0, QR_ECLEVEL_Q, QR_MODE_8, 1);
        qreal ratio = QRCode_Width / qrCode->width;

        dataItem.qrCode = QImage(QRCode_Width, QRCode_Width, QImage::Format_RGB888);
        QPainter painter(&dataItem.qrCode);
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

        processSmallImage(smallImage, dataItem, dir);
        ui->progressBar->setValue(i-1);
    }


    QMessageBox messageBox(QMessageBox::Information, tr("Success"), tr("Genetate successfully!"), QMessageBox::Ok, this);
    messageBox.setButtonText(QMessageBox::Ok, tr("Ok"));
    messageBox.exec();

    ui->generateButton->setEnabled(true);


//    if (ui->dataEdit->text().isEmpty())
//    {
//        return;
//    }
//    else
//    {
//        QFile file(ui->dataEdit->text());
//        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
//            return;
//        QTextStream stream(&file);
//        while (!stream.atEnd())
//        {
//            QString line = stream.readLine();
//            QTextStream in(&line);
//            DataItem item = { 0, "", "" };
//            in >> item.order >> item.IMEI >> item.SN;
//            dataItems.append(item);
//        }
//        file.close();
//    }



//    ui->generateButton->setDisabled(true);



//    QImage smallImage(ui->smallImageEdit->text());

//    QPainter painter(&smallImage);
//    QPen pen(QBrush(Qt::black), 2);
//    painter.setPen(pen);

//    QRect rect(smallImage.rect());
//    rect = rect.marginsRemoved(QMargins(1, 1, 1, 1));
//    painter.drawRect(rect);

//    //QImage bigImage(ui->bigImageEdit->text());

//    QDir outputDir(ui->outputEdit->text());
//    outputDir.mkdir("1");
//    outputDir.mkdir("2");

//    QFileInfoList fileList = codeDir.entryInfoList(QDir::Files, QDir::Name);

//    for (int i = 0; i < fileList.count(); i++)
//    {
//        processSmallImage(smallImage, outputDir, fileList.at(i));
//        //processBigImage(bigImage, outputDir, fileList.at(i));
//        ui->progressBar->setValue(ui->progressBar->value() + 1);
//    }


//    ui->generateButton->setEnabled(true);
}


void Widget::processSmallImage(const QImage &orgImage, DataItem dataItem, QDir outputDir)
{
    QImage outputImage(orgImage);

    outputImage.setDotsPerMeterX(3780);
    outputImage.setDotsPerMeterY(3780);

    QPainter painter(&outputImage);
    painter.drawImage(QRect(9, 11, 160, 160), dataItem.qrCode);
    painter.setFont(QFont("黑体", 16, QFont::Normal, false));
    painter.drawText(QPoint(236, 30), dataItem.IMEI);
    painter.setFont(QFont("黑体", 12, QFont::Normal, false));
    painter.drawText(QPoint(240, 168), QString("SN:") + dataItem.SN);


    outputImage.setDotsPerMeterX(11816);
    outputImage.setDotsPerMeterY(11816);

    outputImage.save(outputDir.filePath(dataItem.SN + "-" + dataItem.IMEI + ".jpg"), "JPG");
}

//void Widget::processBigImage(const QImage &orgImage, QDir outputDir, QFileInfo fileInfo)
//{
//    QString name = fileInfo.baseName();
//    quint32 index = name.remove(".jpg").toUInt();
//    DataItem dataItem = dataItems.at(index-1);

//    QImage outputImage(orgImage);
//    QImage image(fileInfo.absoluteFilePath());


//    QPainter painter(&outputImage);
//    painter.drawImage(QRect(720, 255, 350, 350), image);
//    painter.setFont(QFont("黑体", 25, QFont::Normal, false));
//    painter.drawText(QPoint(837, 636), dataItem.IMEI);
//    painter.setFont(QFont("黑体", 25, QFont::Normal, false));
//    painter.drawText(QPoint(770, 677), dataItem.SN);

//    outputImage.setDotsPerMeterX(11816);
//    outputImage.setDotsPerMeterY(11816);

//    outputImage.save(QDir(outputDir.filePath("2")).filePath(dataItem.SN + "-" + dataItem.IMEI + ".jpg"), "JPG");
//}



