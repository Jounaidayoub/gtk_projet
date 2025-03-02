#ifndef ENTRY_EDITING_H
#define ENTRY_EDITING_H

#include "app_data.h"  // Include app_data.h first
#include "entry.h"
#include "property_panel.h"

// Implementation of functions that were forward-declared in entry.h
GtkWidget *creer_entry_basic_with_editing(entry_type_basic *entry_basic, struct _AppData *app_data) {
    GtkWidget *entry_widget = creer_entry_basic(entry_basic);
    
    // Register for property editing
    register_widget_for_property_editing(entry_widget, app_data);
    
    return entry_widget;
}

GtkWidget *creer_entry_pass_with_editing(entry_type_password *entry_password, struct _AppData *app_data) {
    GtkWidget *entry_widget = creer_entry_pass(entry_password);
    
    // Register for property editing
    register_widget_for_property_editing(entry_widget, app_data);
    
    return entry_widget;
}

#endif /* ENTRY_EDITING_H */
