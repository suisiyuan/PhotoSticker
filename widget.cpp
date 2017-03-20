#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_dataButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "get data file", QDir::homePath(), "text file (*.txt)");
    ui->dataEdit->setText(filePath);
}


void Widget::on_codeButton_clicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, "get qr code folder", QDir::homePath());
    ui->codeEdit->setText(folderPath);
}


void Widget::on_smallImageButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "get image", QDir::homePath(), "image file (*.jpg *.png)");
    ui->smallImageEdit->setText(filePath);
}

void Widget::on_bigImageButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "get image", QDir::homePath(), "image file (*.jpg)");
    ui->bigImageEdit->setText(filePath);
}

void Widget::on_outputButton_clicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, "get output folder", QDir::homePath());
    ui->outputEdit->setText(folderPath);
}

void Widget::on_generateButton_clicked()
{
    dataItems.clear();
    ui->progressBar->setValue(0);

    if (ui->dataEdit->text().isEmpty())
    {
        return;
    }
    else
    {
        QFile file(ui->dataEdit->text());
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
        QTextStream stream(&file);
        while (!stream.atEnd())
        {
            QString line = stream.readLine();
            QTextStream in(&line);
            DataItem item = { 0, "", "" };
            in >> item.order >> item.IMEI >> item.SN;
            dataItems.append(item);
        }
        file.close();
    }

    ui->progressBar->setMaximum(dataItems.size());

    ui->generateButton->setDisabled(true);

    QDir codeDir(ui->codeEdit->text());
    QFileInfoList dirList = codeDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);

    for (int i = 0; i < dirList.count(); i++)
    {
        QDir dir(dirList[i].absoluteFilePath());
        QStringList stringList;
        stringList << "*_*.jpg";
        QFileInfoList fileList = dir.entryInfoList(stringList, QDir::Files, QDir::Name);
        for (int j = 0; j < fileList.count(); j++)
        {
            QString basename = fileList.at(j).baseName();
            QString a = basename.section('_', 0, 0);
            QString b = basename.section('_', 1, 1);
            quint32 index = (a.toUInt() - 1) * 100 + b.toUInt();

            QString orgName = fileList.at(j).absoluteFilePath();
            QString dstName = codeDir.filePath(QString::number(index).append(".jpg"));
            QFile::copy(orgName, dstName);
        }
    }


    QImage smallImage(ui->smallImageEdit->text());

    QPainter painter(&smallImage);
    QPen pen(QBrush(Qt::black), 2);
    painter.setPen(pen);

    QRect rect(smallImage.rect());
    rect = rect.marginsRemoved(QMargins(1, 1, 1, 1));
    painter.drawRect(rect);

    //QImage bigImage(ui->bigImageEdit->text());

    QDir outputDir(ui->outputEdit->text());
    outputDir.mkdir("1");
    outputDir.mkdir("2");

    QFileInfoList fileList = codeDir.entryInfoList(QDir::Files, QDir::Name);

    for (int i = 0; i < fileList.count(); i++)
    {
        processSmallImage(smallImage, outputDir, fileList.at(i));
        //processBigImage(bigImage, outputDir, fileList.at(i));
        ui->progressBar->setValue(ui->progressBar->value() + 1);
    }


    ui->generateButton->setEnabled(true);
}


void Widget::processSmallImage(const QImage &orgImage, QDir outputDir, QFileInfo fileInfo)
{
    QString name = fileInfo.baseName();
    quint32 index = name.remove(".jpg").toUInt();
    DataItem dataItem = dataItems.at(index-1);

    QImage outputImage(orgImage);
    QImage image(fileInfo.absoluteFilePath());

    outputImage.setDotsPerMeterX(3780);
    outputImage.setDotsPerMeterY(3780);

    QPainter painter(&outputImage);
    painter.drawImage(QRect(5, 8, 160, 160), image);
    painter.setFont(QFont("黑体", 16, QFont::Normal, false));
    painter.drawText(QPoint(236, 30), dataItem.IMEI);
    painter.setFont(QFont("黑体", 12, QFont::Normal, false));
    painter.drawText(QPoint(240, 168), QString("SN:") + dataItem.SN);


    outputImage.setDotsPerMeterX(11816);
    outputImage.setDotsPerMeterY(11816);

    outputImage.save(QDir(outputDir.filePath("1")).filePath(dataItem.SN + "-" + dataItem.IMEI + ".jpg"), "JPG");
}

void Widget::processBigImage(const QImage &orgImage, QDir outputDir, QFileInfo fileInfo)
{
    QString name = fileInfo.baseName();
    quint32 index = name.remove(".jpg").toUInt();
    DataItem dataItem = dataItems.at(index-1);

    QImage outputImage(orgImage);
    QImage image(fileInfo.absoluteFilePath());


    QPainter painter(&outputImage);
    painter.drawImage(QRect(720, 255, 350, 350), image);
    painter.setFont(QFont("黑体", 25, QFont::Normal, false));
    painter.drawText(QPoint(837, 636), dataItem.IMEI);
    painter.setFont(QFont("黑体", 25, QFont::Normal, false));
    painter.drawText(QPoint(770, 677), dataItem.SN);

    outputImage.setDotsPerMeterX(11816);
    outputImage.setDotsPerMeterY(11816);

    outputImage.save(QDir(outputDir.filePath("2")).filePath(dataItem.SN + "-" + dataItem.IMEI + ".jpg"), "JPG");
}

