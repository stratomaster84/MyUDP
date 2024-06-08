
#include "TableModel.h"

// ======================================================================

TableModel::TableModel(int rows_count, int cols_count, QObject* parent)
    : QAbstractTableModel(parent)
    , _rows_count(rows_count)
    , _cols_count(cols_count)
{
}

QVariant TableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    return (role == Qt::DisplayRole)
            ? _table_hash.value(index)
            : QVariant();
}

int TableModel::rowCount(const QModelIndex&) const
{
    return _rows_count;
}

int TableModel::columnCount(const QModelIndex&) const
{
    return _cols_count;
}

bool TableModel::newStringRow(const QString& address, const QString& message){

    QModelIndex index1;
    QModelIndex index2;

    beginInsertRows(QModelIndex(), _rows_count, _rows_count);
    _rows_count++;
    endInsertRows();

    index1 = index(_rows_count-1,0,QModelIndex());
    index2 = index(_rows_count-1,1,QModelIndex());
    if (index1.isValid() && index2.isValid()){
        _table_hash[index1] = address;
        _table_hash[index2] = message;
        emit dataChanged(index1, index2);
        return true;
    }
    return false;
}
// ----------------------------------------------------------------------
QVariant TableModel::headerData(int section,
                                Qt::Orientation orientation,
                                int role
                               ) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }
    if(orientation == Qt::Vertical)
        return QString::number(section);
    if(section == 0)
        return QString("Источник");
    if(section == 1)
        return QString("Сообщение");

    return QString::number(section);
}
// ----------------------------------------------------------------------
