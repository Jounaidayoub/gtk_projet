#ifndef PROPERTY_PANEL_H
#define PROPERTY_PANEL_H

#include <gtk/gtk.h>
#include "app_data.h"
#include "hierarchy.h"
#include "tree_sync.h"

// Add forward declarations
extern void update_container_combo(AppData *app_data);
static void create_property_form_for_widget(AppData *app_data, GtkWidget *widget);
void* get_widget_structure(AppData *app_data, GtkWidget *widget);
// Structure to store widget property fields for later retrieval
typedef struct {
    GtkWidget *widget;       // The widget being edited
    GtkWidget *form_grid;    // The grid containing the form fields
    GtkWidget *container;    // The container for the form
    
    // Common fields
    GtkWidget *x_entry;      
    GtkWidget *y_entry;      
    GtkWidget *width_entry;  
    GtkWidget *height_entry;
    GtkWidget *name_entry;
    // Widget-specific fields (union could be used to save memory)
    GtkWidget *placeholder_entry;
    GtkWidget *max_len_entry;
    GtkWidget *default_text_entry;
    GtkWidget *editable_check;
    GtkWidget *visible_check;
    GtkWidget *mask_char_entry;
    GtkWidget *label_text_entry;
    GtkWidget *button_text_entry;
    GtkWidget *spacing_entry;
    GtkWidget *orientation_combo;
    GtkWidget *label_entry; // Add this line

    // For spin buttons
    GtkWidget *value_entry;
    GtkWidget *min_entry;
    GtkWidget *max_entry;
    GtkWidget *step_entry;
    GtkWidget *digits_entry;
    GtkWidget *numeric_check;
    GtkWidget *wrap_check;
    GtkWidget *active_check;

    //font things
    GtkWidget *font_entry;
    GtkWidget *size_entry;
    GtkWidget *color_entry;
    GtkWidget *bold_check;
    GtkWidget *italic_check;    
    GtkWidget *bgcolor_entry;


} PropertyFields;

// Store the current property fields for later retrieval
static PropertyFields current_properties;
static void clear_properties_panel(AppData *app_data) {
    GtkWidget *content = app_data->properties_content;
    
    // Remove all child widgets from the content area
    GList *children, *iter;
    children = gtk_container_get_children(GTK_CONTAINER(content));
    for (iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_container_remove(GTK_CONTAINER(content), GTK_WIDGET(iter->data));
    g_list_free(children);
    
    // Reset the current properties structure
    memset(&current_properties, 0, sizeof(PropertyFields));
    
    // No need to show_all since we've removed everything
}

// Handle widget selection without blocking widget functionality
static gboolean on_widget_button_press_select(GtkWidget *widget, GdkEventButton *event, gpointer user_data) {
    AppData *app_data = (AppData *)user_data;
    clear_properties_panel(app_data);
    
    g_print("Widget clicked: %p\n", widget);
    
    // Only handle left-click for selection
    if (event->button == 1) {
        // Update the selected widget
        app_data->selected_widget = widget;
        
        g_print("Selected widget: %p, creating property form...\n", widget);
        
        // Show the widget's properties
        create_property_form_for_widget(app_data, widget);
        
        // Return FALSE to allow the event to propagate (widget remains functional)
        return FALSE;
    }
    
    return FALSE; // Propagate the event
}

// Function to clear the properties panel


// // Clear selection when clicking on the preview area background
// static gboolean on_preview_area_click(GtkWidget *widget, GdkEventButton *event, gpointer user_data) {
//     AppData *app_data = (AppData *)user_data;
    
//     // If click is directly on the preview area (not on a child widget)
//     if (event->window == gtk_widget_get_window(widget)) {
//         // Clear selection
//         app_data->selected_widget = NULL;
        
//         // Clear the property panel
//         // thsi need to be fixed to clear the properties panel correcly {commentd for later}
//         clear_properties_panel(app_data);
        
//         // Disable buttons
//         gtk_widget_set_sensitive(app_data->apply_button, FALSE);
//         // Clear the property panel
//         // thsi need to be fixed to clear the properties panel correcly {commentd for later}
//         clear_properties_panel(app_data);
        
//         // Disable buttons
//         gtk_widget_set_sensitive(app_data->apply_button, FALSE);
//         gtk_widget_set_sensitive(app_data->remove_button, FALSE);
        
//         g_print("Selection cleared\n");
//     }
    
//     return FALSE; // Propagate the event
// }
static gboolean on_preview_area_click(GtkWidget *widget, GdkEventButton *event, gpointer user_data) {
    AppData *app_data = (AppData *)user_data;
    
    // If click is directly on the preview area (not on a child widget)
    if (event->window == gtk_widget_get_window(widget)) {
        // Clear selection
        app_data->selected_widget = NULL;
        
        // Clear the property panel (only call once)
        clear_properties_panel(app_data);
        
        // Disable buttons
        gtk_widget_set_sensitive(app_data->apply_button, FALSE);
        gtk_widget_set_sensitive(app_data->remove_button, FALSE);
        
        g_print("Selection cleared\n");
    }
    
    return FALSE; // Propagate the event
}

// Create a labeled entry for properties
static GtkWidget *create_labeled_property(const gchar *label_text, GtkWidget **entry_ptr, const gchar *default_value) {
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget *label = gtk_label_new(label_text);
    GtkWidget *entry = gtk_entry_new();
    
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    
    if (default_value != NULL) {
        gtk_entry_set_text(GTK_ENTRY(entry), default_value);
    }
    
    gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 2);
    gtk_box_pack_start(GTK_BOX(hbox), entry, TRUE, TRUE, 2);
    
    // Store the entry widget in the provided pointer for later access
    if (entry_ptr) {
        *entry_ptr = entry;
    }
    
    return hbox;
}

// Create a checkbox property
static GtkWidget *create_checkbox_property(const gchar *label_text, GtkWidget **check_ptr, gboolean initial_state) {
    GtkWidget *check = gtk_check_button_new_with_label(label_text);
    
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check), initial_state);
    
    // Store the checkbox widget in the provided pointer for later access
    if (check_ptr) {
        *check_ptr = check;
    }
    
    return check;
}
// static void add_stled_box(gtkwiget, gpointer data){
//     AppData *app_data = (AppData *)data;
//     clear_properties_panel(app_data);
//     g_print("Adding styled box to preview area\n");
//     add_styled_box_to_preview(app_data);
// }

void add_styled_box_to_preview(GtkWidget *widget, gpointer data) {
    AppData *app_data = (AppData *)data;
    // Create at default position


    StyledBox *box = init_styled_box(
        GTK_ORIENTATION_VERTICAL,
        FALSE,
        5,
        NULL,
        "#f0f0f0",  // Light gray background
        "5px",      // Rounded corners
        "1px solid #cccccc", // Light border
        cord(20, 20),        // Position
        dim(200, 150),       // Size
        app_data->preview_area
    );
    
    // Create the widget
    create_styled_box(box);
    
    // Generate a unique name
    gchar *name = g_strdup_printf("styled_box_%d", g_list_length(app_data->containers) + 1);
    gtk_widget_set_name(box->widget, name);
    
    // Add event handlers to make it selectable
    // g_signal_connect(box->widget, "button-press-event", 
    //                  G_CALLBACK(on_container_click), app_data);
    
    // Add to containers list
    app_data->containers = g_list_append(app_data->containers, box->widget);
    
    // Add to hierarchy tree
    add_widget_to_both_trees(app_data, box->widget, "Styled Box", 
                            app_data->preview_area, TRUE, box);
    
    // Make it visible
    gtk_widget_show_all(box->widget);
    
    // Select it
    app_data->selected_container = box->widget;
    
    // Clean up
    g_free(name);
}
// Create property form for basic entry widget
static void create_basic_entry_form(AppData *app_data, GtkWidget *widget) {
    GtkWidget *content = app_data->properties_content;
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    
    // Store the widget being edited
    current_properties.widget = widget;
    current_properties.container = vbox;
    
    // Get current properties
    const gchar *current_text = gtk_entry_get_text(GTK_ENTRY(widget));
    gboolean is_editable = gtk_editable_get_editable(GTK_EDITABLE(widget));
    gboolean is_visible = gtk_entry_get_visibility(GTK_ENTRY(widget));
    gint max_length = gtk_entry_get_max_length(GTK_ENTRY(widget));
    const gchar *placeholder = gtk_entry_get_placeholder_text(GTK_ENTRY(widget));
    
    // Get position and size
    GtkWidget *parent = gtk_widget_get_parent(widget);
    gint x = 0, y = 0, width = 0, height = 0;
    
    if (GTK_IS_FIXED(parent)) {
        gtk_container_child_get(GTK_CONTAINER(parent), widget, "x", &x, "y", &y, NULL);
    }
    
    gtk_widget_get_size_request(widget, &width, &height);
    
    // Create grid for form layout - same as used in the creation dialog
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
    
    current_properties.form_grid = grid;
    
    // Title
    GtkWidget *title = gtk_label_new("Basic Entry Properties");
    gtk_widget_set_halign(title, GTK_ALIGN_START);
    
    // Format values as strings
    char x_str[32], y_str[32], width_str[32], height_str[32], max_len_str[32];
    sprintf(x_str, "%d", x);
    sprintf(y_str, "%d", y);
    sprintf(width_str, "%d", width);
    sprintf(height_str, "%d", height);
    sprintf(max_len_str, "%d", max_length);
    
    // Position fields
    GtkWidget *x_label = gtk_label_new("X Position:");
    GtkWidget *y_label = gtk_label_new("Y Position:");
    GtkWidget *x_entry = gtk_entry_new();
    GtkWidget *y_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(x_entry), x_str);
    gtk_entry_set_text(GTK_ENTRY(y_entry), y_str);
    current_properties.x_entry = x_entry;
    current_properties.y_entry = y_entry;
    
    // Size fields
    GtkWidget *width_label = gtk_label_new("Width:");
    GtkWidget *height_label = gtk_label_new("Height:");
    GtkWidget *width_entry = gtk_entry_new();
    GtkWidget *height_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(width_entry), width_str);
    gtk_entry_set_text(GTK_ENTRY(height_entry), height_str);
    current_properties.width_entry = width_entry;
    current_properties.height_entry = height_entry;
    
    // Checkbox fields
    GtkWidget *editable_check = gtk_check_button_new_with_label("Editable");
    GtkWidget *visible_check = gtk_check_button_new_with_label("Visible");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(editable_check), is_editable);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(visible_check), is_visible);
    current_properties.editable_check = editable_check;
    current_properties.visible_check = visible_check;
    
    // Text fields
    GtkWidget *placeholder_label = gtk_label_new("Placeholder Text:");
    GtkWidget *max_len_label = gtk_label_new("Max Length:");
    GtkWidget *default_text_label = gtk_label_new("Text:");
    GtkWidget *placeholder_entry = gtk_entry_new();
    GtkWidget *max_len_entry = gtk_entry_new();
    GtkWidget *default_text_entry = gtk_entry_new();
    
    if (placeholder) {
        gtk_entry_set_text(GTK_ENTRY(placeholder_entry), placeholder);
    }
    gtk_entry_set_text(GTK_ENTRY(max_len_entry), max_len_str);
    if (current_text) {
        gtk_entry_set_text(GTK_ENTRY(default_text_entry), current_text);
    }
    
    current_properties.placeholder_entry = placeholder_entry;
    current_properties.max_len_entry = max_len_entry;
    current_properties.default_text_entry = default_text_entry;
    
    // Add widgets to grid (same layout as creation dialog)
    int row = 0;
    
    // Position and size
    gtk_grid_attach(GTK_GRID(grid), x_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), y_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), width_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), width_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), height_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), height_entry, 1, row++, 1, 1);
    
    // Entry properties
    gtk_grid_attach(GTK_GRID(grid), editable_check, 0, row++, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), visible_check, 0, row++, 2, 1);
    
    gtk_grid_attach(GTK_GRID(grid), placeholder_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), placeholder_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), max_len_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), max_len_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), default_text_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), default_text_entry, 1, row++, 1, 1);
    
    // Add title and grid to vbox
    gtk_box_pack_start(GTK_BOX(vbox), title, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), grid, FALSE, FALSE, 0);
    
    // Add the vbox to the content area
    gtk_container_add(GTK_CONTAINER(content), vbox);
    
    // Show all widgets
    gtk_widget_show_all(content);
    
    // Enable Apply and Remove buttons
    gtk_widget_set_sensitive(app_data->apply_button, TRUE);
    gtk_widget_set_sensitive(app_data->remove_button, TRUE);
}

