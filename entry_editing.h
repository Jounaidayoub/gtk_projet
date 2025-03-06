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
    // Create the widget
    GtkWidget *entry_widget = creer_entry_basic(entry_basic);
    
    // Register for property editing
    register_widget_for_property_editing(entry_widget, app_data);
    
    // Add to both tree structures with the structure reference
    add_widget_to_both_trees(app_data, entry_widget, "Basic Entry", 
                           entry_basic->container, 0, entry_basic);
    
    return entry_widget;
}

GtkWidget *creer_entry_pass_with_editing(entry_type_password *entry_password, struct _AppData *app_data) {
    // Create the widget
    GtkWidget *entry_widget = creer_entry_pass(entry_password);
    
    // Register for property editing
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

#endif /* ENTRY_EDITING_H */
