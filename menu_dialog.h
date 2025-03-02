#ifndef MENU_DIALOG_H
#define MENU_DIALOG_H

#include <gtk/gtk.h>
#include "app_data.h"
#include "hierarchy.h"
#include "menu.h"

typedef struct {
    GtkWidget *menu_name_entry;
    GtkWidget *menu_items_view;
    GtkListStore *menu_items_store;
    GList *menu_items;
} MenuDialogData;

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

// Function to show a dialog for creating a menu bar
static void show_menu_bar_dialog(AppData *app_data) {
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *grid;
    GtkWidget *menu_items_scroll;
    GtkWidget *menu_items_view;
    GtkWidget *menu_name_label, *menu_name_entry;
    GtkWidget *add_item_button;
    GtkListStore *menu_items_store;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    gint response;

    // Create the dialog
    dialog = gtk_dialog_new_with_buttons("Create Menu Bar",
                                        GTK_WINDOW(app_data->window),
                                        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                        "Add Menu", GTK_RESPONSE_ACCEPT,
                                        "Cancel", GTK_RESPONSE_CANCEL,
                                        NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);

    // Menu name field
    menu_name_label = gtk_label_new("Menu Name:");
    menu_name_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(menu_name_entry), "File");
    gtk_grid_attach(GTK_GRID(grid), menu_name_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), menu_name_entry, 1, 0, 1, 1);

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
    gtk_grid_attach(GTK_GRID(grid), menu_items_scroll, 0, 1, 2, 1);

    // Button to add menu items
    add_item_button = gtk_button_new_with_label("Add Menu Item");
    gtk_grid_attach(GTK_GRID(grid), add_item_button, 0, 2, 2, 1);

    // Store menu dialog data for callbacks
    MenuDialogData menu_data = {
        .menu_name_entry = menu_name_entry,
        .menu_items_view = menu_items_view,
        .menu_items_store = menu_items_store,
        .menu_items = NULL
    };

    // Connect add item button
    g_signal_connect(add_item_button, "clicked", G_CALLBACK(add_menu_item_to_list), &menu_data);

    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_widget_show_all(dialog);

    // Run the dialog
    response = gtk_dialog_run(GTK_DIALOG(dialog));

    if (response == GTK_RESPONSE_ACCEPT) {
        const gchar *menu_name = gtk_entry_get_text(GTK_ENTRY(menu_name_entry));

        if (menu_name && *menu_name && menu_data.menu_items) {
            // Create a menu bar
            MenuBar *menu_bar = creer_menu_bar();
            
            // Create a menu
            Menu *menu = creer_menu();
            
            // Add menu items
            GList *iter;
            for (iter = menu_data.menu_items; iter != NULL; iter = g_list_next(iter)) {
                MenuItem *item = (MenuItem *)iter->data;
                ajouter_element_au_menu(menu, item->label, NULL); // No callback for now
                g_free(item);
            }
            
            // Add menu to menu bar
            ajouter_menu_a_barre(menu_bar, menu_name, menu);
            
            // Add menu bar to the preview area
            if (!GTK_IS_CONTAINER(app_data->preview_area)) {
                g_print("Error: preview_area is not a container\n");
            } else {
                // Create a box to hold the menu bar at the top
                GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
                
                // Add the menu bar to the box
                ajouter_menu_dans_conteneur(menu_bar, vbox);
                
                // Add the box to the preview area at the top
                gtk_fixed_put(GTK_FIXED(app_data->preview_area), vbox, 0, 0);
                
                // Add to hierarchy
                GtkTreeIter iter;
                gtk_tree_store_append(app_data->hierarchy_store, &iter, NULL);
                gtk_tree_store_set(app_data->hierarchy_store, &iter, 
                                  0, g_strdup_printf("Menu Bar: %s", menu_name), 
                                  1, menu_bar->menu_bar, -1);
                
                // Show the menu bar
                gtk_widget_show_all(app_data->preview_area);
            }
        }
        
        // Clean up menu_items list
        g_list_free(menu_data.menu_items);
    }

    gtk_widget_destroy(dialog);
}

// Button click handler for adding a menu bar
static void add_menu_bar_clicked(GtkWidget *button, gpointer data) {
    AppData *app_data = (AppData *)data;
    show_menu_bar_dialog(app_data);
}

#endif /* MENU_DIALOG_H */
