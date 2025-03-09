#include <gtk/gtk.h>
#include "button.h"
#include "btn.macros.h"
#include "coordonnees.h"
#include "global.h"
#include "includes_button.h"
#include "box.h"

// Callback functions for testing
static void on_button_clicked(GtkWidget *widget, gpointer data) {
    g_print("Button [%s] clicked!\n", gtk_widget_get_name(widget));
}

static void on_toggle_state_changed(GtkWidget *widget, gpointer data) {
    gboolean active = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
    g_print("Toggle button [%s] is now %s\n", 
            gtk_widget_get_name(widget), 
            active ? "active" : "inactive");
}

static void on_spinner_changed(GtkWidget *widget, gpointer data) {
    gdouble value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(widget));
    g_print("Spinner [%s] value changed to: %.2f\n", 
            gtk_widget_get_name(widget), value);
}

static void on_switch_state_changed(GtkWidget *widget, gpointer data) {
    gboolean active = gtk_switch_get_active(GTK_SWITCH(widget));
    g_print("Switch [%s] is now %s\n", 
            gtk_widget_get_name(widget), 
            active ? "ON" : "OFF");
}

int main(int argc, char *argv[]) {
    // Initialize GTK
    gtk_init(&argc, &argv);
    
    // Create main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Button Types Demo");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    // Create a fixed container for our buttons
    GtkWidget *fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), fixed);
    
    // Add section headings
    GtkWidget *heading1 = gtk_label_new("Normal Buttons:");
    gtk_fixed_put(GTK_FIXED(fixed), heading1, 20, 20);
    
    GtkWidget *heading2 = gtk_label_new("Radio Buttons:");
    gtk_fixed_put(GTK_FIXED(fixed), heading2, 20, 120);
    
    GtkWidget *heading3 = gtk_label_new("Checkbox Buttons:");
    gtk_fixed_put(GTK_FIXED(fixed), heading3, 20, 220);
    
    GtkWidget *heading4 = gtk_label_new("Toggle Buttons:");
    gtk_fixed_put(GTK_FIXED(fixed), heading4, 400, 20);
    
    GtkWidget *heading5 = gtk_label_new("Spin Buttons:");
    gtk_fixed_put(GTK_FIXED(fixed), heading5, 400, 120);
    
    GtkWidget *heading6 = gtk_label_new("Switch Buttons:");
    gtk_fixed_put(GTK_FIXED(fixed), heading6, 400, 220);
    
    // SECTION 1: Normal Buttons
    
    // 1.1. Normal button with fixed position
    btn *normal_fixed = btnNormalFixed(
        "normal_fixed",
        "Fixed Normal",
        "This is a fixed normal button",
        fixed,
        cord(20, 50),
        dim(150, 40),
        NULL
    );
    btn *created_normal_fixed = creer_button(normal_fixed);
    g_signal_connect(created_normal_fixed->button, "clicked", G_CALLBACK(on_button_clicked), NULL);
    
    // 1.2. Normal button with image
    MonImage* btn_image = init_image("icon.png", *dim(16, 16), *cord(200, 50));
    if (btn_image != NULL) {
        btn *normal_img = btnNormalFixed(
            "normal_image",
            "Button with Image",
            "This button has an image",
            fixed,
            cord(200, 50),
            dim(180, 40),
            btn_image
        );
        btn *created_normal_img = creer_button(normal_img);
        g_signal_connect(created_normal_img->button, "clicked", G_CALLBACK(on_button_clicked), NULL);
    }
    
    // SECTION 2: Radio Buttons
    
    // 2.1. Radio buttons group with fixed position
    btn *radio1 = btnRadioFixed(
        "radio1",
        "Option 1",
        "Select option 1",
        fixed,
        cord(20, 150),
        NULL,
        NULL
    );
    btn *created_radio1 = creer_button(radio1);
    
    btn *radio2 = btnRadioFixed(
        "radio2",
        "Option 2",
        "Select option 2",
        fixed,
        cord(20, 180),
        created_radio1->button,
        NULL
    );
    btn *created_radio2 = creer_button(radio2);
    
    // SECTION 3: Checkbox Buttons
    
    // 3.1. Checkbox with fixed position
    btn *checkbox1 = btnCheckFixed(
        "checkbox1",
        "Enable Feature 1",
        "Toggle feature 1",
        cord(20, 250),
        fixed,
        FALSE,
        NULL
    );
    btn *created_checkbox1 = creer_button(checkbox1);
    g_signal_connect(created_checkbox1->button, "toggled", G_CALLBACK(on_toggle_state_changed), NULL);
    
    btn *checkbox2 = btnCheckFixed(
        "checkbox2",
        "Enable Feature 2",
        "Toggle feature 2",
        cord(20, 280),
        fixed,
        TRUE,  // Initially checked
        NULL
    );
    btn *created_checkbox2 = creer_button(checkbox2);
    g_signal_connect(created_checkbox2->button, "toggled", G_CALLBACK(on_toggle_state_changed), NULL);
    
    // SECTION 4: Toggle Buttons
    
    // 4.1. Toggle button with fixed position
    btn *toggle = btnToggleFixed(
        "toggle1",
        "Toggle Me",
        "This is a toggle button",
        fixed,
        cord(400, 50),
        dim(150, 40),
        NULL
    );
    btn *created_toggle = creer_button(toggle);
    g_signal_connect(created_toggle->button, "toggled", G_CALLBACK(on_toggle_state_changed), NULL);
    
    // SECTION 5: Spin Buttons
    
    // 5.1. Spin button with fixed position
    spinObj *spin_params = createSpinObj(0.0, -100.0, 100.0, 1.0, 0);
    btn *spinner = btnSpinFixed(
        "spinner1",
        "Spinner",
        "Adjust value with up/down buttons",
        dim(120, 30),
        cord(400, 150),
        fixed,
        spin_params
    );
    btn *created_spinner = creer_button(spinner);
    g_signal_connect(created_spinner->button, "value-changed", G_CALLBACK(on_spinner_changed), NULL);
    
    // 5.2. Decimal spinner
    spinObj *decimal_spin_params = createSpinObj(0.0, -10.0, 10.0, 0.1, 2); // 2 decimal places
    btn *decimal_spinner = btnSpinFixed(
        "decimal_spinner",
        "Decimal Spinner",
        "Spin with decimal values",
        dim(120, 30),
        cord(550, 150),
        fixed,
        decimal_spin_params
    );
    btn *created_decimal_spinner = creer_button(decimal_spinner);
    g_signal_connect(created_decimal_spinner->button, "value-changed", G_CALLBACK(on_spinner_changed), NULL);
    
    // SECTION 6: Switch Buttons
    
    // 6.1. Switch button with fixed position
    btn *switch_btn = btnSwitchFixed(
        "switch1",
        "Switch",
        "Toggle switch on/off",
        fixed,
        cord(400, 250),
        dim(80, 40),
        NULL,
        FALSE  // Initially off
    );
    btn *created_switch = creer_button(switch_btn);
    g_signal_connect(created_switch->button, "notify::active", G_CALLBACK(on_switch_state_changed), NULL);
    
    // Create a box with radio buttons using the helper function
    // Create a styled box for grouping radio buttons
    StyledBox *box1 = init_styled_box(
        GTK_ORIENTATION_VERTICAL,  // Vertical orientation
        FALSE,                     // Not homogeneous
        5,                         // 5px spacing
        NULL,                      // No CSS classes
        "#f0f0f0",                 // Light gray background
        "5px",                     // 5px border radius
        "1px",                     // 1px border
        cord(20, 320),             // Position
        dim(200, 150),             // Size
        fixed                      // Container
    );
    create_styled_box(box1);
    
    // Add a label for the box
    GtkWidget *box_label = gtk_label_new("Radio Group in Box:");
    gtk_fixed_put(GTK_FIXED(fixed), box_label, 20, 300);
    
    // Add radio buttons to the box using the helper function
    char* radio_options[] = {"Red", "Green", "Blue", "Yellow", NULL};
    liste_radios(radio_options, NULL, box1);
    
    // Create another box for checkboxes
    StyledBox *box2 = init_styled_box(
        GTK_ORIENTATION_VERTICAL,  // Vertical orientation
        FALSE,                     // Not homogeneous
        5,                         // 5px spacing
        NULL,                      // No CSS classes
        "#e0e0f0",                 // Light blue background
        "5px",                     // 5px border radius
        "1px",                     // 1px border
        cord(400, 320),            // Position
        dim(200, 150),             // Size
        fixed                      // Container
    );
    create_styled_box(box2);
    
    // Add a label for the box
    GtkWidget *box2_label = gtk_label_new("Checkbox Group in Box:");
    gtk_fixed_put(GTK_FIXED(fixed), box2_label, 400, 300);
    
    // Add checkboxes to the box using the helper function
    char* check_options[] = {"Option A", "Option B", "Option C", "Advanced", NULL};
    liste_checks(check_options, NULL, box2);
    
    // Apply styles to some buttons
    Style *red_style = init_style("Sans", NULL, 12, 1, "#ff6666", 2, 5);
    appliquer_style_button(red_style, created_normal_fixed);
    
    Style *blue_style = init_style("Sans", NULL, 11, 0, "#6699ff", 1, 3);
    appliquer_style_button(blue_style, created_toggle);
    
    // Show all and run main loop
    gtk_widget_show_all(window);
    gtk_main();
    
    return 0;
}
