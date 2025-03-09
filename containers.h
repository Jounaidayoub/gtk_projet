#ifndef CONTAINERS_H
#define CONTAINERS_H

#include <gtk/gtk.h>
#include "app_data.h"
#include "hierarchy.h"
#include "tree_sync.h"
#include "container_utils.h"

// Helper function to update container selection combo box
void update_container_combo(AppData *app_data) {
    GtkComboBoxText *combo = GTK_COMBO_BOX_TEXT(app_data->container_combo);
    
    // Clear existing items
    gtk_combo_box_text_remove_all(combo);
    
    // Add default option (preview area)
    gtk_combo_box_text_append_text(combo, "Window (Default)");
    
    // Add all containers
    GList *iter;
    for (iter = app_data->containers; iter != NULL; iter = iter->next) {
        GtkWidget *container = GTK_WIDGET(iter->data);
        const gchar *name = gtk_widget_get_name(container);
        gtk_combo_box_text_append_text(combo, name);
    }
    
    // Select default option
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 0);
}

// Callback for container selection
static void on_container_selected(GtkComboBox *combo, gpointer user_data) {
    AppData *app_data = (AppData *)user_data;
    gint active = gtk_combo_box_get_active(combo);
    
    if (active == 0) {
        // Default option (preview area)
        app_data->selected_container = NULL;
        g_print("Selected container: Preview Area\n");
    } else {
        // Get the selected container
        app_data->selected_container = g_list_nth_data(app_data->containers, active - 1);
        
        if (app_data->selected_container) {
            // Print info about the selected container for debugging
            const gchar *name = gtk_widget_get_name(app_data->selected_container);
            g_print("Selected container: %s (%p)\n", name ? name : "unnamed", app_data->selected_container);
            
            // Print container type for debugging
            if (GTK_IS_BOX(app_data->selected_container)) {
                g_print("Container type: Box\n");
            } else if (GTK_IS_GRID(app_data->selected_container)) {
                g_print("Container type: Grid\n");
            } else if (GTK_IS_FIXED(app_data->selected_container)) {
                g_print("Container type: Fixed\n");
            } else {
                g_print("Container type: Other\n");
            }
        }
    }
}

