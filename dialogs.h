#ifndef DIALOGS_H
#define DIALOGS_H

#include <gtk/gtk.h>
#include "app_data.h"
#include "hierarchy.h"
#include "entry.h"

// Function to show properties dialog
static void show_properties_dialog(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog, *content_area, *label;
    GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;

    dialog = gtk_dialog_new_with_buttons("Widget Properties",
                                         GTK_WINDOW(data),
                                         flags,
                                         "_OK",
                                         GTK_RESPONSE_OK,
                                         "_Cancel",
                                         GTK_RESPONSE_CANCEL,
                                         NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    label = gtk_label_new("Enter properties for the widget:");
    gtk_container_add(GTK_CONTAINER(content_area), label);

    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// Function to show dialog for basic entry configuration
static void show_basic_entry_dialog(AppData *app_data) {
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *grid;
    GtkWidget *x_label, *y_label, *width_label, *height_label;
    GtkWidget *x_entry, *y_entry, *width_entry, *height_entry;
    GtkWidget *editable_check, *visible_check;
    GtkWidget *placeholder_label, *max_len_label, *default_text_label;
    GtkWidget *placeholder_entry, *max_len_entry, *default_text_entry;
    gint response;
    
    // Create dialog
    dialog = gtk_dialog_new_with_buttons("Basic Entry Properties",
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
    GtkWidget *container_label = gtk_label_new("Add to Container:");
    GtkWidget *container_combo = gtk_combo_box_text_new();
    
    // Add default option (preview area)
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(container_combo), "Window (Default)");
    
    // Add all containers
    GList *iter;
    for (iter = app_data->containers; iter != NULL; iter = iter->next) {
        GtkWidget *container = GTK_WIDGET(iter->data);
        const gchar *name = gtk_widget_get_name(container);
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(container_combo), name);
    }
    
    // Select currently selected container
    gint active_index = 0;
    if (app_data->selected_container) {
        active_index = g_list_index(app_data->containers, app_data->selected_container) + 1;
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(container_combo), active_index);
    
    // Add container selection to the top of the form
    gtk_grid_attach(GTK_GRID(grid), container_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), container_combo, 1, 0, 1, 1);
    
    // Position fields
    x_label = gtk_label_new("X Position:");
    y_label = gtk_label_new("Y Position:");
    x_entry = gtk_entry_new();
    y_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(x_entry), "10");
    gtk_entry_set_text(GTK_ENTRY(y_entry), "10");
    
    // Size fields
    width_label = gtk_label_new("Width:");
    height_label = gtk_label_new("Height:");
    width_entry = gtk_entry_new();
    height_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(width_entry), "150");
    gtk_entry_set_text(GTK_ENTRY(height_entry), "30");
    
    // Checkbox fields
    editable_check = gtk_check_button_new_with_label("Editable");
    visible_check = gtk_check_button_new_with_label("Visible");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(editable_check), TRUE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(visible_check), TRUE);
    
    // Text fields
    placeholder_label = gtk_label_new("Placeholder Text:");
    max_len_label = gtk_label_new("Max Length:");
    default_text_label = gtk_label_new("Default Text:");
    placeholder_entry = gtk_entry_new();
    max_len_entry = gtk_entry_new();
    default_text_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(placeholder_entry), "Enter text here...");
    gtk_entry_set_text(GTK_ENTRY(max_len_entry), "0");
    
    // Add widgets to grid
    gtk_grid_attach(GTK_GRID(grid), x_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_entry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_entry, 1, 2, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), width_label, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), width_entry, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), height_label, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), height_entry, 1, 4, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), editable_check, 0, 5, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), visible_check, 0, 6, 2, 1);
    
    gtk_grid_attach(GTK_GRID(grid), placeholder_label, 0, 7, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), placeholder_entry, 1, 7, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), max_len_label, 0, 8, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), max_len_entry, 1, 8, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), default_text_label, 0, 9, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), default_text_entry, 1, 9, 1, 1);
    
    // Add grid to dialog
    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_widget_show_all(dialog);
    
    // Run dialog
    response = gtk_dialog_run(GTK_DIALOG(dialog));
    
    if (response == GTK_RESPONSE_ACCEPT) {
        // Get values from form
        gint x = atoi(gtk_entry_get_text(GTK_ENTRY(x_entry)));
        gint y = atoi(gtk_entry_get_text(GTK_ENTRY(y_entry)));
        gint width = atoi(gtk_entry_get_text(GTK_ENTRY(width_entry)));
        gint height = atoi(gtk_entry_get_text(GTK_ENTRY(height_entry)));
        gboolean is_editable = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(editable_check));
        gboolean is_visible = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(visible_check));
        const gchar *placeholder = gtk_entry_get_text(GTK_ENTRY(placeholder_entry));
        gint max_len = atoi(gtk_entry_get_text(GTK_ENTRY(max_len_entry)));
        const gchar *default_text = gtk_entry_get_text(GTK_ENTRY(default_text_entry));
        
        // Get the selected container
        gint container_index = gtk_combo_box_get_active(GTK_COMBO_BOX(container_combo));
        GtkWidget *target_container = NULL;
        
        if (container_index > 0) {
            target_container = g_list_nth_data(app_data->containers, container_index - 1);
        } else {
            target_container = app_data->preview_area;
        }
        
        // Create basic entry with the selected container
        entry_type_basic *entry_basic = Init_Entry_Basic(
            dim(width, height),
            is_editable,
            is_visible,
            placeholder,
            max_len,
            default_text,
            target_container,
            cord(x, y)
        );
        
        if (entry_basic != NULL) {
            GtkWidget *entry_widget = creer_entry_basic(entry_basic);
            
            // Add to hierarchy under the correct parent
            if (container_index > 0) {
                // Find the tree iter for the container
                GtkTreeIter parent_iter;
                GtkTreeIter child_iter;
                gboolean found = FALSE;
                
                // Find the container in the tree
                if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(app_data->hierarchy_store), &parent_iter)) {
                    do {
                        GtkWidget *iter_widget;
                        gtk_tree_model_get(GTK_TREE_MODEL(app_data->hierarchy_store), &parent_iter, 1, &iter_widget, -1);
                        
                        if (iter_widget == target_container) {
                            found = TRUE;
                            break;
                        }
                    } while (gtk_tree_model_iter_next(GTK_TREE_MODEL(app_data->hierarchy_store), &parent_iter));
                }
                
                if (found) {
                    // Add as child of container
                    gtk_tree_store_append(app_data->hierarchy_store, &child_iter, &parent_iter);
                    gtk_tree_store_set(app_data->hierarchy_store, &child_iter, 0, "Basic Entry", 1, entry_widget, -1);
                } else {
                    // Fallback to root
                    add_to_hierarchy(app_data, "Basic Entry", entry_widget);
                }
            } else {
                // Add to root
                add_to_hierarchy(app_data, "Basic Entry", entry_widget);
            }
            
            gtk_widget_show_all(app_data->preview_area);
        }
    }
    
    gtk_widget_destroy(dialog);
}

