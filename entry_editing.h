#ifndef ENTRY_EDITING_H
#define ENTRY_EDITING_H

#include "app_data.h"  // Include app_data.h first
#include "entry.h"
#include "property_panel.h"
#include "containers_list.h"

// Helper function to find the Arbre node by widget pointer
Arbre* find_arbre_node_by_widget(Arbre* root, GtkWidget* widget) {
    if (!root || !widget) return NULL;
    
    // Check if this is the node we're looking for
    if (root->widget == widget) return root;
    
    // Check children
    Arbre* found = NULL;
    if (root->fils) {
        found = find_arbre_node_by_widget(root->fils, widget);
        if (found) return found;
    }
    
    // Check siblings
    if (root->frere) {
        found = find_arbre_node_by_widget(root->frere, widget);
        if (found) return found;
    }
    
    return NULL;
}

// Implementation of functions that were forward-declared in entry.h
GtkWidget *creer_entry_basic_with_editing(entry_type_basic *entry_basic, struct _AppData *app_data) {
    g_print("Creating basic entry with editing\n");
    
    // Create the widget
    GtkWidget *entry_widget = creer_entry_basic(entry_basic);
    
    // Register for property editing
    g_print("Registering entry widget %p for property editing\n", entry_widget);
    register_widget_for_property_editing(entry_widget, app_data);
    
    // Add to both tree structures with the structure reference
    add_widget_to_both_trees(app_data, entry_widget, "Basic Entry", 
                           entry_basic->container, 0, entry_basic);
    
    return entry_widget;
}

GtkWidget *creer_entry_pass_with_editing(entry_type_password *entry_password, struct _AppData *app_data) {
    g_print("Creating password entry with editing\n");
    
    // Create the widget
    GtkWidget *entry_widget = creer_entry_pass(entry_password);
    
    // Register for property editing
    g_print("Registering password widget %p for property editing\n", entry_widget);
    register_widget_for_property_editing(entry_widget, app_data);
    
    // Add to both tree structures with the structure reference
    add_widget_to_both_trees(app_data, entry_widget, "Password Entry", 
                           entry_password->container, 0, entry_password);
    
    return entry_widget;
}

// Helper function to get widget structure from Arbre node
void* get_widget_structure(AppData *app_data, GtkWidget *widget) {
    Arbre *node = find_arbre_node_by_widget(app_data->widget_tree, widget);
    if (node) {
        return node->widget_data;
    }
    return NULL;
}

static gboolean on_debug_click(GtkWidget *widget, GdkEventButton *event, gpointer user_data) {
    g_print("DEBUG: Image event box clicked at (%f,%f)\n", event->x, event->y);
    return FALSE; // Continue propagation
}

GtkWidget *creer_image_with_editing(MonImage *img, struct _AppData *app_data) {
    g_print("Creating image with editing\n");
    
    // Create the image widget
    GtkWidget *image_widget = gtk_image_new_from_file(img->path);
    if (!image_widget) {
        g_print("Error: Failed to create image widget from file %s\n", img->path);
        return NULL;
    }

    // Resize the image
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(img->path, NULL);
    if (pixbuf) {
        GdkPixbuf *scaled = gdk_pixbuf_scale_simple(pixbuf, img->dim.width, img->dim.height, 
                                                  GDK_INTERP_BILINEAR);
        gtk_image_set_from_pixbuf(GTK_IMAGE(image_widget), scaled);
        g_object_unref(pixbuf);
        g_object_unref(scaled);
    }
    
    // Create an event box to capture events
    GtkWidget *event_box = gtk_event_box_new();
    gtk_container_add(GTK_CONTAINER(event_box), image_widget);
    
    // CRITICAL FIX 1: Set events BEFORE connecting signals
    gtk_widget_add_events(event_box, GDK_BUTTON_PRESS_MASK | 
                                    GDK_BUTTON_RELEASE_MASK | 
                                    GDK_POINTER_MOTION_MASK);
    
    // CRITICAL FIX 2: Make event box visible window to catch events
    gtk_event_box_set_visible_window(GTK_EVENT_BOX(event_box), TRUE);
    gtk_event_box_set_above_child(GTK_EVENT_BOX(event_box), TRUE);
    
    // Store the image path for property panel
    g_object_set_data_full(G_OBJECT(event_box), "image_file_path", 
                          g_strdup(img->path), g_free);
    
    // Store reference to image widget
    g_object_set_data(G_OBJECT(event_box), "image_widget", image_widget);
    
    // Set size for the event box
    gtk_widget_set_size_request(event_box, img->dim.width, img->dim.height);
    
    // CRITICAL FIX 3: Add a direct debug handler to confirm events
    g_signal_connect(event_box, "button-press-event", G_CALLBACK(on_debug_click), NULL);
    
    // Register for property editing
    g_print("Registering event box with image for property editing\n");
    register_widget_for_property_editing(event_box, app_data);
    
    return event_box;
}

// Add this debug handler function


