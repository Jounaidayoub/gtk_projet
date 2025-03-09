#ifndef MENU_DIALOG_H
#define MENU_DIALOG_H

#include <gtk/gtk.h>
#include "app_data.h"
#include "hierarchy.h"
#include "menu.h"

// Structure to hold our menu creation data
typedef struct {
    GtkWidget *menu_name_entry;
    GtkWidget *menu_items_view;
    GtkListStore *menu_items_store;
    GList *menu_items;
    GtkWidget *menus_tree_view;
    GtkTreeStore *menus_tree_store;
    GList *menus_list;  // List of Menu* that will be added to the menu bar
} MenuDialogData;

// Structure to represent a menu during creation
typedef struct {
    char name[MAXNAME];
    GList *items;  // List of MenuItem*
} MenuCreationData;

// Function to add a menu item to the list
static void add_menu_item_to_list(GtkWidget *button, MenuDialogData *menu_data) {
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *grid;
    GtkWidget *label_entry, *callback_entry;
    GtkWidget *label_label, *callback_label;
    gint response;

    dialog = gtk_dialog_new_with_buttons("Add Menu Item",
                                        NULL,
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
            // Add item to the list store
            GtkTreeIter iter;
            gtk_list_store_append(menu_data->menu_items_store, &iter);
            gtk_list_store_set(menu_data->menu_items_store, &iter, 
                              0, label_text, 
                              1, callback_text, 
                              -1);

            // Create a MenuItem struct and add to list
            MenuItem *item = g_malloc(sizeof(MenuItem));
            strncpy(item->label, label_text, MAXNAME - 1);
            item->label[MAXNAME - 1] = '\0'; // Ensure null termination
            menu_data->menu_items = g_list_append(menu_data->menu_items, item);
        }
    }

    gtk_widget_destroy(dialog);
}

