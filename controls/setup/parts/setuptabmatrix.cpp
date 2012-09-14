#include "setuptabmatrix.h"
#include "ui_setuptabmatrix.h"
//-----------------------------------------------------------------------------
#include "conversionmatrix.h"
#include "matrixpreviewmodel.h"
#include "matrixitemdelegate.h"
//-----------------------------------------------------------------------------
SetupTabMatrix::SetupTabMatrix(ConversionMatrix *matrix, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetupTabMatrix)
{
    ui->setupUi(this);
    this->mMatrix = matrix;

    this->mMatrixModel = new MatrixPreviewModel(this->mMatrix, this);
    this->ui->tableViewOperations->setModel(this->mMatrixModel);
    this->ui->tableViewOperations->resizeColumnsToContents();
    this->ui->tableViewOperations->resizeRowsToContents();

    this->ui->tableViewOperations->verticalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    this->connect(this->ui->tableViewOperations->verticalHeader(), SIGNAL(customContextMenuRequested(QPoint)), SLOT(on_tableViewOperations_customContextMenuRequested(QPoint)));

    this->mMatrixItemDelegate = new MatrixItemDelegate(this);
    this->ui->tableViewOperations->setItemDelegate(this->mMatrixItemDelegate);
}
//-----------------------------------------------------------------------------
SetupTabMatrix::~SetupTabMatrix()
{
    if (this->mMenu != NULL)
        delete this->mMenu;

    delete this->mMatrixItemDelegate;
    delete this->mMatrixModel;

    delete ui;
}
//-----------------------------------------------------------------------------
void SetupTabMatrix::matrixChanged()
{
    this->mMatrixModel->callReset();
    this->ui->tableViewOperations->setModel(NULL);
    this->ui->tableViewOperations->setModel(this->mMatrixModel);
    this->ui->tableViewOperations->update();
    this->ui->tableViewOperations->resizeRowsToContents();
    this->ui->tableViewOperations->resizeColumnsToContents();
}
//-----------------------------------------------------------------------------
void SetupTabMatrix::on_tableViewOperations_customContextMenuRequested(const QPoint &point)
{
    QModelIndex index = this->ui->tableViewOperations->indexAt(point);
    QItemSelectionModel *selection = this->ui->tableViewOperations->selectionModel();

    if (this->mMenu != NULL)
    {
        delete this->mMenu;
        this->mMenu = NULL;
    }

    if (index.isValid())
    {
        MatrixPreviewModel::RowType type = this->mMatrixModel->rowType(index.row());
        QModelIndexList list = selection->selectedIndexes();

        switch (type)
        {
        case MatrixPreviewModel::Source:
        {
            bool found = false;
            for (int i = 0; i < list.length() && !found; i++)
            {
                if (list.at(i).row() == 0)
                    found = true;
            }
            if (found)
            {
                this->mMenu = new QMenu(tr("Source"), this);

                QMenu *left = new QMenu(tr("Add \"Left Shift\""), this->mMenu);
                QMenu *right = new QMenu(tr("Add \"Right Shift\""), this->mMenu);

                this->mMenu->addMenu(left);
                this->mMenu->addMenu(right);

                for (int i = 0; i < 32; i++)
                {
                    QAction *action = left->addAction(QString("<< %1").arg(i), this, SLOT(operationAdd()));
                    action->setData(QVariant(-i));

                    action = right->addAction(QString(">> %1").arg(i), this, SLOT(operationAdd()));
                    action->setData(QVariant(i));
                }

                this->mMenu->exec(this->ui->tableViewOperations->mapToGlobal(point));
            }
            break;
        }
        case MatrixPreviewModel::Operation:
        {
                this->mMenu = new QMenu(tr("Operation"), this);

                int operationIndex = index.row() - 1;

                quint32 mask;
                int shift;
                bool left;

                this->mMatrix->operation(operationIndex, &mask, &shift, &left);

                QAction *actionLeft = this->mMenu->addAction(tr("Shift left"), this, SLOT(operationShift()));
                QAction *actionRight = this->mMenu->addAction(tr("Shift right"), this, SLOT(operationShift()));
                QAction *actionRemove = this->mMenu->addAction(tr("Remove"), this, SLOT(operationRemove()));
                Q_UNUSED(actionRemove)

                quint32 data = operationIndex;

                actionLeft->setData(QVariant(data | 0x80000000));
                actionRight->setData(QVariant(data));
                actionRemove->setData(QVariant(data));

                if (shift >= 31)
                {
                    if (left)
                        actionLeft->setEnabled(false);
                    else
                        actionRight->setEnabled(false);
                }

                this->mMenu->exec(this->ui->tableViewOperations->mapToGlobal(point));
            break;
        }
        case MatrixPreviewModel::MaskUsed:
        case MatrixPreviewModel::MaskAnd:
        case MatrixPreviewModel::MaskOr:
        case MatrixPreviewModel::MaskFill:
        {
            this->mMenu = new QMenu(tr("Mask"), this);

            quint32 data = (quint32)type;

            quint32 bits = 0;
            for (int i = 0; i < list.length(); i++)
            {
                if (list.at(i).row() == index.row())
                {
                    bits |= 0x00000001 << (31 - list.at(i).column());
                }
            }

            // show menu if more than 0 bits was selected
            if (bits != 0)
            {
                QList<QVariant> parameters;
                parameters.append(QVariant(data));
                parameters.append(QVariant(bits));
                parameters.append(QVariant(true));

                QAction *actionSet = this->mMenu->addAction(tr("Set 1"), this, SLOT(maskReset()));
                actionSet->setData(parameters);

                QAction *actionReset = this->mMenu->addAction(tr("Set 0"), this, SLOT(maskReset()));
                parameters.replace(2, QVariant(false));
                actionReset->setData(parameters);
            }

            this->mMenu->exec(this->ui->tableViewOperations->mapToGlobal(point));
            break;
        }
        default:
            break;
        }
    }
}
//-----------------------------------------------------------------------------
void SetupTabMatrix::operationAdd()
{
    QAction *a = qobject_cast<QAction *>(sender());
    QVariant var = a->data();
    bool ok;
    int shift = var.toInt(&ok);

    if (ok)
    {
        QItemSelectionModel *selection = this->ui->tableViewOperations->selectionModel();
        QModelIndexList list = selection->selectedIndexes();

        bool left = shift < 0;
        shift = qAbs(shift);

        quint32 mask = 0;
        for (int i = 0; i < list.length(); i++)
        {
            if (list.at(i).row() == 0)
            {
                mask |= 0x00000001 << (31 - list.at(i).column());
            }
        }
        this->mMatrix->operationAdd(mask, shift, left);
    }
}
//-----------------------------------------------------------------------------
void SetupTabMatrix::operationShift()
{
    QAction *a = qobject_cast<QAction *>(sender());
    QVariant var = a->data();
    bool ok;
    quint32 index = var.toUInt(&ok);

    if (ok)
    {
        bool leftShift = ((index & 0x80000000) != 0);
        index &= 0x7fffffff;

        quint32 mask;
        int shift;
        bool left;
        this->mMatrix->operation(index, &mask, &shift, &left);

        if (leftShift)
        {
            if (shift > 0)
            {
                if (left)
                    shift++;
                else
                    shift--;
            }
            else
            {
                shift = 1;
                left = true;
            }
        }
        else
        {
            if (shift > 0)
            {
                if (left)
                    shift--;
                else
                    shift++;
            }
            else
            {
                shift = 1;
                left = false;
            }
        }

        this->mMatrix->operationReplace(index, mask, shift, left);
    }
}
//-----------------------------------------------------------------------------
void SetupTabMatrix::operationRemove()
{
    QAction *a = qobject_cast<QAction *>(sender());
    QVariant var = a->data();
    bool ok;
    quint32 index = var.toUInt(&ok);

    if (ok)
    {
        this->mMatrix->operationRemove(index);
    }
}
//-----------------------------------------------------------------------------
void SetupTabMatrix::maskReset()
{
    QAction *a = qobject_cast<QAction *>(sender());
    QList<QVariant> vars = a->data().toList();

    bool ok;
    quint32 i = vars.at(0).toUInt(&ok);

    if (ok)
    {
        MatrixPreviewModel::RowType type = (MatrixPreviewModel::RowType)i;

        quint32 mask = vars.at(1).toUInt(&ok);
        if (ok)
        {
            bool setBits = vars.at(2).toBool();

            switch (type)
            {
            case MatrixPreviewModel::MaskUsed:
            {
                if (setBits)
                    this->mMatrix->options()->setMaskUsed( this->mMatrix->options()->maskUsed() | mask);
                else
                    this->mMatrix->options()->setMaskUsed( this->mMatrix->options()->maskUsed() & ~mask);
                break;
            }
            case MatrixPreviewModel::MaskAnd:
            {
                if (setBits)
                    this->mMatrix->options()->setMaskAnd( this->mMatrix->options()->maskAnd() | mask);
                else
                    this->mMatrix->options()->setMaskAnd( this->mMatrix->options()->maskAnd() & ~mask);
                break;
            }
            case MatrixPreviewModel::MaskOr:
            {
                if (setBits)
                    this->mMatrix->options()->setMaskOr( this->mMatrix->options()->maskOr() | mask);
                else
                    this->mMatrix->options()->setMaskOr( this->mMatrix->options()->maskOr() & ~mask);
                break;
            }
            case MatrixPreviewModel::MaskFill:
            {
                if (setBits)
                    this->mMatrix->options()->setMaskFill( this->mMatrix->options()->maskFill() | mask);
                else
                    this->mMatrix->options()->setMaskFill( this->mMatrix->options()->maskFill() & ~mask);
                break;
            }
            default:
                break;
            }
        }
    }
}
//-----------------------------------------------------------------------------
