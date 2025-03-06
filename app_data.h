#ifndef APP_DATA_H
#define APP_DATA_H

#include <gtk/gtk.h>
#include "global.h"
#include "containers_list.h"  // Add this include

// Define the AppData struct with the same name as forward declared elsewhere
typedef struct _AppData {
    GtkWidget *window;
    GtkWidget *preview_area;     // The preview area where widgets are placed
    GtkWidget *hierarchy_view;   // Tree view for widget hierarchy
    GtkTreeStore *hierarchy_store; // Tree store for hierarchy
    GtkWidget *properties_panel;  // Right panel for properties
    GtkWidget *properties_content; // Content area for properties (to be dynamically replaced)
    GList *containers;           // List of available containers
    GtkWidget *selected_container; // Currently selected container (NULL = preview area)
    GtkWidget *container_combo;  // Combo box for container selection
    GtkWidget *selected_widget;  // Currently selected widget for property editing
    GtkWidget *apply_button;     // Apply button for property changes
    GtkWidget *remove_button;    // Remove button for deleting widgets
    Arbre *widget_tree;          // Root of the custom widget tree
    
    // New fields for the Arbre visualization
    GtkWidget *arbre_view;       // Tree view for Arbre visualization
    GtkWidget *arbre_scroll;     // Scrolled window containing the Arbre view
    GtkWidget *arbre_frame;      // Frame containing the Arbre scroll
    
    // New field for XML display
    GtkWidget *xml_text_view;    // TextView to display generated XML
} AppData;

// Function declaration to resolve circular dependency
void update_container_combo(AppData *app_data);

#endif /* APP_DATA_H */
