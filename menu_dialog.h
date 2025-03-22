#ifndef MENU_DIALOG_H
#define MENU_DIALOG_H

#include <gtk/gtk.h>
#include "app_data.h"
#include "hierarchy.h"

#include "property_panel.h"

// Maximum string length for names and labels
#define MAX_LABEL_LENGTH 100

/**
 * Menu orientation enumeration
 */
typedef enum {
    MENU_ORIENTATION_VERTICAL,   // Standard dropdown menu
    MENU_ORIENTATION_HORIZONTAL  // Horizontal menu items
} MenuOrientation;

/**
 * MenuItem structure - represents a single menu item
 */
typedef struct {
    char label[MAX_LABEL_LENGTH];        // Menu item text
    char callback[MAX_LABEL_LENGTH];     // Optional callback function name
    GtkWidget *widget;                   // The GTK menu item widget
    gboolean is_separator;               // Whether this is a separator
} MenuItem;

/**
 * Menu structure - represents a menu that contains menu items
 */
typedef struct Menu {
    char name[MAX_LABEL_LENGTH];         // Menu name (shown in menu bar)
    GtkWidget *menu_widget;              // The GTK menu widget 
    GtkWidget *menu_item;                // Menu's item in parent (bar or submenu)
    GList *items;                        // List of MenuItems and submenus
    MenuOrientation orientation;         // Menu orientation
    struct Menu *parent;                 // Parent menu (NULL if attached to bar)
} Menu;

/**
 * MenuBar structure - represents the top-level menu bar
 */
typedef struct {
    GtkWidget *widget;                   // The GTK menu bar widget
    GList *menus;                        // List of attached menus
    int x_position;                      // X coordinate in parent container
    int y_position;                      // Y coordinate in parent container
} MenuBar;

/**
 * Structure to hold dialog data during menu creation
 */
typedef struct {
    // Current menu being edited
    char current_menu_name[MAX_LABEL_LENGTH];
    GList *current_items;                // List of items for current menu
    Menu *selected_parent;               // Selected parent menu for submenu
    MenuOrientation current_orientation;
    
    // Widget pointers
    GtkWidget *dialog;
    GtkWidget *menu_name_entry;
    GtkWidget *items_list_view;
    GtkListStore *items_list_store;
    GtkWidget *menu_tree_view;
    GtkTreeStore *menu_tree_store;
    GtkWidget *parent_combo;
    GtkWidget *orientation_combo;
    GtkWidget *x_position_spin;
    GtkWidget *y_position_spin;
    
    // Result data
    MenuBar *menu_bar;
    GList *all_menus;                    // All menus created in this session
    
    // Application data
    GtkTreeStore *hierarchy_store;       // App's widget hierarchy store
} MenuDialogData;



// Forward declarations for functions used before defined
static void build_menu_hierarchy(GtkTreeStore *store, GtkTreeIter *parent, GList *items);
static void add_submenus_to_tree(GtkTreeStore *store, GtkTreeIter *parent, const char *parent_name, GList *menus);
static void show_menu_bar_dialog(AppData *app_data);
static void create_submenu_clicked(GtkWidget *button, MenuDialogData *main_dialog_data);




/**
 * Create a new menu item
 */
static MenuItem* create_menu_item(const char *label, const char *callback) {
    MenuItem *item = g_malloc(sizeof(MenuItem));
    
    // Initialize with default values
    strncpy(item->label, label, MAX_LABEL_LENGTH - 1);
    item->label[MAX_LABEL_LENGTH - 1] = '\0';
    
    if (callback) {
        strncpy(item->callback, callback, MAX_LABEL_LENGTH - 1);
        item->callback[MAX_LABEL_LENGTH - 1] = '\0';
    } else {
        item->callback[0] = '\0';
    }
    
    item->widget = NULL;
    item->is_separator = FALSE;
    
    return item;
}

/**
 * Create a menu separator item
 */
static MenuItem* create_menu_separator() {
    MenuItem *item = g_malloc(sizeof(MenuItem));
    
    // Initialize as separator
    item->label[0] = '\0';
    item->callback[0] = '\0';
    item->widget = NULL;
    item->is_separator = TRUE;
    
    return item;
}

/**
 * Create a new menu
 */
static Menu* create_menu(const char *name, MenuOrientation orientation) {
    Menu *menu = g_malloc(sizeof(Menu));
    
    // Initialize menu
    strncpy(menu->name, name, MAX_LABEL_LENGTH - 1);
    menu->name[MAX_LABEL_LENGTH - 1] = '\0';
    
    menu->menu_widget = gtk_menu_new();
    menu->menu_item = NULL;
    menu->items = NULL;
    menu->orientation = orientation;
    menu->parent = NULL;
    
    return menu;
}

/**
 * Create a new menu bar
 */
static MenuBar* create_menu_bar() {
    MenuBar *bar = g_malloc(sizeof(MenuBar));
    
    // Initialize menu bar
    bar->widget = gtk_menu_bar_new();
    bar->menus = NULL;
    bar->x_position = 0;
    bar->y_position = 0;
    
    return bar;
}

/**
 * Add a menu item to a menu
 */
static void add_item_to_menu(Menu *menu, MenuItem *item) {
    if (!menu || !item) return;
    
    // Create the GTK widget for this item
    if (item->is_separator) {
        item->widget = gtk_separator_menu_item_new();
    } else {
        item->widget = gtk_menu_item_new_with_label(item->label);
    }
    
    // Add the item to the menu
    if (menu->orientation == MENU_ORIENTATION_VERTICAL) {
        // Standard vertical menu
        gtk_menu_shell_append(GTK_MENU_SHELL(menu->menu_widget), item->widget);
    } else {
        // Horizontal menu - use menu_attach to place items side by side
        int position = g_list_length(menu->items);
        gtk_menu_attach(GTK_MENU(menu->menu_widget), item->widget, 
                       position, position + 1, 0, 1);
    }
    
    // Store the item in the menu's items list
    menu->items = g_list_append(menu->items, item);
    
    // Show the widget
    gtk_widget_show(item->widget);
}

