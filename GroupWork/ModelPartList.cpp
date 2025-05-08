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
/**
 * @brief Constructs the ModelPartList and initializes the root item with column headers.
 *
 * Creates a root ModelPart that serves as the base of the tree model with default columns
 * such as "Part" and "Visible?".
 *
 * @param data The name or label of the root item (not shown directly in the tree).
 * @param parent The parent QObject, usually nullptr.
 */
ModelPartList::ModelPartList( const QString& data, QObject* parent ) : QAbstractItemModel(parent) {
    /* Have option to specify number of visible properties for each item in tree - the root item
     * acts as the column headers
     */
    rootItem = new ModelPart( { tr("Part"), tr("Visible?") } );
}


/**
 * @brief Destructor for the ModelPartList.
 *
 * Cleans up memory by deleting the root item, which recursively deletes all children.
 */
ModelPartList::~ModelPartList() {
    delete rootItem;
}


/**
 * @brief Returns the number of columns used in the tree model.
 *
 * Delegates to the root item to determine how many data fields each tree item has.
 *
 * @param parent The parent index (not used in column count).
 * @return The number of columns in the model.
 */
int ModelPartList::columnCount( const QModelIndex& parent ) const {
    Q_UNUSED(parent);

    return rootItem->columnCount();
}

/**
 * @brief Returns the data stored at the given index and role.
 *
 * Provides display text for each cell in the tree view by delegating to the corresponding ModelPart.
 * Returns an empty QVariant for unsupported roles or invalid indexes.
 *
 * @param index The index of the data to retrieve.
 * @param role The role in which the data is requested (typically Qt::DisplayRole).
 * @return The data to display or an empty QVariant.
 */
QVariant ModelPartList::data( const QModelIndex& index, int role ) const {
    /* If the item index isnt valid, return a new, empty QVariant (QVariant is generic datatype
     * that could be any valid QT class) */
    if( !index.isValid() )
        return QVariant();

    /* Role represents what this data will be used for, we only need deal with the case
     * when QT is asking for data to create and display the treeview. Return a new,
     * empty QVariant if any other request comes through. */
    if (role != Qt::DisplayRole)
        return QVariant();

    /* Get a a pointer to the item referred to by the QModelIndex */
    ModelPart* item = static_cast<ModelPart*>( index.internalPointer() );

    /* Each item in the tree has a number of columns ("Part" and "Visible" in this 
     * initial example) return the column requested by the QModelIndex */
    return item->data( index.column() );
}

/**allowing double click to modify to treeview item
Qt::ItemFlags ModelPartList::flags( const QModelIndex& index ) const {
    if( !index.isValid() )
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags( index );
}*/


/**
 * @brief Returns the item flags for a given model index.
 *
 * Enables editing, selection, and interaction for valid indexes.
 *
 * @param index The index in question.
 * @return The appropriate Qt item flags.
 */
Qt::ItemFlags ModelPartList::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

/**
 * @brief Provides header data for the tree view.
 *
 * Supplies column headers such as "Part" and "Visible?" based on root item data.
 *
 * @param section Column index.
 * @param orientation Qt::Horizontal for columns, Qt::Vertical is ignored.
 * @param role Role type (typically Qt::DisplayRole).
 * @return Header data or an empty QVariant.
 */
QVariant ModelPartList::headerData( int section, Qt::Orientation orientation, int role ) const {
    if( orientation == Qt::Horizontal && role == Qt::DisplayRole )
        return rootItem->data( section );

    return QVariant();
}

/**
 * @brief Returns the index of the item at the given row and column under a parent.
 *
 * Retrieves a QModelIndex corresponding to the specified child of a parent ModelPart.
 *
 * @param row The row number of the child.
 * @param column The column number of the child.
 * @param parent The parent QModelIndex.
 * @return The QModelIndex for the requested child, or an invalid index if not found.
 */
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

/**
 * @brief Returns the parent index of a given index.
 *
 * Determines the QModelIndex corresponding to the parent of a specified child item.
 *
 * @param index The QModelIndex whose parent is requested.
 * @return The parent QModelIndex, or invalid if the parent is root.
 */
QModelIndex ModelPartList::parent( const QModelIndex& index ) const {
    if (!index.isValid())
        return QModelIndex();

    ModelPart* childItem = static_cast<ModelPart*>(index.internalPointer());
    ModelPart* parentItem = childItem->parentItem();

    if( parentItem == rootItem )
        return QModelIndex();

    return createIndex( parentItem->row(), 0, parentItem );
}

/**
 * @brief Returns the number of rows (children) under a given parent.
 *
 * Determines how many child items exist for a specified parent index.
 *
 * @param parent The parent index.
 * @return The number of children under the parent.
 */
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

/**
 * @brief Retrieves the root item of the model.
 *
 * Provides access to the root ModelPart which anchors the entire tree structure.
 *
 * @return Pointer to the root ModelPart.
 */
ModelPart* ModelPartList::getRootItem() {
    return rootItem; 
}

/**
 * @brief Appends a new child item under the specified parent.
 *
 * Inserts a new ModelPart with the provided data under the given parent index.
 * Notifies the view to update via model signals.
 *
 * @param parent The parent index under which the child will be added.
 * @param data A list of QVariant values representing the child's columns.
 * @return The QModelIndex of the newly inserted child item.
 */
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
/**Can;t avoid empty input, replaced
bool ModelPartList::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    ModelPart* item = static_cast<ModelPart*>(index.internalPointer());
    item->set(index.column(), value);  // 修改 ModelPart 内部数据

    emit dataChanged(index, index);   // 通知视图刷新显示
    return true;
}*/
/**
 * @brief Updates the data of a tree item in response to user edits.
 *
 * Validates and applies edits from the UI. Ignores changes with empty or whitespace-only values.
 * Notifies the view that the data has changed.
 *
 * @param index The QModelIndex being edited.
 * @param value The new value to set.
 * @param role The role of the data edit (typically Qt::EditRole).
 * @return True if the data was successfully updated; false otherwise.
 */
bool ModelPartList::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    QString newValue = value.toString().trimmed();
    if (newValue.isEmpty()) {
        //qDebug() << "Invalid input, ignoring change.";//will show on the console but not by status bar
        //emit statusUpdateMessage("Name cannot be empty. Change ignored.", 2000);needs reference of parameter
        return false;  // 🚫 忽略空白输入
    }

    ModelPart* item = static_cast<ModelPart*>(index.internalPointer());
    item->set(index.column(), newValue);  // ✅ 使用处理后的值

    emit dataChanged(index, index);   // 通知 TreeView 刷新
    return true;
}

/**
 * @brief Removes a model part and all its children from the tree.
 *
 * Deletes the specified part from the data structure and deallocates its memory.
 * Emits model signals to inform the view of the removal.
 *
 * @param index The QModelIndex of the item to remove.
 * @return True if removal was successful; false otherwise.
 */
bool ModelPartList::removePart(const QModelIndex& index) {
    if (!index.isValid())
        return false;

    ModelPart* part = static_cast<ModelPart*>(index.internalPointer());
    ModelPart* parent = part->parentItem();
    if (!part || !parent)
        return false;

    int row = part->row();

    QModelIndex parentIndex = this->parent(index);

    beginRemoveRows(parentIndex, row, row);
    parent->removeChild(part);  // 移除模型结构
    delete part;                // 删除对象本身
    endRemoveRows();

    return true;
}
