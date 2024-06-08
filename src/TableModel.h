#include <QtWidgets>

// ======================================================================
class TableModel : public QAbstractTableModel {
private:
    int                          _rows_count;
    int                          _cols_count;
    QHash<QModelIndex, QVariant> _table_hash;

public:
    TableModel(int rows_cownt, int cols_count, QObject* parent = 0);
 
    QVariant data(const QModelIndex& index, int role) const;

    int rowCount(const QModelIndex&) const;
    int columnCount(const QModelIndex&) const;

    bool newStringRow(const QString& address, const QString& message);

    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole
                       ) const;
};
