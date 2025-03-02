//
// Created by ACER on 06/01/2025.
//

#ifndef TEST1_ENTRY_H
#define TEST1_ENTRY_H

#include "global.h"
#include "xml_utility.h"

// Remove the forward declaration since app_data.h will be included before this
// in files that use both headers
// typedef struct _AppData AppData;

typedef struct {

    GtkWidget *entry;
    // gint h;
    // gint w;
    dimension *dim;
    gboolean is_editable;
    gboolean is_visible;
    gchar *placeholder_text;
    gint maxlen;
    gchar *default_text; // the difference between palce holder and defaust txt , the placehlder will be shown only when the enetry is empty ,
    // but the default text will be shown but will not be removed when u start typig
    GtkWidget* container;
    coordonnees* cord;
} entry_type_basic;

typedef struct {

    GtkWidget *entry;
    // gint h;
    // gint w;
    dimension *dim;
    gchar *placeholder_text;
    gchar invisible_char;
    GtkWidget* container;
    coordonnees* cord
} entry_type_password;

// Add freee funct
void free_entry_basic(entry_type_basic *entry) {
    if (entry) {
        g_free(entry->placeholder_text);
        g_free(entry->default_text);
        g_free(entry);
    }
}

void free_entry_password(entry_type_password *entry) {
    if (entry) {
        g_free(entry->placeholder_text);
        g_free(entry);
    }
}

// fomction diniallisation de las structure basic enrtry
entry_type_basic *Init_Entry_Basic(

        // gint height,
        // gint width,
        dimension *dim,
        gboolean editable,
        gboolean visible,
        const gchar *placeholder,
        gint maxlen,
        const gchar *default_text,
        GtkWidget* container,
        coordonnees* cord
) {

    g_return_val_if_fail(dim->height > 0 && dim->width > 0, NULL);
    g_return_val_if_fail(maxlen >= 0, NULL);

    entry_type_basic *entry_basic = (entry_type_basic *)g_malloc(sizeof(entry_type_basic));


    entry_basic->entry = gtk_entry_new();
    entry_basic->dim = dim;
    entry_basic->is_editable = editable;
    entry_basic->is_visible = visible;
    entry_basic->placeholder_text = g_strdup(placeholder);
    entry_basic->maxlen = maxlen;
    entry_basic->default_text = g_strdup(default_text);
    entry_basic->container = container;
    entry_basic->cord = cord;
    return entry_basic;
}

// fomction diniallisation de las structure password
entry_type_password *Init_Entry_Password(

        // gint height,
        // gint width,
        dimension *dim,
        const gchar *placeholder,
        gchar invisible_char,
        GtkWidget* container,
        coordonnees* cord
) {


    entry_type_password *entry_password = (entry_type_password *)g_malloc(sizeof(entry_type_password));


    entry_password->entry = gtk_entry_new();
    entry_password->dim=dim;
    entry_password->placeholder_text = g_strdup(placeholder);
    entry_password->invisible_char = invisible_char;
    entry_password->container = container;
    entry_password->cord = cord;

    return entry_password;
}

// add fucnt for basci type
GtkWidget *creer_entry_basic(entry_type_basic *entry_basic) {
    // Configure the entry
    gtk_widget_set_size_request(entry_basic->entry, entry_basic->dim->width, entry_basic->dim->height);


    // Set properties
    gtk_entry_set_visibility(GTK_ENTRY(entry_basic->entry), entry_basic->is_visible);
    gtk_editable_set_editable(GTK_EDITABLE(entry_basic->entry), entry_basic->is_editable);
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_basic->entry), entry_basic->placeholder_text);

    if (entry_basic->maxlen > 0) {
        gtk_entry_set_max_length(GTK_ENTRY(entry_basic->entry), entry_basic->maxlen);
    }

    if (entry_basic->default_text != NULL) {
        gtk_entry_set_text(GTK_ENTRY(entry_basic->entry), entry_basic->default_text);
    }

    // Set alignment
    gtk_widget_set_halign(entry_basic->entry, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(entry_basic->entry, GTK_ALIGN_CENTER);

    //Ajouter au conteneur
    if(GTK_IS_FIXED(entry_basic->container)){
        gtk_fixed_put(GTK_FIXED(entry_basic->container), entry_basic->entry, entry_basic->cord->x, entry_basic->cord->y);
    }
    gtk_container_add(GTK_CONTAINER(entry_basic->container), entry_basic->entry);

    return entry_basic->entry;
}

