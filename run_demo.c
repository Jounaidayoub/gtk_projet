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

void run_demo() {
    // Initialisation de GTK
    

    FILE *file = fopen("d.html", "r");
    if (!file) {
        printf("ERREUR d'ouverture du fichier !!");
    }
    creer_object(file, 0);
    
}

int main(int argc, char *argv[]) {
    run_demo();
    return 0;

}