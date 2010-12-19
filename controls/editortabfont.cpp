#include "editortabfont.h"
#include "ui_editortabfont.h"
//-----------------------------------------------------------------------------
#include <QSplitter>
#include <QDomDocument>
#include <QDomProcessingInstruction>
#include <QBuffer>
#include <QFile>
#include <QTextStream>
#include <QFontDatabase>
#include <QPainter>
#include <QImage>
#include <QMessageBox>

#include "widgetbitmapeditor.h"
#include "fontcontainer.h"
//-----------------------------------------------------------------------------
EditorTabFont::EditorTabFont(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditorTabFont)
{
    ui->setupUi(this);

    this->mSplitter = new QSplitter(this);
    this->ui->horizontalLayout->addWidget(this->mSplitter);

    this->mContainer = new FontContainer(this);

    this->mEditor = new WidgetBitmapEditor(this->mContainer, this);
    this->mSplitter->addWidget(this->mEditor);
    this->mSplitter->addWidget(this->ui->listWidgetCharacters);
    this->mSplitter->setChildrenCollapsible(false);

    this->connect(this->mEditor, SIGNAL(dataChanged()), SLOT(on_editor_dataChanged()));

    this->mDocumentName = tr("Font", "new font name");
    this->mFileName = "";
    this->mDataChanged = false;

    this->mAntialiasing = false;
    this->mMonospaced = false;
}
//-----------------------------------------------------------------------------
EditorTabFont::~EditorTabFont()
{
    delete ui;
}
//-----------------------------------------------------------------------------
void EditorTabFont::changeEvent(QEvent *e)
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
//-----------------------------------------------------------------------------
void EditorTabFont::on_editor_dataChanged()
{
    this->mDataChanged = true;
    emit this->dataChanged();
}
//-----------------------------------------------------------------------------
void EditorTabFont::on_listWidgetCharacters_currentTextChanged(const QString &value)
{
    this->mEditor->selectImage(value);
}
//-----------------------------------------------------------------------------
bool EditorTabFont::load(const QString &fileName)
{
    bool result = false;
    /*
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly))
    {
        QDomDocument doc;
        QString errorMsg;
        int errorColumn, errorLine;
        if (doc.setContent(&file, &errorMsg, &errorLine, &errorColumn))
        {
            QDomElement root = doc.documentElement();
            if( root.tagName() == "data" )
            {
                this->mDocumentName = root.attribute("name", fileName);
                QDomNode n = root.firstChild();
                while( !n.isNull() )
                {
                  QDomElement e = n.toElement();
                  if( !e.isNull() )
                  {
                    if( e.tagName() == "picture" )
                    {
                      QString str = e.text();
                      QByteArray ba = QByteArray::fromBase64(str.toAscii());
                      QBuffer buffer(&ba);
                      QImage image;
                      image.load(&buffer, "PNG");
                      this->mContainer->setImage(0, &image);
                      result = true;
                      break;
                    }
                  }

                  n = n.nextSibling();
                }
            }
        }
        file.close();

        this->mFileName = fileName;
        this->mDataChanged = false;
        emit this->dataChanged();
    }
    */
    return result;
}
//-----------------------------------------------------------------------------
bool EditorTabFont::save(const QString &fileName)
{
    bool result = false;
    /*
    QDomDocument doc;
    QDomProcessingInstruction procInstruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
    doc.appendChild(procInstruction);

    QDomElement nodeRoot = doc.createElement("data");
    doc.appendChild(nodeRoot);

    nodeRoot.setAttribute("type", "image");
    nodeRoot.setAttribute("name", this->mDocumentName);

    QDomElement nodePicture = doc.createElement("picture");
    nodeRoot.appendChild(nodePicture);

    nodePicture.setAttribute("format", "png");

    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    this->mContainer->image(0)->save(&buffer, "PNG");
    QString data = ba.toBase64();

    QDomText nodeData = doc.createTextNode(data);
    nodePicture.appendChild(nodeData);

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        doc.save(stream, 4);

        this->mFileName = fileName;
        this->mDataChanged = false;
        file.close();
        result = true;
        emit this->dataChanged();
    }
    */
    return result;
}
//-----------------------------------------------------------------------------
bool EditorTabFont::changed()
{
    return this->mDataChanged;
}
//-----------------------------------------------------------------------------
QString EditorTabFont::fileName()
{
    return this->mFileName;
}
//-----------------------------------------------------------------------------
QString EditorTabFont::documentName()
{
    return this->mDocumentName;
}
//-----------------------------------------------------------------------------
void EditorTabFont::setDocumentName(const QString &value)
{
    this->mDocumentName = value;
    emit this->dataChanged();
}
//-----------------------------------------------------------------------------
WidgetBitmapEditor *EditorTabFont::editor()
{
    return this->mEditor;
}
//-----------------------------------------------------------------------------
void EditorTabFont::setFontCharacters(const QString &chars,
                          const QString &fontFamily,
                          const QString &style,
                          const int size,
                          const bool monospaced,
                          const bool antialiasing)
{
    QFontDatabase fonts;

    bool needRecreate = false;
    bool cancel = false;
    if (this->mContainer->count() > 1)
    {
        if (this->mFont.family() != fontFamily ||
            this->mStyle != style ||
            this->mFont.pointSize() != size ||
            this->mMonospaced != monospaced ||
            this->mAntialiasing != antialiasing)
        {
            // Font changed, need recreate all images
            // TODO: may be allow add only new characters with other font? :
            // * recreate all images and add new
            // * only recreate old images with new font
            // * add new characters with new font
            // * add new character with old font
            // * cancel
            needRecreate = true;
        }
    }
    if (needRecreate)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Font parameters was changed"));
        msgBox.setInformativeText(tr("Do you want update all characters?"));
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        int ret = msgBox.exec();
        if (ret == QMessageBox::Cancel)
            cancel = true;
    }

    if (!cancel)
    {
        // list of exists characters
        QList<QString> keys = this->mContainer->keys();
        if (needRecreate)
        {
            this->mContainer->clear();
        }
        else
        {
            // remove characters, which not present in new characters list
            QListIterator<QString> it(keys);
            it.toFront();
            while(it.hasNext())
            {
                QString a = it.next();
                if (!chars.contains(a))
                {
                    this->mContainer->remove(a);
                }
            }
        }
        this->mFont = fonts.font(fontFamily, style, size);
        this->ui->listWidgetCharacters->setFont(this->mFont);

        //this->mCharacters = chars;
        this->mStyle = style;
        this->mMonospaced = monospaced;
        this->mAntialiasing = antialiasing;

        if (this->mAntialiasing)
            this->mFont.setStyleStrategy(QFont::PreferAntialias);
        else
            this->mFont.setStyleStrategy(QFont::NoAntialias);
        int width = 0, height = 0;
        if (this->mMonospaced)
        {
            QFontMetrics metrics(this->mFont);
            for (int i = 0; i < chars.count(); i++)
            {
                width = qMax(width, metrics.width(chars.at(i)));
            }
            height = metrics.height();
        }

        // clear items of listwidget
        this->ui->listWidgetCharacters->clear();
        // list of exists characters, what present in new characters list
        keys = this->mContainer->keys();
        for (int i = 0; i < chars.count(); i++)
        {
            QString key = QString(chars.at(i));

            //this->ui->listWidgetCharacters->addItem(key);
            // if character not exists, create it
            if (!keys.contains(key))
            {
                QImage image = this->drawCharacter(chars.at(i),
                                                   this->mFont,
                                                   this->mEditor->color1(),
                                                   this->mEditor->color2(),
                                                   width,
                                                   height,
                                                   this->mAntialiasing);
                this->mContainer->setImage(key, new QImage(image));
            }
        }
        keys = this->mContainer->keys();
        QStringList list(keys);
        this->ui->listWidgetCharacters->addItems(list);
        this->on_editor_dataChanged();
    }
}
//-----------------------------------------------------------------------------
void EditorTabFont::fontCharacters(QString *chars,
                                   QString *fontFamily,
                                   QString *style,
                                   int *size,
                                   bool *monospaced,
                                   bool *antialiasing)
{
    QStringList charList(this->mContainer->keys());
    //*chars = this->mCharacters;
    *chars = charList.join("");
    *fontFamily = this->mFont.family();
    *size = this->mFont.pointSize();
    *style = this->mStyle;
    *monospaced = this->mMonospaced;
    *antialiasing = this->mAntialiasing;
}
//-----------------------------------------------------------------------------
QImage EditorTabFont::drawCharacter(const QChar value,
                                    const QFont &font,
                                    const QColor &foreground,
                                    const QColor &background,
                                    const int width,
                                    const int height,
                                    const bool antialiasing)
{
    QFontMetrics fontMetrics(font);

    int charWidth = fontMetrics.width(value);
    int charHeight = fontMetrics.height();

    int imageWidth = width;
    int imageHeight = height;

    if (width == 0 || height == 0)
    {
        imageWidth = charWidth;
        imageHeight = charHeight;
    }
    QImage result(imageWidth, imageHeight, QImage::Format_RGB32);

    QPainter painter(&result);
    painter.setFont(font);

    painter.setRenderHint(QPainter::Antialiasing, antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing, antialiasing);

    painter.setPen(foreground);

    painter.fillRect(result.rect(), background);

    painter.drawText((imageWidth / 2) - (charWidth / 2),
                     fontMetrics.ascent(),//+4
                     QString(value));

    return result;
}
//-----------------------------------------------------------------------------
/*
 Storage data format:
<?xml version="1.0" encoding="utf-8"?>
<data type="image" name="documentName">
    <picture format="png">
        Base64 string
    </picture>
</data>

<?xml version="1.0" encoding="utf-8"?>
<data type="image" name="Image">
    <picture format="png">iVBORw0KGgoAAAANSUhEUgAAAAoAAAAKCAIAAAACUFjqAAAAA3NCSVQICAjb4U/gAAAACXBIWXMAAAsTAAALEwEAmpwYAAAARklEQVQYlYWPSQrAMAwDpZAX5x/Nl6eHgLMQt8IniRGWAeWqkux+zaD5my5B2z0uzHq0XegjW1+ZdLhbB4mkB0iHjY6fYS/sJjZR2Wu+lAAAAABJRU5ErkJggg==</picture>
</data>

 */