/**
 * Add a submenu to a parent menu
 */
static void add_submenu_to_menu(Menu *parent, Menu *submenu) {
    if (!parent || !submenu) return;
    
    // Create a menu item for the submenu
    submenu->menu_item = gtk_menu_item_new_with_label(submenu->name);
    
    // Set the menu as submenu of this item
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(submenu->menu_item), submenu->menu_widget);
    
    // Add the menu item to the parent menu
    if (parent->orientation == MENU_ORIENTATION_VERTICAL) {
        gtk_menu_shell_append(GTK_MENU_SHELL(parent->menu_widget), submenu->menu_item);
    } else {
        int position = g_list_length(parent->items);
        gtk_menu_attach(GTK_MENU(parent->menu_widget), submenu->menu_item,
                       position, position + 1, 0, 1);
    }
    
    // Update parent reference
    submenu->parent = parent;
    
    // Add to parent's items list (for tracking)
    parent->items = g_list_append(parent->items, submenu);
    
    // Show the widget
    gtk_widget_show(submenu->menu_item);
}

/**
 * Add a menu to a menu bar
 */
static void add_menu_to_bar(MenuBar *bar, Menu *menu) {
    if (!bar || !menu) return;
    
    // Create a menu item for the menu
    menu->menu_item = gtk_menu_item_new_with_label(menu->name);
    
    // Set the menu widget as submenu of this item
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu->menu_item), menu->menu_widget);
    
    // Add the menu item to the menu bar
    gtk_menu_shell_append(GTK_MENU_SHELL(bar->widget), menu->menu_item);
    
    // Add to bar's menus list
    bar->menus = g_list_append(bar->menus, menu);
    
    // Show the widget
    gtk_widget_show(menu->menu_item);
}

/**
 * Free memory used by a menu item
 */
static void free_menu_item(MenuItem *item) {
    if (item) {
        // GTK will handle freeing the widget
        g_free(item);
    }
}

/**
 * Free memory used by a menu and its items
 */
static void free_menu(Menu *menu) {
    if (menu) {
        // Free all menu items
        g_list_free_full(menu->items, (GDestroyNotify)free_menu_item);
        
        // GTK will handle freeing the widgets
        g_free(menu);
    }
}

/**
 * Free memory used by a menu bar and all its menus
 */
static void free_menu_bar(MenuBar *bar) {
    if (bar) {
        // Free all menus
        g_list_free_full(bar->menus, (GDestroyNotify)free_menu);
        
        // GTK will handle freeing the widget
        g_free(bar);
    }
}

/**
 * Add menu to container at specified position
 */
static void add_menu_bar_to_container(GtkWidget *container, MenuBar *bar) {
    if (!GTK_IS_FIXED(container) || !bar) return;
    // MenuBar
    // / Register the widget for property editing
            // g_signal_connect(bar, "button-press-event",
                            //  G_CALLBACK(on_widget_button_press_select), app_data);
    // Add the menu bar to the fixed container at specified position
    gtk_fixed_put(GTK_FIXED(container), bar->widget, bar->x_position, bar->y_position);
    
    // Show the widget
    gtk_widget_show(bar->widget);
}

/**
 * Add a menu item to the current menu being created
 */
static void add_item_clicked(GtkWidget *button, MenuDialogData *dialog_data) {
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *grid;
    GtkWidget *label_entry, *callback_entry;
    GtkWidget *label_label, *callback_label;
    gint response;

    dialog = gtk_dialog_new_with_buttons("Add Menu Item",
                                        GTK_WINDOW(dialog_data->dialog),
                                        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                        "OK", GTK_RESPONSE_ACCEPT,
                                        "Cancel", GTK_RESPONSE_CANCEL,
                                        NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);

    // Label for menu item
    label_label = gtk_label_new("Menu Item Label:");
    label_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), label_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_entry, 1, 0, 1, 1);

    // Callback name (optional)
    callback_label = gtk_label_new("Callback Name (optional):");
    callback_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), callback_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), callback_entry, 1, 1, 1, 1);

    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_widget_show_all(dialog);

    response = gtk_dialog_run(GTK_DIALOG(dialog));

    if (response == GTK_RESPONSE_ACCEPT) {
        const gchar *label_text = gtk_entry_get_text(GTK_ENTRY(label_entry));
        const gchar *callback_text = gtk_entry_get_text(GTK_ENTRY(callback_entry));

        if (label_text && *label_text) {
            // Create a new menu item
            MenuItem *item = create_menu_item(label_text, callback_text);
            
            // Add to current items list
            dialog_data->current_items = g_list_append(dialog_data->current_items, item);
            
            // Add to list store for display
            GtkTreeIter iter;
            gtk_list_store_append(dialog_data->items_list_store, &iter);
            gtk_list_store_set(dialog_data->items_list_store, &iter, 
                              0, label_text, 
                              1, callback_text, 
                              -1);
        }
    }

    gtk_widget_destroy(dialog);
}

/**
 * Add a separator to the current menu being created
 */
static void add_separator_clicked(GtkWidget *button, MenuDialogData *dialog_data) {
    // Create a separator item
    MenuItem *separator = create_menu_separator();
    
    // Add to current items list
    dialog_data->current_items = g_list_append(dialog_data->current_items, separator);
    
    // Add to list store for display
    GtkTreeIter iter;
    gtk_list_store_append(dialog_data->items_list_store, &iter);
    gtk_list_store_set(dialog_data->items_list_store, &iter, 
                       0, "--- Separator ---", 
                       1, "", 
                       -1);
}