// Container creation callback for Box
static void create_box_container(GtkWidget *widget, gpointer data) {
    AppData *app_data = (AppData *)data;
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *grid;
    GtkWidget *orientation_label, *spacing_label, *name_label;
    GtkWidget *name_entry, *spacing_entry;
    GtkWidget *orientation_combo;
    GtkWidget *x_label, *y_label, *width_label, *height_label;
    GtkWidget *x_entry, *y_entry, *width_entry, *height_entry;
    gint response;
    
    // Create dialog
    dialog = gtk_dialog_new_with_buttons("Create Box Container",
                                        GTK_WINDOW(app_data->window),
                                        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                        "OK", GTK_RESPONSE_ACCEPT,
                                        "Cancel", GTK_RESPONSE_CANCEL,
                                        NULL);
    
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    
    // Create grid for form layout
    grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
    
    // Add container selection at the top of the dialog
    GtkWidget *parent_container_label = gtk_label_new("Add to Container:");
    GtkWidget *parent_container_combo = gtk_combo_box_text_new();
    
    // Add default option (preview area)
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(parent_container_combo), "Window (Default)");
    
    // Add all containers
    GList *iter;
    for (iter = app_data->containers; iter != NULL; iter = iter->next) {
        GtkWidget *container = GTK_WIDGET(iter->data);
        const gchar *name = gtk_widget_get_name(container);
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(parent_container_combo), name);
    }
    
    // Select currently selected container
    gint active_index = 0;
    if (app_data->selected_container) {
        active_index = g_list_index(app_data->containers, app_data->selected_container) + 1;
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(parent_container_combo), active_index);
    
    // Add container selection to the top of the form
    gtk_grid_attach(GTK_GRID(grid), parent_container_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), parent_container_combo, 1, 0, 1, 1);
    
    // Box-specific fields
    name_label = gtk_label_new("Container Name:");
    name_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(name_entry), "MyBox");
    
    gtk_grid_attach(GTK_GRID(grid), name_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), name_entry, 1, 1, 1, 1);
    
    // Orientation field
    orientation_label = gtk_label_new("Orientation:");
    orientation_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(orientation_combo), "Horizontal");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(orientation_combo), "Vertical");
    gtk_combo_box_set_active(GTK_COMBO_BOX(orientation_combo), 0);
    
    gtk_grid_attach(GTK_GRID(grid), orientation_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), orientation_combo, 1, 2, 1, 1);
    
    // Spacing field
    spacing_label = gtk_label_new("Spacing:");
    spacing_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(spacing_entry), "5");
    
    gtk_grid_attach(GTK_GRID(grid), spacing_label, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), spacing_entry, 1, 3, 1, 1);
    
    // Position fields
    x_label = gtk_label_new("X Position:");
    y_label = gtk_label_new("Y Position:");
    x_entry = gtk_entry_new();
    y_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(x_entry), "10");
    gtk_entry_set_text(GTK_ENTRY(y_entry), "10");
    
    gtk_grid_attach(GTK_GRID(grid), x_label, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_entry, 1, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_label, 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_entry, 1, 5, 1, 1);
    
    // Size fields
    width_label = gtk_label_new("Width:");
    height_label = gtk_label_new("Height:");
    width_entry = gtk_entry_new();
    height_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(width_entry), "300");
    gtk_entry_set_text(GTK_ENTRY(height_entry), "200");
    
    gtk_grid_attach(GTK_GRID(grid), width_label, 0, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), width_entry, 1, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), height_label, 0, 7, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), height_entry, 1, 7, 1, 1);
    
    // Add grid to dialog
    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_widget_show_all(dialog);
    
    // Run dialog
    response = gtk_dialog_run(GTK_DIALOG(dialog));
    
    if (response == GTK_RESPONSE_ACCEPT) {
        // Get values from form
        const gchar *name = gtk_entry_get_text(GTK_ENTRY(name_entry));
        gint x = atoi(gtk_entry_get_text(GTK_ENTRY(x_entry)));
        gint y = atoi(gtk_entry_get_text(GTK_ENTRY(y_entry)));
        gint width = atoi(gtk_entry_get_text(GTK_ENTRY(width_entry)));
        gint height = atoi(gtk_entry_get_text(GTK_ENTRY(height_entry)));
        gint spacing = atoi(gtk_entry_get_text(GTK_ENTRY(spacing_entry)));
        gint orientation_index = gtk_combo_box_get_active(GTK_COMBO_BOX(orientation_combo));
        GtkOrientation orientation = (orientation_index == 0) ? 
                                     GTK_ORIENTATION_HORIZONTAL : GTK_ORIENTATION_VERTICAL;
        
        // Get the selected container
        gint container_index = gtk_combo_box_get_active(GTK_COMBO_BOX(parent_container_combo));
        GtkWidget *target_container = NULL;
        
        if (container_index > 0) {
            target_container = g_list_nth_data(app_data->containers, container_index - 1);
        } else {
            target_container = app_data->preview_area;
        }
        
        // Create the box container
        GtkWidget *box = gtk_box_new(orientation, spacing);
        gtk_widget_set_name(box, name);
        gtk_widget_set_size_request(box, width, height);
        
        // Add box to appropriate container
        if (target_container == app_data->preview_area) {
            // Add to preview area directly
            gtk_fixed_put(GTK_FIXED(target_container), box, x, y);
            
            // Create a basic frame around the box to make it visible
            GtkWidget *frame = gtk_frame_new(name);
            gtk_container_add(GTK_CONTAINER(frame), box);
            gtk_fixed_put(GTK_FIXED(target_container), frame, x, y);
        } else {
            // Add to another container
            add_widget_to_container_by_type(box, target_container, x, y, width, height);
        }
        
        // Add the container to our list
        app_data->containers = g_list_append(app_data->containers, box);
        
        // Add to both tree structures
        add_widget_to_both_trees(app_data, box, g_strdup_printf("Box: %s", name), 
                               target_container, TRUE, NULL);
        
        // Update container selection dropdown
        update_container_combo(app_data);
        
        // Show the box
        gtk_widget_show_all(app_data->preview_area);
    }
    
    gtk_widget_destroy(dialog);
}

#endif /* CONTAINERS_H */