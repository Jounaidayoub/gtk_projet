#ifndef CONTAINER_UTILS_H
#define CONTAINER_UTILS_H

#include <gtk/gtk.h>

// Helper function to add a widget to various container types
gboolean add_widget_to_container_by_type(GtkWidget *widget, GtkWidget *container, 
                                        gint x, gint y, gint width, gint height) {
    if (!container) {
        return FALSE;
    }
    
    // Set the widget size
    if (width > 0 && height > 0) {
        gtk_widget_set_size_request(widget, width, height);
    }
    
    if (GTK_IS_FIXED(container)) {
        // Fixed container - position at exact coordinates
        gtk_fixed_put(GTK_FIXED(container), widget, x, y);
        return TRUE;
    }
    else if (GTK_IS_BOX(container)) {
        // Box container - add as a packed widget
        gtk_box_pack_start(GTK_BOX(container), widget, FALSE, FALSE, 5);
        return TRUE;
    }
    else if (GTK_IS_GRID(container)) {
        // Grid container - default to first available cell
        // In a real app, you'd want to specify row/column
        gtk_grid_attach(GTK_GRID(container), widget, 0, 0, 1, 1);
        return TRUE;
    }
    else if (GTK_IS_CONTAINER(container)) {
        // Generic container fallback
        gtk_container_add(GTK_CONTAINER(container), widget);
        return TRUE;
    }
    
    return FALSE;
}

// Helper to wrap a widget in a frame with a label
GtkWidget* wrap_in_frame(GtkWidget *widget, const gchar *frame_label) {
    GtkWidget *frame = gtk_frame_new(frame_label);
    gtk_container_add(GTK_CONTAINER(frame), widget);
    return frame;
}

#endif /* CONTAINER_UTILS_H */