/**
 * Remove the selected item from the current menu being created
 */
static void remove_item_clicked(GtkWidget *button, MenuDialogData *dialog_data) {
    GtkTreeSelection *selection = gtk_tree_view_get_selection(
        GTK_TREE_VIEW(dialog_data->items_list_view));
    GtkTreeModel *model;
    GtkTreeIter iter;
    
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        // Get the path to determine item index
        GtkTreePath *path = gtk_tree_model_get_path(model, &iter);
        gint *indices = gtk_tree_path_get_indices(path);
        gint index = indices[0];
        
        // Find and remove the item from the list
        GList *item_node = g_list_nth(dialog_data->current_items, index);
        if (item_node) {
            // Free the menu item
            free_menu_item((MenuItem*)item_node->data);
            
            // Remove from the list
            dialog_data->current_items = g_list_delete_link(
                dialog_data->current_items, item_node);
        }
        
        // Remove from the list store
        gtk_list_store_remove(dialog_data->items_list_store, &iter);
        
        gtk_tree_path_free(path);
    }
}

/**
 * Find a menu by name in the list of all menus
 */
static Menu* find_menu_by_name(MenuDialogData *dialog_data, const char *name) {
    for (GList *node = dialog_data->all_menus; node; node = node->next) {
        Menu *menu = (Menu*)node->data;
        if (strcmp(menu->name, name) == 0) {
            return menu;
        }
    }
    return NULL;
}

/**
 * Parse a menu name from a tree label
 * Handles both "Menu: Name (Orientation)" and "Submenu: Name (Orientation)" formats
 */
static char* parse_menu_name(const char *tree_label) {
    char *result = NULL;
    char *label_copy = g_strdup(tree_label);
    
    // Find the first space after "Menu:" or "Submenu:"
    char *start = strchr(label_copy, ' ');
    if (start) {
        start++; // Skip the space
        
        // Find the opening parenthesis for orientation
        char *end = strchr(start, '(');
        if (end) {
            end--; // Back up before the space
            *end = '\0'; // Terminate the string
            result = g_strdup(start);
        } else {
            // No orientation part found
            result = g_strdup(start);
        }
    } else {
        // Unexpected format, return whole string
        result = g_strdup(label_copy);
    }
    
    g_free(label_copy);
    return result;
}

/**
 * Toggle position controls based on parent selection
 */
static void on_parent_selection_changed(GtkComboBox *combo, MenuDialogData *dialog_data) {
    gint active = gtk_combo_box_get_active(combo);
    
    // Only show position controls for menu bar items
    gboolean show_position = (active == 0); // 0 = "Menu Bar"
    
    gtk_widget_set_visible(gtk_widget_get_parent(dialog_data->x_position_spin), show_position);
    gtk_widget_set_visible(gtk_widget_get_parent(dialog_data->y_position_spin), show_position);
}

/**
 * Add the current menu to the menu structure
 */
