/**     @file ModelPartList.h
  *
  *     EEEE2076 - Software Engineering & VR Project
  *
  *     Template for model part list that will be used to create the trewview.
  *
  *     P Evans 2022
  */

#include "ModelPartList.h"
#include "ModelPart.h"

ModelPartList::ModelPartList( const QString& data, QObject* parent ) : QAbstractItemModel(parent) {
    /* Have option to specify number of visible properties for each item in tree - the root item
     * acts as the column headers
     */
    rootItem = new ModelPart( { tr("Part"), tr("Visible?") } );
}



ModelPartList::~ModelPartList() {
    delete rootItem;
}


int ModelPartList::columnCount( const QModelIndex& parent ) const {
    Q_UNUSED(parent);
    Q_UNUSED(parent);
    return 4; // Part, Colour, Visible;
}

bool ModelPartList::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!index.isValid())
        return false;

    ModelPart* item = static_cast<ModelPart*>(index.internalPointer());

    if (index.column() == 2 && role == Qt::CheckStateRole) { // Visibility column
        bool isChecked = (value.toInt() == Qt::Checked);
        item->setVisible(isChecked); // Update model data

        emit dataChanged(index, index, {Qt::CheckStateRole, Qt::DisplayRole}); // Force UI update
        return true;
    }

    return false;
}


QVariant ModelPartList::data(const QModelIndex& index, int role) const {
    if (!index.isValid())
        return QVariant();

    ModelPart* item = static_cast<ModelPart*>(index.internalPointer());

    if (role == Qt::DisplayRole) {
        if (index.column() == 0) { // Part Name Column
            return item->data(0).toString();
        }
        if (index.column() == 1) { // Colour Column
            return QString("%1,%2,%3")
                .arg(item->getColourR())
                .arg(item->getColourG())
                .arg(item->getColourB());
        }
    }

    // Ensure visibility is correctly retrieved for checkboxes
    if (role == Qt::CheckStateRole && index.column() == 2) {
        return item->visible() ? Qt::Checked : Qt::Unchecked;
    }

    return QVariant();
}




Qt::ItemFlags ModelPartList::flags(const QModelIndex& index) const {
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    if (index.column() == 2) { // Make the visibility column interactive
        flags |= Qt::ItemIsUserCheckable | Qt::ItemIsEditable; // Enable checkbox interaction
    }

    return flags;
}




QVariant ModelPartList::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0: return "Part";
        case 1: return "Colour";
        case 2: return "Visible?";
        }
    }
    return QVariant();
}



QModelIndex ModelPartList::index(int row, int column, const QModelIndex& parent) const {
    ModelPart* parentItem;
    
    if( !parent.isValid() || !hasIndex(row, column, parent) )
        parentItem = rootItem;              // default to selecting root 
    else
        parentItem = static_cast<ModelPart*>(parent.internalPointer());

    ModelPart* childItem = parentItem->child(row);
    if( childItem )
        return createIndex(row, column, childItem);
    
    
    return QModelIndex();
}


QModelIndex ModelPartList::parent( const QModelIndex& index ) const {
    if (!index.isValid())
        return QModelIndex();

    ModelPart* childItem = static_cast<ModelPart*>(index.internalPointer());
    ModelPart* parentItem = childItem->parentItem();

    if( parentItem == rootItem )
        return QModelIndex();

    return createIndex( parentItem->row(), 0, parentItem );
}


int ModelPartList::rowCount( const QModelIndex& parent ) const {
    ModelPart* parentItem;
    if( parent.column() > 0 )
        return 0;

    if( !parent.isValid() )
        parentItem = rootItem;
    else
        parentItem = static_cast<ModelPart*>(parent.internalPointer());

    return parentItem->childCount();
}


ModelPart* ModelPartList::getRootItem() {
    return rootItem; 
}



QModelIndex ModelPartList::appendChild(QModelIndex& parent, const QList<QVariant>& data) {      
    ModelPart* parentPart;

    if (parent.isValid())
        parentPart = static_cast<ModelPart*>(parent.internalPointer());
    else {
        parentPart = rootItem;
        parent = createIndex(0, 0, rootItem );
    }

    beginInsertRows( parent, rowCount(parent), rowCount(parent) ); 

    ModelPart* childPart = new ModelPart( data, parentPart );

    parentPart->appendChild(childPart);

    QModelIndex child = createIndex(0, 0, childPart);

    endInsertRows();

    emit layoutChanged();

    return child;
}