// Create property form for check button widget
static void create_property_form_for_check_button(AppData *app_data, GtkWidget *widget) {
    GtkWidget *content = app_data->properties_content;
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    
    // Store the widget being edited
    current_properties.widget = widget;
    current_properties.container = vbox;
    
    // Get current properties
    const gchar *label_text = gtk_button_get_label(GTK_BUTTON(widget));
    gboolean is_active = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
    
    // Get position and size
    GtkWidget *parent = gtk_widget_get_parent(widget);
    gint x = 0, y = 0, width = 0, height = 0;
    
    if (GTK_IS_FIXED(parent)) {
        gtk_container_child_get(GTK_CONTAINER(parent), widget, "x", &x, "y", &y, NULL);
    }
    
    gtk_widget_get_size_request(widget, &width, &height);
    
    // Create grid for form layout
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
    
    current_properties.form_grid = grid;
    
    // Title
    GtkWidget *title = gtk_label_new("Check Button Properties");
    gtk_widget_set_halign(title, GTK_ALIGN_START);
    
    // Format values as strings
    char x_str[32], y_str[32], width_str[32], height_str[32];
    sprintf(x_str, "%d", x);
    sprintf(y_str, "%d", y);
    sprintf(width_str, "%d", width);
    sprintf(height_str, "%d", height);
    
    // Position fields
    GtkWidget *x_label = gtk_label_new("X Position:");
    GtkWidget *y_label = gtk_label_new("Y Position:");
    GtkWidget *x_entry = gtk_entry_new();
    GtkWidget *y_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(x_entry), x_str);
    gtk_entry_set_text(GTK_ENTRY(y_entry), y_str);
    current_properties.x_entry = x_entry;
    current_properties.y_entry = y_entry;
    
    // Size fields
    GtkWidget *width_label = gtk_label_new("Width:");
    GtkWidget *height_label = gtk_label_new("Height:");
    GtkWidget *width_entry = gtk_entry_new();
    GtkWidget *height_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(width_entry), width_str);
    gtk_entry_set_text(GTK_ENTRY(height_entry), height_str);
    current_properties.width_entry = width_entry;
    current_properties.height_entry = height_entry;
    
    // Check button specific fields
    GtkWidget *label_text_label = gtk_label_new("Label Text:");
    GtkWidget *label_text_entry = gtk_entry_new();
    if (label_text) {
        gtk_entry_set_text(GTK_ENTRY(label_text_entry), label_text);
    }
    current_properties.label_text_entry = label_text_entry;
    
    GtkWidget *active_check = gtk_check_button_new_with_label("Checked");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(active_check), is_active);
    
    // Add widgets to grid
    int row = 0;
    
    // Position and size
    gtk_grid_attach(GTK_GRID(grid), x_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), y_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), width_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), width_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), height_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), height_entry, 1, row++, 1, 1);
    
    // Check button specific properties
    gtk_grid_attach(GTK_GRID(grid), label_text_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_text_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), active_check, 0, row++, 2, 1);
    
    // Add title and grid to vbox
    gtk_box_pack_start(GTK_BOX(vbox), title, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), grid, FALSE, FALSE, 0);
    
    // Add the vbox to the content area
    gtk_container_add(GTK_CONTAINER(content), vbox);
    
    // Show all widgets
    gtk_widget_show_all(content);
    
    // // Enable Apply and Remove buttons
    // gtk_widget_set_sensitive(app_data->apply_button, TRUE);
    // gtk_widget_set_sensitive(app_data->remove_button, TRUE);
}

// Create property form for spin button widget
static void create_property_form_for_spin_button(AppData *app_data, GtkWidget *widget) {
    GtkWidget *content = app_data->properties_content;
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    
    // Store the widget being edited
    current_properties.widget = widget;
    current_properties.container = vbox;
    
    // Get current properties
    gdouble value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(widget));
    gdouble min = 0, max = 0, step = 0;
    gtk_spin_button_get_range(GTK_SPIN_BUTTON(widget), &min, &max);
    gtk_spin_button_get_increments(GTK_SPIN_BUTTON(widget), &step, NULL);
    guint digits = gtk_spin_button_get_digits(GTK_SPIN_BUTTON(widget));
    gboolean numeric = gtk_spin_button_get_numeric(GTK_SPIN_BUTTON(widget));
    gboolean wrap = gtk_spin_button_get_wrap(GTK_SPIN_BUTTON(widget));
    
    // Get position and size
    GtkWidget *parent = gtk_widget_get_parent(widget);
    gint x = 0, y = 0, width = 0, height = 0;
    
    if (GTK_IS_FIXED(parent)) {
        gtk_container_child_get(GTK_CONTAINER(parent), widget, "x", &x, "y", &y, NULL);
    }
    
    gtk_widget_get_size_request(widget, &width, &height);
    
    // Create grid for form layout
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
    
    current_properties.form_grid = grid;
    
    // Title
    GtkWidget *title = gtk_label_new("Spin Button Properties");
    gtk_widget_set_halign(title, GTK_ALIGN_START);
    
    // Format values as strings
    char x_str[32], y_str[32], width_str[32], height_str[32];
    char value_str[32], min_str[32], max_str[32], step_str[32], digits_str[32];
    
    sprintf(x_str, "%d", x);
    sprintf(y_str, "%d", y);
    sprintf(width_str, "%d", width);
    sprintf(height_str, "%d", height);
    sprintf(value_str, "%g", value);
    sprintf(min_str, "%g", min);
    sprintf(max_str, "%g", max);
    sprintf(step_str, "%g", step);
    sprintf(digits_str, "%u", digits);
    
    // Position fields
    GtkWidget *x_label = gtk_label_new("X Position:");
    GtkWidget *y_label = gtk_label_new("Y Position:");
    GtkWidget *x_entry = gtk_entry_new();
    GtkWidget *y_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(x_entry), x_str);
    gtk_entry_set_text(GTK_ENTRY(y_entry), y_str);
    current_properties.x_entry = x_entry;
    current_properties.y_entry = y_entry;
    
    // Size fields
    GtkWidget *width_label = gtk_label_new("Width:");
    GtkWidget *height_label = gtk_label_new("Height:");
    GtkWidget *width_entry = gtk_entry_new();
    GtkWidget *height_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(width_entry), width_str);
    gtk_entry_set_text(GTK_ENTRY(height_entry), height_str);
    current_properties.width_entry = width_entry;
    current_properties.height_entry = height_entry;
    
    // Spin button specific fields
    GtkWidget *value_label = gtk_label_new("Value:");
    GtkWidget *min_label = gtk_label_new("Minimum:");
    GtkWidget *max_label = gtk_label_new("Maximum:");
    GtkWidget *step_label = gtk_label_new("Step Increment:");
    GtkWidget *digits_label = gtk_label_new("Digits:");
    
    GtkWidget *value_entry = gtk_entry_new();
    GtkWidget *min_entry = gtk_entry_new();
    GtkWidget *max_entry = gtk_entry_new();
    GtkWidget *step_entry = gtk_entry_new();
    GtkWidget *digits_entry = gtk_entry_new();
    
    gtk_entry_set_text(GTK_ENTRY(value_entry), value_str);
    gtk_entry_set_text(GTK_ENTRY(min_entry), min_str);
    gtk_entry_set_text(GTK_ENTRY(max_entry), max_str);
    gtk_entry_set_text(GTK_ENTRY(step_entry), step_str);
    gtk_entry_set_text(GTK_ENTRY(digits_entry), digits_str);
    
    // Store entries for later retrieval
    current_properties.value_entry = value_entry;
    current_properties.min_entry = min_entry;
    current_properties.max_entry = max_entry;
    current_properties.step_entry = step_entry;
    current_properties.digits_entry = digits_entry;
    
    // Checkboxes
    GtkWidget *numeric_check = gtk_check_button_new_with_label("Numeric Only");
    GtkWidget *wrap_check = gtk_check_button_new_with_label("Wrap Around");
    
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(numeric_check), numeric);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wrap_check), wrap);
    
    current_properties.numeric_check = numeric_check;
    current_properties.wrap_check = wrap_check;
    
    // Add widgets to grid
    int row = 0;
    
    // Position and size
    gtk_grid_attach(GTK_GRID(grid), x_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), y_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), width_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), width_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), height_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), height_entry, 1, row++, 1, 1);
    
    // Spin button specific properties
    gtk_grid_attach(GTK_GRID(grid), value_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), value_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), min_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), min_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), max_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), max_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), step_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), step_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), digits_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), digits_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), numeric_check, 0, row++, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), wrap_check, 0, row++, 2, 1);
    
    // Add title and grid to vbox
    gtk_box_pack_start(GTK_BOX(vbox), title, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), grid, FALSE, FALSE, 0);
    
    // Add the vbox to the content area
    gtk_container_add(GTK_CONTAINER(content), vbox);
    
    // Show all widgets
    gtk_widget_show_all(content);
    
    // Enable Apply and Remove buttons
    gtk_widget_set_sensitive(app_data->apply_button, TRUE);
    gtk_widget_set_sensitive(app_data->remove_button, TRUE);
}



