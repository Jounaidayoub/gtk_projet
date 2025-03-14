#ifndef TREE_SYNC_H
#define TREE_SYNC_H

#include <gtk/gtk.h>
#include "app_data.h"
#include "containers_list.h"
#include "widget_props.h"

// Forward declaration for callback function
static void on_show_arbre_clicked(GtkWidget *button, gpointer data);

// Generate a unique name for a widget based on its type and pointer    
static gchar* generate_widget_name(const gchar *widget_type, GtkWidget *widget) {
    gchar* name = g_malloc(MAX_NAME_LENGTH);
    g_snprintf(name, MAX_NAME_LENGTH, "%s_%p", widget_type, (void*)widget);
    return name;
}

// Recursive function to find a widget in the tree
gboolean find_widget_in_tree_recursive(GtkTreeModel *model, GtkTreeIter *parent_iter, 
                                    GtkWidget *search_widget, GtkTreeIter *result_iter) {
    GtkTreeIter iter;
    gboolean valid;
    
    if (parent_iter == NULL) {
        // Start at the root level
        valid = gtk_tree_model_get_iter_first(model, &iter);
    } else {
        // Start with the children of parent_iter
        valid = gtk_tree_model_iter_children(model, &iter, parent_iter);
    }
    
    while (valid) {
        GtkWidget *current_widget;
        gtk_tree_model_get(model, &iter, 1, &current_widget, -1);
        
        if (current_widget == search_widget) {
            // Found the widget
            *result_iter = iter;
            return TRUE;
        }
        
        // Check if this node has children
        if (gtk_tree_model_iter_has_child(model, &iter)) {
            // Recursively search children
            if (find_widget_in_tree_recursive(model, &iter, search_widget, result_iter)) {
                return TRUE;
            }
        }
        
        // Move to next sibling
        valid = gtk_tree_model_iter_next(model, &iter);
    }
    
    return FALSE;
}

// After adding or removing widgets, we need to update the Arbre view
void update_arbre_view(AppData *app_data) {
    if (app_data->arbre_scroll && app_data->widget_tree) {
        // Remove the old view
        GList *children = gtk_container_get_children(GTK_CONTAINER(app_data->arbre_scroll));
        if (children) {
            gtk_widget_destroy(GTK_WIDGET(children->data));
            g_list_free(children);
        }
        
        // Create and add the new updated view
        GtkWidget *arbre_view = creer_arborescence_visuelle(app_data->widget_tree);
        gtk_container_add(GTK_CONTAINER(app_data->arbre_scroll), arbre_view);
        app_data->arbre_view = arbre_view;
        
        // Show the new view
        gtk_widget_show_all(app_data->arbre_scroll);
    }
}

