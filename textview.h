//
// Created by ACER on 06/01/2025.
//

#ifndef TEST1_TEXTVIEW_H
#define TEST1_TEXTVIEW_H
#include "global.h"

// Définition de la structure MonTextView
typedef struct {
    GtkWidget *elem;        // Widget du TextView
    gchar *texte;           // Texte du TextView
    gchar *titre;           // Titre du TextView (optionnel)
    coordonnees Crd;        // Coordonnées du TextView
    dimension dim;          // Dimensions du TextView
    GtkWidget* container;//conteneur
} MonTextView;



// Fonction pour initialiser un TextView
MonTextView *init_textview(const gchar *texte, const gchar *tit, coordonnees C, dimension D, GtkWidget* container) {
    if (!texte) {
        printf("Texte vide pour le TextView!\n");
        exit(-1);
    }

    MonTextView *T = (MonTextView *)malloc(sizeof(MonTextView));
    if (!T) {
        printf("Erreur d'allocation mémoire pour le TextView.\n");
        exit(-1);
    }

    // Allocation et initialisation du titre
    if (tit) {
        T->titre = (gchar *)malloc(30 * sizeof(gchar));
        if (!T->titre) {
            printf("Erreur d'allocation mémoire pour le titre du TextView.\n");
            exit(-1);
        }
        strcpy(T->titre, tit);
    } else {
        T->titre = NULL;
    }

    // Allocation et initialisation du texte
    T->texte = (gchar *)malloc(300 * sizeof(gchar));
    if (!T->texte) {
        printf("Erreur d'allocation mémoire pour le texte du TextView.\n");
        exit(-1);
    }
    strcpy(T->texte, texte);

    // Initialisation des coordonnées et dimensions
    T->Crd = C;
    T->dim = D;
    if(container) T->container = container;

    return T;
}

// Fonction pour créer un TextView GTK à partir d'un objet MonTextView
MonTextView *creer_textview(MonTextView *T) {
    T->elem = gtk_text_view_new();
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(T->elem));
    gtk_text_buffer_set_text(buffer, T->texte, -1);

    if (T->titre) {
        gtk_widget_set_name(T->elem, T->titre);
    }

    // Create a GtkScrolledWindow and add the TextView to it
    GtkWidget *text_scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(text_scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(text_scroll, T->dim.width, T->dim.height);
    gtk_container_add(GTK_CONTAINER(text_scroll), T->elem);

    // Create a frame and add the scrolled window to it
    // GtkWidget *frame = gtk_frame_new(T->titre);
    // gtk_container_add(GTK_CONTAINER(frame), text_scroll);

    // Add the frame to the container
    if (GTK_IS_FIXED(T->container)) {
        gtk_fixed_put(GTK_FIXED(T->container), text_scroll, T->Crd.x, T->Crd.y);
    } else {
        gtk_container_add(GTK_CONTAINER(T->container), text_scroll);
    }

    return T;
}


void TextView_xml(FILE *file, int  parent)
{
    char text[MAX], height[MAX], width[MAX], bgColor[MAX], icon[MAX];
    char x[10], y[10];
    char car;
    //const gchar *texte, const gchar *tit, coordonnees C, dimension D, GtkWidget* container
    // Si on trouve une balise "property", on continue la lecture
    while ((balise(file)) == 22 )
    {
        char mot[MAX];
        int ind = 0;
        Epeurerblanc(file);
        fseek(file, 6, SEEK_CUR);
        fscanf(file, "%s", mot);
        // Récupération du texte à afficher dans le TextView
        if (!(strcmp("text\"", mot)))
        {
            lire_gchar_str_with_deplacement(file,text,10);
        }
            // Récupération du buffer du TextView
            // Récupération de la position en X
        else if (!(strcmp("x\"", mot)))
        {
            lire_gchar_str_with_deplacement(file,x,10);
        }
            // Récupération de la position en Y
        else if (!(strcmp("y\"", mot)))
        {
            lire_gchar_str_with_deplacement(file,y,10);
        }
            // Récupération de la largeur
        else if (!(strcmp("width\"", mot)))
        {
            lire_gchar_str_with_deplacement(file,width,10);
        }
            // Récupération de la hauteur
        else if (!(strcmp("height\"", mot)))
        {
            lire_gchar_str_with_deplacement(file,height,10);
        }
    }

    MonTextView* tt = init_textview(text, text, *cord(atoi(x), atoi(y)),
                                    *dim(atoi(width), atoi(height)), parents[parent]);

    creer_textview(tt);
    // Appeler récursivement la fonction pour traiter d'autres éléments
    creer_object(file, parent);
}

GtkWidget* create_text_view_with_scrolled_window(dimension *dim, const gchar *title) {
    // Create a scrolled window
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    
    // Set the size of the scrolled window based on the provided dimensions
    gtk_widget_set_size_request(scrolled_window, dim->width, dim->height);
    
    // Create a text view
    GtkWidget *text_view = gtk_text_view_new();
    
    // Add the text view to the scrolled window
    gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);
    
    // Create a frame and add the scrolled window to it
    GtkWidget *frame = gtk_frame_new(title);
    gtk_container_add(GTK_CONTAINER(frame), scrolled_window);
    
    // Show the text view, scrolled window, and frame
    gtk_widget_show(text_view);
    gtk_widget_show(scrolled_window);
    gtk_widget_show(frame);
    
    return frame;
}

#endif //TEST1_TEXTVIEW_H