// Create property form for normal button widget
//For editing
static void create_property_form_for_button_normal(AppData *app_data, GtkWidget *widget) {
    GtkWidget *content = app_data->properties_content;
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    
    //get the struct
    btn* b = (btn*) get_widget_structure(app_data, widget);
    // Store the widget being edited
    current_properties.widget = widget;
    current_properties.container = vbox;
    
    // Get current properties
    const gchar *label_text = gtk_button_get_label(GTK_BUTTON(widget));
    const gchar *tooltip_text = gtk_widget_get_tooltip_text(widget);
    const gchar *name = gtk_widget_get_name(widget);
    
    // Get position and size
    GtkWidget *parent = gtk_widget_get_parent(widget);
    gint x = 0, y = 0, width = 0, height = 0;
    
    if (GTK_IS_FIXED(parent)) {
        gtk_container_child_get(GTK_CONTAINER(parent), widget, "x", &x, "y", &y, NULL);
    }
    
    gtk_widget_get_size_request(widget, &width, &height);
    
    // Create grid for form layout
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
    
    current_properties.form_grid = grid;
    
    // Title
    GtkWidget *title = gtk_label_new("Button Properties");
    gtk_widget_set_halign(title, GTK_ALIGN_START);
    
    // Format values as strings
    char x_str[32], y_str[32], width_str[32], height_str[32];
    sprintf(x_str, "%d", x);
    sprintf(y_str, "%d", y);
    sprintf(width_str, "%d", width);
    sprintf(height_str, "%d", height);
    
    // Position fields
    GtkWidget *x_label = gtk_label_new("X Position:");
    GtkWidget *y_label = gtk_label_new("Y Position:");
    GtkWidget *x_entry = gtk_entry_new();
    GtkWidget *y_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(x_entry), x_str);
    gtk_entry_set_text(GTK_ENTRY(y_entry), y_str);
    current_properties.x_entry = x_entry;
    current_properties.y_entry = y_entry;
    
    // Size fields
    GtkWidget *width_label = gtk_label_new("Width:");
    GtkWidget *height_label = gtk_label_new("Height:");
    GtkWidget *width_entry = gtk_entry_new();
    GtkWidget *height_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(width_entry), width_str);
    gtk_entry_set_text(GTK_ENTRY(height_entry), height_str);
    current_properties.width_entry = width_entry;
    current_properties.height_entry = height_entry;
    
    // Name field
    GtkWidget *name_label = gtk_label_new("Button Name:");
    GtkWidget *name_entry = gtk_entry_new();
    if (name && *name) {
        gtk_entry_set_text(GTK_ENTRY(name_entry), name);
    }
    current_properties.name_entry = name_entry;
    // Button specific fields
    GtkWidget *label_text_label = gtk_label_new("Button Text:");
    GtkWidget *label_text_entry = gtk_entry_new();
    if (label_text) {
        gtk_entry_set_text(GTK_ENTRY(label_text_entry), label_text);
    }
    current_properties.label_entry = label_text_entry;
    
    // Tooltip field
    GtkWidget *tooltip_label = gtk_label_new("Tooltip:");
    GtkWidget *tooltip_entry = gtk_entry_new();
    if (tooltip_text) {
        gtk_entry_set_text(GTK_ENTRY(tooltip_entry), tooltip_text);
    }
    
    // Image path field
    GtkWidget *image_path_label = gtk_label_new("Image Path:");
    GtkWidget *path_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget *image_path_entry = gtk_entry_new();
    GtkWidget *browse_button = gtk_button_new_with_label("Browse...");
    
    // Get button image if it exists
    GtkWidget *current_image = gtk_button_get_image(GTK_BUTTON(widget));
    if (current_image != NULL) {
        gchar *image_path = g_object_get_data(G_OBJECT(widget), "image_file_path");
        if (image_path) {
            gtk_entry_set_text(GTK_ENTRY(image_path_entry), image_path);
        }
    }
    
    gtk_box_pack_start(GTK_BOX(path_box), image_path_entry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(path_box), browse_button, FALSE, FALSE, 0);
    
    g_signal_connect(browse_button, "clicked", G_CALLBACK(NULL), image_path_entry);
    
    // Mnemonic option
    GtkWidget *has_mnemonic_check = gtk_check_button_new_with_label("Has Mnemonic (_X for shortcuts)");
    gboolean has_mnemonic = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(widget), "has_mnemonic"));
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(has_mnemonic_check), has_mnemonic);
    
    // Style fields
    GtkWidget *font_label = gtk_label_new("Font:");
    GtkWidget *font_entry = gtk_entry_new();
    
    GtkWidget *color_label = gtk_label_new("Text Color:");
    GtkWidget *color_entry = gtk_entry_new();
    
    GtkWidget *size_label = gtk_label_new("Font Size:");
    GtkWidget *size_entry = gtk_entry_new();
    
    GtkWidget *bold_check = gtk_check_button_new_with_label("Bold");
    
    GtkWidget *bgcolor_label = gtk_label_new("Background Color:");
    GtkWidget *bgcolor_entry = gtk_entry_new();
    
    current_properties.font_entry = font_entry;
    current_properties.color_entry = color_entry;
    current_properties.size_entry = size_entry;
    current_properties.bold_check = bold_check;
    current_properties.bgcolor_entry = bgcolor_entry;

 
        // gtk_entry_set_text(GTK_ENTRY(font_entry), style->police ? style->police : "Sans");
        gtk_entry_set_text(GTK_ENTRY(font_entry), b->police ? b->police : "Sans");
        g_print("\nPolice: %s", b->police);
        gtk_entry_set_text(GTK_ENTRY(color_entry), b->color ? b->color : "#000000");
        char* taille_str;
        sprintf(taille_str,"%d",b->taille);
        gtk_entry_set_text(GTK_ENTRY(size_entry), b->taille ? taille_str : "12");        
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(bold_check), b->gras ? TRUE : FALSE);
        gtk_entry_set_text(GTK_ENTRY(bgcolor_entry), b->bgcolor ? b->bgcolor : "#FFFFFF");
    
    // Store references for property retrieval in apply function
    GtkWidget *dummy_entries[8];  // Temp storage for entries that don't have dedicated fields
    int dummy_index = 0;
    
    // Save these in a consistent place for the apply function
    dummy_entries[dummy_index++] = name_entry;
    dummy_entries[dummy_index++] = tooltip_entry;
    dummy_entries[dummy_index++] = image_path_entry;
    dummy_entries[dummy_index++] = font_entry;
    dummy_entries[dummy_index++] = color_entry;
    dummy_entries[dummy_index++] = size_entry;
    dummy_entries[dummy_index++] = bgcolor_entry;
    g_object_set_data_full(G_OBJECT(widget), "property_entries", 
                           g_memdup(dummy_entries, sizeof(GtkWidget*) * dummy_index),
                           g_free);
    
    g_object_set_data(G_OBJECT(widget), "has_mnemonic_check", has_mnemonic_check);
    g_object_set_data(G_OBJECT(widget), "bold_check", bold_check);
    
    // Add widgets to grid
    int row = 0;
    
    // Name field
    gtk_grid_attach(GTK_GRID(grid), name_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), name_entry, 1, row++, 1, 1);
    
    // Position and size
    gtk_grid_attach(GTK_GRID(grid), x_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), y_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), width_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), width_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), height_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), height_entry, 1, row++, 1, 1);
    
    // Button specific properties
    gtk_grid_attach(GTK_GRID(grid), label_text_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_text_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), tooltip_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), tooltip_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), image_path_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), path_box, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), has_mnemonic_check, 0, row++, 2, 1);
    
    // Style properties
    gtk_grid_attach(GTK_GRID(grid), font_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), font_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), color_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), color_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), size_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), size_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), bold_check, 0, row++, 2, 1);
    
    gtk_grid_attach(GTK_GRID(grid), bgcolor_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), bgcolor_entry, 1, row++, 1, 1);
    
    // Add title and grid to vbox
    gtk_box_pack_start(GTK_BOX(vbox), title, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), grid, FALSE, FALSE, 0);
    
    // Add the vbox to the content area
    gtk_container_add(GTK_CONTAINER(content), vbox);
    
    // Show all widgets
    gtk_widget_show_all(content);
    
    // Enable Apply and Remove buttons
    gtk_widget_set_sensitive(app_data->apply_button, TRUE);
    gtk_widget_set_sensitive(app_data->remove_button, TRUE);
}

