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
// Container creation callback for StyledBox
static void create_styled_box_container(GtkWidget *widget, gpointer data) {
    AppData *app_data = (AppData *)data;
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *grid;
    GtkWidget *orientation_label, *spacing_label, *name_label;
    GtkWidget *name_entry, *spacing_entry;
    GtkWidget *orientation_combo;
    GtkWidget *x_label, *y_label, *width_label, *height_label;
    GtkWidget *x_entry, *y_entry, *width_entry, *height_entry;
    GtkWidget *bg_color_label, *bg_color_entry;
    GtkWidget *border_radius_label, *border_radius_entry;
    GtkWidget *border_label, *border_entry;
    GtkWidget *homogeneous_check;
    gint response;
    
    // Create dialog
    dialog = gtk_dialog_new_with_buttons("Create Styled Box Container",
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
    gtk_entry_set_text(GTK_ENTRY(name_entry), "MyStyledBox");
    
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
    
    // Homogeneous checkbox
    homogeneous_check = gtk_check_button_new_with_label("Homogeneous");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(homogeneous_check), FALSE);
    
    gtk_grid_attach(GTK_GRID(grid), homogeneous_check, 0, 4, 2, 1);
    
    // Position fields
    x_label = gtk_label_new("X Position:");
    y_label = gtk_label_new("Y Position:");
    x_entry = gtk_entry_new();
    y_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(x_entry), "10");
    gtk_entry_set_text(GTK_ENTRY(y_entry), "10");
    
    gtk_grid_attach(GTK_GRID(grid), x_label, 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_entry, 1, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_label, 0, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_entry, 1, 6, 1, 1);
    
    // Size fields
    width_label = gtk_label_new("Width:");
    height_label = gtk_label_new("Height:");
    width_entry = gtk_entry_new();
    height_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(width_entry), "300");
    gtk_entry_set_text(GTK_ENTRY(height_entry), "200");
    
    gtk_grid_attach(GTK_GRID(grid), width_label, 0, 7, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), width_entry, 1, 7, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), height_label, 0, 8, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), height_entry, 1, 8, 1, 1);
    
    // Styling fields
    bg_color_label = gtk_label_new("Background Color:");
    GtkWidget *bg_color_button = gtk_color_button_new_with_rgba(NULL);
    GdkRGBA color;
    gdk_rgba_parse(&color, "#f0f0f0");
    gtk_color_button_set_rgba(GTK_COLOR_BUTTON(bg_color_button), &color);
    gtk_color_button_set_title(GTK_COLOR_BUTTON(bg_color_button), "Select Background Color");

    
    border_radius_label = gtk_label_new("Border Radius:");
    border_radius_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(border_radius_entry), "5px");
    
    border_label = gtk_label_new("Border:");
    border_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(border_entry), "1px solid #cccccc");
    
    gtk_grid_attach(GTK_GRID(grid), bg_color_label, 0, 9, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), bg_color_button, 1, 9, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), border_radius_label, 0, 10, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), border_radius_entry, 1, 10, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), border_label, 0, 11, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), border_entry, 1, 11, 1, 1);
    
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
        gboolean homogeneous = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(homogeneous_check));
        
        // Get styling values
        GdkRGBA bg_color_rgba;
        gtk_color_button_get_rgba(GTK_COLOR_BUTTON(bg_color_button), &bg_color_rgba);
        gchar *bg_color = gdk_rgba_to_string(&bg_color_rgba);
        const gchar *border_radius = gtk_entry_get_text(GTK_ENTRY(border_radius_entry));
        const gchar *border = gtk_entry_get_text(GTK_ENTRY(border_entry));
        
        // Get the selected container
        gint container_index = gtk_combo_box_get_active(GTK_COMBO_BOX(parent_container_combo));
        GtkWidget *target_container = NULL;
        
        if (container_index > 0) {
            target_container = g_list_nth_data(app_data->containers, container_index - 1);
        } else {
            target_container = app_data->preview_area;
        }
        
        // Create position and dimension structures
        coordonnees *pos = cord(x, y);
        dimension *dim = dim(width, height);
        
        // Create the styled box container
        StyledBox *styled_box = allocate_styled_box();
        styled_box->orientation = orientation;
        styled_box->homogeneous = homogeneous;
        styled_box->spacing = spacing;
        styled_box->background_color = g_strdup(bg_color);
        styled_box->border_radius = g_strdup(border_radius);
        styled_box->border = g_strdup(border);
        styled_box->cord = pos;
        styled_box->dim = dim;
        styled_box->container = target_container;
        
        // Create the actual widget
        styled_box->widget = gtk_box_new(orientation, spacing);
        gtk_box_set_homogeneous(GTK_BOX(styled_box->widget), homogeneous);
        gtk_widget_set_name(styled_box->widget, name);
        gtk_widget_set_size_request(styled_box->widget, width, height);
        
        // Apply CSS styling
        if (styled_box->background_color || styled_box->border_radius || styled_box->border) {
            GtkStyleContext *context = gtk_widget_get_style_context(styled_box->widget);
            GtkCssProvider *provider = gtk_css_provider_new();
            
            // Create CSS string
            gchar *css = g_strdup_printf(
                "box { background-color: %s; border-radius: %s; border: %s; }",
                styled_box->background_color ? styled_box->background_color : "transparent",
                styled_box->border_radius ? styled_box->border_radius : "0px",
                styled_box->border ? styled_box->border : "none"
            );
            
            // Load and apply CSS
            GError *error = NULL;
            if (!gtk_css_provider_load_from_data(provider, css, -1, &error)) {
                g_print("CSS error: %s\n", error->message);
                g_error_free(error);
            }
            
            gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider),
                                        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
            
            g_object_unref(provider);
            g_free(css);
        }
        
        // Add to container
        if (GTK_IS_FIXED(target_container)) {
            gtk_fixed_put(GTK_FIXED(target_container), styled_box->widget, x, y);
        } else {
            gtk_container_add(GTK_CONTAINER(target_container), styled_box->widget);
        }
        
        // Add the container to our list
        app_data->containers = g_list_append(app_data->containers, styled_box->widget);
        
        // Add to both tree structures
        add_widget_to_both_trees(app_data, styled_box->widget, g_strdup_printf("StyledBox: %s", name), 
                               target_container, TRUE, styled_box);
        
        // Update container selection dropdown
        update_container_combo(app_data);
        
        // Show the styled box
        gtk_widget_show_all(app_data->preview_area);
    }
    
    gtk_widget_destroy(dialog);
}
#endif /* CONTAINERS_H */