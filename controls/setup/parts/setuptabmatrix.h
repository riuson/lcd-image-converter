#ifndef SETUPTABMATRIX_H
#define SETUPTABMATRIX_H
//-----------------------------------------------------------------------------
#include <QWidget>
//-----------------------------------------------------------------------------
namespace Ui {
class SetupTabMatrix;
}
//-----------------------------------------------------------------------------
class Preset;
class MatrixPreviewModel;
class MatrixItemDelegate;
class QMenu;
//-----------------------------------------------------------------------------
class SetupTabMatrix : public QWidget
{
    Q_OBJECT

public:
    explicit SetupTabMatrix(Preset *preset, QWidget *parent = 0);
    virtual ~SetupTabMatrix();

public slots:
    void matrixChanged();

private:
    Ui::SetupTabMatrix *ui;
    Preset *mPreset;
    MatrixPreviewModel *mMatrixModel;
    MatrixItemDelegate *mMatrixItemDelegate;
    QMenu *mMenu;

private slots:
    void on_comboBoxConversionType_currentIndexChanged(int index);
    void on_tableViewOperations_customContextMenuRequested(const QPoint &point);
    void operationAdd();
    void operationShift();
    void operationRemove();
    void maskReset();
};
//-----------------------------------------------------------------------------
#endif // SETUPTABMATRIX_H
