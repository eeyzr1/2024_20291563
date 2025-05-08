/**     @file ModelPart.cpp
  *
  *     EEEE2076 - Software Engineering & VR Project
  *
  *     Template for model parts that will be added as treeview items
  *
  *     P Evans 2022
  */

#include "ModelPart.h"


/* Commented out for now, will be uncommented later when you have
 * installed the VTK library
 */
//
#include <vtkProperty.h>
#include <vtkSmartPointer.h>
//
#include <vtkDataSetMapper.h>
#include <vtkPolyDataMapper.h>
#include <vtkSTLReader.h>
#include <vtkActor.h>
//for filter
#include <vtkShrinkFilter.h>
#include <vtkClipDataSet.h>
#include <vtkPlane.h>
#include <vtkClipPolyData.h>//for clip
#include <vtkShrinkPolyData.h>  // 更适用于 polydata，兼容性更高




/**
 * @brief Constructs a ModelPart instance with given data and parent.
 *
 * Initializes the model part node with column data and sets its parent for tree hierarchy.
 *
 * @param data List of column values associated with the model part.
 * @param parent Pointer to the parent ModelPart, if any.
 */
ModelPart::ModelPart(const QList<QVariant>& data, ModelPart* parent )
    : m_itemData(data), m_parentItem(parent) {

    /* You probably want to give the item a default colour */
}

/**
 * @brief Destructor for ModelPart.
 *
 * Releases all child items owned by this part to prevent memory leaks.
 */
ModelPart::~ModelPart() {
    qDeleteAll(m_childItems);
}

/**
 * @brief Appends a child ModelPart to this part.
 *
 * Sets the child's parent to this item and adds it to the internal child list.
 *
 * @param item Pointer to the child ModelPart to be added.
 */
void ModelPart::appendChild( ModelPart* item ) {
    /* Add another model part as a child of this part
     * (it will appear as a sub-branch in the treeview)
     */
    item->m_parentItem = this;
    m_childItems.append(item);
}


/**
 * @brief Returns the child ModelPart at the specified row.
 *
 * @param row Index of the child to retrieve.
 * @return Pointer to the child ModelPart, or nullptr if the index is out of bounds.
 */
ModelPart* ModelPart::child( int row ) {
    /* Return pointer to child item in row below this item.
     */
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

/**
 * @brief Returns the number of child items of this part.
 *
 * @return The count of child ModelPart instances.
 */
int ModelPart::childCount() const {
    /* Count number of child items
     */
    return m_childItems.count();
}

/**
 * @brief Returns the number of columns (data fields) for this item.
 *
 * @return The number of columns in the item data.
 */
int ModelPart::columnCount() const {
    /* Count number of columns (properties) that this item has.
     */
    return m_itemData.count();
}

/**
 * @brief Retrieves the data stored in a specific column.
 *
 * @param column The column index.
 * @return The QVariant data at the specified column, or an invalid QVariant if out of bounds.
 */
QVariant ModelPart::data(int column) const {
    /* Return the data associated with a column of this item 
     *  Note on the QVariant type - it is a generic placeholder type
     *  that can take on the type of most Qt classes. It allows each 
     *  column or property to store data of an arbitrary type.
     */
    if (column < 0 || column >= m_itemData.size())
        return QVariant();
    return m_itemData.at(column);
}

/**
 * @brief Sets the data in a specified column.
 *
 * Replaces the existing value at the given column index with a new QVariant value.
 *
 * @param column The column index to update.
 * @param value The new value to set.
 */
void ModelPart::set(int column, const QVariant &value) {
    /* Set the data associated with a column of this item 
     */
    if (column < 0 || column >= m_itemData.size())
        return;

    m_itemData.replace(column, value);
}

/**
 * @brief Returns the parent ModelPart of this item.
 *
 * @return Pointer to the parent item, or nullptr if this is a root item.
 */
ModelPart* ModelPart::parentItem() {
    return m_parentItem;
}

/**
 * @brief Returns the row index of this item within its parent's child list.
 *
 * @return The row index, or 0 if this is a root item.
 */
int ModelPart::row() const {
    /* Return the row index of this item, relative to it's parent.
     */
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<ModelPart*>(this));
    return 0;
}

