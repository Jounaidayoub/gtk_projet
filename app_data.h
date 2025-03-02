#ifndef APP_DATA_H
#define APP_DATA_H

#include <gtk/gtk.h>
#include "global.h"

// Enhanced application data structure with container tracking
typedef struct {
    GtkWidget *window;
    GtkWidget *preview_area;     // The preview area where widgets are placed
    GtkWidget *hierarchy_view;   // Tree view for widget hierarchy
    GtkTreeStore *hierarchy_store; // Tree store for hierarchy
    GtkWidget *properties_panel;  // Right panel for properties
    GList *containers;           // List of available containers
    GtkWidget *selected_container; // Currently selected container (NULL = preview area)
    GtkWidget *container_combo;  // Combo box for container selection
} AppData;

#endif /* APP_DATA_H */
