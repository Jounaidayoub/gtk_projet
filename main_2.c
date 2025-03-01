#include <gtk/gtk.h>
#include "global.h"
#include "entry.h"
// #include "Scale.h"
// #include "scroll.h"

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

// Forward declarations
static void show_properties_dialog(GtkWidget *widget, gpointer data);
static void on_drag_data_received(GtkWidget *widget, GdkDragContext *context, gint x, gint y,
                          GtkSelectionData *data, guint info, guint time, gpointer user_data);
static void export_to_xml(GtkWidget *widget, gpointer data);
static void run_demo(GtkWidget *widget, gpointer data);
static void add_basic_entry_clicked(GtkWidget *widget, gpointer data);
static void add_password_entry_clicked(GtkWidget *widget, gpointer data);
static void show_basic_entry_dialog(AppData *app_data);
static void show_password_entry_dialog(AppData *app_data);
static void add_to_hierarchy(AppData *app_data, const gchar *widget_type, GtkWidget *widget);
static void create_box_container(GtkWidget *widget, gpointer data);
static void update_container_combo(AppData *app_data);
static void on_container_selected(GtkComboBox *combo, gpointer user_data);
static gboolean add_widget_to_container(GtkWidget *widget, GtkWidget *container, gint x, gint y);

// Function to show properties dialog
void show_properties_dialog(GtkWidget *widget, gpointer data) {
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

// Function to handle drag data received
void on_drag_data_received(GtkWidget *widget, GdkDragContext *context, gint x, gint y,
                           GtkSelectionData *data, guint info, guint time, gpointer user_data) {
    const gchar *widget_type = (const gchar *)gtk_selection_data_get_data(data);
    GtkWidget *new_widget = NULL;

    if (g_strcmp0(widget_type, "Label") == 0) {
        new_widget = gtk_label_new("New Label");
    } else if (g_strcmp0(widget_type, "Button") == 0) {
        new_widget = gtk_button_new_with_label("New Button");
    } else if (g_strcmp0(widget_type, "Entry") == 0) {
        new_widget = gtk_entry_new();
    } else if (g_strcmp0(widget_type, "ComboBox") == 0) {
        new_widget = gtk_combo_box_text_new();
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(new_widget), "Option 1");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(new_widget), "Option 2");
    } else if (g_strcmp0(widget_type, "Scale") == 0) {
        new_widget = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 100, 1);
    }

    if (new_widget) {
        gtk_grid_attach(GTK_GRID(widget), new_widget, x / 100, y / 50, 1, 1);
        gtk_widget_show_all(widget);
    }

    gtk_drag_finish(context, TRUE, FALSE, time);
}

// Function to export layout to XML
void export_to_xml(GtkWidget *widget, gpointer data) {
    // Implement XML export logic here
    g_print("Export to XML\n");
}

// Function to run demo
void run_demo(GtkWidget *widget, gpointer data) {
    // Implement demo run logic here
    g_print("Run Demo\n");
}