// Create property form for normal button widget
// static void create_property_form_for_button_normal(AppData *app_data, GtkWidget *widget) {
//     GtkWidget *content = app_data->properties_content;
//     GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    
//     // Store the widget being edited
//     current_properties.widget = widget;
//     current_properties.container = vbox;
    
//     // Get current properties
//     const gchar *label_text = gtk_button_get_label(GTK_BUTTON(widget));
    
//     // Get position and size
//     GtkWidget *parent = gtk_widget_get_parent(widget);
//     gint x = 0, y = 0, width = 0, height = 0;
    
//     if (GTK_IS_FIXED(parent)) {
//         gtk_container_child_get(GTK_CONTAINER(parent), widget, "x", &x, "y", &y, NULL);
//     }
    
//     gtk_widget_get_size_request(widget, &width, &height);
    
//     // Create grid for form layout
//     GtkWidget *grid = gtk_grid_new();
//     gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
//     gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
//     gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
    
//     current_properties.form_grid = grid;
    
//     // Title
//     GtkWidget *title = gtk_label_new("Button Properties");
//     gtk_widget_set_halign(title, GTK_ALIGN_START);
    
//     // Format values as strings
//     char x_str[32], y_str[32], width_str[32], height_str[32];
//     sprintf(x_str, "%d", x);
//     sprintf(y_str, "%d", y);
//     sprintf(width_str, "%d", width);
//     sprintf(height_str, "%d", height);
    
//     // Position fields
//     GtkWidget *x_label = gtk_label_new("X Position:");
//     GtkWidget *y_label = gtk_label_new("Y Position:");
//     GtkWidget *x_entry = gtk_entry_new();
//     GtkWidget *y_entry = gtk_entry_new();
//     gtk_entry_set_text(GTK_ENTRY(x_entry), x_str);
//     gtk_entry_set_text(GTK_ENTRY(y_entry), y_str);
//     current_properties.x_entry = x_entry;
//     current_properties.y_entry = y_entry;
    
//     // Size fields
//     GtkWidget *width_label = gtk_label_new("Width:");
//     GtkWidget *height_label = gtk_label_new("Height:");
//     GtkWidget *width_entry = gtk_entry_new();
//     GtkWidget *height_entry = gtk_entry_new();
//     gtk_entry_set_text(GTK_ENTRY(width_entry), width_str);
//     gtk_entry_set_text(GTK_ENTRY(height_entry), height_str);
//     current_properties.width_entry = width_entry;
//     current_properties.height_entry = height_entry;
    
//     // Button specific fields
//     GtkWidget *label_text_label = gtk_label_new("Button Text:");
//     GtkWidget *label_text_entry = gtk_entry_new();
//     if (label_text) {
//         gtk_entry_set_text(GTK_ENTRY(label_text_entry), label_text);
//     }
//     current_properties.label_entry = label_text_entry;
    
//     // Add widgets to grid
//     int row = 0;
    
//     // Position and size
//     gtk_grid_attach(GTK_GRID(grid), x_label, 0, row, 1, 1);
//     gtk_grid_attach(GTK_GRID(grid), x_entry, 1, row++, 1, 1);
    
//     gtk_grid_attach(GTK_GRID(grid), y_label, 0, row, 1, 1);
//     gtk_grid_attach(GTK_GRID(grid), y_entry, 1, row++, 1, 1);
    
//     gtk_grid_attach(GTK_GRID(grid), width_label, 0, row, 1, 1);
//     gtk_grid_attach(GTK_GRID(grid), width_entry, 1, row++, 1, 1);
    
//     gtk_grid_attach(GTK_GRID(grid), height_label, 0, row, 1, 1);
//     gtk_grid_attach(GTK_GRID(grid), height_entry, 1, row++, 1, 1);
    
//     // Button specific properties
//     gtk_grid_attach(GTK_GRID(grid), label_text_label, 0, row, 1, 1);
//     gtk_grid_attach(GTK_GRID(grid), label_text_entry, 1, row++, 1, 1);
    
//     // Add title and grid to vbox
//     gtk_box_pack_start(GTK_BOX(vbox), title, FALSE, FALSE, 5);
//     gtk_box_pack_start(GTK_BOX(vbox), grid, FALSE, FALSE, 0);
    
//     // Add the vbox to the content area
//     gtk_container_add(GTK_CONTAINER(content), vbox);
    
//     // Show all widgets
//     gtk_widget_show_all(content);
    
//     // Enable Apply and Remove buttons
//     gtk_widget_set_sensitive(app_data->apply_button, TRUE);
//     gtk_widget_set_sensitive(app_data->remove_button, TRUE);
// }

// Create property form for spin button widget
// static void create_property_form_for_button_normal(AppData *app_data, GtkWidget *widget) {
//     GtkWidget *content = app_data->properties_content;
//     GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    
//     // Store the widget being edited
//     current_properties.widget = widget;
//     current_properties.container = vbox;
    
//     // Get current properties
//     gdouble value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(widget));
//     gdouble min = 0, max = 0, step = 0;
//     gtk_spin_button_get_range(GTK_SPIN_BUTTON(widget), &min, &max);
//     gtk_spin_button_get_increments(GTK_SPIN_BUTTON(widget), &step, NULL);
//     guint digits = gtk_spin_button_get_digits(GTK_SPIN_BUTTON(widget));
//     gboolean numeric = gtk_spin_button_get_numeric(GTK_SPIN_BUTTON(widget));
//     gboolean wrap = gtk_spin_button_get_wrap(GTK_SPIN_BUTTON(widget));
    
//     // Get position and size
//     GtkWidget *parent = gtk_widget_get_parent(widget);
//     gint x = 0, y = 0, width = 0, height = 0;
    
//     if (GTK_IS_FIXED(parent)) {
//         gtk_container_child_get(GTK_CONTAINER(parent), widget, "x", &x, "y", &y, NULL);
//     }
    
//     gtk_widget_get_size_request(widget, &width, &height);
    
//     // Create grid for form layout
//     GtkWidget *grid = gtk_grid_new();
//     gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
//     gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
//     gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
    
//     current_properties.form_grid = grid;
    
//     // Title
//     GtkWidget *title = gtk_label_new("Spin Button Properties");
//     gtk_widget_set_halign(title, GTK_ALIGN_START);
    
//     // Format values as strings
//     char x_str[32], y_str[32], width_str[32], height_str[32];
//     char value_str[32], min_str[32], max_str[32], step_str[32], digits_str[32];
    
//     sprintf(x_str, "%d", x);
//     sprintf(y_str, "%d", y);
//     sprintf(width_str, "%d", width);
//     sprintf(height_str, "%d", height);
//     sprintf(value_str, "%g", value);
//     sprintf(min_str, "%g", min);
//     sprintf(max_str, "%g", max);
//     sprintf(step_str, "%g", step);
//     sprintf(digits_str, "%u", digits);
    
//     // Position fields
//     GtkWidget *x_label = gtk_label_new("X Position:");
//     GtkWidget *y_label = gtk_label_new("Y Position:");
//     GtkWidget *x_entry = gtk_entry_new();
//     GtkWidget *y_entry = gtk_entry_new();
//     gtk_entry_set_text(GTK_ENTRY(x_entry), x_str);
//     gtk_entry_set_text(GTK_ENTRY(y_entry), y_str);
//     current_properties.x_entry = x_entry;
//     current_properties.y_entry = y_entry;
    
//     // Size fields
//     GtkWidget *width_label = gtk_label_new("Width:");
//     GtkWidget *height_label = gtk_label_new("Height:");
//     GtkWidget *width_entry = gtk_entry_new();
//     GtkWidget *height_entry = gtk_entry_new();
//     gtk_entry_set_text(GTK_ENTRY(width_entry), width_str);
//     gtk_entry_set_text(GTK_ENTRY(height_entry), height_str);
//     current_properties.width_entry = width_entry;
//     current_properties.height_entry = height_entry;
    
//     // Spin button specific fields
//     GtkWidget *value_label = gtk_label_new("Value:");
//     GtkWidget *min_label = gtk_label_new("Minimum:");
//     GtkWidget *max_label = gtk_label_new("Maximum:");
//     GtkWidget *step_label = gtk_label_new("Step Increment:");
//     GtkWidget *digits_label = gtk_label_new("Digits:");
    
//     GtkWidget *value_entry = gtk_entry_new();
//     GtkWidget *min_entry = gtk_entry_new();
//     GtkWidget *max_entry = gtk_entry_new();
//     GtkWidget *step_entry = gtk_entry_new();
//     GtkWidget *digits_entry = gtk_entry_new();
    
//     gtk_entry_set_text(GTK_ENTRY(value_entry), value_str);
//     gtk_entry_set_text(GTK_ENTRY(min_entry), min_str);
//     gtk_entry_set_text(GTK_ENTRY(max_entry), max_str);
//     gtk_entry_set_text(GTK_ENTRY(step_entry), step_str);
//     gtk_entry_set_text(GTK_ENTRY(digits_entry), digits_str);
    