// Function to add the current menu to the menus list
static void add_current_menu(GtkWidget *button, MenuDialogData *menu_data) {
    const gchar *menu_name = gtk_entry_get_text(GTK_ENTRY(menu_data->menu_name_entry));
    
    if (!menu_name || !*menu_name || !menu_data->menu_items) {
        // Show an error message if no name or items
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                                  GTK_DIALOG_MODAL,
                                                  GTK_MESSAGE_ERROR,
                                                  GTK_BUTTONS_OK,
                                                  "Please enter a menu name and add at least one menu item.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    
    // Create a new MenuCreationData structure
    MenuCreationData *menu_creation = g_malloc(sizeof(MenuCreationData));
    strncpy(menu_creation->name, menu_name, MAXNAME - 1);
    menu_creation->name[MAXNAME - 1] = '\0';  // Ensure null termination
    
    // Copy all menu items to the menu creation data
    menu_creation->items = NULL;
    GList *iter;
    for (iter = menu_data->menu_items; iter != NULL; iter = g_list_next(iter)) {
        MenuItem *original_item = (MenuItem *)iter->data;
        MenuItem *item_copy = g_malloc(sizeof(MenuItem));
        strncpy(item_copy->label, original_item->label, MAXNAME - 1);
        item_copy->label[MAXNAME - 1] = '\0';
        menu_creation->items = g_list_append(menu_creation->items, item_copy);
    }
    
    // Add to menus list
    menu_data->menus_list = g_list_append(menu_data->menus_list, menu_creation);
    
    // Add to tree view
    GtkTreeIter iter_tree;
    gtk_tree_store_append(menu_data->menus_tree_store, &iter_tree, NULL);
    gtk_tree_store_set(menu_data->menus_tree_store, &iter_tree, 0, menu_name, -1);
    
    // Add all menu items as children
    for (iter = menu_data->menu_items; iter != NULL; iter = g_list_next(iter)) {
        MenuItem *item = (MenuItem *)iter->data;
        GtkTreeIter child_iter;
        gtk_tree_store_append(menu_data->menus_tree_store, &child_iter, &iter_tree);
        gtk_tree_store_set(menu_data->menus_tree_store, &child_iter, 0, item->label, -1);
    }
    
    // Expand the tree to show all items
    gtk_tree_view_expand_all(GTK_TREE_VIEW(menu_data->menus_tree_view));
    
    // Clear the current menu items list and tree view to prepare for the next menu
    g_list_free_full(menu_data->menu_items, g_free);
    menu_data->menu_items = NULL;
    gtk_list_store_clear(menu_data->menu_items_store);
    
    // Clear the menu name entry for the next menu
    gtk_entry_set_text(GTK_ENTRY(menu_data->menu_name_entry), "");
}

// Function to show a dialog for creating a complete menu bar
static void show_menu_bar_dialog(AppData *app_data) {
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *paned;  // Use a paned container to separate menu creation and menu list
    GtkWidget *left_box, *right_box;
    GtkWidget *menu_items_scroll, *menus_scroll;
    GtkWidget *menu_items_view, *menus_tree_view;
    GtkWidget *menu_name_label, *menu_name_entry;
    GtkWidget *add_item_button, *add_menu_button;
    GtkListStore *menu_items_store;
    GtkTreeStore *menus_tree_store;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    gint response;

    // Create the dialog
    dialog = gtk_dialog_new_with_buttons("Create Menu Bar",
                                        GTK_WINDOW(app_data->window),
                                        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                        "Create Menu Bar", GTK_RESPONSE_ACCEPT,
                                        "Cancel", GTK_RESPONSE_CANCEL,
                                        NULL);
    
    // Make the dialog larger
    gtk_window_set_default_size(GTK_WINDOW(dialog), 600, 400);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    
    // Create a horizontal paned container
    paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_container_add(GTK_CONTAINER(content_area), paned);
    
    // Left side - Menu creation
    left_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(left_box), 10);
    
    // Right side - Menu list
    right_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(right_box), 10);
    
    // LEFT SIDE WIDGETS

    // Menu name field
    GtkWidget *name_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    menu_name_label = gtk_label_new("Menu Name:");
    menu_name_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(name_box), menu_name_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(name_box), menu_name_entry, TRUE, TRUE, 0);
    
    // Title
    GtkWidget *title_label = gtk_label_new("1. Create Menus");
    GtkWidget *subtitle_label = gtk_label_new("Add menu items to current menu:");
    gtk_widget_set_halign(title_label, GTK_ALIGN_START);
    gtk_widget_set_halign(subtitle_label, GTK_ALIGN_START);
    
    // Menu items list with scrolled window
    menu_items_scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(menu_items_scroll),
                                  GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(menu_items_scroll, -1, 150);

    // Create a list store and tree view for menu items
    menu_items_store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
    menu_items_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(menu_items_store));

    // Add columns for label and callback
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Label", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(menu_items_view), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Callback", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(menu_items_view), column);
    
    gtk_container_add(GTK_CONTAINER(menu_items_scroll), menu_items_view);
    
    // Button box for menu item actions
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    add_item_button = gtk_button_new_with_label("Add Item");
    GtkWidget *remove_item_button = gtk_button_new_with_label("Remove Item");
    
    gtk_box_pack_start(GTK_BOX(button_box), add_item_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), remove_item_button, TRUE, TRUE, 0);
    
    // Add menu button 
    add_menu_button = gtk_button_new_with_label("Add Current Menu to Menu Bar →");
    
    // Pack everything into the left box
    gtk_box_pack_start(GTK_BOX(left_box), title_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(left_box), name_box, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(left_box), subtitle_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(left_box), menu_items_scroll, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(left_box), button_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(left_box), add_menu_button, FALSE, FALSE, 10);
    
    // RIGHT SIDE WIDGETS
    
    // Title
    GtkWidget *right_title = gtk_label_new("2. Menu Bar Structure");
    gtk_widget_set_halign(right_title, GTK_ALIGN_START);
    
    // Create tree store and view for menu structure
    menus_tree_store = gtk_tree_store_new(1, G_TYPE_STRING);
    menus_tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(menus_tree_store));
    
    // Add column for menu/item names
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Menu Structure", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(menus_tree_view), column);
    
    // Add scrolled window for the tree view
    menus_scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(menus_scroll),
                                  GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(menus_scroll), menus_tree_view);
    
    // Remove menu button
    GtkWidget *remove_menu_button = gtk_button_new_with_label("Remove Selected Menu");
    
    // Pack everything into the right box
    gtk_box_pack_start(GTK_BOX(right_box), right_title, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(right_box), menus_scroll, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(right_box), remove_menu_button, FALSE, FALSE, 0);
    
    // Add both sides to the paned widget
    gtk_paned_add1(GTK_PANED(paned), left_box);
    gtk_paned_add2(GTK_PANED(paned), right_box);
    
    // Set initial position
    gtk_paned_set_position(GTK_PANED(paned), 300);
    
    // Store menu dialog data for callbacks
    MenuDialogData menu_data = {
        .menu_name_entry = menu_name_entry,
        .menu_items_view = menu_items_view,
        .menu_items_store = menu_items_store,
        .menu_items = NULL,
        .menus_tree_view = menus_tree_view,
        .menus_tree_store = menus_tree_store,
        .menus_list = NULL
    };

    // Connect button signals
    g_signal_connect(add_item_button, "clicked", G_CALLBACK(add_menu_item_to_list), &menu_data);
    g_signal_connect(add_menu_button, "clicked", G_CALLBACK(add_current_menu), &menu_data);
    
    gtk_widget_show_all(dialog);

    // Run the dialog
    response = gtk_dialog_run(GTK_DIALOG(dialog));

    if (response == GTK_RESPONSE_ACCEPT && menu_data.menus_list) {
        // Create a menu bar
        MenuBar *menu_bar = creer_menu_bar();
        
        // Add all menus to the menu bar
        GList *menu_iter;
        for (menu_iter = menu_data.menus_list; menu_iter != NULL; menu_iter = g_list_next(menu_iter)) {
            MenuCreationData *menu_creation = (MenuCreationData *)menu_iter->data;
            
            // Create a menu
            Menu *menu = creer_menu();
            
            // Add all items to the menu
            GList *item_iter;
            for (item_iter = menu_creation->items; item_iter != NULL; item_iter = g_list_next(item_iter)) {
                MenuItem *item = (MenuItem *)item_iter->data;
                ajouter_element_au_menu(menu, item->label, NULL);
            }
            
            // Add menu to menu bar
            ajouter_menu_a_barre(menu_bar, menu_creation->name, menu);
            
            // Free the item list
            g_list_free_full(menu_creation->items, g_free);
            g_free(menu_creation);
        }
        
        // Add menu bar to the preview area
        if (GTK_IS_CONTAINER(app_data->preview_area)) {
            // Create a box to hold the menu bar at the top
            GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
            
            // Add the menu bar to the box
            ajouter_menu_dans_conteneur(menu_bar, vbox);
            
            // Add the box to the preview area at the top
            gtk_fixed_put(GTK_FIXED(app_data->preview_area), vbox, 0, 0);
            
            // Add to hierarchy
            GtkTreeIter iter;
            gtk_tree_store_append(app_data->hierarchy_store, &iter, NULL);
            gtk_tree_store_set(app_data->hierarchy_store, &iter, 0, "Menu Bar", 1, menu_bar->menu_bar, -1);
            
            // Add all menus as children in the hierarchy
            for (int i = 0; i < menu_bar->num_menus; i++) {
                GtkTreeIter child_iter;
                gtk_tree_store_append(app_data->hierarchy_store, &child_iter, &iter);
                
                // Get the name of the menu (we saved it in the tree store)
                GtkTreeIter menu_iter;
                if (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(menus_tree_store), &menu_iter, NULL, i)) {
                    gchar *menu_name;
                    gtk_tree_model_get(GTK_TREE_MODEL(menus_tree_store), &menu_iter, 0, &menu_name, -1);
                    gtk_tree_store_set(app_data->hierarchy_store, &child_iter, 0, g_strdup_printf("Menu: %s", menu_name), -1);
                    g_free(menu_name);
                }
            }
            
            // Expand the hierarchy to show the menu structure
            gtk_tree_view_expand_all(GTK_TREE_VIEW(app_data->hierarchy_view));
            
            // Show the menu bar
            gtk_widget_show_all(app_data->preview_area);
        }
        
        // Clean up menus_list (items were already freed above)
        g_list_free(menu_data.menus_list);
    }

    gtk_widget_destroy(dialog);
}

// Button click handler for adding a menu bar
static void add_menu_bar_clicked(GtkWidget *button, gpointer data) {
    AppData *app_data = (AppData *)data;
    show_menu_bar_dialog(app_data);
}

#endif /* MENU_DIALOG_H */