// Updated: Add a widget to both tree structures with widget structure reference
void add_widget_to_both_trees(AppData *app_data, GtkWidget *widget, 
                             const gchar *widget_type_str, GtkWidget *parent_container, 
                             gboolean is_container, void *widget_structure) {
    // Generate a unique name for the widget
    gchar *widget_name = generate_widget_name(widget_type_str, widget);
    gtk_widget_set_name(widget, widget_name);
    
    // PART 1: Add to GtkTreeStore for visualization in hierarchy view
    GtkTreeIter parent_iter, new_iter;
    gboolean found = FALSE;
    
    if (parent_container && parent_container != app_data->preview_area) {
        // Find parent in GtkTreeStore - need to search more thoroughly
        GtkTreeModel *model = GTK_TREE_MODEL(app_data->hierarchy_store);
        
        // Recursive search function to find a widget in the tree
        found = find_widget_in_tree_recursive(model, NULL, parent_container, &parent_iter);
    }
    
    if (found) {
        // Add as child in hierarchy view
        gtk_tree_store_append(app_data->hierarchy_store, &new_iter, &parent_iter);
    } else {
        // Add at root level in hierarchy view
        gtk_tree_store_append(app_data->hierarchy_store, &new_iter, NULL);
    }
    
    // Set data in GtkTreeStore
    gtk_tree_store_set(app_data->hierarchy_store, &new_iter, 
                      0, g_strdup_printf("%s", widget_type_str), 
                      1, widget, -1);
    
    // PART 2: Add to Arbre structure
    // Determine parent name for Arbre insertion
    const gchar *parent_name = "preview_area";  // Default parent
    
    if (parent_container && parent_container != app_data->preview_area) {
        // Get the parent container's name
        char *found_name = getNomConteneurByWidget(app_data->widget_tree, parent_container);
        if (found_name != NULL) {
            parent_name = found_name;
        }
    }
    
    // Create new node and insert into Arbre
    Arbre *new_node = allouer_arbre(widget_name, widget, NULL, NULL, is_container);
    
    // Determine widget type enum
    WidgetType widget_type;
    
    // Convert string type to enum type
    if (strstr(widget_type_str, "Basic Entry") != NULL) {
        widget_type = WIDGET_ENTRY_BASIC;
    } else if (strstr(widget_type_str, "Password Entry") != NULL) {
        widget_type = WIDGET_ENTRY_PASSWORD;
    } else if (strstr(widget_type_str, "Box") != NULL) {
        widget_type = WIDGET_BOX;
    } else if (strstr(widget_type_str, "Label") != NULL) {
        widget_type = WIDGET_LABEL;
    } else if (strstr(widget_type_str, "Button") != NULL) {
        widget_type = WIDGET_BUTTON;
    } else if (strstr(widget_type_str, "radio") != NULL) {
        widget_type = WIDGET_RADIO;
    } else if (strstr(widget_type_str, "radioList") != NULL) {
        widget_type = WIDGET_RADIO_LIST;
    } else {
        widget_type = WIDGET_UNKNOWN;
    }
    
    // Set the widget type in the node
    new_node->type = widget_type;
    
    // Store the widget structure directly
    new_node->widget_data = widget_structure;
    
    // Populate properties from the structure
    if (widget_structure) {
        populate_widget_properties(new_node, widget_structure, widget_type);
    }
    
    app_data->widget_tree = insererArbre(app_data->widget_tree, new_node, parent_name);
    
    // Cleanup
    g_free(widget_name);
    
    // Expand the tree view to show all items
    gtk_tree_view_expand_all(GTK_TREE_VIEW(app_data->hierarchy_view));
    
    // After adding to both structures, update the Arbre view
    update_arbre_view(app_data);
}

// Remove a widget from both tree structures
void remove_widget_from_both_trees(AppData *app_data, GtkWidget *widget) {
    // PART 1: Remove from GtkTreeStore
    GtkTreeModel *model = GTK_TREE_MODEL(app_data->hierarchy_store);
    GtkTreeIter iter;
    gboolean found = FALSE;
    
    if (gtk_tree_model_get_iter_first(model, &iter)) {
        do {
            GtkWidget *iter_widget;
            gtk_tree_model_get(model, &iter, 1, &iter_widget, -1);
            
            if (iter_widget == widget) {
                found = TRUE;
                break;
            }
            
            // Check children recursively
            if (gtk_tree_model_iter_has_child(model, &iter)) {
                GtkTreeIter child_iter;
                if (gtk_tree_model_iter_children(model, &child_iter, &iter)) {
                    do {
                        gtk_tree_model_get(model, &child_iter, 1, &iter_widget, -1);
                        if (iter_widget == widget) {
                            iter = child_iter;  // Point to the child
                            found = TRUE;
                            break;
                        }
                    } while (!found && gtk_tree_model_iter_next(model, &child_iter));
                }
            }
        } while (!found && gtk_tree_model_iter_next(model, &iter));
    }
    
    if (found) {
        gtk_tree_store_remove(app_data->hierarchy_store, &iter);
    }
    
    // PART 2: Remove from Arbre
    // Note: For now, we don't have a direct removal function for Arbre
    // The most straightforward approach would be to recreate the Arbre
    // This would be inefficient but simple to implement
    
    // TODO: Implement a proper node removal from Arbre structure
    // For now, we'll just print a message
    g_print("Note: Widget removed from GtkTreeStore but not from Arbre structure\n");
    
    // After removing from GtkTreeStore, update the Arbre view
    update_arbre_view(app_data);
}