// Function to add an item to the hierarchy tree
static void add_to_hierarchy(AppData *app_data, const gchar *widget_type, GtkWidget *widget) {
    GtkTreeIter iter;
    gtk_tree_store_append(app_data->hierarchy_store, &iter, NULL);
    gtk_tree_store_set(app_data->hierarchy_store, &iter, 0, widget_type, 1, widget, -1);
    
    // Expand all rows to show the newly added item
    gtk_tree_view_expand_all(GTK_TREE_VIEW(app_data->hierarchy_view));
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
    gtk_entry_set_text(GTK_ENTRY(width_entry), "300");
    gtk_entry_set_text(GTK_ENTRY(height_entry), "200");
    
    // Box-specific fields
    name_label = gtk_label_new("Container Name:");
    name_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(name_entry), "MyBox");
    
    orientation_label = gtk_label_new("Orientation:");
    orientation_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(orientation_combo), "Horizontal");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(orientation_combo), "Vertical");
    gtk_combo_box_set_active(GTK_COMBO_BOX(orientation_combo), 0);
    
    spacing_label = gtk_label_new("Spacing:");
    spacing_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(spacing_entry), "5");
    
    // Add widgets to grid
    gtk_grid_attach(GTK_GRID(grid), name_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), name_entry, 1, 0, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), orientation_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), orientation_combo, 1, 1, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), spacing_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), spacing_entry, 1, 2, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), x_label, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_entry, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_label, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_entry, 1, 4, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), width_label, 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), width_entry, 1, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), height_label, 0, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), height_entry, 1, 6, 1, 1);
    
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
        
        // Create the box container
        GtkWidget *box = gtk_box_new(orientation, spacing);
        gtk_widget_set_name(box, name);
        gtk_widget_set_size_request(box, width, height);
        
        // Add box to preview area
        gtk_fixed_put(GTK_FIXED(app_data->preview_area), box, x, y);
        
        // Create a basic frame around the box to make it visible
        GtkWidget *frame = gtk_frame_new(name);
        gtk_container_add(GTK_CONTAINER(frame), box);
        gtk_fixed_put(GTK_FIXED(app_data->preview_area), frame, x, y);
        
        // Add the container to our list and tree view
        app_data->containers = g_list_append(app_data->containers, box);
        
        // Add to hierarchy
        GtkTreeIter iter;
        gtk_tree_store_append(app_data->hierarchy_store, &iter, NULL);
        gtk_tree_store_set(app_data->hierarchy_store, &iter, 0, g_strdup_printf("Box: %s", name), 1, box, -1);
        
        // Update container selection dropdown
        update_container_combo(app_data);
        
        // Show the box
        gtk_widget_show_all(app_data->preview_area);
    }
    
    gtk_widget_destroy(dialog);
}

// Helper function to update container selection combo box
static void update_container_combo(AppData *app_data) {
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
    } else {
        // Get the selected container
        app_data->selected_container = g_list_nth_data(app_data->containers, active - 1);
    }
}