static void add_menu_clicked(GtkWidget *button, MenuDialogData *dialog_data) {
    const char *menu_name = gtk_entry_get_text(GTK_ENTRY(dialog_data->menu_name_entry));
    
    // Validate menu name
    if (!menu_name || !*menu_name) {
        GtkWidget *error_dialog = gtk_message_dialog_new(
            GTK_WINDOW(dialog_data->dialog),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Please enter a menu name"
        );
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        return;
    }
    
    // Validate that we have at least one item
    if (!dialog_data->current_items) {
        GtkWidget *error_dialog = gtk_message_dialog_new(
            GTK_WINDOW(dialog_data->dialog),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Please add at least one menu item"
        );
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        return;
    }
    
    // Get orientation
    gint orientation_idx = gtk_combo_box_get_active(GTK_COMBO_BOX(dialog_data->orientation_combo));
    MenuOrientation orientation = (orientation_idx == 0) ? 
                                MENU_ORIENTATION_VERTICAL : MENU_ORIENTATION_HORIZONTAL;
    
    // Create the menu
    Menu *menu = create_menu(menu_name, orientation);
    
    // Add all items to the menu
    for (GList *node = dialog_data->current_items; node; node = node->next) {
        MenuItem *item = (MenuItem*)node->data;
        add_item_to_menu(menu, item);
    }
    
    // Get parent selection
    gint parent_idx = gtk_combo_box_get_active(GTK_COMBO_BOX(dialog_data->parent_combo));
    
    if (parent_idx == 0) {
        // Attach to menu bar
        if (!dialog_data->menu_bar) {
            dialog_data->menu_bar = create_menu_bar();
        }
        
        // Get position values
        dialog_data->menu_bar->x_position = 
            gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(dialog_data->x_position_spin));
        dialog_data->menu_bar->y_position = 
            gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(dialog_data->y_position_spin));
        
        // Add menu to bar
        add_menu_to_bar(dialog_data->menu_bar, menu);
        
        // Add to tree view (top level)
        GtkTreeIter iter;
        gtk_tree_store_append(dialog_data->menu_tree_store, &iter, NULL);
        gtk_tree_store_set(dialog_data->menu_tree_store, &iter, 0, 
                          g_strdup_printf("Menu: %s (%s)", menu->name, 
                                         orientation == MENU_ORIENTATION_VERTICAL ? 
                                         "Vertical" : "Horizontal"), 
                          -1);
        
        // Add menu items as children
        for (GList *node = menu->items; node; node = node->next) {
            MenuItem *item = (MenuItem*)node->data;
            
            GtkTreeIter child_iter;
            gtk_tree_store_append(dialog_data->menu_tree_store, &child_iter, &iter);
            
            if (item->is_separator) {
                gtk_tree_store_set(dialog_data->menu_tree_store, &child_iter, 
                                  0, "--- Separator ---", -1);
            } else {
                gtk_tree_store_set(dialog_data->menu_tree_store, &child_iter, 
                                  0, item->label, -1);
            }
        }
    } else {
        // Attach to selected menu as submenu
        GtkTreeSelection *selection = gtk_tree_view_get_selection(
            GTK_TREE_VIEW(dialog_data->menu_tree_view));
        GtkTreeModel *model;
        GtkTreeIter iter;
        
        if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
            gchar *parent_label;
            gtk_tree_model_get(model, &iter, 0, &parent_label, -1);
            
            // Extract parent menu name
            char *parent_name = parse_menu_name(parent_label);
            Menu *parent_menu = find_menu_by_name(dialog_data, parent_name);
            
            if (parent_menu) {
                // Add as submenu
                add_submenu_to_menu(parent_menu, menu);
                
                // Add to tree view (as child of selected item)
                GtkTreeIter submenu_iter;
                gtk_tree_store_append(dialog_data->menu_tree_store, &submenu_iter, &iter);
                gtk_tree_store_set(dialog_data->menu_tree_store, &submenu_iter, 0, 
                                  g_strdup_printf("Submenu: %s (%s)", menu->name, 
                                               orientation == MENU_ORIENTATION_VERTICAL ? 
                                               "Vertical" : "Horizontal"), 
                                  -1);
                
                // Add menu items as children
                for (GList *node = menu->items; node; node = node->next) {
                    MenuItem *item = (MenuItem*)node->data;
                    
                    GtkTreeIter child_iter;
                    gtk_tree_store_append(dialog_data->menu_tree_store, &child_iter, &submenu_iter);
                    
                    if (item->is_separator) {
                        gtk_tree_store_set(dialog_data->menu_tree_store, &child_iter, 
                                          0, "--- Separator ---", -1);
                    } else {
                        gtk_tree_store_set(dialog_data->menu_tree_store, &child_iter, 
                                          0, item->label, -1);
                    }
                }
            } else {
                // Parent menu not found
                GtkWidget *error_dialog = gtk_message_dialog_new(
                    GTK_WINDOW(dialog_data->dialog),
                    GTK_DIALOG_MODAL,
                    GTK_MESSAGE_ERROR,
                    GTK_BUTTONS_OK,
                    "Parent menu not found"
                );
                gtk_dialog_run(GTK_DIALOG(error_dialog));
                gtk_widget_destroy(error_dialog);
                g_free(parent_name);
                g_free(parent_label);
                free_menu(menu);
                return;
            }
            
            g_free(parent_name);
            g_free(parent_label);
        } else {
            // No item selected
            GtkWidget *error_dialog = gtk_message_dialog_new(
                GTK_WINDOW(dialog_data->dialog),
                GTK_DIALOG_MODAL,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_OK,
                "Please select a parent menu in the tree"
            );
            gtk_dialog_run(GTK_DIALOG(error_dialog));
            gtk_widget_destroy(error_dialog);
            free_menu(menu);
            return;
        }
    }
    
    // Add to all_menus list
    dialog_data->all_menus = g_list_append(dialog_data->all_menus, menu);
    
    // Expand tree to show new items
    gtk_tree_view_expand_all(GTK_TREE_VIEW(dialog_data->menu_tree_view));
    
    // Clear current menu data
    dialog_data->current_menu_name[0] = '\0';
    g_list_free(dialog_data->current_items);
    dialog_data->current_items = NULL;
    
    // Clear items list store
    gtk_list_store_clear(dialog_data->items_list_store);
    
    // Clear menu name entry
    gtk_entry_set_text(GTK_ENTRY(dialog_data->menu_name_entry), "");
}

/**
 * Remove the selected menu from the menu structure
 */
static void remove_menu_clicked(GtkWidget *button, MenuDialogData *dialog_data) {
    GtkTreeSelection *selection = gtk_tree_view_get_selection(
        GTK_TREE_VIEW(dialog_data->menu_tree_view));
    GtkTreeModel *model;
    GtkTreeIter iter;
    
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gchar *menu_label;
        gtk_tree_model_get(model, &iter, 0, &menu_label, -1);
        
        // Check if this is a menu/submenu or just an item
        if (strstr(menu_label, "Menu:") == menu_label || 
            strstr(menu_label, "Submenu:") == menu_label) {
            
            // Extract menu name
            char *menu_name = parse_menu_name(menu_label);
            Menu *menu_to_remove = find_menu_by_name(dialog_data, menu_name);
            
            if (menu_to_remove) {
                // Remove from parent's list
                if (menu_to_remove->parent) {
                    // Remove from parent menu
                    menu_to_remove->parent->items = g_list_remove(
                        menu_to_remove->parent->items, menu_to_remove);
                    
                    // GTK will handle removing the widget when we destroy it
                } else if (dialog_data->menu_bar) {
                    // Remove from menu bar
                    dialog_data->menu_bar->menus = g_list_remove(
                        dialog_data->menu_bar->menus, menu_to_remove);
                }
                
                // Remove from all_menus list
                dialog_data->all_menus = g_list_remove(
                    dialog_data->all_menus, menu_to_remove);
                
                // Free the menu and its items
                free_menu(menu_to_remove);
                
                // Remove from tree view
                gtk_tree_store_remove(dialog_data->menu_tree_store, &iter);
            }
            
            g_free(menu_name);
        } else {
            // This is just a menu item, not a menu - get parent menu
            GtkTreeIter parent_iter;
            if (gtk_tree_model_iter_parent(model, &parent_iter, &iter)) {
                gchar *parent_label;
                gtk_tree_model_get(model, &parent_iter, 0, &parent_label, -1);
                
                // Extract parent menu name
                char *parent_name = parse_menu_name(parent_label);
                Menu *parent_menu = find_menu_by_name(dialog_data, parent_name);
                
                if (parent_menu) {
                    // Get path to determine item index
                    GtkTreePath *path = gtk_tree_model_get_path(model, &iter);
                    gint *indices = gtk_tree_path_get_indices(path);
                    gint index = indices[1]; // Second level is item index
                    
                    // Find and remove the item
                    GList *item_node = g_list_nth(parent_menu->items, index);
                    if (item_node) {
                        MenuItem *item = (MenuItem*)item_node->data;
                        
                        // Remove from parent's menu widget
                        if (item->widget && GTK_IS_WIDGET(item->widget)) {
                            gtk_container_remove(
                                GTK_CONTAINER(parent_menu->menu_widget), item->widget);
                        }
                        
                        // Free the menu item
                        free_menu_item(item);
                        
                        // Remove from the list
                        parent_menu->items = g_list_delete_link(
                            parent_menu->items, item_node);
                    }
                    
                    gtk_tree_path_free(path);
                }
                
                g_free(parent_name);
                g_free(parent_label);
            }
            
            // Remove from tree view
            gtk_tree_store_remove(dialog_data->menu_tree_store, &iter);
        }
        
        g_free(menu_label);
    }
}

