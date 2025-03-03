# GTK UI Builder - Technical Documentation

## 1. Project Overview

GTK UI Builder is a visual interface builder for GTK applications. It allows users to design user interfaces by dragging and dropping widgets, arranging them in containers, and setting their properties. The application generates XML output that can be used in GTK applications.

### Key Features

- Visual design of GTK interfaces
- Widget property customization through a property panel
- Container management (Box, Frame, Grid, etc.)
- Hierarchical view of the UI structure
- Menu bar creation and configuration
- XML export for use in GTK applications

## 2. Architecture and Design

The application follows a modular architecture with clear separation of concerns:

- **Core UI**: The main interface with panels for widget selection, hierarchy view, and properties
- **Widget Management**: Creation, configuration and property editing of widgets
- **Container Management**: Special handling for container widgets (Box, Frame, etc.)
- **Hierarchy View**: Tree representation of the UI structure
- **Property Panel**: Dynamic property editing for selected widgets
- **Menu System**: Creation and management of menu bars and menu items
- **XML Export**: Generation of XML representation of the designed UI

### Key Components

- `AppData` structure: Central data structure that holds application state and UI references
- Widget creation and property functions for different widget types
- Container management functions
- Property panel management code
- Menu bar creation dialogs and functions

## 3. Code Structure

The project is organized into multiple header files, each focused on specific functionality:

- `global.h`: Common types, macros, and utility functions
- `app_data.h`: Main data structure definitions
- `entry.h`: Entry widget creation and management
- `property_panel.h`: Widget property editing functionality
- `hierarchy.h`: UI hierarchy view management
- `containers.h`: Container widget management
- `menu.h`: Menu bar and menu item structures and functions
- `menu_dialog.h`: Dialogs for menu creation
- `dialogs.h`: Widget creation dialogs
- `callbacks.h`: Event callbacks for the application
- `entry_editing.h`: Special editing functionality for entry widgets
- `demo.c`: Main application code

## 4. How to Add New Widgets

To add new widget types, follow these steps:

### 1. Create a Header File for Your Widget

Create a header file (e.g., `button.h`) with:

```c
// button.h
#ifndef BUTTON_H
#define BUTTON_H

#include "global.h"
#include "app_data.h"

// Define widget structure
typedef struct {
    GtkWidget *button;
    dimension *dim;
    gchar *label_text;
    GtkWidget *container;
    coordonnees *cord;
} button_type;

// Function to free widget resources
void free_button(button_type *button);

// Function to initialize widget
button_type *Init_Button(
    dimension *dim,
    const gchar *label_text,
    GtkWidget *container,
    coordonnees *cord
);

// Function to create widget
GtkWidget *creer_button(button_type *button);

// Function to create widget with property editing support
GtkWidget *creer_button_with_editing(button_type *button, AppData *app_data);

// Function to handle XML export/import
void button_xml(FILE *file, int parent);

#endif // BUTTON_H
```

### 2. Implement Your Widget Functions

Create a C file (or implement in the header) with the functions:

```c
// Implementation example
button_type *Init_Button(dimension *dim, const gchar *label_text, 
                         GtkWidget *container, coordonnees *cord) {
    button_type *button = g_malloc(sizeof(button_type));
    button->button = gtk_button_new_with_label(label_text);
    button->dim = dim;
    button->label_text = g_strdup(label_text);
    button->container = container;
    button->cord = cord;
    return button;
}

GtkWidget *creer_button(button_type *button) {
    gtk_widget_set_size_request(button->button, 
                                button->dim->width, 
                                button->dim->height);
                                
    if (GTK_IS_FIXED(button->container)) {
        gtk_fixed_put(GTK_FIXED(button->container), 
                      button->button, 
                      button->cord->x, 
                      button->cord->y);
    }
    gtk_container_add(GTK_CONTAINER(button->container), button->button);
    return button->button;
}
```

### 3. Create a Dialog Module for Your Widget

Add a function to `dialogs.h` to show a dialog for creating your widget:

```c
// Add to dialogs.h
static void show_button_dialog(AppData *app_data) {
    // Create dialog with form fields for button properties
    // Similar to show_basic_entry_dialog but with button-specific fields
    
    // When dialog is accepted:
    if (response == GTK_RESPONSE_ACCEPT) {
        button_type *button = Init_Button(...);
        GtkWidget *button_widget = creer_button_with_editing(button, app_data);
        // Add to hierarchy
    }
}

// Button click handler
static void add_button_clicked(GtkWidget *widget, gpointer data) {
    AppData *app_data = (AppData *)data;
    show_button_dialog(app_data);
}
```

### 4. Add Property Editing Support

Create widget-specific functions in `property_panel.h`:

```c
// Add to property_panel.h
static void create_button_form(AppData *app_data, GtkWidget *widget) {
    // Create form fields specific to button properties
}

static void apply_button_changes(GtkWidget *widget) {
    // Apply property changes to the button
}

// Update create_property_form_for_widget to handle your widget type:
if (GTK_IS_BUTTON(widget)) {
    create_button_form(app_data, widget);
}
```

### 5. Update the Main Application

Add your widget to the main interface in `demo.c`:

```c
// Add button to the Widgets section
GtkWidget *button_btn = gtk_button_new_with_label("Add Button");
g_signal_connect(button_btn, "clicked", G_CALLBACK(add_button_clicked), &app_data);
gtk_box_pack_start(GTK_BOX(widgets_box), button_btn, FALSE, FALSE, 2);
```

### 6. Add Property Editing Registration

