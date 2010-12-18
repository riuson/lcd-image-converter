#include "editortabimage.h"
#include "ui_editortabimage.h"
//-----------------------------------------------------------------------------
#include <QHBoxLayout>
#include <QDomDocument>
#include <QDomProcessingInstruction>
#include <QBuffer>
#include <QFile>
#include <QTextStream>

#include "widgetbitmapeditor.h"
#include "bitmapcontainer.h"
//-----------------------------------------------------------------------------
EditorTabImage::EditorTabImage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditorTabImage)
{
    ui->setupUi(this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    this->setLayout(layout);

    this->mEditor = new WidgetBitmapEditor(this);
    layout->addWidget(this->mEditor);

    this->mContainer = new BitmapContainer(this);
    this->mEditor->assignImage(this->mContainer->image());

    this->connect(this->mEditor, SIGNAL(dataChanged()), SLOT(onDataChanged()));

    this->mDocumentName = tr("Image", "new image name");
    this->mFileName = "";
    this->mDataChanged = false;
}
//-----------------------------------------------------------------------------
EditorTabImage::~EditorTabImage()
{
    delete ui;
}
//-----------------------------------------------------------------------------
void EditorTabImage::changeEvent(QEvent *e)
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
void EditorTabImage::onDataChanged()
{
    this->mDataChanged = true;
}
//-----------------------------------------------------------------------------
void EditorTabImage::load(const QString &fileName)
{
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
                      this->mContainer->image()->load(&buffer, "PNG");
                      this->mEditor->assignImage(this->mContainer->image());
                    }
                  }

                  n = n.nextSibling();
                }
            }
        }
        file.close();

        this->mFileName = fileName;
    }
}
//-----------------------------------------------------------------------------
void EditorTabImage::save(const QString &fileName)
{
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
    this->mContainer->image()->save(&buffer, "PNG");
    QString data = ba.toBase64();

    QDomText nodeData = doc.createTextNode(data);
    nodePicture.appendChild(nodeData);

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        doc.save(stream, 4);
        file.close();
    }
}
//-----------------------------------------------------------------------------
bool EditorTabImage::changed()
{
    return this->mDataChanged;
}
//-----------------------------------------------------------------------------
QString EditorTabImage::fileName()
{
    return this->mFileName;
}
//-----------------------------------------------------------------------------
QString EditorTabImage::documentName()
{
    return this->mDocumentName;
}
//-----------------------------------------------------------------------------
void EditorTabImage::setDocumentName(const QString &value)
{
    this->mDocumentName = value;
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
