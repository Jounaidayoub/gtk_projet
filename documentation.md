# GTK UI Builder - Technical Documentation

## 1. Project Overview

The GTK UI Builder is a graphical user interface tool that allows users to design GTK+-based interfaces by adding widgets, configuring their properties, and organizing them in a visual manner. The application generates XML output that can be used to recreate the designed interface.

## 2. System Architecture

### 2.1 Core Components

```
┌─────────────────┐     ┌───────────────────┐     ┌──────────────┐
│ UI Builder Main │────►│ Widget Management │────►│ XML          │
│ Application     │◄────│ & Tree Structures │◄────│ Generation   │
└─────────────────┘     └───────────────────┘     └──────────────┘
        │                        │                       │
        ▼                        ▼                       ▼
┌─────────────────┐     ┌───────────────────┐     ┌──────────────┐
│ Property Panel  │     │ Container         │     │ Demo         │
│ & Editing       │◄───►│ Management        │◄───►│ Execution    │
└─────────────────┘     └───────────────────┘     └──────────────┘
```

1. **Main Application (`demo.c`)** - Entry point that sets up the UI and initializes core structures
2. **Widget Management** - Handles widget creation, organization, and tree representation
3. **Property Panel** - Provides UI for editing widget properties 
4. **Container Management** - Handles container widgets that can hold other widgets
5. **XML Generation** - Converts the widget tree to XML format
6. **Demo Execution** - Runs the generated XML using the renderer

### 2.2 Data Flow

1. User interacts with the UI to create widgets
2. Widgets are stored in two synchronized data structures:
   - `GtkTreeStore` for visual hierarchy representation
   - `Arbre` custom tree for internal representation with properties
3. When editing properties, changes are stored in the Arbre structure
4. When exporting, the Arbre structure is traversed to generate XML
5. The XML can be displayed in the XML tab and saved to a file
6. The demo system uses the saved XML file to render a functioning UI

## 3. Core Data Structures

### 3.1 AppData Structure (`app_data.h`)

The central data structure that holds references to all major components:

```c
typedef struct _AppData {
    GtkWidget *window;                // Main application window
    GtkWidget *preview_area;          // The preview area where widgets are placed
    GtkWidget *hierarchy_view;        // Tree view for widget hierarchy display
    GtkTreeStore *hierarchy_store;    // Data model for hierarchy view
    GtkWidget *properties_panel;      // Right panel for properties
    GtkWidget *properties_content;    // Content area for property editing
    GList *containers;                // List of container widgets
    GtkWidget *selected_container;    // Currently selected container
    GtkWidget *container_combo;       // Container selection dropdown
    GtkWidget *selected_widget;       // Currently selected widget for editing
    GtkWidget *apply_button;          // Apply button for property changes
    GtkWidget *remove_button;         // Delete widget button
    Arbre *widget_tree;               // Custom tree structure for widgets
    
    // Visualization widgets
    GtkWidget *arbre_view;            // Tree view for Arbre structure
    GtkWidget *arbre_scroll;          // Scrolled window for Arbre view
    GtkWidget *arbre_frame;           // Frame containing Arbre view
    
    // XML display
    GtkWidget *xml_text_view;         // TextView to display XML code
} AppData;
```

### 3.2 Arbre Structure (`containers_list.h`)

A custom tree structure that represents the widget hierarchy with properties:

```c
typedef struct arbre {
    char nom[MAX_NAME_LENGTH];             // Widget name
    GtkWidget *widget;                     // GTK widget pointer
    struct arbre *frere;                   // Next sibling
    struct arbre *fils;                    // First child
    int is_container;                      // Whether it can contain other widgets
    
    // Widget properties
    WidgetProperty properties[MAX_PROPS];  // Array of properties
    int prop_count;                        // Number of properties
    
    // Style properties
    WidgetProperty style_props[MAX_PROPS]; // Style properties
    int style_prop_count;                  // Number of style properties
    
    // Widget type info
    char widget_type[MAX_NAME_LENGTH];     // Widget type identifier
} Arbre;
```

### 3.3 WidgetProperty Structure (`containers_list.h`)

Holds name-value pairs for widget properties:

```c
typedef struct {
    char name[MAX_PROP_NAME];   // Property name
    char value[MAX_PROP_VALUE]; // Property value
} WidgetProperty;
```

## 4. Key Modules & Files

### 4.1 Main Application (`demo.c`)

Entry point that:
- Initializes GTK
- Creates the application UI layout
- Sets up panels (left, center, right)
- Creates the widget tree structures
- Sets up signal handlers

### 4.2 Tree Synchronization (`tree_sync.h`)

Maintains two parallel tree structures:
1. `GtkTreeStore` - Used for visual representation in the UI
2. `Arbre` - Custom tree for internal data with properties

Key functions:
- `add_widget_to_both_trees()` - Adds widgets to both structures
- `remove_widget_from_both_trees()` - Removes widgets from both
- `update_arbre_view()` - Updates the visual representation when the tree changes

### 4.3 Widget Properties (`widget_props.h`)

Handles the capturing and storage of widget properties:

