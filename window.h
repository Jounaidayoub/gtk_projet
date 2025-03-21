//
// Created by ACER on 16/12/2024.
//

#ifndef PROJET_GTK_WINDOW_H
#define PROJET_GTK_WINDOW_H
#include "global.h"


#define MAX 100

typedef struct window {
    GtkWidget *window;
    dimension dim;        // Dimensions de la fenêtre
    gint border_size;     // Épaisseur de la bordure de la fenêtre
    gchar title[MAX];     // Titre de la fenêtre (maximum MAX caractères)
    gchar icon_name[MAX]; // Nom de l'icône de la fenêtre
    gint resisable;       // Indique si la fenêtre est redimensionnable (0: FALSE, 1: TRUE)
    gchar position;       // Position initiale de la fenêtre [c: GTK_WIN_POS_CENTER]
    // ou [m: GTK_WIN_POS_MOUSE] ...
    coordonnees cord;     // Coordonnées de positionnement de la fenêtre par rapport à l'axe y et x
    HexColor  bgColor;   // Couleur de fond de la fenêtre
    MonImage bgImg;      // Image de fond de la fenêtre (pointeur pour éviter la copie)
    gchar subtitle[MAX]; // Sous-titre de la fenêtre (maximum MAX caractères)
} Mywindow;


// Fonction pour initialiser la structure Mywindow
Mywindow* init_window(const gchar *title, const gchar *icon_name, dimension dim, gint border_size,
                      gint resizable, gchar position, coordonnees cord, HexColor bgColor, MonImage bgImg) {


    // Allocation dynamique de mémoire pour une nouvelle instance de window
    Mywindow* maFenetre=NULL;
    maFenetre=(Mywindow*)malloc(sizeof(Mywindow));
    // Vérification de l'allocation de mémoire
    if(!maFenetre)
    {
        printf("\nerreur d'allocation de memoire.");
        exit(0);
    }
    // Initialisation des champs de la structure window avec les valeurs fournies
    maFenetre->dim=dim;
    maFenetre->border_size=border_size;
    strcpy(maFenetre->title,title);
    strcpy(maFenetre->icon_name,icon_name);
    maFenetre->resisable=resizable;
    maFenetre->position=position;
    maFenetre->bgColor=bgColor;
    maFenetre->cord=cord;

    ///////////////////////////////////////////////////////////////////////////////////////::
    maFenetre->bgImg=bgImg;
   // maFenetre->bgImg.Image = gtk_image_new_from_file(maFenetre->bgImg.path);
    creer_image(&(maFenetre->bgImg));


    ////////////////////////////////////////////////////////////////////////////////
    // Retour de la structure window initialisée
    return ((Mywindow*)maFenetre);


}
void create_window(Mywindow *maFenetre)
{
    // Crée une nouvelle fenêtre GTK+ de type toplevel
    maFenetre->window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    // Définit la taille par défaut de la fenêtre en utilisant les dimensions fournies
    gtk_window_set_default_size(GTK_WINDOW(maFenetre->window),maFenetre->dim.width,maFenetre->dim.height);
    // Définit la position initiale de la fenêtre
    gtk_window_set_position(GTK_WINDOW(maFenetre->window), maFenetre->position);
    // Définit le titre de la fenêtre
    gtk_window_set_title(GTK_WINDOW(maFenetre->window), maFenetre->title);
    // Définit l'épaisseur de la bordure de la fenêtre
    gtk_container_set_border_width(GTK_CONTAINER(maFenetre->window), maFenetre->border_size);
    // Définit si la fenêtre est redimensionnable en fonction de la valeur de resizable
    if(!maFenetre->resisable)
        gtk_window_set_resizable(GTK_WINDOW(maFenetre->window),FALSE);
    else
        gtk_window_set_resizable(GTK_WINDOW(maFenetre->window),TRUE);
    // Modifie la couleur de fond de la fenêtre
    // gtk_widget_override_background_color(maFenetre->window, GTK_STATE_FLAG_NORMAL,maFenetre->bgColor.color);
    // Déplace la fenêtre aux coordonnées spécifiées dans cord
    gtk_window_move(GTK_WINDOW(maFenetre->window), maFenetre->cord.x,maFenetre->cord.y);
    // Charge l'icône de la fenêtre à partir du chemin donné
    GdkPixbuf *icon= create_icon_from_file(maFenetre->icon_name);

    //appliquer l'icon
    gtk_window_set_icon(GTK_WINDOW(maFenetre->window), icon);
    MonImage *background_image = init_image(maFenetre->bgImg.path,maFenetre->dim,maFenetre->cord);
    // Ajoute le widget d'image à la fenêtre
    // gtk_container_add(GTK_CONTAINER(maFenetre->window), background_image->Image);
}