// Function to add a widget to the selected container
static gboolean add_widget_to_container(GtkWidget *widget, GtkWidget *container, gint x, gint y) {
    if (!container) {
        // Add to preview area (default)
        GtkWidget *parent = gtk_widget_get_parent(widget);
        if (GTK_IS_FIXED(parent)) {
            gtk_fixed_move(GTK_FIXED(parent), widget, x, y);
        } else {
            gtk_fixed_put(GTK_FIXED(parent), widget, x, y);
        }
        return TRUE;
    } else if (GTK_IS_BOX(container)) {
        // Add to box container
        gtk_box_pack_start(GTK_BOX(container), widget, FALSE, FALSE, 5);
        return TRUE;
    } else if (GTK_IS_CONTAINER(container)) {
        // Generic container
        gtk_container_add(GTK_CONTAINER(container), widget);
        return TRUE;
    }
    
    return FALSE;
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

int main(int argc, char *argv[]) {
    // Initialisation de GTK
    gtk_init(&argc, &argv);
    
    // App data structure to hold our widgets
    AppData app_data;

    // Initialize container management in AppData
    app_data.containers = NULL;
    app_data.selected_container = NULL;

    // Dimensions et coordonnées de la fenêtre
    dimension dim = {900, 700}; // Increased size
    coordonnees cord = {400, 250};

    // Couleur de fond
    HexColor *bg_color = hex_color_init("#ffffff");

    // Image de fond
    MonImage bg_img = {
           .path ="ex_img.jpg",
             .dim = dim,
           .cord = cord
    };

    // Initialisation de la fenêtre
    Mywindow *maFenetre = init_window("GTK UI Builder", "icon1.png",
            dim,
            0,
            TRUE,                      // Make window resizable
            GTK_WIN_POS_CENTER,
            cord,
            *bg_color,
            bg_img
    );
    app_data.window = maFenetre->window;

    // Création de la fenêtre
    create_window(maFenetre);

    // Create main horizontal box
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_container_add(GTK_CONTAINER(maFenetre->window), main_box);
    
    // Create left panel with fixed width
    GtkWidget *left_panel_scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(left_panel_scroll),
                                  GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(left_panel_scroll, 220, -1);
    
    GtkWidget *left_panel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(left_panel_scroll), left_panel);
    
    // Create sections in left panel
    GtkWidget *widgets_frame = gtk_frame_new("Widgets");
    GtkWidget *widgets_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_set_border_width(GTK_CONTAINER(widgets_box), 5);
    gtk_container_add(GTK_CONTAINER(widgets_frame), widgets_box);
    
    // Add standard widget buttons to widget box
    const char *widget_labels[] = {"Label", "Button", "Entry", "ComboBox", "Scale", "Checkbox"};
    for (int i = 0; i < 6; i++) {
        GtkWidget *button = gtk_button_new_with_label(widget_labels[i]);
        g_signal_connect(button, "clicked", G_CALLBACK(show_properties_dialog), &app_data);
        gtk_box_pack_start(GTK_BOX(widgets_box), button, FALSE, FALSE, 2);
    }
    
    // Add custom entries section
    GtkWidget *entries_frame = gtk_frame_new("Custom Entries");
    GtkWidget *entries_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_set_border_width(GTK_CONTAINER(entries_box), 5);
    gtk_container_add(GTK_CONTAINER(entries_frame), entries_box);
    
    // Add the two entry type buttons
    GtkWidget *basic_entry_btn = gtk_button_new_with_label("Add Basic Entry");
    GtkWidget *password_entry_btn = gtk_button_new_with_label("Add Password Entry");
    
    g_signal_connect(basic_entry_btn, "clicked", G_CALLBACK(add_basic_entry_clicked), &app_data);
    g_signal_connect(password_entry_btn, "clicked", G_CALLBACK(add_password_entry_clicked), &app_data);
    
    gtk_box_pack_start(GTK_BOX(entries_box), basic_entry_btn, FALSE, FALSE, 2);
    gtk_box_pack_start(GTK_BOX(entries_box), password_entry_btn, FALSE, FALSE, 2);
    
    // Add containers section
    GtkWidget *containers_frame = gtk_frame_new("Containers");
    GtkWidget *containers_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_set_border_width(GTK_CONTAINER(containers_box), 5);
    gtk_container_add(GTK_CONTAINER(containers_frame), containers_box);
    
    // Add container buttons
    const char *container_labels[] = {"Box", "Frame", "Grid", "ScrolledWindow"};
    for (int i = 0; i < 4; i++) {
        GtkWidget *button = gtk_button_new_with_label(container_labels[i]);
        
        if (i == 0) { // "Box" is the first one
            g_signal_connect(button, "clicked", G_CALLBACK(create_box_container), &app_data);
        }
        
        gtk_box_pack_start(GTK_BOX(containers_box), button, FALSE, FALSE, 2);
    }
    
    // Add all sections to left panel
    gtk_box_pack_start(GTK_BOX(left_panel), widgets_frame, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(left_panel), entries_frame, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(left_panel), containers_frame, FALSE, FALSE, 0);
    
    // Create hierarchy section
    GtkWidget *hierarchy_frame = gtk_frame_new("UI Hierarchy");
    GtkWidget *hierarchy_scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(hierarchy_scroll),
                                  GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(hierarchy_scroll, -1, 250);
    
    // Create tree view for hierarchy
    app_data.hierarchy_store = gtk_tree_store_new(2, G_TYPE_STRING, G_TYPE_POINTER);
    app_data.hierarchy_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(app_data.hierarchy_store));
    
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes(
        "Widget Tree", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(app_data.hierarchy_view), column);
    
    // Add root node for UI
    GtkTreeIter iter;
    gtk_tree_store_append(app_data.hierarchy_store, &iter, NULL);
    gtk_tree_store_set(app_data.hierarchy_store, &iter, 0, "UI Root", -1);
    
    gtk_container_add(GTK_CONTAINER(hierarchy_scroll), app_data.hierarchy_view);
    gtk_container_add(GTK_CONTAINER(hierarchy_frame), hierarchy_scroll);
    gtk_box_pack_end(GTK_BOX(left_panel), hierarchy_frame, TRUE, TRUE, 0);
    
    // Add container selection to main UI
    GtkWidget *container_frame = gtk_frame_new("Current Container");
    app_data.container_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(app_data.container_combo), "Window (Default)");
    gtk_combo_box_set_active(GTK_COMBO_BOX(app_data.container_combo), 0);
    g_signal_connect(app_data.container_combo, "changed", G_CALLBACK(on_container_selected), &app_data);
    
    GtkWidget *container_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_set_border_width(GTK_CONTAINER(container_box), 5);
    gtk_container_add(GTK_CONTAINER(container_frame), container_box);
    gtk_box_pack_start(GTK_BOX(container_box), app_data.container_combo, FALSE, FALSE, 2);
    
    // Add container selection after the container buttons
    gtk_box_pack_start(GTK_BOX(left_panel), container_frame, FALSE, FALSE, 0);
    
    // Pack left panel into main box
    gtk_box_pack_start(GTK_BOX(main_box), left_panel_scroll, FALSE, FALSE, 0);
    
    // Create central preview area with frame
    GtkWidget *preview_frame = gtk_frame_new("Preview Area");
    app_data.preview_area = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(preview_frame), app_data.preview_area);
    gtk_box_pack_start(GTK_BOX(main_box), preview_frame, TRUE, TRUE, 0);
    
    // Create right properties panel
    app_data.properties_panel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_size_request(app_data.properties_panel, 200, -1);
    
    GtkWidget *properties_frame = gtk_frame_new("Properties");
    GtkWidget *properties_scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(properties_scroll),
                                  GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    
    GtkWidget *properties_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_set_border_width(GTK_CONTAINER(properties_box), 5);
    
    // Add placeholder content for properties panel
    gtk_box_pack_start(GTK_BOX(properties_box), gtk_label_new("Select a widget to edit properties"), FALSE, FALSE, 5);
    
    // Add action buttons at bottom
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_set_border_width(GTK_CONTAINER(button_box), 5);
    
    GtkWidget *export_button = gtk_button_new_with_label("Export XML");
    GtkWidget *run_button = gtk_button_new_with_label("Run Demo");
    GtkWidget *exit_button = gtk_button_new_with_label("Exit");
    
    g_signal_connect(export_button, "clicked", G_CALLBACK(export_to_xml), &app_data);
    g_signal_connect(run_button, "clicked", G_CALLBACK(run_demo), &app_data);
    g_signal_connect(exit_button, "clicked", G_CALLBACK(gtk_main_quit), NULL);
    
    gtk_box_pack_start(GTK_BOX(button_box), export_button, FALSE, FALSE, 2);
    gtk_box_pack_start(GTK_BOX(button_box), run_button, FALSE, FALSE, 2);
    gtk_box_pack_start(GTK_BOX(button_box), exit_button, FALSE, FALSE, 2);
    
    gtk_container_add(GTK_CONTAINER(properties_scroll), properties_box);
    gtk_container_add(GTK_CONTAINER(properties_frame), properties_scroll);
    
    gtk_box_pack_start(GTK_BOX(app_data.properties_panel), properties_frame, TRUE, TRUE, 0);
    gtk_box_pack_end(GTK_BOX(app_data.properties_panel), button_box, FALSE, FALSE, 0);
    
    gtk_box_pack_start(GTK_BOX(main_box), app_data.properties_panel, FALSE, FALSE, 0);
    
    // Show all widgets
    gtk_widget_show_all(maFenetre->window);
    gtk_main();
    
    return 0;
}