//     // Store entries for later retrieval
//     current_properties.value_entry = value_entry;
//     current_properties.min_entry = min_entry;
//     current_properties.max_entry = max_entry;
//     current_properties.step_entry = step_entry;
//     current_properties.digits_entry = digits_entry;
    
//     // Checkboxes
//     GtkWidget *numeric_check = gtk_check_button_new_with_label("Numeric Only");
//     GtkWidget *wrap_check = gtk_check_button_new_with_label("Wrap Around");
    
//     gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(numeric_check), numeric);
//     gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wrap_check), wrap);
    
//     current_properties.numeric_check = numeric_check;
//     current_properties.wrap_check = wrap_check;
    
//     // Add widgets to grid
//     int row = 0;
    
//     // Position and size
//     gtk_grid_attach(GTK_GRID(grid), x_label, 0, row, 1, 1);
//     gtk_grid_attach(GTK_GRID(grid), x_entry, 1, row++, 1, 1);
    
//     gtk_grid_attach(GTK_GRID(grid), y_label, 0, row, 1, 1);
//     gtk_grid_attach(GTK_GRID(grid), y_entry, 1, row++, 1, 1);
    
//     gtk_grid_attach(GTK_GRID(grid), width_label, 0, row, 1, 1);
//     gtk_grid_attach(GTK_GRID(grid), width_entry, 1, row++, 1, 1);
    
//     gtk_grid_attach(GTK_GRID(grid), height_label, 0, row, 1, 1);
//     gtk_grid_attach(GTK_GRID(grid), height_entry, 1, row++, 1, 1);
    
//     // Spin button specific properties
//     gtk_grid_attach(GTK_GRID(grid), value_label, 0, row, 1, 1);
//     gtk_grid_attach(GTK_GRID(grid), value_entry, 1, row++, 1, 1);
    
//     gtk_grid_attach(GTK_GRID(grid), min_label, 0, row, 1, 1);
//     gtk_grid_attach(GTK_GRID(grid), min_entry, 1, row++, 1, 1);
    
//     gtk_grid_attach(GTK_GRID(grid), max_label, 0, row, 1, 1);
//     gtk_grid_attach(GTK_GRID(grid), max_entry, 1, row++, 1, 1);
    
//     gtk_grid_attach(GTK_GRID(grid), step_label, 0, row, 1, 1);
//     gtk_grid_attach(GTK_GRID(grid), step_entry, 1, row++, 1, 1);
    
//     gtk_grid_attach(GTK_GRID(grid), digits_label, 0, row, 1, 1);
//     gtk_grid_attach(GTK_GRID(grid), digits_entry, 1, row++, 1, 1);
    
//     gtk_grid_attach(GTK_GRID(grid), numeric_check, 0, row++, 2, 1);
//     gtk_grid_attach(GTK_GRID(grid), wrap_check, 0, row++, 2, 1);
    
//     // Add title and grid to vbox
//     gtk_box_pack_start(GTK_BOX(vbox), title, FALSE, FALSE, 5);
//     gtk_box_pack_start(GTK_BOX(vbox), grid, FALSE, FALSE, 0);
    
//     // Add the vbox to the content area
//     gtk_container_add(GTK_CONTAINER(content), vbox);
    
//     // Show all widgets
//     gtk_widget_show_all(content);
    
//     // Enable Apply and Remove buttons
//     gtk_widget_set_sensitive(app_data->apply_button, TRUE);
//     gtk_widget_set_sensitive(app_data->remove_button, TRUE);
// }
// Create property form for TextView widget
static void create_property_form_for_text_view(AppData *app_data, GtkWidget *widget) {
    GtkWidget *content = app_data->properties_content;
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    
    // Store the widget being edited
    current_properties.widget = widget;
    current_properties.container = vbox;
    
    // Get the MonTextView data
    MonTextView *textview = g_object_get_data(G_OBJECT(widget), "textview_data");
    if (!textview) {
        g_print("Error: Could not find TextView data\n");
        return;
    }
    
    // Get text content
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widget));
    GtkTextIter start, end;
    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_get_end_iter(buffer, &end);
    gchar *current_text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
    
    // Get position and size
    gint x = textview->Crd.x;
    gint y = textview->Crd.y;
    gint width = textview->dim.width;
    gint height = textview->dim.height;
    
    // Create grid for form layout
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
    
    current_properties.form_grid = grid;
    
    // Title
    GtkWidget *title = gtk_label_new("TextView Properties");
    gtk_widget_set_halign(title, GTK_ALIGN_START);
    
    // Format values as strings
    char x_str[32], y_str[32], width_str[32], height_str[32];
    sprintf(x_str, "%d", x);
    sprintf(y_str, "%d", y);
    sprintf(width_str, "%d", width);
    sprintf(height_str, "%d", height);
    
    // Position fields
    GtkWidget *x_label = gtk_label_new("X Position:");
    GtkWidget *y_label = gtk_label_new("Y Position:");
    GtkWidget *x_entry = gtk_entry_new();
    GtkWidget *y_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(x_entry), x_str);
    gtk_entry_set_text(GTK_ENTRY(y_entry), y_str);
    current_properties.x_entry = x_entry;
    current_properties.y_entry = y_entry;
    
    // Size fields
    GtkWidget *width_label = gtk_label_new("Width:");
    GtkWidget *height_label = gtk_label_new("Height:");
    GtkWidget *width_entry = gtk_entry_new();
    GtkWidget *height_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(width_entry), width_str);
    gtk_entry_set_text(GTK_ENTRY(height_entry), height_str);
    current_properties.width_entry = width_entry;
    current_properties.height_entry = height_entry;
    
    // Text content field
    GtkWidget *text_label = gtk_label_new("Text Content:");
    GtkWidget *text_view = gtk_text_view_new();
    GtkWidget *text_scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(text_scroll), 100);
    gtk_container_add(GTK_CONTAINER(text_scroll), text_view);
    
    // Set current text content
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_buffer_set_text(buffer, textview->texte ? textview->texte : "", -1);
    
    // TextView-specific properties
    GtkWidget *editable_check = gtk_check_button_new_with_label("Editable");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(editable_check), 
                                gtk_text_view_get_editable(GTK_TEXT_VIEW(widget)));
    
    // Wrap mode field
    GtkWidget *wrap_label = gtk_label_new("Wrap Mode:");
    GtkWidget *wrap_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(wrap_combo), "None");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(wrap_combo), "Character");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(wrap_combo), "Word");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(wrap_combo), "Word & Character");
    
    // Set current wrap mode
    GtkWrapMode current_wrap = gtk_text_view_get_wrap_mode(GTK_TEXT_VIEW(widget));
    gtk_combo_box_set_active(GTK_COMBO_BOX(wrap_combo), current_wrap);
    
    // Store references for property retrieval in apply function
    g_object_set_data(G_OBJECT(widget), "prop_text_view", text_view);
    g_object_set_data(G_OBJECT(widget), "prop_editable_check", editable_check);
    g_object_set_data(G_OBJECT(widget), "prop_wrap_combo", wrap_combo);
    
    // Add widgets to grid
    int row = 0;
    
    // Position and size
    gtk_grid_attach(GTK_GRID(grid), x_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), y_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), width_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), width_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), height_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), height_entry, 1, row++, 1, 1);
    
    // TextView-specific properties
    gtk_grid_attach(GTK_GRID(grid), text_label, 0, row, 2, 1);
    row++;
    gtk_grid_attach(GTK_GRID(grid), text_scroll, 0, row, 2, 3);
    row += 3;
    
    gtk_grid_attach(GTK_GRID(grid), editable_check, 0, row++, 2, 1);
    
    gtk_grid_attach(GTK_GRID(grid), wrap_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), wrap_combo, 1, row++, 1, 1);
    
    // Add title and grid to vbox
    gtk_box_pack_start(GTK_BOX(vbox), title, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), grid, FALSE, FALSE, 0);
    
    // Add the vbox to the content area
    gtk_container_add(GTK_CONTAINER(content), vbox);
    
    // Show all widgets
    gtk_widget_show_all(content);
    
    // Enable Apply and Remove buttons
    gtk_widget_set_sensitive(app_data->apply_button, TRUE);
    gtk_widget_set_sensitive(app_data->remove_button, TRUE);
    
    // Clean up
    g_free(current_text);
}