// Create a visual representation of the Arbre structure for debugging
GtkWidget* create_arbre_view(AppData *app_data) {
    // Create a scrolled window to hold the tree view
    GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled),
                                  GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    
    // Create a tree view representation of the Arbre structure
    GtkWidget *tree_view = creer_arborescence_visuelle(app_data->widget_tree);
    gtk_container_add(GTK_CONTAINER(scrolled), tree_view);
    
    return scrolled;
}

// Add a debug button to show the Arbre structure
void add_arbre_debug_button(AppData *app_data, GtkWidget *container) {
    GtkWidget *button = gtk_button_new_with_label("Show Arbre Tree Structure");
    
    g_signal_connect(button, "clicked", G_CALLBACK(on_show_arbre_clicked), app_data);
    
    gtk_container_add(GTK_CONTAINER(container), button);
}

// Callback for debug button
static void on_show_arbre_clicked(GtkWidget *button, gpointer data) {
    AppData *app_data = (AppData *)data;
    
    // Create a new dialog window
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Arbre Tree Structure",
                                                 GTK_WINDOW(app_data->window),
                                                 GTK_DIALOG_MODAL,
                                                 "Close", GTK_RESPONSE_CLOSE,
                                                 NULL);
    
    // Get the content area
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    
    // Create the Arbre view
    GtkWidget *arbre_view = create_arbre_view(app_data);
    gtk_widget_set_size_request(arbre_view, 400, 500);
    
    // Add the view to the dialog
    gtk_container_add(GTK_CONTAINER(content_area), arbre_view);
    
    // Also print to console for debugging
    g_print("Current Arbre Tree Structure:\n");
    afficher_arbre(app_data->widget_tree, 0);
    
    // Show all content
    gtk_widget_show_all(dialog);
    
    // Run the dialog
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// Test Arbre functionality
static void test_arbre_functions(AppData *app_data) {
    GtkWidget *dialog;
    GtkWidget *content;
    GtkWidget *grid;
    GtkWidget *result_label;
    
    // Create dialog
    dialog = gtk_dialog_new_with_buttons("Arbre Function Tests",
                                       GTK_WINDOW(app_data->window),
                                       GTK_DIALOG_MODAL,
                                       "Close", GTK_RESPONSE_CLOSE,
                                       NULL);
    
    content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    grid = gtk_grid_new();
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
    
    // Create result label
    result_label = gtk_label_new("");
    gtk_label_set_line_wrap(GTK_LABEL(result_label), TRUE);
    gtk_label_set_justify(GTK_LABEL(result_label), GTK_JUSTIFY_LEFT);
    
    // Run some tests
    GString *result = g_string_new("Arbre Tests Results:\n\n");
    
    // Test 1: Lookup widget by name
    g_string_append(result, "Test 1: Find preview_area by name\n");
    GtkWidget *found = getWidgetByNom(app_data->widget_tree, "preview_area");
    if (found == app_data->preview_area) {
        g_string_append(result, "✓ Successfully found preview_area\n\n");
    } else {
        g_string_append(result, "✗ Failed to find preview_area\n\n");
    }
    
    // Test 2: Get container names
    g_string_append(result, "Test 2: List container names\n");
    char *container_names[MAX_CONTAINER_NAMES];
    getNomsConteneurs(app_data->widget_tree, container_names);
    if (container_names[0] != NULL) {
        g_string_append(result, "✓ Found containers:\n");
        for (int i = 0; container_names[i] != NULL; i++) {
            g_string_append_printf(result, "  - %s\n", container_names[i]);
        }
        g_string_append(result, "\n");
    } else {
        g_string_append(result, "✗ No containers found\n\n");
    }
    
    // Test 3: Get container name by widget
    g_string_append(result, "Test 3: Get container name for preview_area\n");
    char *name = getNomConteneurByWidget(app_data->widget_tree, app_data->preview_area);
    if (name != NULL) {
        g_string_append_printf(result, "✓ Found name: %s\n\n", name);
    } else {
        g_string_append(result, "✗ Name not found\n\n");
    }
    
    // Set results text
    gtk_label_set_text(GTK_LABEL(result_label), result->str);
    g_string_free(result, TRUE);
    
    // Add to dialog
    gtk_container_add(GTK_CONTAINER(grid), result_label);
    gtk_container_add(GTK_CONTAINER(content), grid);
    
    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

#endif /* TREE_SYNC_H */