//not used
void ModelPart::setColour(const unsigned char R, const unsigned char G, const unsigned char B) {
    /* This is a placeholder function that you will need to modify if you want to use it */
    
    /* As the name suggests ... */
}
//not used
unsigned char ModelPart::getColourR() {
    /* This is a placeholder function that you will need to modify if you want to use it */
    
    /* As the name suggests ... */
    return 0;   // needs updating
}
//not used
unsigned char ModelPart::getColourG() {
    /* This is a placeholder function that you will need to modify if you want to use it */
    
    /* As the name suggests ... */
    return 0;   // needs updating
}
//not used
unsigned char ModelPart::getColourB() {
   /* This is a placeholder function that you will need to modify if you want to use it */
    
    /* As the name suggests ... */
    return 0;   // needs updating
}

/**
 * @brief Sets the visibility of this model part and updates its actor accordingly.
 *
 * Adjusts both the internal visibility state and the VTK actor's visibility,
 * and updates the visibility flag shown in the tree view.
 *
 * @param isVisible True to make the part visible; false to hide it.
 */
void ModelPart::setVisible(bool isVisible) {
    /* This is a placeholder function that you will need to modify if you want to use it */
    this->isVisible = isVisible;
    if (actor) actor->SetVisibility(isVisible ? 1 : 0);
    /* As the name suggests ... */
    // 更新 TreeView 第1列显示
    this->set(1, isVisible ? "true" : "false");
}


/**
 * @brief Checks if the model part is currently visible.
 *
 * @return True if the part is visible; otherwise, false.
 */
bool ModelPart::visible() {
    /* This is a placeholder function that you will need to modify if you want to use it */
    return this->isVisible;
    /* As the name suggests ... */
    return false;
}

/**
 * @brief Loads an STL file into the model part and initializes its VTK components.
 *
 * Uses vtkSTLReader to read the file, sets up a mapper and actor for rendering,
 * and stores the file path for future reference.
 *
 * @param fileName The absolute path to the STL file to load.
 */
void ModelPart::loadSTL(QString fileName) {
    /* This is a placeholder function that you will need to modify if you want to use it */

    /* 1. Use the vtkSTLReader class to load the STL file
     *     https://vtk.org/doc/nightly/html/classvtkSTLReader.html
     */
    this->file = vtkSmartPointer<vtkSTLReader>::New();
    file->SetFileName(fileName.toStdString().c_str());
    file->Update();
    /* 2. Initialise the part's vtkMapper */
    this->mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(file->GetOutputPort());
    /* 3. Initialise the part's vtkActor and link to the mapper */
    this->actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    /* 可选：设置初始颜色
    actor->GetProperty()->SetColor(1.0, 1.0, 1.0); // 白色*/
    this->filePath = fileName;//get file name for saving

}

/**
 * @brief Retrieves the VTK actor associated with this model part.
 *
 * @return A smart pointer to the vtkActor representing this model part.
 */
vtkSmartPointer<vtkActor> ModelPart::getActor() {
    /* This is a placeholder function that you will need to modify if you want to use it */
    return this->actor;
    /* Needs to return a smart pointer to the vtkActor to allow
     * part to be rendered.
     */
}

//vtkActor* ModelPart::getNewActor() {
    /* This is a placeholder function that you will need to modify if you want to use it
     * 
     * The default mapper/actor combination can only be used to render the part in 
     * the GUI, it CANNOT also be used to render the part in VR. This means you need
     * to create a second mapper/actor combination for use in VR - that is the role
     * of this function. */
     
     
     /* 1. Create new mapper */
     
     /* 2. Create new actor and link to mapper */
     
     /* 3. Link the vtkProperties of the original actor to the new actor. This means 
      *    if you change properties of the original part (colour, position, etc), the
      *    changes will be reflected in the GUI AND VR rendering.
      *    
      *    See the vtkActor documentation, particularly the GetProperty() and SetProperty()
      *    functions.
      */
    

    /* The new vtkActor pointer must be returned here */