// Create property form for switch widget
static void create_property_form_for_switch(AppData *app_data, GtkWidget *widget) {
    GtkWidget *content = app_data->properties_content;
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    
    // Store the widget being edited
    current_properties.widget = widget;
    current_properties.container = vbox;
    
    // Get current properties
    gboolean active = gtk_switch_get_active(GTK_SWITCH(widget));
    
    // Get position and size
    GtkWidget *parent = gtk_widget_get_parent(widget);
    gint x = 0, y = 0, width = 0, height = 0;
    
    if (GTK_IS_FIXED(parent)) {
        gtk_container_child_get(GTK_CONTAINER(parent), widget, "x", &x, "y", &y, NULL);
    }
    
    gtk_widget_get_size_request(widget, &width, &height);
    
    // Create grid for form layout
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
    
    current_properties.form_grid = grid;
    
    // Title
    GtkWidget *title = gtk_label_new("Switch Properties");
    gtk_widget_set_halign(title, GTK_ALIGN_START);
    
    // Format values as strings
    char x_str[32], y_str[32], width_str[32], height_str[32];
    sprintf(x_str, "%d", x);
    sprintf(y_str, "%d", y);
    sprintf(width_str, "%d", width);
    sprintf(height_str, "%d", height);
    
    // Position fields
    GtkWidget *x_label = gtk_label_new("X Position:");
    GtkWidget *y_label = gtk_label_new("Y Position:");
    GtkWidget *x_entry = gtk_entry_new();
    GtkWidget *y_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(x_entry), x_str);
    gtk_entry_set_text(GTK_ENTRY(y_entry), y_str);
    current_properties.x_entry = x_entry;
    current_properties.y_entry = y_entry;
    
    // Size fields
    GtkWidget *width_label = gtk_label_new("Width:");
    GtkWidget *height_label = gtk_label_new("Height:");
    GtkWidget *width_entry = gtk_entry_new();
    GtkWidget *height_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(width_entry), width_str);
    gtk_entry_set_text(GTK_ENTRY(height_entry), height_str);
    current_properties.width_entry = width_entry;
    current_properties.height_entry = height_entry;
    
    // Switch specific fields
    GtkWidget *active_check = gtk_check_button_new_with_label("Active (ON)");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(active_check), active);
    current_properties.active_check = active_check;
    
    // Add widgets to grid
    int row = 0;
    
    // Position and size
    gtk_grid_attach(GTK_GRID(grid), x_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), y_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), width_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), width_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), height_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), height_entry, 1, row++, 1, 1);
    
    // Switch specific properties
    gtk_grid_attach(GTK_GRID(grid), active_check, 0, row++, 2, 1);
    
    // Add title and grid to vbox
    gtk_box_pack_start(GTK_BOX(vbox), title, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), grid, FALSE, FALSE, 0);
    
    // Add the vbox to the content area
    gtk_container_add(GTK_CONTAINER(content), vbox);
    
    // Show all widgets
    gtk_widget_show_all(content);
    
    // Enable Apply and Remove buttons
    gtk_widget_set_sensitive(app_data->apply_button, TRUE);
    gtk_widget_set_sensitive(app_data->remove_button, TRUE);
}


// Create property form for password entry widget
static void create_password_entry_form(AppData *app_data, GtkWidget *widget) {
    GtkWidget *content = app_data->properties_content;
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    
    // Store the widget being edited
    current_properties.widget = widget;
    current_properties.container = vbox;
    
    // Get current properties
    const gchar *current_text = gtk_entry_get_text(GTK_ENTRY(widget));
    const gchar *placeholder = gtk_entry_get_placeholder_text(GTK_ENTRY(widget));
    gunichar invisible_char = gtk_entry_get_invisible_char(GTK_ENTRY(widget));
    
    // Get position and size
    GtkWidget *parent = gtk_widget_get_parent(widget);
    gint x = 0, y = 0, width = 0, height = 0;
    
    if (GTK_IS_FIXED(parent)) {
        gtk_container_child_get(GTK_CONTAINER(parent), widget, "x", &x, "y", &y, NULL);
    }
    
    gtk_widget_get_size_request(widget, &width, &height);
    
    // Create grid for form layout - same as used in the creation dialog
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
    
    current_properties.form_grid = grid;
    
    // Title
    GtkWidget *title = gtk_label_new("Password Entry Properties");
    gtk_widget_set_halign(title, GTK_ALIGN_START);
    
    // Format values as strings
    char x_str[32], y_str[32], width_str[32], height_str[32], invisible_char_str[8];
    sprintf(x_str, "%d", x);
    sprintf(y_str, "%d", y);
    sprintf(width_str, "%d", width);
    sprintf(height_str, "%d", height);
    g_unichar_to_utf8(invisible_char, invisible_char_str);
    invisible_char_str[1] = '\0'; // Ensure null termination
    
    // Position fields
    GtkWidget *x_label = gtk_label_new("X Position:");
    GtkWidget *y_label = gtk_label_new("Y Position:");
    GtkWidget *x_entry = gtk_entry_new();
    GtkWidget *y_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(x_entry), x_str);
    gtk_entry_set_text(GTK_ENTRY(y_entry), y_str);
    current_properties.x_entry = x_entry;
    current_properties.y_entry = y_entry;
    
    // Size fields
    GtkWidget *width_label = gtk_label_new("Width:");
    GtkWidget *height_label = gtk_label_new("Height:");
    GtkWidget *width_entry = gtk_entry_new();
    GtkWidget *height_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(width_entry), width_str);
    gtk_entry_set_text(GTK_ENTRY(height_entry), height_str);
    current_properties.width_entry = width_entry;
    current_properties.height_entry = height_entry;
    
    // Password fields
    GtkWidget *placeholder_label = gtk_label_new("Placeholder Text:");
    GtkWidget *mask_char_label = gtk_label_new("Mask Character:");
    GtkWidget *text_label = gtk_label_new("Password:");
    GtkWidget *placeholder_entry = gtk_entry_new();
    GtkWidget *mask_char_entry = gtk_entry_new();
    GtkWidget *text_entry = gtk_entry_new();
    
    if (placeholder) {
        gtk_entry_set_text(GTK_ENTRY(placeholder_entry), placeholder);
    }
    gtk_entry_set_text(GTK_ENTRY(mask_char_entry), invisible_char_str);
    gtk_entry_set_max_length(GTK_ENTRY(mask_char_entry), 1);
    if (current_text) {
        gtk_entry_set_text(GTK_ENTRY(text_entry), current_text);
    }
    
    current_properties.placeholder_entry = placeholder_entry;
    current_properties.mask_char_entry = mask_char_entry;
    current_properties.default_text_entry = text_entry;
    
    // Add widgets to grid (same layout as creation dialog)
    int row = 0;
    
    // Position and size
    gtk_grid_attach(GTK_GRID(grid), x_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), y_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), width_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), width_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), height_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), height_entry, 1, row++, 1, 1);
    
    // Password properties
    gtk_grid_attach(GTK_GRID(grid), placeholder_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), placeholder_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), mask_char_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), mask_char_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), text_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), text_entry, 1, row++, 1, 1);
    
    // Add title and grid to vbox
    gtk_box_pack_start(GTK_BOX(vbox), title, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), grid, FALSE, FALSE, 0);
    
    // Add the vbox to the content area
    gtk_container_add(GTK_CONTAINER(content), vbox);
    
    // Show all widgets
    gtk_widget_show_all(content);
    
    // Enable Apply and Remove buttons
    gtk_widget_set_sensitive(app_data->apply_button, TRUE);
    gtk_widget_set_sensitive(app_data->remove_button, TRUE);
}

void register_widget_for_property_editing(GtkWidget *widget, AppData *app_data) {
    g_print("Registering widget %p for property editing\n", widget);
    g_signal_connect(widget, "button-press-event", G_CALLBACK(on_widget_button_press_select), app_data);
}

// Add this function to property_panel.h

static inline void  on_browse_image_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *entry = GTK_WIDGET(user_data);
    GtkWidget *dialog;
    
    dialog = gtk_file_chooser_dialog_new("Open Image",
                                       NULL,
                                       GTK_FILE_CHOOSER_ACTION_OPEN,
                                       "Cancel", GTK_RESPONSE_CANCEL,
                                       "Open", GTK_RESPONSE_ACCEPT,
                                       NULL);
    
    gtk_window_set_default_size(GTK_WINDOW(dialog), 600, 400);

    // Add filters for image files
    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "Image Files");
    gtk_file_filter_add_pixbuf_formats(filter);
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);
    
    // Show the dialog and get the result
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        gtk_entry_set_text(GTK_ENTRY(entry), filename);
        g_free(filename);
    }
    
    gtk_widget_destroy(dialog);
}