/**
 * Add submenu items to the tree recursively
 */
static void add_submenus_to_tree(GtkTreeStore *store, GtkTreeIter *parent, const char *parent_name, GList *menus) {
    for (GList *node = menus; node; node = node->next) {
        Menu *submenu = (Menu*)node->data;
        
        // Only process menus where this is the parent
        if (!submenu->parent || strcmp(submenu->parent->name, parent_name) != 0)
            continue;
            
        // Add this submenu to the tree
        GtkTreeIter submenu_iter;
        gtk_tree_store_append(store, &submenu_iter, parent);
        
        // Add with its type
        gtk_tree_store_set(store, &submenu_iter, 0, 
                         g_strdup_printf("Submenu: %s", submenu->name),
                         1, submenu->menu_item, -1);
        
        // Add menu items
        for (GList *item_node = submenu->items; item_node; item_node = item_node->next) {
            // Check if this is a MenuItem
            MenuItem *item = (MenuItem*)item_node->data;
            if (!item->is_separator) {
                GtkTreeIter item_iter;
                gtk_tree_store_append(store, &item_iter, &submenu_iter);
                gtk_tree_store_set(store, &item_iter, 0, item->label, 1, item->widget, -1);
            }
        }
        
        // Recursively add submenus of this submenu
        add_submenus_to_tree(store, &submenu_iter, submenu->name, menus);
    }
}

/**
 * Show the dialog for creating a menu bar
 */