- `populate_widget_properties()` - Extracts properties from GTK widgets
- Property accessor functions for different widget types
- Manages widget type-specific properties

### 4.4 XML Generation (`callbacks.h`)

Converts the widget tree to XML format:

- `generate_xml_from_arbre()` - Recursively traverses the tree to generate XML
- `export_to_xml()` - Entry point for XML generation with proper structure
- Handles special formatting for different widget types

### 4.5 Container Management (`containers.h`)

Manages container widgets that can hold other widgets:

- `update_container_combo()` - Updates container selection dropdown
- `on_container_selected()` - Handles container selection
- `create_box_container()` - Creates box container widgets
- Container property handling functions

### 4.6 Dialog System (`dialogs.h`)

Provides dialog boxes for widget creation and configuration:

- `show_properties_dialog()` - General properties dialog
- `show_basic_entry_dialog()` - Dialog for entry widgets
- `show_password_entry_dialog()` - Dialog for password entries
- Widget-specific configuration dialogs

## 5. Widget Tree Management

### 5.1 Tree Structure Operations

The application maintains a custom tree structure (`Arbre`) with these operations:

- `allouer_arbre()` - Allocates a new tree node
- `insererArbre()` - Inserts a node into the tree
- `afficher_arbre()` - Prints the tree structure (debug)
- `getWidgetByNom()` - Finds a widget by name
- `getNomConteneurByWidget()` - Gets container name for a widget
- `creer_arborescence_visuelle()` - Creates visual representation of the tree

### 5.2 Tree Synchronization Mechanism

The application maintains two parallel trees:
1. `GtkTreeStore` for the UI hierarchy view
2. `Arbre` for internal representation with properties

Synchronization happens in these key points:
- Widget addition: `add_widget_to_both_trees()`
- Widget removal: `remove_widget_from_both_trees()`
- Container changes: Update both structures when containers change

## 6. XML Generation Process

### 6.1 XML Generation Algorithm

The XML generation process follows these steps:

1. Start with the window element and add window properties
2. Create a fixed container for the preview area
3. Recursively traverse the Arbre structure
4. For each widget:
   - Output the opening tag with widget type
   - Output all properties
   - Output style properties with appropriate tags
   - For containers, add children wrapped in container tags
   - Process siblings
   - Output closing tag

### 6.2 XML Structure

The generated XML follows this structure:

```xml
<window>
    <property name="title">GTK UI Builder</property>
    <!-- Other window properties -->
    <child>
        <fixed>
            <children>
                <!-- Widgets go here -->
                <label>
                    <property name="text_label">Label Text</property>
                    <!-- Other properties -->
                    <labelstyle name="police">Arial</labelstyle>
                    <!-- Other style properties -->
                </label>
                
                <button>
                    <property name="text">Button Text</property>
                    <!-- Other properties -->
                </button>
                
                <box>
                    <property name="orientation">h</property>
                    <child>
                        <!-- Box children -->
                    </child>
                </box>
            </children>
        </fixed>
    </child>
</window>
```

## 7. Demo Execution

### 7.1 Demo Process

The demo execution process follows these steps:

1. Export the current interface to `demo.html` file
2. Copy this file to `tst.html` (required by xml_generator.c)
3. Execute the demo through the build script or directly
4. Display a dialog to inform the user of the process

### 7.2 Demo Implementation

The demo execution uses:

- `export_to_xml()` to generate the XML
- `run_demo.sh` script for execution
- `system()` calls to launch the renderer
- The `xml_generator.c` program to parse and render the interface

## 8. Integration Points

### 8.1 Widget Creation to Tree Addition

1. User clicks a widget button in the UI
2. A dialog appears for configuration
3. Widget is created with specified properties
4. `add_widget_to_both_trees()` adds it to both tree structures
5. Properties are captured with `populate_widget_properties()`

### 8.2 Property Editing to Tree Update

1. User selects a widget in the hierarchy view
2. Properties panel is populated with widget properties
3. User edits properties in the panel
4. Changes are stored in the Arbre structure's properties arrays
5. Widget is updated in the preview

### 8.3 XML Generation and Export

1. User clicks "Export XML" button
2. `export_to_xml()` is called
3. Function generates XML from the Arbre structure
4. XML is displayed in the XML tab
5. XML is saved to `demo.html` file

### 8.4 Demo Execution Flow

1. User clicks "Run Demo" button
2. `run_demo()` function is called
3. XML is exported to `demo.html`
4. File is copied to `tst.html`
5. Demo renderer is executed via script or direct command
6. Resulting UI is displayed

## 9. Error Handling

The application implements several error handling mechanisms:

- Memory allocation checks in tree operations
- File operation error checking during XML export
- Container validity checks when adding widgets
- Dialog responses handling for user cancellations

## 10. Future Extensibility

The modular structure allows for:

1. Adding new widget types by extending the widget creation system
2. Adding property types by extending the property storage mechanism
3. Supporting additional XML formats by modifying the generation algorithm
4. Adding new container types by extending the container management system

This architecture ensures that the application can evolve as GTK or requirements change.