// Create property form for image widget
static void create_property_form_for_image(AppData *app_data, GtkWidget *widget) {


    GtkWidget *content = app_data->properties_content;
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    
    // Store the widget being edited
    current_properties.widget = widget;
    current_properties.container = vbox;
    
    // Get the actual image from the event box
    GtkWidget *image_widget = g_object_get_data(G_OBJECT(widget), "image_widget");
    if (!image_widget || !GTK_IS_IMAGE(image_widget)) {
        g_print("Could not find image widget in event box\n");
        return;
    }
    
    // Get position and size
    GtkWidget *parent = gtk_widget_get_parent(widget);
    gint x = 0, y = 0, width = 0, height = 0;
    
    if (GTK_IS_FIXED(parent)) {
        gtk_container_child_get(GTK_CONTAINER(parent), widget, "x", &x, "y", &y, NULL);
    }
    
    gtk_widget_get_size_request(widget, &width, &height);
    
    // Create grid for form layout
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
    
    current_properties.form_grid = grid;
    
    // Title
    GtkWidget *title = gtk_label_new("Image Properties");
    gtk_widget_set_halign(title, GTK_ALIGN_START);
    
    // Format values as strings
    char x_str[32], y_str[32], width_str[32], height_str[32];
    sprintf(x_str, "%d", x);
    sprintf(y_str, "%d", y);
    sprintf(width_str, "%d", width);
    sprintf(height_str, "%d", height);
    
    // Position fields
    GtkWidget *x_label = gtk_label_new("X Position:");
    GtkWidget *y_label = gtk_label_new("Y Position:");
    GtkWidget *x_entry = gtk_entry_new();
    GtkWidget *y_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(x_entry), x_str);
    gtk_entry_set_text(GTK_ENTRY(y_entry), y_str);
    current_properties.x_entry = x_entry;
    current_properties.y_entry = y_entry;
    
    // Size fields
    GtkWidget *width_label = gtk_label_new("Width:");
    GtkWidget *height_label = gtk_label_new("Height:");
    GtkWidget *width_entry = gtk_entry_new();
    GtkWidget *height_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(width_entry), width_str);
    gtk_entry_set_text(GTK_ENTRY(height_entry), height_str);
    current_properties.width_entry = width_entry;
    current_properties.height_entry = height_entry;
    
    // Image path field
    GtkWidget *path_label = gtk_label_new("Image Path:");
    GtkWidget *path_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget *path_entry = gtk_entry_new();
    GtkWidget *browse_button = gtk_button_new_with_label("Browse...");
    
    // Get image path
    gchar *image_path = g_object_get_data(G_OBJECT(widget), "image_file_path");
    if (image_path) {
        gtk_entry_set_text(GTK_ENTRY(path_entry), image_path);
    }
    
    gtk_box_pack_start(GTK_BOX(path_box), path_entry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(path_box), browse_button, FALSE, FALSE, 0);
    
    g_signal_connect(browse_button, "clicked", G_CALLBACK(NULL), path_entry);
    
    // Keep aspect ratio checkbox
    GtkWidget *aspect_check = gtk_check_button_new_with_label("Keep Aspect Ratio");
    gboolean keep_aspect = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(widget), "keep_aspect"));
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(aspect_check), keep_aspect);
    
    // Store references for later use in apply button
    current_properties.label_entry = path_entry;  // Reuse label_entry for image path
    current_properties.active_check = aspect_check;  // Reuse active_check for aspect ratio
    
    // Add widgets to grid
    int row = 0;
    
    // Position and size
    gtk_grid_attach(GTK_GRID(grid), x_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), y_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), width_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), width_entry, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), height_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), height_entry, 1, row++, 1, 1);
    
    // Image specific properties
    gtk_grid_attach(GTK_GRID(grid), path_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), path_box, 1, row++, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), aspect_check, 0, row++, 2, 1);
    
    // Add title and grid to vbox
    gtk_box_pack_start(GTK_BOX(vbox), title, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), grid, FALSE, FALSE, 0);
    
    // Add the vbox to the content area
    gtk_container_add(GTK_CONTAINER(content), vbox);
    
    // Show all widgets
    gtk_widget_show_all(content);
    
    // Enable Apply and Remove buttons
    gtk_widget_set_sensitive(app_data->apply_button, TRUE);
    gtk_widget_set_sensitive(app_data->remove_button, TRUE);
}
// Create property form based on widget type
static void create_property_form_for_widget(AppData *app_data, GtkWidget *widget) {
    // Clear existing content
    clear_properties_panel(app_data);
    g_print("\nWidget type: %s\n", G_OBJECT_TYPE_NAME(widget));
    
    // CRITICAL FIX: Make the event box check first and complete
    if (GTK_IS_EVENT_BOX(widget) && g_object_get_data(G_OBJECT(widget), "image_widget")) {
        g_print("\nImage Event Box detected\n");
        create_property_form_for_image(app_data, widget);
    }
    else if (GTK_IS_SPIN_BUTTON(widget)) {
        g_print("\nSpin تشاسيرشاستيرشتيسا\n");
        create_property_form_for_spin_button(app_data, widget);
    } else if (GTK_IS_SWITCH(widget)) {
        g_print("\nSwitch\n");
        create_property_form_for_switch(app_data, widget);
    }
        else if (GTK_IS_TEXT_VIEW(widget)) {
        g_print("\nTextView\n");
        create_property_form_for_text_view(app_data, widget);
    }
    else if (GTK_IS_EVENT_BOX(widget)){
        g_print("\nEvent Box\n");
        // create_property_form_for_image(app_data, widget);
    }

    else if (GTK_IS_EVENT_BOX(widget) && 
    g_object_get_data(G_OBJECT(widget), "image_widget")) {
    create_property_form_for_image(app_data, widget);
    }

    // Create form based on widget type
     else if (GTK_IS_ENTRY(widget)) {
        if (!gtk_entry_get_visibility(GTK_ENTRY(widget))) {
            create_password_entry_form(app_data, widget);
        } else {
            create_basic_entry_form(app_data, widget);
        }
    } else if (GTK_IS_BUTTON(widget)) {
        if (GTK_IS_CHECK_BUTTON(widget)) {
            // Create checkbox form (not implemented in this example)
            // GtkWidget *label = gtk_label_new("Checkbox properties coming soon");
            // clear_properties_panel(app_data);
            create_property_form_for_check_button(app_data, widget);
            // gtk_container_add(GTK_CONTAINER(app_data->properties_content), label);
            // current_properties.container = label;
            // gtk_widget_show_all(app_data->properties_content);
            
            // Enable Remove button only
            gtk_widget_set_sensitive(app_data->apply_button, TRUE);
            gtk_widget_set_sensitive(app_data->remove_button, TRUE);
            g_print("\nCheck Button\n");
        }
        else if (GTK_IS_SWITCH(widget)) {
            g_print("\nsadasdasdkjabsldjasdbkajdbjasSwitch\n");
            create_property_form_for_switch(app_data, widget);
        }
        else if (GTK_IS_SPIN_BUTTON(widget)) {
            g_print("\nSpin Button\n");
            create_property_form_for_spin_button(app_data, widget);
        }
        else {
            // Create button form (not implemented in this example)
            // GtkWidget *label = gtk_label_new("Button properties coming soon");
            // gtk_container_add(GTK_CONTAINER(app_data->properties_content), label);
            // current_properties.container = label;
            // gtk_widget_show_all(app_data->properties_content);
            g_print("iammmm heeeeeeeere\n");

            //edit a button normale
            create_property_form_for_button_normal(app_data, widget);     

            // Enable Remove button only
            // gtk_widget_set_sensitive(app_data->apply_button, FALSE);
            // gtk_widget_set_sensitive(app_data->remove_button, TRUE);
        }
    
    
    } else if (GTK_IS_LABEL(widget)) {
        // GTK_IS_RADIO_BUTTON()
        // Create label form (not implemented in this example)
        GtkWidget *label = gtk_label_new("Label properties coming soon");
        gtk_container_add(GTK_CONTAINER(app_data->properties_content), label);
        current_properties.container = label;
        gtk_widget_show_all(app_data->properties_content);
        
        // Enable Remove button only
        gtk_widget_set_sensitive(app_data->apply_button, FALSE);
        gtk_widget_set_sensitive(app_data->remove_button, TRUE);
    } else {
        // Unsupported widget type
        GtkWidget *label = gtk_label_new("This widget type is not supported yet");
        gtk_container_add(GTK_CONTAINER(app_data->properties_content), label);
        current_properties.container = label;
        gtk_widget_show_all(app_data->properties_content);
        
        // Enable Remove button only
        gtk_widget_set_sensitive(app_data->apply_button, FALSE);
        gtk_widget_set_sensitive(app_data->remove_button, TRUE);
    }
}