static void show_menu_bar_dialog(AppData *app_data) {
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *paned;
    GtkWidget *left_box, *right_box;
    GtkWidget *config_frame, *config_grid;
    GtkWidget *items_frame, *items_box;
    GtkWidget *menu_name_label, *menu_name_entry;
    GtkWidget *parent_label, *parent_combo;
    GtkWidget *orientation_label, *orientation_combo;
    GtkWidget *x_position_label, *y_position_label;
    GtkWidget *x_position_spin, *y_position_spin;
    GtkWidget *items_list_view, *menu_tree_view;
    GtkWidget *add_item_button, *add_separator_button, *remove_item_button;
    GtkWidget *add_menu_button, *remove_menu_button;
    GtkListStore *items_list_store;
    GtkTreeStore *menu_tree_store;
    
    // Create dialog
    dialog = gtk_dialog_new_with_buttons("Create Menu Bar",
                                       GTK_WINDOW(app_data->window),
                                       GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                       "Create Menu Bar", GTK_RESPONSE_ACCEPT,
                                       "Cancel", GTK_RESPONSE_CANCEL,
                                       NULL);
    
    // Set size
    gtk_window_set_default_size(GTK_WINDOW(dialog), 700, 500);
    
    // Get content area
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    
    // Create paned container
    paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_container_add(GTK_CONTAINER(content_area), paned);
    
    // Left side - Menu creation
    left_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(left_box), 10);
    
    // Menu configuration frame
    config_frame = gtk_frame_new("Menu Configuration");
    config_grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(config_grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(config_grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(config_grid), 10);
    gtk_container_add(GTK_CONTAINER(config_frame), config_grid);
    
    // Menu name field
    menu_name_label = gtk_label_new("Menu Name:");
    gtk_widget_set_halign(menu_name_label, GTK_ALIGN_START);
    menu_name_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(config_grid), menu_name_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(config_grid), menu_name_entry, 1, 0, 2, 1);
    
    // Parent selection
    parent_label = gtk_label_new("Attach To:");
    gtk_widget_set_halign(parent_label, GTK_ALIGN_START);
    parent_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(parent_combo), "Menu Bar");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(parent_combo), "Selected Menu");
    gtk_combo_box_set_active(GTK_COMBO_BOX(parent_combo), 0);
    // gtk_grid_attach(GTK_GRID(config_grid), parent_label, 0, 1, 1, 1);
    // gtk_grid_attach(GTK_GRID(config_grid), parent_combo, 1, 1, 2, 1);
    
    // Orientation selection
    orientation_label = gtk_label_new("Menu Orientation:");
    gtk_widget_set_halign(orientation_label, GTK_ALIGN_START);
    orientation_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(orientation_combo), "Vertical");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(orientation_combo), "Horizontal");
    gtk_combo_box_set_active(GTK_COMBO_BOX(orientation_combo), 0);
    gtk_grid_attach(GTK_GRID(config_grid), orientation_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(config_grid), orientation_combo, 1, 2, 2, 1);
    
    // Position controls
    x_position_label = gtk_label_new("X Position:");
    x_position_spin = gtk_spin_button_new_with_range(0, 2000, 1);
    gtk_grid_attach(GTK_GRID(config_grid), x_position_label, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(config_grid), x_position_spin, 1, 3, 2, 1);
    
    y_position_label = gtk_label_new("Y Position:");
    y_position_spin = gtk_spin_button_new_with_range(0, 2000, 1);
    gtk_grid_attach(GTK_GRID(config_grid), y_position_label, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(config_grid), y_position_spin, 1, 4, 2, 1);
    
    // Items list section
    items_frame = gtk_frame_new("Menu Items");
    items_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(items_box), 10);
    gtk_container_add(GTK_CONTAINER(items_frame), items_box);
    
    // List store and view for items
    items_list_store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
    items_list_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(items_list_store));
    
    // Add columns
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes(
        "Label", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(items_list_view), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes(
        "Callback", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(items_list_view), column);
    
    // Add scrolled window for items list
    GtkWidget *items_scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(items_scroll),
                                 GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(items_scroll, -1, 150);
    gtk_container_add(GTK_CONTAINER(items_scroll), items_list_view);
    
    // Item buttons
    GtkWidget *item_buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    add_item_button = gtk_button_new_with_label("Add Item");
    add_separator_button = gtk_button_new_with_label("Add Separator");
    remove_item_button = gtk_button_new_with_label("Remove Item");
    
    gtk_box_pack_start(GTK_BOX(item_buttons), add_item_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(item_buttons), add_separator_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(item_buttons), remove_item_button, TRUE, TRUE, 0);
    
    // Add controls to items box
    gtk_box_pack_start(GTK_BOX(items_box), items_scroll, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(items_box), item_buttons, FALSE, FALSE, 0);
    
    // Add menu button
    add_menu_button = gtk_button_new_with_label("Add Current Menu");
    
    // Add all components to left box
    gtk_box_pack_start(GTK_BOX(left_box), config_frame, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(left_box), items_frame, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(left_box), add_menu_button, FALSE, FALSE, 0);
    
    // Create submenu button
    GtkWidget *create_submenu_button = gtk_button_new_with_label("Create Submenu");
    gtk_box_pack_start(GTK_BOX(left_box), create_submenu_button, FALSE, FALSE, 5);
    
    // Right side - Menu structure
    right_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(right_box), 10);
    
    // Add title
    GtkWidget *right_title = gtk_label_new("Menu Structure");
    gtk_widget_set_halign(right_title, GTK_ALIGN_START);
    
    // Menu tree
    menu_tree_store = gtk_tree_store_new(1, G_TYPE_STRING);
    menu_tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(menu_tree_store));
    
    // Add column
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes(
        "Menu Structure", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(menu_tree_view), column);
    
    // Add scrolled window for tree
    GtkWidget *tree_scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(tree_scroll),
                                 GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(tree_scroll), menu_tree_view);
    
    // Remove menu button
    remove_menu_button = gtk_button_new_with_label("Remove Selected Menu");
    
    // Add to right box
    gtk_box_pack_start(GTK_BOX(right_box), right_title, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(right_box), tree_scroll, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(right_box), remove_menu_button, FALSE, FALSE, 0);
    
    // Add sides to paned
    gtk_paned_add1(GTK_PANED(paned), left_box);
    gtk_paned_add2(GTK_PANED(paned), right_box);
    gtk_paned_set_position(GTK_PANED(paned), 350);
    
    // Initialize dialog data
    MenuDialogData dialog_data = {
        .dialog = dialog,
        .menu_name_entry = menu_name_entry,
        .items_list_view = items_list_view,
        .items_list_store = items_list_store,
        .menu_tree_view = menu_tree_view,
        .menu_tree_store = menu_tree_store,
        .parent_combo = parent_combo,
        .orientation_combo = orientation_combo,
        .x_position_spin = x_position_spin,
        .y_position_spin = y_position_spin,
        .current_items = NULL,
        .all_menus = NULL,
        .menu_bar = NULL,
        .hierarchy_store = app_data->hierarchy_store
    };
    
    // Connect signals
    g_signal_connect(add_item_button, "clicked", G_CALLBACK(add_item_clicked), &dialog_data);
    g_signal_connect(add_separator_button, "clicked", G_CALLBACK(add_separator_clicked), &dialog_data);
    g_signal_connect(remove_item_button, "clicked", G_CALLBACK(remove_item_clicked), &dialog_data);
    g_signal_connect(add_menu_button, "clicked", G_CALLBACK(add_menu_clicked), &dialog_data);
    g_signal_connect(remove_menu_button, "clicked", G_CALLBACK(remove_menu_clicked), &dialog_data);
    g_signal_connect(parent_combo, "changed", G_CALLBACK(on_parent_selection_changed), &dialog_data);
    g_signal_connect(create_submenu_button, "clicked", G_CALLBACK(create_submenu_clicked), &dialog_data);
    
    // Show dialog
    gtk_widget_show_all(dialog);
    
    // Run dialog
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT && dialog_data.menu_bar) {
        // / Register the widget for property editing
            g_signal_connect(dialog_data.menu_bar->widget, "button-press-event",
                             G_CALLBACK(on_widget_button_press_select), app_data);
        // Add menu bar to preview area
        add_menu_bar_to_container(app_data->preview_area, dialog_data.menu_bar);
        
        // Add to hierarchy
        GtkTreeIter iter;
        gtk_tree_store_append(app_data->hierarchy_store, &iter, NULL);
        gtk_tree_store_set(app_data->hierarchy_store, &iter, 
                          0, g_strdup_printf("Menu Bar (x:%d, y:%d)", 
                                           dialog_data.menu_bar->x_position,
                                           dialog_data.menu_bar->y_position),
                          1, dialog_data.menu_bar->widget, -1);
        
        // Build hierarchy
        build_menu_hierarchy(app_data->hierarchy_store, &iter, dialog_data.all_menus);
        
        // Expand tree
        gtk_tree_view_expand_all(GTK_TREE_VIEW(app_data->hierarchy_view));
    }
    
    // Cleanup
    g_list_free_full(dialog_data.current_items, (GDestroyNotify)free_menu_item);
    // Note: dialog_data.all_menus and dialog_data.menu_bar are now owned by the app
    
    gtk_widget_destroy(dialog);
}

