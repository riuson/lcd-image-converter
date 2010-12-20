#include "widgetconvoptionsgray.h"
#include "ui_widgetconvoptionsgray.h"
//-----------------------------------------------------------------------------
#include <QButtonGroup>

#include "bytelistitemdelegate.h"
//-----------------------------------------------------------------------------
WidgetConvOptionsGray::WidgetConvOptionsGray(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WidgetConvOptionsGray)
{
    ui->setupUi(this);
    this->mGroupByteOrder = new QButtonGroup(this);
    this->mGroupDataSize = new QButtonGroup(this);

    this->mGroupByteOrder->addButton(this->ui->radioButtonBigEndian);
    this->mGroupByteOrder->addButton(this->ui->radioButtonLittleEndian);

    this->mGroupDataSize->addButton(this->ui->radioButtonData8);
    this->mGroupDataSize->addButton(this->ui->radioButtonData16);
    this->mGroupDataSize->addButton(this->ui->radioButtonData32);

    this->mDelegate = new ByteListItemDelegate(this);
    this->ui->listWidget->setItemDelegate(this->mDelegate);

    this->ui->radioButtonBigEndian->setChecked(true);
    this->ui->radioButtonLittleEndian->setChecked(false);

    this->ui->radioButtonData8->setChecked(true);
}
//-----------------------------------------------------------------------------
WidgetConvOptionsGray::~WidgetConvOptionsGray()
{
    delete ui;
}
//-----------------------------------------------------------------------------
void WidgetConvOptionsGray::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
//-----------------------------------------------------------------------------
void WidgetConvOptionsGray::updatePreview()
{
    this->ui->listWidget->clear();
    QStringList list;

    int bits = 8;
    if (this->ui->radioButtonData16->isChecked())
        bits = 16;
    if (this->ui->radioButtonData32->isChecked())
        bits = 32;
    this->mDelegate->setBitsCount(bits);

    // 3 - 2 bits
    // 7 - 3 bits
    // 15 - 4 bits
    // 31 - 5 bits
    // 63 - 6 bits
    // 127 - 7 bits
    // 255 - 8 bits

    int bitsPerPoint = this->ui->spinBoxBitsPerPoint->value();

    bool littleEndian = this->ui->radioButtonLittleEndian->isChecked();
    bool mirror = this->ui->checkBoxMirrorBits->isChecked();
    bool pack = this->ui->checkBoxPack->isChecked();

    for (int i = 0, j = 0, k = 0; i < 80; i++)
    {
        int a = k % bitsPerPoint;
        int b = bitsPerPoint - a - 1;
        k++;
        list.append(QString("b%1.%2").arg(j).arg(b));
        //QString str = QString("b%1").arg(b);
        //QListWidgetItem *item = new QListWidgetItem(str, this->ui->listWidget, b);
        //this->ui->listWidget->addItem(item);
        if (b == 0)
        {
            j++;
            if (!pack)
            {
                if ((i % 8) + bitsPerPoint > 7)
                {
                    //for (int k = i % 8; k < 7; k++)
                    while (i % 8 != 7)
                    {
                        list.append("0");
                        i++;
                    }
                }
            }
        }
    }

    if (littleEndian)
    {
        QStringList outlist;
        for (int i = 0; i < list.count(); i+= bits)
        {
            for (int j = (bits / 8); j > 0; j--)//1, 2 or 4
            {
                int start = i + (j - 1) * 8;
                if (start >= 0 && (start + 8 - 1) <= list.count() )
                outlist.append(list.mid(start, 8));
            }
        }
        list = outlist;
    }

    if (mirror)
    {
        QStringList outlist;
        for (int i = 0; i < list.count(); i+= 8)
        {
            QStringList tmp = list.mid(i, 8);
            for (int j = tmp.count() - 1; j >= 0; j--)
            {
                outlist.append(tmp.at(j));
            }
        }
        list = outlist;
    }
    this->ui->listWidget->addItems(list);
}
//-----------------------------------------------------------------------------