///////////////////////////////////////////////////////////////////////////////////////////
GtkWidget* create_image_widget_from_file(const gchar *file_path)
{
    GdkPixbuf *pixbuf;
    GtkWidget *image_widget;

    // Charger l'image à partir du fichier spécifié
    pixbuf = gdk_pixbuf_new_from_file(file_path, NULL);
    if (!pixbuf) {
        // Afficher une erreur si le chargement de l'image échoue
        g_printerr("Erreur lors du chargement de l'image : %s\n", file_path);
        return NULL;
    }
    // Créer un widget GtkImage à partir de l'image chargée
    image_widget = gtk_image_new_from_pixbuf(pixbuf);
    // Libérer la mémoire utilisée par le pixbuf, car le widget GtkImage le détient désormais
    g_object_unref(pixbuf);
    return image_widget;
}//FIN fonction create_image_widget_from_file()
void set_icon_size(GtkWidget *image_widget, int width, int height)
{
    GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(image_widget));
    if (pixbuf != NULL)
    {
        GdkPixbuf *scaled_pixbuf = gdk_pixbuf_scale_simple(pixbuf, width, height, GDK_INTERP_BILINEAR);
        gtk_image_set_from_pixbuf(GTK_IMAGE(image_widget), scaled_pixbuf);
        g_object_unref(scaled_pixbuf);  // Free the scaled pixbuf after use
    }
}
////////////////////////////////////////////////////////////////////////
//------> fonction pour ajouter headerBar au fenêtre au lieur de header ordinaire
////////////////////////////////////////////////////////////////////////

/*************************************************************
Nom          : ajouterHeader()
Entrée       : window *maFenetre- Un pointeur vers la structure de fenêtre (window)
              int headerHeight  - La hauteur souhaitée de la barre d'en-tête
              char titre[60] - Le titre à afficher dans la barre d'en-tête
              char icon[60] - Le chemin d'accès à l'icône
              int iconWidth - La largeur de l'icône.
              int iconHeight - La hauteur de l'icône.
Sortie       : -
Description  : cette fonction personnalise l'apparence de la fenêtre en
ajoutant une barre d'en-tête
*************************************************************/
#include "includes_button.h"
GtkWidget* ajouterHeader(Mywindow* maFenetre,int headerHeight,int headerWidth,char titre[60],
                    char icon[60],int iconWidth,int iconHeight, gchar* subtitle)
{
    GtkWidget *header_bar = gtk_header_bar_new();
    gtk_widget_set_size_request(header_bar, headerWidth, headerHeight);
    gtk_header_bar_set_title(GTK_HEADER_BAR(header_bar), titre);
    gtk_header_bar_set_subtitle(GTK_HEADER_BAR(header_bar), subtitle);
    gtk_header_bar_set_show_close_button (GTK_HEADER_BAR (header_bar), TRUE);
    // gtk_header_bar_set_decoration_layout (GTK_HEADER_BAR (header_bar), ":minimize,maximize,close");
    // gtk_header_bar_set_decoration_layout (GTK_HEADER_BAR (header_bar), ":maximize,close");
    // gtk_header_bar_set_decoration_layout (GTK_HEADER_BAR (header_bar), ":close");
    // Création d'une icône pour la barre d'en-tête

    if (strlen(maFenetre->icon_name) > 0 && strlen(icon) > 0) {
        GtkWidget *icon_image = create_image_widget_from_file(icon);
        set_icon_size(icon_image, iconWidth, iconHeight);
        gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), icon_image);
    } else {
        GdkPixbuf *empty_icon = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, 1, 1);
        gdk_pixbuf_fill(empty_icon, 0x00000000); // Fill the icon with transparency
        GtkWidget *empty_icon_image = gtk_image_new_from_pixbuf(empty_icon);
        gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), empty_icon_image);
        g_object_unref(empty_icon); // Free the memory allocated for the empty icon
    }

    //     // Create custom buttons for minimize, maximize, and close
    // GtkWidget *minimize_button = gtk_button_new_with_label("–"); // You can use icons instead
    // GtkWidget *maximize_button = gtk_button_new_with_label("□");
    // GtkWidget *close_button    = gtk_button_new_with_label("X");
    // // Pack custom buttons into the header bar (right side)
    // gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), close_button);
    // gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), maximize_button);
    // gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), minimize_button);
    //Create buttons in left side of the header
    // GtkWidget *box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    // gtk_style_context_add_class (gtk_widget_get_style_context (box), "linked");
    // GtkWidget *button1 = gtk_button_new ();
    // gtk_container_add (GTK_CONTAINER (button1), gtk_image_new_from_icon_name ("pan-start-symbolic", GTK_ICON_SIZE_BUTTON));
    // gtk_container_add (GTK_CONTAINER (box), button1);
    // GtkWidget *button2 = gtk_button_new ();
    // gtk_container_add (GTK_CONTAINER (button2), gtk_image_new_from_icon_name ("pan-end-symbolic", GTK_ICON_SIZE_BUTTON));
    // gtk_container_add (GTK_CONTAINER (box), button2);
    // gtk_header_bar_pack_start (GTK_HEADER_BAR (header_bar), box);
    gtk_window_set_titlebar (GTK_WINDOW(maFenetre->window), header_bar);

    return header_bar;
}//Fin Fonction ajouterHeader()

////////////////////////////////////////////////////////////////////////
//-------> ajouter button à header
void ajouterButtonHeaderDebut(GtkWidget *header_bar,GtkWidget *button)
{
    gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), button);
}
void ajouterButtonHeaderFin(GtkWidget *header_bar,GtkWidget *button)
{
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), button);
}
#endif //PROJET_GTK_WINDOW_H
