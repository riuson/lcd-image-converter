#include "dialogabout.h"
#include "ui_dialogabout.h"

#include <QTextStream>
#include <QFile>
//-----------------------------------------------------------------------------
DialogAbout::DialogAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAbout)
{
    ui->setupUi(this);

    QFile file(":/text/gpl3");
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        QString license = stream.readAll();
        file.close();

        this->ui->textEdit->setText(license);
    }
}
//-----------------------------------------------------------------------------
DialogAbout::~DialogAbout()
{
    delete ui;
}
//-----------------------------------------------------------------------------
