#ifndef HIERARCHY_H
#define HIERARCHY_H

#include <gtk/gtk.h>
#include "app_data.h"

// Function to add an item to the hierarchy tree
static void add_to_hierarchy(AppData *app_data, const gchar *widget_type, GtkWidget *widget) {
    GtkTreeIter iter;
    gtk_tree_store_append(app_data->hierarchy_store, &iter, NULL);
    gtk_tree_store_set(app_data->hierarchy_store, &iter, 0, widget_type, 1, widget, -1);
    
    // Expand all rows to show the newly added item
    gtk_tree_view_expand_all(GTK_TREE_VIEW(app_data->hierarchy_view));
}

#endif /* HIERARCHY_H */