//    return nullptr;
    
//}
/**
 * @brief Removes a specified child from this model part's child list.
 *
 * @param child Pointer to the child ModelPart to remove.
 */
void ModelPart::removeChild(ModelPart* child) {
    m_childItems.removeOne(child);
}

/**
 * @brief Applies a shrink filter to this model part's geometry.
 *
 * Modifies the input of the VTK mapper using vtkShrinkPolyData to reduce
 * the mesh size visually. Triggers re-mapping to reflect the changes.
 */
void ModelPart::applyShrinkFilter() {
    if (!actor || !mapper) return;

    vtkSmartPointer<vtkShrinkPolyData> shrinkFilter = vtkSmartPointer<vtkShrinkPolyData>::New();
    shrinkFilter->SetInputConnection(mapper->GetInputConnection(0, 0));
    shrinkFilter->SetShrinkFactor(0.8);
    shrinkFilter->Update();

    mapper->SetInputConnection(shrinkFilter->GetOutputPort());

    actor->SetMapper(mapper);  // ⬅️ 这里必须调用，确保 actor 显示更新后数据
}

/**
 * @brief Applies a clipping filter to this model part's geometry.
 *
 * Clips the mesh using a vtkPlane aligned along the X-axis at the model's center,
 * and updates the mapper to use the clipped output.
 */
void ModelPart::applyClipFilter() {
    if (!actor || !mapper) return;

    double bounds[6];
    actor->GetBounds(bounds);
    double center[3] = {
        (bounds[0] + bounds[1]) / 2.0,
        (bounds[2] + bounds[3]) / 2.0,
        (bounds[4] + bounds[5]) / 2.0
    };

    vtkSmartPointer<vtkPlane> clipPlane = vtkSmartPointer<vtkPlane>::New();
    clipPlane->SetOrigin(center);
    clipPlane->SetNormal(-1.0, 0.0, 0.0);  // 向右裁剪

    vtkSmartPointer<vtkClipPolyData> clipFilter = vtkSmartPointer<vtkClipPolyData>::New();
    clipFilter->SetInputConnection(mapper->GetInputConnection(0, 0));  // 使用当前 mapper 输入
    clipFilter->SetClipFunction(clipPlane);
    clipFilter->Update();

    mapper->SetInputConnection(clipFilter->GetOutputPort());
    actor->SetMapper(mapper);

}
/*
 * void ModelPart::resetGeometry() {
    if (filePath.isEmpty()) return;

    vtkSmartPointer<vtkSTLReader> newReader = vtkSmartPointer<vtkSTLReader>::New();
    newReader->SetFileName(filePath.toStdString().c_str());
    newReader->Update();

    file = newReader;

    mapper->SetInputConnection(file->GetOutputPort());
    actor->SetMapper(mapper);

    // 可选：继承颜色和可见性
    actor->GetProperty()->SetColor(colour[0], colour[1], colour[2]);
    actor->SetVisibility(isVisible ? 1 : 0);
}
 */

/**
 * @brief Resets the geometry of the model part to its original state.
 *
 * Reloads the STL file from disk, restoring its geometry. The current color and
 * visibility state are preserved and reapplied to the actor.
 */
void ModelPart::resetGeometry() {
    if (filePath.isEmpty()) return;

    // Step 1: 保存当前颜色和可见性
    double rgb[3] = {1.0, 1.0, 1.0};
    bool visible = true;

    if (actor && actor->GetProperty()) {
        actor->GetProperty()->GetColor(rgb);
        visible = actor->GetVisibility();
    }

    // Step 2: 重新加载 STL
    vtkSmartPointer<vtkSTLReader> newReader = vtkSmartPointer<vtkSTLReader>::New();
    newReader->SetFileName(filePath.toStdString().c_str());
    newReader->Update();

    file = newReader;

    mapper->SetInputConnection(file->GetOutputPort());
    actor->SetMapper(mapper);

    // Step 3: 恢复颜色和可见性
    actor->GetProperty()->SetColor(rgb);
    actor->SetVisibility(visible ? 1 : 0);
}