/**
 * Build menu hierarchy in application's tree view
 */
static void build_menu_hierarchy(GtkTreeStore *store, GtkTreeIter *parent, GList *menus) {
    for (GList *node = menus; node; node = node->next) {
        Menu *menu = (Menu*)node->data;
        
        // Skip submenus - they'll be added by their parents
        if (menu->parent)
            continue;
        
        // Add this menu to the tree
        GtkTreeIter menu_iter;
        gtk_tree_store_append(store, &menu_iter, parent);
        
        // Add menu with its type as the label
        gtk_tree_store_set(store, &menu_iter, 0, 
                         g_strdup_printf("Menu: %s", menu->name),
                         1, menu->menu_item, -1);
        
        // Add menu items
        for (GList *item_node = menu->items; item_node; item_node = item_node->next) {
            // Check if this is a MenuItem
            MenuItem *item = (MenuItem*)item_node->data;
            if (!item->is_separator) {
                GtkTreeIter item_iter;
                gtk_tree_store_append(store, &item_iter, &menu_iter);
                gtk_tree_store_set(store, &item_iter, 0, item->label, 1, item->widget, -1);
            }
        }
            
        // Recursively add its submenus
        add_submenus_to_tree(store, &menu_iter, menu->name, menus);
    }
}

/**
 * Create a submenu in a separate dialog
 */
