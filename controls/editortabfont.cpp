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
void EditorTabFont::assignFontCharacters(const QString &chars,
                          const QString &fontFamily,
                          const QString &style,
                          const int size)
{
    QFontDatabase fonts;
    this->mFont = fonts.font(fontFamily, style, size);
    this->ui->listWidgetCharacters->setFont(this->mFont);

    for (int i = 0; i < chars.count(); i++)
    {
        this->ui->listWidgetCharacters->addItem(QString(chars.at(i)));
        this->mContainer->setImage(QString(chars.at(i)), new QImage(":/images/template"));
    }
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
