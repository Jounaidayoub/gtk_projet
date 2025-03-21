//
// Created by ACER on 06/01/2025.
//

#ifndef TEST1_MENU_H
#define TEST1_MENU_H
#include "global.h"

#define MAXNAME 50






// Structure pour un élément de menu
typedef struct {
    GtkWidget *menu_item; // L'élément du menu
    char label[MAXNAME];  // Libellé de l'élément
} MenuItem;

// Structure pour un menu
typedef struct {
    GtkWidget *menu;      // Le menu GTK
    MenuItem *items;      // Tableau des éléments du menu
    int num_items;        // Nombre d'éléments dans le menu
} Menu;

// Structure pour une barre de menu
typedef struct {
    GtkWidget *menu_bar;  // La barre de menu GTK
    Menu *menus;          // Tableau des menus
    int num_menus;        // Nombre de menus
} MenuBar;

// Fonction pour créer un menu vide
Menu* creer_menu() {
    Menu *menu = (Menu*) malloc(sizeof(Menu));
    if (!menu) {
        printf("Erreur d'allocation mémoire pour le menu\n");
        exit(-1);
    }
    menu->menu = gtk_menu_new();  // Crée un nouveau menu
    menu->items = NULL;
    menu->num_items = 0;
    return menu;
}

// Fonction pour ajouter un élément à un menu
void ajouter_element_au_menu(Menu *menu, const char *label, GCallback callback) {
    menu->num_items++;
    menu->items = realloc(menu->items, menu->num_items * sizeof(MenuItem));
    if (!menu->items) {
        printf("Erreur d'allocation mémoire pour les éléments du menu\n");
        exit(-1);
    }

    // Initialiser l'élément de menu
    MenuItem *item = &menu->items[menu->num_items - 1];
    item->menu_item = gtk_menu_item_new_with_label(label);
    strncpy(item->label, label, MAXNAME);

    // Connecter un signal à l'élément de menu
    if (callback != NULL) {
        g_signal_connect(item->menu_item, "activate", callback, NULL);
    }

    // Ajouter l'élément au menu
    gtk_menu_shell_append(GTK_MENU_SHELL(menu->menu), item->menu_item);
}

// Fonction pour créer une barre de menu vide
MenuBar* creer_menu_bar() {
    MenuBar *menu_bar = (MenuBar*) malloc(sizeof(MenuBar));
    if (!menu_bar) {
        printf("Erreur d'allocation mémoire pour la barre de menu\n");
        exit(-1);
    }
    menu_bar->menu_bar = gtk_menu_bar_new();  // Crée une nouvelle barre de menu
    menu_bar->menus = NULL;
    menu_bar->num_menus = 0;
    return menu_bar;
}

// Fonction pour ajouter un menu à la barre de menu
void ajouter_menu_a_barre(MenuBar *menu_bar, const char *label, Menu *menu) {
    GtkWidget *menu_item = gtk_menu_item_new_with_label(label);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_item), menu->menu);  // Associer un sous-menu à cet élément de la barre
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar->menu_bar), menu_item);
    menu_bar->num_menus++;
    menu_bar->menus = realloc(menu_bar->menus, menu_bar->num_menus * sizeof(Menu));
    menu_bar->menus[menu_bar->num_menus - 1] = *menu;
}

// Fonction pour ajouter la barre de menu à un conteneur à un endroit spécifique
void ajouter_menu_dans_conteneur(MenuBar *menu_bar, GtkWidget *container) {
    gtk_container_add(GTK_CONTAINER(container), menu_bar->menu_bar);  // Ajouter la barre de menu au conteneur
    gtk_widget_show_all(container);  // Afficher tous les widgets dans le conteneur
}

// Exemple d'une fonction de callback
void on_menu_item_activate(GtkWidget *widget, gpointer data) {
    char* msg = (char*) data;
    g_print("Un élément du menu a été activé\n");
    g_print("Vous avez choisi : %s", msg);
}


//Creer un sous menu avec des options saisies

GtkWidget* creer_sous_menu(char* items[256]) {
    GtkWidget *sous_menu = gtk_menu_new();
    // Ajouter des éléments au sous-menu
    int j = -1;
    while(items[++j]){
        GtkWidget *item_sous_menu1 = gtk_menu_item_new_with_label(items[j]);
        g_signal_connect(item_sous_menu1, "activate", G_CALLBACK(on_menu_item_activate), items[j]);
        gtk_menu_shell_append(GTK_MENU_SHELL(sous_menu), item_sous_menu1);
    }

    /*
    GtkWidget *item_sous_menu1 = gtk_menu_item_new_with_label("Sous-menu 1");
    g_signal_connect(item_sous_menu1, "activate", G_CALLBACK(on_menu_item_activate), "Sous-menu 1");

    GtkWidget *item_sous_menu2 = gtk_menu_item_new_with_label("Sous-menu 2");
    g_signal_connect(item_sous_menu2, "activate", G_CALLBACK(on_menu_item_activate), "Sous-menu 2");

    // Ajouter les éléments au sous-menu
    gtk_menu_shell_append(GTK_MENU_SHELL(sous_menu), item_sous_menu1);
    gtk_menu_shell_append(GTK_MENU_SHELL(sous_menu), item_sous_menu2);
*/
    return sous_menu;
}
#endif //TEST1_MENU_H




















