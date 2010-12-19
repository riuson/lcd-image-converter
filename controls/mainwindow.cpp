#include "mainwindow.h"
#include "ui_mainwindow.h"
//-----------------------------------------------------------------------------
#include <QList>
#include <QFileDialog>
#include <QInputDialog>
#include <QTextStream>

#include "editortabimage.h"
#include "editortabfont.h"
#include "bitmapcontainer.h"
#include "dialogsavechanges.h"
#include "widgetbitmapeditor.h"
#include "bitmaphelper.h"
#include "dialogresize.h"
#include "dialogcharacters.h"
//-----------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //QImage im(":/images/winter");
    //QImage im2 = im.scaled(im.width() * 4, im.height() * 4, Qt::KeepAspectRatio, Qt::FastTransformation);
    //QPixmap pix = QPixmap::fromImage(im2);
    //this->ui->label->setPixmap(pix);
    this->mEditor = NULL;

    this->updateMenuState();
}
//-----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete ui;
}
//-----------------------------------------------------------------------------
void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
//-----------------------------------------------------------------------------
QString MainWindow::findAvailableName(const QString &prefix)
{
    QString result = prefix;
    QList<QString> names;
    for (int i = 0; i < this->ui->tabWidget->count(); i++)
    {
        QWidget *w = this->ui->tabWidget->widget(i);
        IDocument *doc = dynamic_cast<IDocument *> (w);
        names.append(doc->documentName());
    }
    int i = 1;
    if (names.contains(prefix))
    {
        while (true)
        {
            QString name = QString("%1 %2").arg(prefix).arg(i++);
            if (!names.contains(name))
            {
                result = name;
                break;
            }
        }
    }
    return result;
}
//-----------------------------------------------------------------------------
void MainWindow::updateMenuState()
{
    int index = this->ui->tabWidget->currentIndex();
    QWidget *w = this->ui->tabWidget->widget(index);
    bool editorSelected = false;
    if (EditorTabImage *eti = qobject_cast<EditorTabImage *>(w))
    {
        this->ui->menuFont->setEnabled(false);
        this->mEditor = eti->editor();
        editorSelected = true;
    }
    if (EditorTabFont *etf = qobject_cast<EditorTabFont *>(w))
    {
        this->ui->menuFont->setEnabled(true);
        editorSelected = true;
    }
    else
    {
        this->ui->menuFont->setEnabled(false);
    }
    this->ui->menuImage->setEnabled(editorSelected);

    this->ui->actionSave->setEnabled(editorSelected);
    this->ui->actionSave_As->setEnabled(editorSelected);
    this->ui->actionClose->setEnabled(editorSelected);
    this->ui->actionConvert->setEnabled(editorSelected);

    if (!editorSelected)
        this->mEditor = NULL;
}
//-----------------------------------------------------------------------------
void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    QWidget *w = this->ui->tabWidget->widget(index);
    IDocument *doc = dynamic_cast<IDocument *> (w);
    bool cancel = false;
    if (doc != NULL && doc->changed())
    {
        DialogSaveChanges dialog(this);
        dialog.exec();
        switch (dialog.answer())
        {
        case DialogSaveChanges::Save:
            {
                if (!doc->save(doc->fileName()))
                    cancel = true;
            }
            break;
        case DialogSaveChanges::SaveAs:
            {
                QFileDialog dialog(this);
                dialog.setAcceptMode(QFileDialog::AcceptSave);
                dialog.setFileMode(QFileDialog::AnyFile);
                dialog.setFilter(tr("XML Files (*.xml)"));
                dialog.setWindowTitle(tr("Save file as"));
                if (dialog.exec() == QDialog::Accepted)
                {
                    QString filename = dialog.selectedFiles().at(0);
                    if (!doc->save(filename))
                        cancel = true;
                }
                else
                    cancel = true;
            }
            break;
        case DialogSaveChanges::DontSave:
            break;
        case DialogSaveChanges::Cancel:
            cancel = true;
            break;
        }
    }
    if (!cancel)
    {
        if (this->ui->tabWidget->currentIndex() == index)
        {
            this->mEditor = NULL;
        }
        this->ui->tabWidget->removeTab(index);
        delete w;
    }
}
//-----------------------------------------------------------------------------
void MainWindow::on_tabWidget_currentChanged(int index)
{
    Q_UNUSED(index);
    this->updateMenuState();
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionNew_Image_triggered()
{
    bool ok;
    QString name = QInputDialog::getText(this,
                                         tr("Enter image name"),
                                         tr("Image name:"),
                                         QLineEdit::Normal,
                                         tr("Image", "new image name"),
                                         &ok);
    if (ok)
    {
        EditorTabImage *ed = new EditorTabImage(this);
        this->connect(ed, SIGNAL(dataChanged()), SLOT(on_editor_dataChanged()));

        name = this->findAvailableName(name);
        ed->setDocumentName(name);
        this->ui->tabWidget->addTab(ed, name);
    }
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionNew_Font_triggered()
{
    bool ok;
    QString name = QInputDialog::getText(this,
                                         tr("Enter font name"),
                                         tr("Font name:"),
                                         QLineEdit::Normal,
                                         tr("Font", "new font name"),
                                         &ok);
    if (ok)
    {
        DialogCharacters dialog(this);
        if (dialog.exec() == QDialog::Accepted)
        {
            EditorTabFont *ed = new EditorTabFont(this);
            this->connect(ed, SIGNAL(dataChanged()), SLOT(on_editor_dataChanged()));

            QString chars = dialog.selectedCharacters();
            int size;
            QString family, style;
            dialog.selectedFont(&family, &style, &size);
            ed->assignFontCharacters(chars, family, style, size);

            name = this->findAvailableName(name);
            ed->setDocumentName(name);
            this->ui->tabWidget->addTab(ed, name);
        }
    }
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionOpen_triggered()
{
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setFilter(tr("XML Files (*.xml)"));
    dialog.setWindowTitle(tr("Open file"));

    if (dialog.exec() == QDialog::Accepted)
    {
        bool isImage = false;
        bool isFont = false;

        QString filename = dialog.selectedFiles().at(0);
        QFile file(filename);
        if (file.open(QIODevice::ReadWrite))
        {
            QTextStream stream(&file);
            while (!stream.atEnd())
            {
                QString readedLine = stream.readLine();
                if (readedLine.contains("<data type=\"image\""))
                {
                    isImage = true;
                    break;
                }
                if (readedLine.contains("<data type=\"font\""))
                {
                    isFont = true;
                    break;
                }
            }
            file.close();
        }
        if (isImage)
        {
            EditorTabImage *ed = new EditorTabImage(this);
            this->connect(ed, SIGNAL(dataChanged()), SLOT(on_editor_dataChanged()));

            int index = this->ui->tabWidget->addTab(ed, "");
            ed->load(filename);
            this->ui->tabWidget->setTabText(index, ed->documentName());
        }
    }
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionSave_triggered()
{
    QWidget *w = this->ui->tabWidget->currentWidget();
    IDocument *doc = dynamic_cast<IDocument *> (w);
    if (QFile::exists(doc->fileName()))
        doc->save(doc->fileName());
    else
        this->on_actionSave_As_triggered();
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionSave_As_triggered()
{
    QWidget *w = this->ui->tabWidget->currentWidget();
    IDocument *doc = dynamic_cast<IDocument *> (w);

    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setFilter(tr("XML Files (*.xml)"));
    dialog.setWindowTitle(tr("Save file as"));
    if (dialog.exec() == QDialog::Accepted)
    {
        QString filename = dialog.selectedFiles().at(0);
        doc->save(filename);
    }
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionClose_triggered()
{
    this->on_tabWidget_tabCloseRequested(this->ui->tabWidget->currentIndex());
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionQuit_triggered()
{
    this->close();
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionImageFlip_Horizontal_triggered()
{
    if (this->mEditor != NULL)
    {
        int index = this->mEditor->currentImageIndex();
        QImage *original = this->mEditor->dataContainer()->image(index);
        QImage result = BitmapHelper::flipHorizontal(original);
        this->mEditor->dataContainer()->setImage(index, &result);
    }
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionImageFlip_Vertical_triggered()
{
    if (this->mEditor != NULL)
    {
        int index = this->mEditor->currentImageIndex();
        QImage *original = this->mEditor->dataContainer()->image(index);
        QImage result = BitmapHelper::flipVertical(original);
        this->mEditor->dataContainer()->setImage(index, &result);
    }
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionImageRotate_90_Clockwise_triggered()
{
    if (this->mEditor != NULL)
    {
        int index = this->mEditor->currentImageIndex();
        QImage *original = this->mEditor->dataContainer()->image(index);
        QImage result = BitmapHelper::rotate90(original);
        this->mEditor->dataContainer()->setImage(index, &result);
    }
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionImageRotate_180_triggered()
{
    if (this->mEditor != NULL)
    {
        int index = this->mEditor->currentImageIndex();
        QImage *original = this->mEditor->dataContainer()->image(index);
        QImage result = BitmapHelper::rotate180(original);
        this->mEditor->dataContainer()->setImage(index, &result);
    }
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionImageRotate_90_Counter_Clockwise_triggered()
{
    if (this->mEditor != NULL)
    {
        int index = this->mEditor->currentImageIndex();
        QImage *original = this->mEditor->dataContainer()->image(index);
        QImage result = BitmapHelper::rotate270(original);
        this->mEditor->dataContainer()->setImage(index, &result);
    }
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionImageInverse_triggered()
{
    if (this->mEditor != NULL)
    {
        int index = this->mEditor->currentImageIndex();
        QImage *original = this->mEditor->dataContainer()->image(index);
        QImage result(*original);
        result.invertPixels();
        this->mEditor->dataContainer()->setImage(index, &result);
    }
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionImageResize_triggered()
{
    if (this->mEditor != NULL)
    {
        int index = this->mEditor->currentImageIndex();
        QImage *original = this->mEditor->dataContainer()->image(index);
        DialogResize dialog(original->width(), original->height(), 0, 0, false, this);
        if (dialog.exec() == QDialog::Accepted)
        {
            int width, height, offsetX, offsetY;
            bool center;
            dialog.getResizeInfo(&width, &height, &offsetX, &offsetY, &center);
            QImage result = BitmapHelper::resize(original, width, height, offsetX, offsetY, center, this->mEditor->color2());
            this->mEditor->dataContainer()->setImage(index, &result);
        }
    }
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionImageImport_triggered()
{
    if (this->mEditor != NULL)
    {
        QFileDialog dialog(this);
        dialog.setAcceptMode(QFileDialog::AcceptOpen);
        dialog.setFileMode(QFileDialog::ExistingFile);
        dialog.setFilter(tr("Images (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm)"));
        dialog.setWindowTitle(tr("Open image file"));

        if (dialog.exec() == QDialog::Accepted)
        {
            QImage image;
            image.load(dialog.selectedFiles().at(0));
            int index = this->mEditor->currentImageIndex();
            this->mEditor->dataContainer()->setImage(index, &image);
        }
    }
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionImageExport_triggered()
{
    if (this->mEditor != NULL)
    {
        QFileDialog dialog(this);
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        dialog.setFileMode(QFileDialog::AnyFile);
        QString filter = tr("Windows Bitmap (*.bmp);;" \
                            "Joint Photographic Experts Group (*.jpg *.jpeg);;" \
                            "Portable Network Graphics (*.png);;" \
                            "Portable Pixmap (*.ppm);;" \
                            "Tagged Image File Format (*.tiff);;" \
                            "X11 Bitmap (*.xbm);;" \
                            "X11 Bitmap (*.xpm)");
        dialog.setFilter(filter);
        dialog.setWindowTitle(tr("Save image file"));

        if (dialog.exec() == QDialog::Accepted)
        {
            filter = dialog.selectedFilter();
            QString ext = "png";
            if (filter.contains("bmp"))
                ext = "bmp";
            else if (filter.contains("jpg"))
                ext = "jpg";
            else if (filter.contains("png"))
                ext = "png";
            else if (filter.contains("ppm"))
                ext = "ppm";
            else if (filter.contains("tiff"))
                ext = "tiff";
            else if (filter.contains("xbm"))
                ext = "xbm";
            else if (filter.contains("xpm"))
                ext = "xpm";

            QString filename = dialog.selectedFiles().at(0);
            QFileInfo info(filename);
            QString fileExt = info.suffix().toLower();
            if (fileExt.isEmpty() || fileExt != ext)
            {
                filename += "." + ext;
            }
            int index = this->mEditor->currentImageIndex();
            this->mEditor->dataContainer()->image(index)->save(filename);
        }
    }
}
//-----------------------------------------------------------------------------
void MainWindow::on_editor_dataChanged()
{
    QWidget *w = qobject_cast<QWidget *>(sender());
    int index = this->ui->tabWidget->indexOf(w);
    IDocument *doc = dynamic_cast<IDocument *> (w);

    if (doc->changed())
        this->ui->tabWidget->setTabText(index, "* " + doc->documentName());
    else
        this->ui->tabWidget->setTabText(index, doc->documentName());
    this->ui->tabWidget->setTabToolTip(index, doc->fileName());
}
//-----------------------------------------------------------------------------
