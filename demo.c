#include <gtk/gtk.h>
#include "global.h"
#include "app_data.h"     // Must come first
#include "entry.h"
#include "hierarchy.h"
#include "containers.h"   // Moved before property_panel.h
#include "property_panel.h"
#include "dialogs.h"
#include "callbacks.h"
#include "menu.h"          // Add the menu.h include
#include "menu_dialog.h"   // Add the new menu_dialog.h include
#include "entry_editing.h"  // Include this new header last

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
            1,  // Changed from 0 to 1 to set maximized state
            TRUE,                      // Make window resizable
            GTK_WIN_POS_CENTER,
            cord,
            *bg_color,
            bg_img
    );
    app_data.window = maFenetre->window;

    // Création de la fenêtre
    create_window(maFenetre);
    
    // Explicitly maximize the window to ensure it starts maximized
    gtk_window_maximize(GTK_WINDOW(maFenetre->window));

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
    
    // Add menu bar button
    GtkWidget *menu_bar_btn = gtk_button_new_with_label("Add Menu Bar");
    g_signal_connect(menu_bar_btn, "clicked", G_CALLBACK(add_menu_bar_clicked), &app_data);
    gtk_box_pack_start(GTK_BOX(containers_box), menu_bar_btn, FALSE, FALSE, 2);
    
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
    
    // Register background click handler for the preview area
    g_signal_connect(app_data.preview_area, "button-press-event", 
                    G_CALLBACK(on_preview_area_click), &app_data);
    
    // Create right properties panel
    app_data.properties_panel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_size_request(app_data.properties_panel, 250, -1);  // Make it a bit wider
    
    // Initialize property panel (from property_panel.h)
    init_property_panel(&app_data);
    
    // Create property-related event handlers
    app_data.selected_widget = NULL;  // No widget selected initially
    
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
    
    gtk_box_pack_end(GTK_BOX(app_data.properties_panel), button_box, FALSE, FALSE, 0);
    
    gtk_box_pack_start(GTK_BOX(main_box), app_data.properties_panel, FALSE, FALSE, 0);
    
    // Show all widgets
    gtk_widget_show_all(maFenetre->window);
    gtk_main();
    
    return 0;
}
