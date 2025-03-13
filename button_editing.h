#ifndef BUTTON_EDITING_H
#define BUTTON_EDITING_H

#include <gtk/gtk.h>
#include "app_data.h"
#include "button.h"
#include "tree_sync.h"
#include "property_panel.h"

// Function to create a button with editing capabilities
GtkWidget* create_button_with_editing(btn *button, AppData *app_data, const char *widget_type_str) {
    GtkWidget *widget = NULL;
    
    // Create the button using the existing function
    button = creer_button(button);
    
    // Get the actual GTK widget
    widget = button->button;
    
    // Register the widget for property editing
    register_widget_for_property_editing(widget, app_data);
    
    // Add widget to both trees
    add_widget_to_both_trees(app_data, widget, widget_type_str, 
                           button->container, FALSE, button);
    
    return widget;
}

// Function for normal button with editing capabilities
GtkWidget* create_normal_button_with_editing(btn *button, AppData *app_data) {
    return create_button_with_editing(button, app_data, "Normal Button");
}

// Function for radio button with editing capabilities
GtkWidget* create_radio_button_with_editing(btn *button, AppData *app_data) {
    return create_button_with_editing(button, app_data, "Radio Button");
}

// Function for checkbox button with editing capabilities
GtkWidget* create_checkbox_button_with_editing(btn *button, AppData *app_data) {
    return create_button_with_editing(button, app_data, "Checkbox");
}

// Function for toggle button with editing capabilities
GtkWidget* create_toggle_button_with_editing(btn *button, AppData *app_data) {
    return create_button_with_editing(button, app_data, "Toggle Button");
}

// Function for spin button with editing capabilities
GtkWidget* create_spin_button_with_editing(btn *button, AppData *app_data) {
    return create_button_with_editing(button, app_data, "Spin Button");
}

// Function for switch button with editing capabilities
GtkWidget* create_switch_button_with_editing(btn *button, AppData *app_data) {
    return create_button_with_editing(button, app_data, "Switch");
}

#endif /* BUTTON_EDITING_H */
