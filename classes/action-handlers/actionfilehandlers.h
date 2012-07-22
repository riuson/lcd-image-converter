#ifndef ACTIONFILEHANDLERS_H
#define ACTIONFILEHANDLERS_H
//-----------------------------------------------------------------------------
#include <QObject>

#include "actionhandlersbase.h"
//-----------------------------------------------------------------------------
class IMainWindow;
//-----------------------------------------------------------------------------
class ActionFileHandlers : public ActionHandlersBase
{
    Q_OBJECT
public:
    explicit ActionFileHandlers(QObject *parent = 0);
    
signals:
    void newFileOpened(const QString &filename);
    void closeRequest(QWidget *tab);
    void exitRequest();
    void tabChanged(QWidget *w, const QString &documentName, const QString &filename);
    
public slots:
    void on_actionNew_Image_triggered();
    void on_actionNew_Font_triggered();
    void on_actionOpen_triggered();
    void on_actionRename_triggered();
    void on_actionSave_triggered();
    void on_actionSave_As_triggered();
    void on_actionClose_triggered();
    void on_actionConvert_triggered();
    void on_actionQuit_triggered();
    void openFile(const QString &filename);
private slots:
    void documentChanged(bool changed, const QString &documentName, const QString &filename);
};
//-----------------------------------------------------------------------------
#endif // ACTIONFILEHANDLERS_H