static void create_submenu_clicked(GtkWidget *button, MenuDialogData *main_dialog_data) {
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *grid;
    GtkWidget *name_label, *name_entry;
    GtkWidget *items_frame, *items_box;
    GtkWidget *items_list_view;
    GtkListStore *items_list_store;
    GtkWidget *add_item_button, *add_separator_button, *remove_item_button;
    GtkWidget *orientation_label, *orientation_combo;
     
    // Check if a parent menu is selected in the tree view
    GtkTreeSelection *selection = gtk_tree_view_get_selection(
        GTK_TREE_VIEW(main_dialog_data->menu_tree_view));
    GtkTreeModel *model;
    GtkTreeIter selected_iter;
     
    if (!gtk_tree_selection_get_selected(selection, &model, &selected_iter)) {
        GtkWidget *error_dialog = gtk_message_dialog_new(
            GTK_WINDOW(main_dialog_data->dialog),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Please select a parent menu in the tree"
        );
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        return;
    }
     
    // Get selected menu name
    gchar *parent_label;
    gtk_tree_model_get(model, &selected_iter, 0, &parent_label, -1);
    char *parent_name = parse_menu_name(parent_label);
    Menu *parent_menu = find_menu_by_name(main_dialog_data, parent_name);
     
    if (!parent_menu) {
        GtkWidget *error_dialog = gtk_message_dialog_new(
            GTK_WINDOW(main_dialog_data->dialog),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Invalid parent menu selected"
        );
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        g_free(parent_name);
        g_free(parent_label);
        return;
    }
     
    // Create submenu dialog
    dialog = gtk_dialog_new_with_buttons("Create Submenu",
                                        GTK_WINDOW(main_dialog_data->dialog),
                                        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                        "Add Submenu", GTK_RESPONSE_ACCEPT,
                                        "Cancel", GTK_RESPONSE_CANCEL,
                                        NULL);
     
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
     
    // Submenu name
    name_label = gtk_label_new("Submenu Name:");
    name_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), name_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), name_entry, 1, 0, 2, 1);
     
    // Orientation selection
    orientation_label = gtk_label_new("Orientation:");
    orientation_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(orientation_combo), "Vertical");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(orientation_combo), "Horizontal");
    gtk_combo_box_set_active(GTK_COMBO_BOX(orientation_combo), 0);
    gtk_grid_attach(GTK_GRID(grid), orientation_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), orientation_combo, 1, 1, 2, 1);
     
    // Parent menu info
    GtkWidget *parent_info = gtk_label_new(g_strdup_printf("Will be added to menu: %s", parent_name));
    gtk_grid_attach(GTK_GRID(grid), parent_info, 0, 2, 3, 1);
     
    gtk_container_add(GTK_CONTAINER(content_area), grid);
     
    // Items section
    items_frame = gtk_frame_new("Menu Items");
    items_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(items_box), 10);
    gtk_container_add(GTK_CONTAINER(items_frame), items_box);
     
    // List store and view for items
     items_list_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(items_list_store));
     
     // Add columns
     GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
     GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes(
         "Label", renderer, "text", 0, NULL);
     gtk_tree_view_append_column(GTK_TREE_VIEW(items_list_view), column);
     
     renderer = gtk_cell_renderer_text_new();
     column = gtk_tree_view_column_new_with_attributes(
         "Callback", renderer, "text", 1, NULL);
     gtk_tree_view_append_column(GTK_TREE_VIEW(items_list_view), column);
     
     // Scrolled window for list
     GtkWidget *items_scroll = gtk_scrolled_window_new(NULL, NULL);
     gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(items_scroll),
                                  GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
     gtk_widget_set_size_request(items_scroll, -1, 150);
     gtk_container_add(GTK_CONTAINER(items_scroll), items_list_view);
     
     // Buttons for item management
     GtkWidget *buttons_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
     add_item_button = gtk_button_new_with_label("Add Item");
     add_separator_button = gtk_button_new_with_label("Add Separator");
     remove_item_button = gtk_button_new_with_label("Remove Item");
     
     gtk_box_pack_start(GTK_BOX(buttons_box), add_item_button, TRUE, TRUE, 0);
     gtk_box_pack_start(GTK_BOX(buttons_box), add_separator_button, TRUE, TRUE, 0);
     gtk_box_pack_start(GTK_BOX(buttons_box), remove_item_button, TRUE, TRUE, 0);
     
     gtk_box_pack_start(GTK_BOX(items_box), items_scroll, TRUE, TRUE, 0);
     gtk_box_pack_start(GTK_BOX(items_box), buttons_box, FALSE, FALSE, 0);
     
     gtk_box_pack_start(GTK_BOX(content_area), items_frame, TRUE, TRUE, 10);
     
     // Data for submenu creation
     MenuDialogData submenu_data = {
         .dialog = dialog,
         .menu_name_entry = name_entry,
         .items_list_view = items_list_view,
         .items_list_store = items_list_store,
         .orientation_combo = orientation_combo,
         .current_items = NULL,
         .selected_parent = parent_menu,
         .menu_tree_store = main_dialog_data->menu_tree_store,
         .menu_tree_view = main_dialog_data->menu_tree_view,
         .all_menus = main_dialog_data->all_menus
     };
     
     // Connect signals for item management
     g_signal_connect(add_item_button, "clicked", G_CALLBACK(add_item_clicked), &submenu_data);
     g_signal_connect(add_separator_button, "clicked", G_CALLBACK(add_separator_clicked), &submenu_data);
     g_signal_connect(remove_item_button, "clicked", G_CALLBACK(remove_item_clicked), &submenu_data);
     
     gtk_widget_show_all(dialog);
     
     // Run dialog
     if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
         const char *submenu_name = gtk_entry_get_text(GTK_ENTRY(name_entry));
         
         // Validate submenu name
         if (!submenu_name || !*submenu_name) {
             GtkWidget *error_dialog = gtk_message_dialog_new(
                 GTK_WINDOW(dialog),
                 GTK_DIALOG_MODAL,
                 GTK_MESSAGE_ERROR,
                 GTK_BUTTONS_OK,
                 "Please enter a submenu name"
             );
             gtk_dialog_run(GTK_DIALOG(error_dialog));
             gtk_widget_destroy(error_dialog);
         } else if (!submenu_data.current_items) {
             // Validate that we have at least one item
             GtkWidget *error_dialog = gtk_message_dialog_new(
                 GTK_WINDOW(dialog),
                 GTK_DIALOG_MODAL,
                 GTK_MESSAGE_ERROR,
                 GTK_BUTTONS_OK,
                 "Please add at least one menu item"
             );
             gtk_dialog_run(GTK_DIALOG(error_dialog));
             gtk_widget_destroy(error_dialog);
         } else {
             // Get orientation
             gint orientation_idx = gtk_combo_box_get_active(GTK_COMBO_BOX(orientation_combo));
             MenuOrientation orientation = (orientation_idx == 0) ? 
                                        MENU_ORIENTATION_VERTICAL : MENU_ORIENTATION_HORIZONTAL;
             
             // Create the submenu
             Menu *submenu = create_menu(submenu_name, orientation);
             
             // Add all items to the submenu
             for (GList *node = submenu_data.current_items; node; node = node->next) {
                 MenuItem *item = (MenuItem*)node->data;
                 add_item_to_menu(submenu, item);
             }
             
             // Add submenu to parent menu
             add_submenu_to_menu(parent_menu, submenu);
             
             // Add to all_menus list
             main_dialog_data->all_menus = g_list_append(main_dialog_data->all_menus, submenu);
             
             // Update tree view
             GtkTreeIter submenu_iter;
             gtk_tree_store_append(main_dialog_data->menu_tree_store, &submenu_iter, &selected_iter);
             gtk_tree_store_set(main_dialog_data->menu_tree_store, &submenu_iter, 0, 
                              g_strdup_printf("Submenu: %s (%s)", submenu->name, 
                                            orientation == MENU_ORIENTATION_VERTICAL ? 
                                            "Vertical" : "Horizontal"), 
                              -1);
             
             // Add menu items as children in tree view
             for (GList *node = submenu->items; node; node = node->next) {
                 MenuItem *item = (MenuItem*)node->data;
                 
                 GtkTreeIter item_iter;
                 gtk_tree_store_append(main_dialog_data->menu_tree_store, &item_iter, &submenu_iter);
                 
                 if (item->is_separator) {
                     gtk_tree_store_set(main_dialog_data->menu_tree_store, &item_iter, 
                                       0, "--- Separator ---", -1);
                 } else {
                     gtk_tree_store_set(main_dialog_data->menu_tree_store, &item_iter, 
                                       0, item->label, -1);
                 }
             }
             
             // Expand tree to show new items
             gtk_tree_view_expand_all(GTK_TREE_VIEW(main_dialog_data->menu_tree_view));
         }
     }
     
     // Cleanup
     g_list_free_full(submenu_data.current_items, (GDestroyNotify)free_menu_item);
     g_free(parent_name);
     g_free(parent_label);
     
     gtk_widget_destroy(dialog);
 }
 

// Button click handler for adding a menu bar
static void add_menu_bar_clicked(GtkWidget *button, gpointer data) {
    AppData *app_data = (AppData *)data;
    show_menu_bar_dialog(app_data);
}

#endif /* MENU_DIALOG_H */