// Function to show dialog for password entry configuration
static void show_password_entry_dialog(AppData *app_data) {
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *grid;
    GtkWidget *x_label, *y_label, *width_label, *height_label;
    GtkWidget *x_entry, *y_entry, *width_entry, *height_entry;
    GtkWidget *placeholder_label, *invisible_char_label;
    GtkWidget *placeholder_entry, *invisible_char_entry;
    gint response;
    
    // Create dialog
    dialog = gtk_dialog_new_with_buttons("Password Entry Properties",
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
    GtkWidget *container_label = gtk_label_new("Add to Container:");
    GtkWidget *container_combo = gtk_combo_box_text_new();
    
    // Add default option (preview area)
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(container_combo), "Window (Default)");
    
    // Add all containers
    GList *iter;
    for (iter = app_data->containers; iter != NULL; iter = iter->next) {
        GtkWidget *container = GTK_WIDGET(iter->data);
        const gchar *name = gtk_widget_get_name(container);
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(container_combo), name);
    }
    
    // Select currently selected container
    gint active_index = 0;
    if (app_data->selected_container) {
        active_index = g_list_index(app_data->containers, app_data->selected_container) + 1;
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(container_combo), active_index);
    
    // Add container selection to the top of the form
    gtk_grid_attach(GTK_GRID(grid), container_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), container_combo, 1, 0, 1, 1);
    
    // Position fields
    x_label = gtk_label_new("X Position:");
    y_label = gtk_label_new("Y Position:");
    x_entry = gtk_entry_new();
    y_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(x_entry), "10");
    gtk_entry_set_text(GTK_ENTRY(y_entry), "50");  // Different default Y from basic entry
    
    // Size fields
    width_label = gtk_label_new("Width:");
    height_label = gtk_label_new("Height:");
    width_entry = gtk_entry_new();
    height_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(width_entry), "150");
    gtk_entry_set_text(GTK_ENTRY(height_entry), "30");
    
    // Password-specific fields
    placeholder_label = gtk_label_new("Placeholder Text:");
    invisible_char_label = gtk_label_new("Mask Character:");
    placeholder_entry = gtk_entry_new();
    invisible_char_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(placeholder_entry), "Enter password...");
    gtk_entry_set_text(GTK_ENTRY(invisible_char_entry), "*");
    gtk_entry_set_max_length(GTK_ENTRY(invisible_char_entry), 1);  // Only one character
    
    // Add widgets to grid
    gtk_grid_attach(GTK_GRID(grid), x_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_entry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_entry, 1, 2, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), width_label, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), width_entry, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), height_label, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), height_entry, 1, 4, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), placeholder_label, 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), placeholder_entry, 1, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), invisible_char_label, 0, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), invisible_char_entry, 1, 6, 1, 1);
    
    // Add grid to dialog
    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_widget_show_all(dialog);
    
    // Run dialog
    response = gtk_dialog_run(GTK_DIALOG(dialog));
    
    if (response == GTK_RESPONSE_ACCEPT) {
        // Get values from form
        gint x = atoi(gtk_entry_get_text(GTK_ENTRY(x_entry)));
        gint y = atoi(gtk_entry_get_text(GTK_ENTRY(y_entry)));
        gint width = atoi(gtk_entry_get_text(GTK_ENTRY(width_entry)));
        gint height = atoi(gtk_entry_get_text(GTK_ENTRY(height_entry)));
        const gchar *placeholder = gtk_entry_get_text(GTK_ENTRY(placeholder_entry));
        const gchar *invisible_char_text = gtk_entry_get_text(GTK_ENTRY(invisible_char_entry));
        
        // Default to '*' if no character is provided
        gchar invisible_char = (invisible_char_text && invisible_char_text[0]) ? invisible_char_text[0] : '*';
        
        // Get the selected container
        gint container_index = gtk_combo_box_get_active(GTK_COMBO_BOX(container_combo));
        GtkWidget *target_container = NULL;
        
        if (container_index > 0) {
            target_container = g_list_nth_data(app_data->containers, container_index - 1);
        } else {
            target_container = app_data->preview_area;
        }
        
        // Create password entry
        entry_type_password *entry_password = Init_Entry_Password(
            dim(width, height),  // Using the dimension macro
            placeholder,
            invisible_char,
            target_container,
            cord(x, y)           // Using the coordinates macro
        );
        
        if (entry_password != NULL) {
            GtkWidget *entry_widget = creer_entry_pass(entry_password);
            
            // Add to hierarchy under the correct parent
            if (container_index > 0) {
                // Find the tree iter for the container
                GtkTreeIter parent_iter;
                GtkTreeIter child_iter;
                gboolean found = FALSE;
                
                // Find the container in the tree
                if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(app_data->hierarchy_store), &parent_iter)) {
                    do {
                        GtkWidget *iter_widget;
                        gtk_tree_model_get(GTK_TREE_MODEL(app_data->hierarchy_store), &parent_iter, 1, &iter_widget, -1);
                        
                        if (iter_widget == target_container) {
                            found = TRUE;
                            break;
                        }
                    } while (gtk_tree_model_iter_next(GTK_TREE_MODEL(app_data->hierarchy_store), &parent_iter));
                }
                
                if (found) {
                    // Add as child of container
                    gtk_tree_store_append(app_data->hierarchy_store, &child_iter, &parent_iter);
                    gtk_tree_store_set(app_data->hierarchy_store, &child_iter, 0, "Password Entry", 1, entry_widget, -1);
                } else {
                    // Fallback to root
                    add_to_hierarchy(app_data, "Password Entry", entry_widget);
                }
            } else {
                // Add to root
                add_to_hierarchy(app_data, "Password Entry", entry_widget);
            }
            
            gtk_widget_show_all(app_data->preview_area);
        }
    }
    
    gtk_widget_destroy(dialog);
}

// Function to handle basic entry button click
static void add_basic_entry_clicked(GtkWidget *widget, gpointer data) {
    AppData *app_data = (AppData *)data;
    show_basic_entry_dialog(app_data);
}

// Function to handle password entry button click
static void add_password_entry_clicked(GtkWidget *widget, gpointer data) {
    AppData *app_data = (AppData *)data;
    show_password_entry_dialog(app_data);
}

#endif /* DIALOGS_H */