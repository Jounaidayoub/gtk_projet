#include <gtk/gtk.h>
#include "global.h"
#include "app_data.h"     // Must come first
#include "entry.h"
#include "hierarchy.h"
#include "containers.h"   // Moved before property_panel.h
#include "property_panel.h"
#include "dialogs.h"
#include "callbacks.h"
#include "menu.h"
#include "menu_dialog.h"
#include "widget_types.h"  // Add the widget type enum header
// #include "forms.h"         // Add the forms system header
#include "widget_props.h"  // Add the widget properties header
#include "entry_editing.h"  // Include this header last

int main(int argc, char *argv[]) {
    // Initialize GTK
    gtk_init(&argc, &argv);
    
    // App data structure to hold our widgets
    AppData app_data;

    // Initialize container management in AppData
    app_data.containers = NULL;
    app_data.selected_container = NULL;

    // Window dimensions and coordinates
    dimension dim = {900, 700};
    coordonnees cord = {400, 250};

    // Background color
    HexColor *bg_color = hex_color_init("#ffffff");

    // Background image
    MonImage bg_img = {
           .path ="ex_img.jpg",
           .dim = dim,
           .cord = cord
    };

    // Initialize the window
    Mywindow *maFenetre = init_window("GTK UI Builder", "icon1.png",
            dim,
            1,  // Set maximized state
            TRUE,                      // Make window resizable
            GTK_WIN_POS_CENTER,
            cord,
            *bg_color,
            bg_img
    );
    app_data.window = maFenetre->window;

    // Create the window
    create_window(maFenetre);
    
    // Maximize the window to ensure it starts maximized
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
    WidgetType widget_types[] = {WIDGET_LABEL, WIDGET_BUTTON, WIDGET_ENTRY_BASIC, 
                              WIDGET_COMBOBOX, WIDGET_SCALE, WIDGET_CHECKBOX};
    
    for (int i = 0; i < 6; i++) {
        GtkWidget *button = gtk_button_new_with_label(widget_labels[i]);
        g_object_set_data(G_OBJECT(button), "widget_type", GINT_TO_POINTER(widget_types[i]));
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
    WidgetType container_types[] = {WIDGET_BOX, WIDGET_FRAME, WIDGET_GRID, WIDGET_SCROLLED_WINDOW};
    
    for (int i = 0; i < 4; i++) {
        GtkWidget *button = gtk_button_new_with_label(container_labels[i]);
        g_object_set_data(G_OBJECT(button), "widget_type", GINT_TO_POINTER(container_types[i]));
        
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
    gtk_widget_set_size_request(hierarchy_scroll, -1, 150);  // Reduced height to make room
    
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
    gtk_tree_store_set(app_data.hierarchy_store, &iter, 0, "Window", 1, NULL, -1);
    
    // Add the hierarchy view to the scrolled window and frame
    gtk_container_add(GTK_CONTAINER(hierarchy_scroll), app_data.hierarchy_view);
    gtk_container_add(GTK_CONTAINER(hierarchy_frame), hierarchy_scroll);
    gtk_box_pack_start(GTK_BOX(left_panel), hierarchy_frame, TRUE, TRUE, 0);
    
    // Add Arbre view section
    GtkWidget *arbre_frame = gtk_frame_new("Arbre Structure");
    GtkWidget *arbre_scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(arbre_scroll),
                                  GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(arbre_scroll, -1, 150);  // Same height as hierarchy view
    
    // Create a placeholder widget - we'll replace it with the actual Arbre view after initialization
    GtkWidget *placeholder_label = gtk_label_new("Arbre view will appear here");
    gtk_container_add(GTK_CONTAINER(arbre_scroll), placeholder_label);
    gtk_container_add(GTK_CONTAINER(arbre_frame), arbre_scroll);
    gtk_box_pack_start(GTK_BOX(left_panel), arbre_frame, TRUE, TRUE, 0);
    
    // Store these widgets in app_data for later updating
    app_data.arbre_scroll = arbre_scroll;
    app_data.arbre_frame = arbre_frame;
    
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
    gtk_widget_set_name(app_data.preview_area, "preview_area");  // Set a name for lookup
    gtk_container_add(GTK_CONTAINER(preview_frame), app_data.preview_area);
    
    // Create a notebook for the main content area
    GtkWidget *notebook = gtk_notebook_new();
    gtk_box_pack_start(GTK_BOX(main_box), notebook, TRUE, TRUE, 0);
    
    // Create the Preview tab
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), preview_frame, gtk_label_new("Preview"));
    
    // Create the XML tab
    GtkWidget *xml_scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(xml_scroll),
                                  GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    
    // Create a TextView to display the XML
    app_data.xml_text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(app_data.xml_text_view), FALSE);
    gtk_container_add(GTK_CONTAINER(xml_scroll), app_data.xml_text_view);
    
    // Add the XML tab to the notebook
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), xml_scroll, gtk_label_new("XML Code"));
    
    // Initialize the Arbre tree structure with preview area as root
    app_data.widget_tree = allouer_arbre("preview_area", app_data.preview_area, NULL, NULL, 1);
    
    // Set the widget type of the preview area
    app_data.widget_tree->type = WIDGET_FIXED;
    
    // Now replace the placeholder with the actual Arbre view
    if (GTK_IS_CONTAINER(arbre_scroll)) {
        GList *children = gtk_container_get_children(GTK_CONTAINER(arbre_scroll));
        if (children) {
            gtk_widget_destroy(GTK_WIDGET(children->data)); // Remove placeholder
            g_list_free(children);
        }
        
        // Create and add the Arbre view
        GtkWidget *arbre_view = creer_arborescence_visuelle(app_data.widget_tree);
        gtk_container_add(GTK_CONTAINER(arbre_scroll), arbre_view);
        app_data.arbre_view = arbre_view;  // Store for later updates
    }
    
    // Create right properties panel with explicit size and frame
    GtkWidget *properties_frame = gtk_frame_new("Properties");
    app_data.properties_panel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_size_request(app_data.properties_panel, 250, -1);
    gtk_container_add(GTK_CONTAINER(properties_frame), app_data.properties_panel);
    
    // Initialize property panel
    init_property_panel(&app_data);
    
    // Create property-related event handlers
    app_data.selected_widget = NULL;  // No widget selected initially
    
    // Add action buttons at bottom
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_set_border_width(GTK_CONTAINER(button_box), 5);
    
    GtkWidget *export_button = gtk_button_new_with_label("Export XML");
    GtkWidget *run_button = gtk_button_new_with_label("Run Demo");
    GtkWidget *debug_button = gtk_button_new_with_label("Debug Tree Structure");
    GtkWidget *exit_button = gtk_button_new_with_label("Exit");
    
    g_signal_connect(export_button, "clicked", G_CALLBACK(export_to_xml), &app_data);
    // g_signal_connect(run_button, "clicked", G_CALLBACK(run_demo), &app_data);
    g_signal_connect(debug_button, "clicked", G_CALLBACK(on_show_arbre_clicked), &app_data);
    g_signal_connect(exit_button, "clicked", G_CALLBACK(gtk_main_quit), NULL);
    
    gtk_box_pack_start(GTK_BOX(button_box), export_button, FALSE, FALSE, 2);
    gtk_box_pack_start(GTK_BOX(button_box), run_button, FALSE, FALSE, 2);
    gtk_box_pack_start(GTK_BOX(button_box), debug_button, FALSE, FALSE, 2);
    gtk_box_pack_start(GTK_BOX(button_box), exit_button, FALSE, FALSE, 2);
    
    // Add test functions button
    GtkWidget *test_functions_button = gtk_button_new_with_label("Test Arbre Functions");
    g_signal_connect(test_functions_button, "clicked", G_CALLBACK(test_arbre_functions), &app_data);
    gtk_box_pack_start(GTK_BOX(button_box), test_functions_button, FALSE, FALSE, 2);
    
    gtk_box_pack_end(GTK_BOX(app_data.properties_panel), button_box, FALSE, FALSE, 0);
    
    // Add properties panel to the main box
    gtk_box_pack_start(GTK_BOX(main_box), properties_frame, FALSE, FALSE, 0);
    
    // Show all widgets
    gtk_widget_show_all(maFenetre->window);
    
    // Print the initial tree structure for debugging
    printf("Initial Arbre Tree Structure:\n");
    afficher_arbre(app_data.widget_tree, 0);
    
    // Start main loop
    gtk_main();
    
    return 0;
}