In your `button_editing.h` file:

```c
// button_editing.h
GtkWidget *creer_button_with_editing(button_type *button, AppData *app_data) {
    GtkWidget *button_widget = creer_button(button);
    register_widget_for_property_editing(button_widget, app_data);
    return button_widget;
}
```

## 5. Managing Containers

Containers require special handling because they can contain other widgets and affect the layout of the UI.

### Adding a New Container Type

1. Update `containers.h` with a function for your container (similar to `create_box_container`).
2. Create a dialog for configuring the container's properties.
3. Add it to the container list in `AppData`.
4. Update the container selection dropdown.
5. Add it to the hierarchy view.

### Example: Adding a Grid Container

```c
// Add to containers.h
static void create_grid_container(GtkWidget *widget, gpointer data) {
    AppData *app_data = (AppData *)data;
    // Show dialog for grid configuration
    // Create grid with properties from dialog
    GtkWidget *grid = gtk_grid_new();
    // Set grid properties
    // Add to preview area
    // Add to containers list
    app_data->containers = g_list_append(app_data->containers, grid);
    // Add to hierarchy
    // Update container selection dropdown
    update_container_combo(app_data);
    // Show the grid
}
```

## 6. Property Panel

The property panel provides a UI for editing the properties of selected widgets:

### How the Property Panel Works

1. When a widget is selected, `on_widget_button_press_select` is called.
2. The selected widget is stored in `app_data->selected_widget`.
3. `create_property_form_for_widget` creates a form based on widget type.
4. When Apply is clicked, `on_apply_clicked` applies the changes to the widget.
5. Property editing is enabled via `register_widget_for_property_editing`.

### Adding Property Editing Support for a New Widget

1. Create a form function specific to your widget (e.g., `create_button_form`).
2. Create an apply function (e.g., `apply_button_changes`).
3. Update the switch statement in `create_property_form_for_widget`.
4. Update the switch statement in `on_apply_clicked`.

## 7. Hierarchy View

The hierarchy view shows the structure of the UI as a tree:

### How the Hierarchy View Works

1. Each widget is added to `app_data->hierarchy_store` with a label and pointer.
2. Widgets are added as children of their containers when appropriate.
3. When a widget is removed, it's also removed from the hierarchy.

### Adding a Widget to the Hierarchy

```c
static void add_to_hierarchy(AppData *app_data, const gchar *widget_type, GtkWidget *widget) {
    GtkTreeIter iter;
    gtk_tree_store_append(app_data->hierarchy_store, &iter, NULL);
    gtk_tree_store_set(app_data->hierarchy_store, &iter, 0, widget_type, 1, widget, -1);
    gtk_tree_view_expand_all(GTK_TREE_VIEW(app_data->hierarchy_view));
}

// For children of containers, find the parent iter first, then append to it
```

## 8. Menu Bar Integration

The application includes support for creating menu bars:

### How Menu Creation Works

1. `show_menu_bar_dialog` shows a dialog for creating menus.
2. Users can add menu items to menus and multiple menus to the menu bar.
3. The menu structure is built using `menu.h` functions.
4. The menu bar is added to the preview area and hierarchy view.

### Extending Menu Support

1. To add new menu features, modify `menu_dialog.h`.
2. To add support for menu item icons or accelerators, update `MenuItem` structure.
3. To handle menu item activation, implement callbacks in your application.

## 9. Build and Run Instructions

### Prerequisites

- GTK 3 development libraries
- GCC or compatible C compiler

### Building the Project

```bash
# Compile the project
gcc -o gtk_ui_builder demo.c `pkg-config --cflags --libs gtk+-3.0`

# Run the application
./gtk_ui_builder
```

### Using the Generated XML

1. Export your UI to XML using the Export button.
2. Use the XML in your application by loading it with GTK's XML parsing functions or your own parser.

## 10. Best Practices and Tips

### Consistent Widget Definition Pattern

1. Define a widget structure (e.g., `button_type`).
2. Create initialization and creation functions.
3. Add property editing support.
4. Implement XML serialization/deserialization.

### Widget Integration Checklist

- [ ] Widget structure definition
- [ ] Initialization function
- [ ] Creation function
- [ ] Dialog for configuration
- [ ] Property panel integration
- [ ] Hierarchy view integration
- [ ] XML export/import support

### Common Issues

- **Widget Not Showing**: Check container adding and visibility settings.
- **Property Not Saving**: Ensure proper connection between property fields and apply function.
- **Hierarchy Not Updating**: Check hierarchy code in your widget creation function.

### Performance Considerations

- When adding many widgets, use batch operations where possible.
- Use g_signal_handler_block/unblock to prevent signal recursion during bulk updates.

## 11. Extending the Project

Ideas for future extensions:

1. **CSS Styling**: Add support for styling widgets with CSS.
2. **Layout Templates**: Save and load common layout templates.
3. **Code Generation**: Generate C code instead of just XML.
4. **Drag and Drop**: Add drag-and-drop functionality for rearranging widgets.
5. **Undo/Redo**: Implement an undo/redo system.

## 12. Troubleshooting

### Widget Creation Issues
- Check initialization parameters
- Verify container handling
- Ensure proper memory allocation

### Property Panel Issues
- Ensure property update functions are correctly implemented
- Check signal connections
- Verify property retrieval from form fields

### Build Issues
- Check include order to avoid circular dependencies
- Verify GTK version compatibility
- Check for missing function implementations

---

For further assistance, contact project maintainers or refer to GTK documentation.
