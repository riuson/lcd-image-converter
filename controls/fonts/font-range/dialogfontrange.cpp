#include "dialogfontrange.h"
#include "ui_dialogfontrange.h"
//-----------------------------------------------------------------------------
#include "fontoptions.h"
#include <QTextCodec>
//-----------------------------------------------------------------------------
DialogFontRange::DialogFontRange(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogFontRange)
{
    ui->setupUi(this);

    this->connect(this->ui->comboBoxEncoding, SIGNAL(currentIndexChanged(QString)), SLOT(updatePreview()));
    this->connect(this->ui->spinBoxFrom, SIGNAL(valueChanged(int)), SLOT(updatePreview()));
    this->connect(this->ui->spinBoxTo, SIGNAL(valueChanged(int)), SLOT(updatePreview()));
    this->connect(this->ui->radioButtonBigEndian, SIGNAL(toggled(bool)), SLOT(updatePreview()));
    this->connect(this->ui->radioButtonLittleEndian, SIGNAL(toggled(bool)), SLOT(updatePreview()));

    this->mResultString = QString();

    this->ui->comboBoxEncoding->addItems(FontOptions::encodings());
    int index = this->ui->comboBoxEncoding->findText("system", Qt::MatchFixedString);
    if (index >= 0)
        this->ui->comboBoxEncoding->setCurrentIndex(index);
}
//-----------------------------------------------------------------------------
DialogFontRange::~DialogFontRange()
{
    delete ui;
}
//-----------------------------------------------------------------------------
const QString &DialogFontRange::resultString() const
{
    return this->mResultString;
}
//-----------------------------------------------------------------------------
void DialogFontRange::updatePreview()
{
    QString encoding = this->ui->comboBoxEncoding->currentText();
    int from = this->ui->spinBoxFrom->value();
    int to = this->ui->spinBoxTo->value();
    bool bigEndian = this->ui->radioButtonBigEndian->isChecked();
    this->updatePreview(encoding, from, to, bigEndian);
}
//-----------------------------------------------------------------------------
void DialogFontRange::updatePreview(const QString &encoding, int from, int to, bool bigEndian)
{
    QTextCodec *codec = QTextCodec::codecForName(encoding.toLatin1());
    QString result;

    if (from > to)
        qSwap(from, to);

    for (int i = from; i <= to; ++i)
    {
        int code = i;
        if (code > 0)
        {
            QByteArray array;

            while (code != 0)
            {
                if (bigEndian)
                    array.insert(0, (char)(code & 0xff));
                else
                    array.append((char)(code & 0xff));

                code = code >> 8;
            }

            QString str = codec->toUnicode(array);
            result += str;
        }
        else
        {
            result += QChar(QChar::Null);
        }
    }

    this->ui->plainTextEditPreview->setPlainText(result);
}
//-----------------------------------------------------------------------------
void DialogFontRange::on_plainTextEditPreview_textChanged()
{
    this->mResultString = this->ui->plainTextEditPreview->toPlainText();
}
//-----------------------------------------------------------------------------