// Apply changes to the selected widget
static void on_apply_clicked(GtkButton *button, gpointer user_data) {
    AppData *app_data = (AppData *)user_data;
    GtkWidget *widget = current_properties.widget;
    
    if (!widget) {
        g_print("No widget selected!\n");
        return;
    }
    
    // Get common position and size values
    gint x = atoi(gtk_entry_get_text(GTK_ENTRY(current_properties.x_entry)));
    gint y = atoi(gtk_entry_get_text(GTK_ENTRY(current_properties.y_entry)));
    gint width = atoi(gtk_entry_get_text(GTK_ENTRY(current_properties.width_entry)));
    gint height = atoi(gtk_entry_get_text(GTK_ENTRY(current_properties.height_entry)));
    gchar* font = gtk_entry_get_text(GTK_ENTRY(current_properties.font_entry));
    gchar* color = gtk_entry_get_text(GTK_ENTRY(current_properties.color_entry));
    gint size = atoi(gtk_entry_get_text(GTK_ENTRY(current_properties.size_entry)));
    gint bold = atoi(gtk_entry_get_text(GTK_ENTRY(current_properties.bold_check)));
    gchar* bgcolor = gtk_entry_get_text(GTK_ENTRY(current_properties.bgcolor_entry));
    gchar *label_text = gtk_entry_get_text(GTK_ENTRY(current_properties.label_entry));
    gboolean is_active = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(current_properties.active_check));


    //update the btn structure
    btn* bb = get_widget_structure(app_data, widget);
    bb->dim->height = height;
    bb->dim->width = width;
    bb->pos->x = x;
    bb->pos->y = y;
    bb->color = strdup(color);
    bb->bgcolor = strdup(bgcolor);
    bb->police = strdup(font);
    bb->gras = bold;
    printf("\nbb->label: %s, label_text: %s", bb->label, label_text);
    strcpy(bb->label, label_text);
    strcpy(bb->tooltip, label_text);
    bb->isChecked = is_active;

    // Update the widget
    // Update position if widget is in a fixed container
    GtkWidget *parent = gtk_widget_get_parent(widget);
    if (GTK_IS_FIXED(parent)) {
        gtk_fixed_move(GTK_FIXED(parent), widget, x, y);
    }
    gtk_widget_set_size_request(widget, width, height);
    gtk_button_set_label(GTK_BUTTON(widget), label_text);
    gtk_widget_set_tooltip_text(widget, label_text);
    // gtk_widget_modify_font(widget, pango_font_description_from_string(font));
    // gtk_widget_modify_fg(widget, GTK_STATE_NORMAL, get_gdk_color(color));
    // gtk_widget_modify_bg(widget, GTK_STATE_NORMAL, get_gdk_color(bgcolor));
    // gtk_widget_modify_base(widget, GTK_STATE_NORMAL, get_gdk_color(bgcolor));
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget), is_active);


    if (GTK_IS_SPIN_BUTTON(widget)) {
        // Apply spin button specific properties
        g_print("\nApplying spin button properties\n");
        gdouble value = atof(gtk_entry_get_text(GTK_ENTRY(current_properties.value_entry)));
        gdouble min = atof(gtk_entry_get_text(GTK_ENTRY(current_properties.min_entry)));
        gdouble max = atof(gtk_entry_get_text(GTK_ENTRY(current_properties.max_entry)));
        gdouble step = atof(gtk_entry_get_text(GTK_ENTRY(current_properties.step_entry)));
        guint digits = atoi(gtk_entry_get_text(GTK_ENTRY(current_properties.digits_entry)));
        gboolean numeric = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(current_properties.numeric_check));
        gboolean wrap = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(current_properties.wrap_check));
        
        // Update properties
        gtk_spin_button_set_range(GTK_SPIN_BUTTON(widget), min, max);
        gtk_spin_button_set_increments(GTK_SPIN_BUTTON(widget), step, step * 10);
        gtk_spin_button_set_digits(GTK_SPIN_BUTTON(widget), digits);
        gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(widget), numeric);
        gtk_spin_button_set_wrap(GTK_SPIN_BUTTON(widget), wrap);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(widget), value);
        bb->sp->digits = digits;
        bb->sp->step = step;
        bb->sp->borneInf = min;
        bb->sp->borneSup = max;
    }
    // Apply widget-specific properties

        else if (GTK_IS_TEXT_VIEW(widget)) {
        // Get the MonTextView data
        MonTextView *textview = g_object_get_data(G_OBJECT(widget), "textview_data");
        if (!textview) {
            g_print("Error: TextView data not found!\n");
            return;
        }
        
        // Get property values from form widgets
        GtkWidget *prop_text_view = g_object_get_data(G_OBJECT(widget), "prop_text_view");
        GtkWidget *editable_check = g_object_get_data(G_OBJECT(widget), "prop_editable_check");
        GtkWidget *wrap_combo = g_object_get_data(G_OBJECT(widget), "prop_wrap_combo");
        
        // Get text content
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(prop_text_view));
        GtkTextIter start, end;
        gtk_text_buffer_get_start_iter(buffer, &start);
        gtk_text_buffer_get_end_iter(buffer, &end);
        gchar *new_text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
        
        // Update position and size in the MonTextView structure
        textview->Crd.x = x;
        textview->Crd.y = y;
        textview->dim.width = width;
        textview->dim.height = height;
        
        // Update text content
        if (textview->texte) {
            g_free(textview->texte);
        }
        textview->texte = g_strdup(new_text);
        g_free(new_text);
        
        // Apply changes to the widget
        
        // Update widget text
        buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widget));
        gtk_text_buffer_set_text(buffer, textview->texte, -1);
        
        // Update editable state
        gtk_text_view_set_editable(GTK_TEXT_VIEW(widget), 
                                  gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(editable_check)));
        
        // Update wrap mode
        int wrap_index = gtk_combo_box_get_active(GTK_COMBO_BOX(wrap_combo));
        gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(widget), (GtkWrapMode)wrap_index);
        
        // Apply size - THIS IS CRITICAL
        gtk_widget_set_size_request(widget, width, height);
        
        // Apply position if in fixed container - THIS IS CRITICAL
        GtkWidget *parent = gtk_widget_get_parent(widget);
        if (GTK_IS_FIXED(parent)) {
            // Make sure we're moving the actual widget, not just updating internal data
            gtk_fixed_move(GTK_FIXED(parent), widget, x, y);
        }
        
        // Force a redraw
        gtk_widget_queue_resize(widget);
    }
    else if (GTK_IS_ENTRY(widget)) {
        if (!gtk_entry_get_visibility(GTK_ENTRY(widget))) {
            // Password entry
            const gchar *placeholder = gtk_entry_get_text(GTK_ENTRY(current_properties.placeholder_entry));
            const gchar *mask_char_text = gtk_entry_get_text(GTK_ENTRY(current_properties.mask_char_entry));
            const gchar *text = gtk_entry_get_text(GTK_ENTRY(current_properties.default_text_entry));
            
            gtk_entry_set_placeholder_text(GTK_ENTRY(widget), placeholder);
            
            if (mask_char_text && mask_char_text[0]) {
                gtk_entry_set_invisible_char(GTK_ENTRY(widget), mask_char_text[0]);
            }
            
            gtk_entry_set_text(GTK_ENTRY(widget), text);
        } else {
            // Basic entry
            gboolean is_editable = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(current_properties.editable_check));
            gboolean is_visible = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(current_properties.visible_check));
            const gchar *placeholder = gtk_entry_get_text(GTK_ENTRY(current_properties.placeholder_entry));
            gint max_len = atoi(gtk_entry_get_text(GTK_ENTRY(current_properties.max_len_entry)));
            const gchar *text = gtk_entry_get_text(GTK_ENTRY(current_properties.default_text_entry));
            
            gtk_editable_set_editable(GTK_EDITABLE(widget), is_editable);
            gtk_entry_set_visibility(GTK_ENTRY(widget), is_visible);
            
            if (max_len > 0) {
                gtk_entry_set_max_length(GTK_ENTRY(widget), max_len);
            }
            
            gtk_entry_set_placeholder_text(GTK_ENTRY(widget), placeholder);
            gtk_entry_set_text(GTK_ENTRY(widget), text);
        }
    }
    // Add to on_apply_clicked function after the entry handling
    
    else if (GTK_IS_SWITCH(widget)) {
        // Apply switch specific properties
        gboolean active = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(current_properties.active_check));
        gtk_switch_set_active(GTK_SWITCH(widget), active);
    }
    // Inside the on_apply_clicked function, add this case after the GTK_IS_ENTRY case:
    else if (GTK_IS_CHECK_BUTTON(widget)) {
        // Apply check button specific properties
        // const gchar *label_text = gtk_entry_get_text(GTK_ENTRY(current_properties.label_text_entry));
        // gboolean is_active = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(current_properties.active_check));
        
        // gtk_button_set_label(GTK_BUTTON(widget), label_text);
        // gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget), is_active);
        // update btn structure
        // strcpy(bb->label, label_text);
        // bb->isChecked = is_active;
    }
    // Handle other widget types here
    
    // Refresh display
    gtk_widget_show_all(app_data->preview_area);
}

// Remove the selected widget
static void on_remove_clicked(GtkButton *button, gpointer user_data) {
    AppData *app_data = (AppData *)user_data;
    GtkWidget *widget = current_properties.widget;
    
    if (!widget) {
        g_print("No widget selected!\n");
        return;
    }
    
    // Remove from both tree structures
    remove_widget_from_both_trees(app_data, widget);
    
    // Remove the widget from its parent
    GtkWidget *parent = gtk_widget_get_parent(widget);
    if (parent != NULL) {
        gtk_container_remove(GTK_CONTAINER(parent), widget);
    }
    
    // If this widget is a container, remove it from containers list
    GList *link = g_list_find(app_data->containers, widget);
    if (link != NULL) {
        app_data->containers = g_list_delete_link(app_data->containers, link);
        update_container_combo(app_data);
    }
    
    // Clear the properties panel
    clear_properties_panel(app_data);
    
    // Update UI
    gtk_widget_show_all(app_data->preview_area);
    
    // Clear selected widget
    app_data->selected_widget = NULL;
    
    // Disable buttons
    gtk_widget_set_sensitive(app_data->apply_button, FALSE);
    gtk_widget_set_sensitive(app_data->remove_button, FALSE);
}

// Register a widget for property editing - make it extern so it can be called from other files
// void register_widget_for_property_editing(GtkWidget *widget, AppData *app_data) {
//     g_print("Registering widget %p for property editing\n", widget);
//     g_signal_connect(widget, "button-press-event", G_CALLBACK(on_widget_button_press_select), app_data);
// }

// Initialize the property panel
static void init_property_panel(AppData *app_data) {
    // Create scrolled window for content
    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),
                                  GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    
    // Create content area for properties
    app_data->properties_content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_set_border_width(GTK_CONTAINER(app_data->properties_content), 10);
    
    // Add a default label
    // GtkWidget *default_label = gtk_label_new("Select a widget to edit properties");
    // gtk_box_pack_start(GTK_BOX(app_data->properties_content), default_label, FALSE, FALSE, 5);
    
    // Add the content box to the scrolled window
    gtk_container_add(GTK_CONTAINER(scroll), app_data->properties_content);
    
    // Create buttons box
    GtkWidget *buttons_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    
    // Create Apply and Remove buttons
    app_data->apply_button = gtk_button_new_with_label("Apply");
    app_data->remove_button = gtk_button_new_with_label("Remove");
    
    // Connect signals
    g_signal_connect(app_data->apply_button, "clicked", G_CALLBACK(on_apply_clicked), app_data);
    g_signal_connect(app_data->remove_button, "clicked", G_CALLBACK(on_remove_clicked), app_data);
    
    // Initially disable buttons
    gtk_widget_set_sensitive(app_data->apply_button, FALSE);
    gtk_widget_set_sensitive(app_data->remove_button, FALSE);
    
    // Add buttons to the box
    gtk_box_pack_start(GTK_BOX(buttons_box), app_data->apply_button, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(buttons_box), app_data->remove_button, TRUE, TRUE, 5);
    
    // Add the scrolled window and buttons to the properties panel
    gtk_box_pack_start(GTK_BOX(app_data->properties_panel), scroll, TRUE, TRUE, 0);
    gtk_box_pack_end(GTK_BOX(app_data->properties_panel), buttons_box, FALSE, FALSE, 5);
    
    // Connect click handler to preview area for deselection
    g_signal_connect(app_data->preview_area, "button-press-event", 
                    G_CALLBACK(on_preview_area_click), app_data);
}

// Register preview area background click to clear selection
static void register_preview_area_click(GtkWidget *preview_area, AppData *app_data) {
    g_signal_connect(preview_area, "button-press-event", 
                    G_CALLBACK(on_preview_area_click), app_data);
}

#endif /* PROPERTY_PANEL_H */

// #endif // PROPERTY_PANEL_H
