/**     @file ModelPart.h
  *
  *     EEEE2076 - Software Engineering & VR Project
  *
  *     Template for model parts that will be added as treeview items
  *
  *     P Evans 2022
  *
  *     @brief Defines the ModelPart class used to represent nodes in a hierarchical tree structure for 3D model components.
  *
  *     Each ModelPart contains properties such as visibility, name, VTK rendering data (mapper, actor, STL reader),
  *     and supports operations including STL loading, filter application (shrink, clip), color/lighting control, and hierarchy management.
  *
  *     This class forms the core data structure for displaying and manipulating 3D models within a Qt TreeView and VTK-rendered scene.
  *     Edited Zhixing REN 2025
  */
#ifndef VIEWER_MODELPART_H
#define VIEWER_MODELPART_H

#include <QString>
#include <QList>
#include <QVariant>

/* VTK headers - will be needed when VTK used in next worksheet,
 * commented out for now
 *
 * Note that there are a few function definitions and variables
 * commented out below - this is because you haven't yet installed
 * the VTK library which is needed.
 */
#include <vtkSmartPointer.h>
#include <vtkMapper.h>
#include <vtkActor.h>
#include <vtkSTLReader.h>
#include <vtkColor.h>

/**
 * @class ModelPart
 * @brief Represents an individual part of a 3D model used in visualization and interaction.
 *
 * Each ModelPart instance stores metadata and rendering parameters,
 * which can be managed and visualized via the UI and VTK.
 */
class ModelPart {
public:
    /** Constructor
     * @param data is a List (array) of strings for each property of this item (part name and visiblity in our case
     * @param parent is the parent of this item (one level up in tree)
     */
    ModelPart(const QList<QVariant>& data, ModelPart* parent = nullptr);

    /** Destructor
      * Needs to free array of child items
      */
    ~ModelPart();

    /** Add a child to this item.
      * @param item Pointer to child object (must already be allocated using new)
      */
    void appendChild(ModelPart* item);

    /** Return child at position 'row' below this item
      * @param row is the row number (below this item)
      * @return pointer to the item requested.
      */
    ModelPart* child(int row);

    /** Return number of children to this item
      * @return number of children
      */
    int childCount() const;         /* Note on the 'const' keyword - it means that this function is valid for
                                     * constant instances of this class. If a class is declared 'const' then it
                                     * cannot be modifed, this means that 'set' type functions are usually not
                                     * valid, but 'get' type functions are.
                                     */

    /** Get number of data items (2 - part name and visibility string) in this case.
      * @return number of visible data columns
      */
    int columnCount() const;

    /** Return the data item at a particular column for this item.
      * i.e. either part name of visibility
      * used by Qt when displaying tree
      * @param column is column index
      * @return the QVariant (represents string)
      */
    QVariant data(int column) const;


    /** Default function required by Qt to allow setting of part
      * properties within treeview.
      * @param column is the index of the property to set
      * @param value is the value to apply
      */
    void set( int column, const QVariant& value );

    /** Get pointer to parent item
      * @return pointer to parent item
      */
    ModelPart* parentItem();

    /** Get row index of item, relative to parent item
      * @return row index
      */
    int row() const;


    /** Set colour
      * (0-255 RGB values as ints)
      */
    void setColour(const unsigned char R, const unsigned char G, const unsigned char B);

    unsigned char getColourR();
    unsigned char getColourG();
    unsigned char getColourB();

    /** Set visible flag
      * @param isVisible sets visible/non-visible
      */
    void setVisible(bool isVisible);

    /** Get visible flag
      * @return visible flag as boolean 
      */
    bool visible();
	
	/** Load STL file
      * @param fileName
      */
    void loadSTL(QString fileName);

    /** Return actor
      * @return pointer to default actor for GUI rendering
      */
    vtkSmartPointer<vtkActor> getActor();

    /** Return new actor for use in VR
      * @return pointer to new actor
      */
    vtkActor* getNewActor();

    /**
     * file path for saving and opening file
     */
    QString                                     filePath;

    /**
     * @brief removeChild
     * @param child
     */
    void removeChild(ModelPart* child);//remove part

    //filters
/**
 * @brief Applies a shrink filter to this model part's geometry.
 *
 * Uses VTK's vtkShrinkPolyData to visually reduce the size of the mesh while maintaining its topology.
 */
    void applyShrinkFilter();

/**
 * @brief Applies a clipping filter to this model part's geometry.
 *
 * Clips the geometry using a VTK plane, removing a portion of the model along a defined axis.
 */
    void applyClipFilter();
/**
 * @brief Resets the model part's geometry to its original STL state.
 *
 * Reloads the STL file and restores the original mesh, while preserving visual attributes like color and visibility.
 */
    void resetGeometry();
/**
 * @brief Enables or disables the shrink filter state flag.
 *
 * @param enabled True to enable the shrink filter; false to disable.
 */
    void setShrinkEnabled(bool enabled) { shrinkEnabled = enabled; }
/**
 * @brief Checks if the shrink filter is currently enabled.
 *
 * @return True if shrink filter is active; otherwise, false.
 */
    bool isShrinkEnabled() const { return shrinkEnabled; }
/**
 * @brief Enables or disables the clip filter state flag.
 *
 * @param enabled True to enable the clip filter; false to disable.
 */
    void setClipEnabled(bool enabled) { clipEnabled = enabled; }
/**
 * @brief Checks if the clip filter is currently enabled.
 *
 * @return True if clip filter is active; otherwise, false.
 */
    bool isClipEnabled() const { return clipEnabled; }


/**
 * @brief Sets the lighting intensity value associated with this model part.
 *
 * @param value A double in the range [0.0, 1.0] representing lighting strength.
 */
    void setLightValue(double value) { lightValue = value; }
/**
 * @brief Retrieves the current lighting intensity value for this model part.
 *
 * @return A double representing the light scale (default is 1.0).
 */
    double getLightValue() const { return lightValue; }


private:
    QList<ModelPart*>                           m_childItems;       /**< List (array) of child items */
    QList<QVariant>                             m_itemData;         /**< List (array of column data for item */
    ModelPart*                                  m_parentItem;       /**< Pointer to parent */

    /* These are some typical properties that I think the part will need, you might
     * want to add you own.
     */
    bool                                        isVisible;          /**< True/false to indicate if should be visible in model rendering */
	
	/* These are vtk properties that will be used to load/render a model of this part,
	 * commented out for now but will be used later
	 */
    vtkSmartPointer<vtkSTLReader>               file;               /**< Datafile from which part loaded */
    vtkSmartPointer<vtkMapper>                  mapper;             /**< Mapper for rendering */
    vtkSmartPointer<vtkActor>                   actor;              /**< Actor for rendering */
    vtkColor3<unsigned char>                    colour;             /**< User defineable colour */


    // Filtering and lighting state
    bool shrinkEnabled = false;  ///< Whether shrink filter is active
    bool clipEnabled = false;    ///< Whether clip filter is active
    double lightValue = 1.0;     ///< Lighting scale (1.0 = default)





};  


#endif