//si vous Attacher ce signal à un bouton, alors si on clicke sur le bouton on va appeler la fonction on_entry_activated qui va faire un traitement pour l'entry
void simule_entry_activation(GtkButton *button, gpointer user_data) {
    GtkEntry *entry = GTK_ENTRY(user_data);
    g_signal_emit_by_name(entry, "activate"); // Simule l'appui sur Entrée
}
void on_entry_activated(GtkEntry *entry, gpointer user_data) {
    g_print("Entrée soumise : %s\n", gtk_entry_get_text(entry));
}

// add fucn for the password type
GtkWidget *creer_entry_pass(entry_type_password *entry_password) {
    gtk_widget_set_size_request(entry_password->entry, entry_password->dim->width, entry_password->dim->height);


    gtk_entry_set_visibility(GTK_ENTRY(entry_password->entry), FALSE);
    gtk_entry_set_invisible_char(GTK_ENTRY(entry_password->entry), entry_password->invisible_char);
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_password->entry), entry_password->placeholder_text);

    gtk_widget_set_halign(entry_password->entry, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(entry_password->entry, GTK_ALIGN_CENTER);

    //Ajouter au conteneur
    if(GTK_IS_FIXED(entry_password->container)){
        gtk_fixed_put(GTK_FIXED(entry_password->container), entry_password->entry, entry_password->cord->x, entry_password->cord->y);
    }
    gtk_container_add(GTK_CONTAINER(entry_password->container), entry_password->entry);

    //Ajouter un signal
    g_signal_connect(entry_password->entry, "activate", G_CALLBACK(on_entry_activated), NULL);

    return entry_password->entry;
}

// Extended functions for entry creation that register them for property editing
// Function declaration - implementation will be elsewhere
struct _AppData; // Forward declaration
GtkWidget *creer_entry_basic_with_editing(entry_type_basic *entry_basic, struct _AppData *app_data);
GtkWidget *creer_entry_pass_with_editing(entry_type_password *entry_password, struct _AppData *app_data);

// adding tehs css to stylize the entriess
void entry_set_css_class(GtkWidget *entry, const gchar *class_name) {
    GtkStyleContext *context = gtk_widget_get_style_context(entry);
    gtk_style_context_add_class(context, class_name);
}


void entry_xml(FILE *file,int parent)
{
    char x[10],y[10], type[10], is_editable[2],is_visible[2],
     width[10], height[10], max[10];
    char placeholder[256],default_text[256];

    while((balise(file))==22) {
        char mot[MAX];
        int ind = 0;
        Epeurerblanc(file);
        fseek(file, 6, SEEK_CUR);
        fscanf(file, "%s", mot);

        if (!(strcmp("type\"", mot))) {
            lire_gchar_str_with_deplacement(file, type, 10);
        }
        //recuperation du text par defaut
        if (!(strcmp("placeholder\"", mot))) {
            lire_gchar_str_with_deplacement(file, placeholder, 10);
        }
       else if (!(strcmp("visible\"", mot))) {
            lire_gchar_str_with_deplacement(file, is_visible, 10);
        }
        else if (!(strcmp("editable\"", mot))) {
            lire_gchar_str_with_deplacement(file, is_editable, 10);
        }
        else if (!(strcmp("x\"", mot))) {
            lire_gchar_str_with_deplacement(file, x, 10);
        } else if (!(strcmp("y\"", mot))) {
            lire_gchar_str_with_deplacement(file, y, 10);
        } else if (!(strcmp("width\"", mot))) {
            lire_gchar_str_with_deplacement(file, width, 10);
        } else if (!(strcmp("height\"", mot)))
        {
            lire_gchar_str_with_deplacement(file, height, 10);
        } else if (!(strcmp("max\"", mot))) { //max caractéres
            lire_gchar_str_with_deplacement(file, max, 10);
        } else if (!(strcmp("default_text\"", mot))) { //default_text
            lire_gchar_str_with_deplacement(file, default_text, 10);
        }

    }
    if(!strcmp(type, "basic")){
        entry_type_basic* eb = Init_Entry_Basic(dim(atoi(width), atoi(height)), atoi(is_editable),
                                            atoi(is_visible), placeholder, atoi(max), default_text,
                                            parents[parent], cord(atoi(x), atoi(y)));
        creer_entry_basic(eb);
    }
    else if(!strcmp(type, "pass")){
        entry_type_password* ep = Init_Entry_Password(dim(atoi(width), atoi(height)), placeholder,
                                                      '*',parents[parent],
                                                      cord(atoi(x), atoi(y)));
        creer_entry_pass(ep);
    }
    creer_object(file,parent);
}



#endif //TEST1_ENTRY_H