// GtkWidget *creer_image_with_editing(MonImage *img, struct _AppData *app_data) {
//     g_print("Creating image with editing\n");
    
//     // Create the image widget
//     GtkWidget *image_widget = gtk_image_new_from_file(img->path);
//     if (!image_widget) {
//         g_print("Error: Failed to create image widget from file %s\n", img->path);
//         return NULL;
//     }

//     // Resize the image
//     GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(img->path, NULL);
//     if (pixbuf) {
//         GdkPixbuf *scaled = gdk_pixbuf_scale_simple(pixbuf, img->dim.width, img->dim.height, 
//                                                   GDK_INTERP_BILINEAR);
//         gtk_image_set_from_pixbuf(GTK_IMAGE(image_widget), scaled);
//         g_object_unref(pixbuf);
//         g_object_unref(scaled);
//     }
    
//     // Create an event box to capture events
//     GtkWidget *event_box = gtk_event_box_new();
//     gtk_container_add(GTK_CONTAINER(event_box), image_widget);
    
//     // CRITICAL: Enable button press events explicitly
//     gtk_widget_set_events(event_box, GDK_BUTTON_PRESS_MASK);
    
//     // Make event box visible and reactive to mouse events
//     gtk_event_box_set_visible_window(GTK_EVENT_BOX(event_box), FALSE);
//     gtk_event_box_set_above_child(GTK_EVENT_BOX(event_box), TRUE);
    
//     // Store the image path for property panel
//     g_object_set_data_full(G_OBJECT(event_box), "image_file_path", 
//                           g_strdup(img->path), g_free);
    
//     // Store reference to image widget
//     g_object_set_data(G_OBJECT(event_box), "image_widget", image_widget);
    
//     // Set size for the event box
//     gtk_widget_set_size_request(event_box, img->dim.width, img->dim.height);
    
//     // Register the EVENT BOX (not the raw image) for property editing
//     g_print("Registering event box with image for property editing\n");
//     register_widget_for_property_editing(event_box, app_data);
    
//     // Add to both tree structures - use the EVENT BOX as the widget
//     add_widget_to_both_trees(app_data, event_box, "Image", 
//                            app_data->selected_container, FALSE, img);
    
//     return event_box;
// }

// GtkWidget *creer_image_with_editing(MonImage *img, struct _AppData *app_data) {
//     g_print("Creating image with editing\n");
    
//     // Create the image widget
//     GtkWidget *image_widget = gtk_image_new_from_file(img->path);
//     if (!image_widget) {
//         g_print("Error: Failed to create image widget from file %s\n", img->path);
//         return NULL;
//     }

//     // Resize the image
//     GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(img->path, NULL);
//     if (pixbuf) {
//         GdkPixbuf *scaled = gdk_pixbuf_scale_simple(pixbuf, img->dim.width, img->dim.height, 
//                                                   GDK_INTERP_BILINEAR);
//         gtk_image_set_from_pixbuf(GTK_IMAGE(image_widget), scaled);
//         g_object_unref(pixbuf);
//         g_object_unref(scaled);
//     }
    
//     // Create an event box to capture events
//     GtkWidget *event_box = gtk_event_box_new();
//     gtk_container_add(GTK_CONTAINER(event_box), image_widget);
    
//     // Store the image path for property panel
//     g_object_set_data_full(G_OBJECT(event_box), "image_file_path", 
//                           g_strdup(img->path), g_free);
    
//     // Store reference to image widget
//     g_object_set_data(G_OBJECT(event_box), "image_widget", image_widget);
    
//     // Set size for the event box
//     gtk_widget_set_size_request(event_box, img->dim.width, img->dim.height);
    
//     // Register the EVENT BOX (not the raw image) for property editing
//     g_print("Registering event box with image for property editing\n");
//     register_widget_for_property_editing(event_box, app_data);
    
//     // Add to both tree structures - use the EVENT BOX as the widget
//     add_widget_to_both_trees(app_data, event_box, "Image", 
//                            app_data->selected_container, FALSE, img);
    
//     return event_box;
// }

// GtkWidget *creer_image_with_editing(MonImage *img, struct _AppData *app_data) {
//     g_print("Creating image with editing\n");

//     // Create the image widget
//     GtkWidget *image_widget = gtk_image_new_from_file(img->path);
//     if (!image_widget) {
//         g_print("Error: Failed to create image widget from file %s\n", img->path);
//         return NULL;
//     }

//     // Resize the image
//     image_widget = redimensionner_image(image_widget, img->dim.width, img->dim.height);
//     if (!image_widget) {
//         g_print("Error: Failed to resize image\n");
//         return NULL;
//     }

//     // Register for property editing
//     g_print("Registering image widget %p for property editing\n", image_widget);
//     register_widget_for_property_editing(image_widget, app_data);

//     // Add to both tree structures
//     add_widget_to_both_trees(app_data, image_widget, "Image", app_data->selected_container, 0, img);

//     return image_widget;
// }


#endif /* ENTRY_EDITING_H */
