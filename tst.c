#include <gtk/gtk.h>
#include "global.h"
// #include "Scale.h"
// #include "scroll.h"

int main(int argc, char *argv[]) {
    // Initialisation de GTK
    gtk_init(&argc, &argv);
////////////////////////////////---création de window---///////////////////////////////////////////////
    // Dimensions et coordonnées de la fenêtre
    dimension dim = {800, 850}; // Largeur: 800px, Hauteur: 600px
    coordonnees cord = {200, 100}; // Position: x=200, y=100

    // Couleur de fond
    HexColor *bg_color = hex_color_init("#FFC0CB"); // Couleur rose

    // Image de fond
    MonImage bg_img = {
           .path ="background.png" , // Remplacez par le chemin de votre image
             .dim = dim,
           .cord = cord
    };

    // Initialisation de la fenêtre
    Mywindow *maFenetre = init_window("                                             test des fonctions GTK+", "icon1.png",
            dim,                       // Dimensions
            0,                        // Bordure
            FALSE,                      // Redimensionnable
            GTK_WIN_POS_CENTER,        // Position: Centrée
            cord,                      // Coordonnées
            *bg_color,                 // Couleur de fond
            bg_img                     // Image de fond


    );

    // Création de la fenêtre
    create_window(maFenetre);

   // GtkWidget *fixed;
   // fixed=gtk_fixed_new();
  //  gtk_container_add(GTK_CONTAINER(maFenetre->window),fixed);


  //  gtk_fixed_put(GTK_FIXED(fixed),maFenetre->bgImg.Image,0,0);
    // Affiche tous les widgets dans la fenêtre
////////////////////////////////////////////////////////////////////////////////////////////////////////////:

///////////////////////////////////////---fixed---//////////////////////////////////////////////////////

    fixedo *fixed;
    fixed = init_fixed("principal", dim);
    creer_fixed(fixed);

    // Créer une fenêtre de défilement
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_size_request(scrolled_window, dim.width, dim.height);

    // Activer le défilement automatique
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                   GTK_POLICY_ALWAYS,  // Toujours afficher horizontalement
                                   GTK_POLICY_ALWAYS); // Toujours afficher verticalement

    // Définir une taille plus grande pour permettre le défilement
    gtk_widget_set_size_request(fixed->fixed_container, dim.width * 2, dim.height * 2);

    // Ajouter le conteneur fixe à la fenêtre de défilement
    gtk_container_add(GTK_CONTAINER(scrolled_window), fixed->fixed_container);

    // Ajouter la fenêtre de défilement à la fenêtre principale
    gtk_container_add(GTK_CONTAINER(maFenetre->window), scrolled_window);

    // Ajouter des widgets à l'intérieur du conteneur fixe
    fixed_add_widget(fixed, maFenetre->bgImg.Image, 0, 0);



 //////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////


    /////////////////////////////////////////////////////////////////////////


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Création d'une fenêtre de défilement
    //GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);

    // Définir la taille de la zone de défilement
    gtk_widget_set_size_request(scrolled_window, 300, 400);

    // Activer le scrolling horizontal et vertical
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                   GTK_POLICY_AUTOMATIC,
                                   GTK_POLICY_AUTOMATIC);

    // Ajouter le TextView à la fenêtre de défilement
    //gtk_container_add(GTK_CONTAINER(scrolled_window), created_textview->elem);

    // Ajouter la fenêtre de défilement au conteneur fixe
    fixed_add_widget(fixed, scrolled_window, 100, 300);

    // Forcer l'affichage des widgets
    gtk_widget_show_all(scrolled_window);

/////////////////////////////////////---label---////////////////////////////////////////////////////////////:

    // Définition des coordonnées et des dimensions
    coordonnees C1 = {0, 20};  // Exemple de coordonnées (x, y)
    dimension D1 = {200, 50};   // Exemple de dimensions (largeur, hauteur)

    // Initialisation du label avec un texte, un titre, des coordonnées et des dimensions
    Monlabel *label = init_label("Ceci est un exemple de texte dans le Label.", "MonLabelTitre", C1, D1);

    // Création du widget GTK Label à partir de l'objet Monlabel
    Monlabel *created_label = creer_label(label);
    fixed_add_widget(fixed,label->elem,C1.x,C1.y);




/////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////---label---////////////////////////////////////////////////////////////:

    // Définition des coordonnées et des dimensions
    coordonnees C3 = {0, 190};  // Exemple de coordonnées (x, y)
    dimension D3 = {200, 50};   // Exemple de dimensions (largeur, hauteur)
    coordonnees C4 = {5, 240};  // Exemple de coordonnées (x, y)
    dimension D4 = {200, 50};   // Exemple de dimensions (largeur, hauteur)

    // Initialisation du label avec un texte, un titre, des coordonnées et des dimensions
    Monlabel *label3 = init_label("Ceci est un exemple de texte dans le Label.", "MonLabelTitre", C3, D3);
    Monlabel *label4 = init_label("Ceci", "MonLabelTitre", C4, D4);

    // Création du widget GTK Label à partir de l'objet Monlabel
    Monlabel *created_labelll = creer_label(label3);
    fixed_add_widget(fixed,label3->elem,C3.x,C3.y);
    Monlabel *created_labellll = creer_label(label4);
    fixed_add_widget(fixed,label4->elem,C4.x,C4.y);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Créer un cadre (zone avec bordure noire)
    GtkWidget *frame2 = gtk_frame_new(NULL);
    gtk_widget_set_size_request(frame2, 300, 70); // Définir la taille (largeur, hauteur)

    // Appliquer une bordure noire
    gtk_widget_override_background_color(frame2, GTK_STATE_FLAG_NORMAL, &(GdkRGBA){0, 0, 1, 1}); // Bordure noire

    // Rendre l'intérieur transparent
    gtk_widget_override_background_color(frame2, GTK_STATE_FLAG_NORMAL, &(GdkRGBA){0, 0, 0, 0}); // Transparent

    // Créer un conteneur fixe à l'intérieur du cadre
    GtkWidget *fixed_zone2 = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(frame2), fixed_zone2);

    // Ajouter le cadre à la fenêtre principale ou un autre conteneur
    gtk_fixed_put(GTK_FIXED(fixed->fixed_container), frame2, 10, 310); // Positionner le cadre

/////////////////////////////////////---comboBox---//////////////////////////////////////////////////////////////////

    // Définir les dimensions et les coordonnées de la ComboBox
    dimension dim1 = {50, 50};  // Largeur et hauteur
    coordonnees cord1 = {50, 320};  // Position (x, y)

    // Initialiser la structure gtkComboBox
    gtkComboBox *combo = init_comboBox(dim1, cord1, "monComboBox", fixed->fixed_container);

    // Créer la ComboBox avec la fonction create_ComboBox
    create_ComboBox(combo);

    // Ajouter des éléments à la ComboBox
    ajouterElementComboBox(combo, "1", "1");
    ajouterElementComboBox(combo, "2", "2");
    ajouterElementComboBox(combo, "3", "3");

    // Définir l'élément actif (sélectionner l'option 1)
    elementActive(combo, 0);


////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////---menu---///////////////////////////////////////////////////////
    // Créer une barre de menu
    MenuBar *menu_bar = creer_menu_bar();
    // Créer un menu "Edition"
    Menu *menu_edition = creer_menu();
    ajouter_element_au_menu(menu_edition, "Couper", G_CALLBACK(on_menu_item_activate));
    ajouter_element_au_menu(menu_edition, "Copier", G_CALLBACK(on_menu_item_activate));
    ajouter_element_au_menu(menu_edition, "Coller", G_CALLBACK(on_menu_item_activate));
    ajouter_menu_a_barre(menu_bar, "Édition", menu_edition);
    // Créer un menu "Fichier"
    Menu *menu_fichier = creer_menu();
    ajouter_element_au_menu(menu_fichier, "Ouvrir", G_CALLBACK(on_menu_item_activate));
    //ajouter_element_au_menu(menu_fichier, "Enregistrer", G_CALLBACK(on_menu_item_activate));

    // Ajouter un sous-menu à "Options"
    GtkWidget *sous_menu = creer_sous_menu();
    GtkWidget *item_options = gtk_menu_item_new_with_label("Options");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(item_options), sous_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_fichier->menu), item_options);

    ajouter_element_au_menu(menu_fichier, "Quitter", G_CALLBACK(gtk_main_quit));
    ajouter_menu_a_barre(menu_bar, "Fichier", menu_fichier);



    // Créer un menu "Aide"
    Menu *menu_aide = creer_menu();
    ajouter_element_au_menu(menu_aide, "À propos", G_CALLBACK(on_menu_item_activate));
    ajouter_menu_a_barre(menu_bar, "Aide", menu_aide);
    fixed_add_widget(fixed,menu_bar->menu_bar,500,0);
    // Ajouter la barre de menu à la fenêtre


//////////////////////////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////---boite_Dialogue---///////////////////////////////////////
    // Exemple de boîte de dialogue avec position prédéfinie


    // Création de la dimension
    dimension *dim3 = creer_dimension(300, 300);

    // Création du label
    GtkWidget *content_label = gtk_label_new("Voulez-vous afficher une image ?");

    // Chargement de l'image
    GtkWidget *image1 = gtk_image_new_from_file("image.jpg");  // Remplace "image.jpg" par ton chemin réel

    // Conteneur vertical pour l'image et le texte
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(vbox), image1, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), content_label, FALSE, FALSE, 5);

    // Création de la boîte de dialogue
    boite_dialog *dialog1 = DIALOG(
        vbox,
        "Dialogue Prédefini",
        dim3,
        NULL,
        maFenetre->window,
        TRUE,
        GTK_WIN_POS_CENTER_ON_PARENT
    );

    // Création et affichage de la boîte de dialogue
    creer_boite_dialogue(dialog1);

    // Gestion de la fermeture de la boîte de dialogue
    g_signal_connect(dialog1->dialog, "response", G_CALLBACK(gtk_widget_destroy), NULL);
////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////:---image---////////////////////////////////////////////////////:
 MonImage *image;
 dimension *img_dim= creer_dimension(100,100);
 coordonnees *img_coord= create_coordonnees(1000,100);
 image= init_image("ex_img.jpg",*img_dim,*img_coord);
    creer_image(image);
    fixed_add_widget(fixed,image->Image,img_coord->x,img_coord->y);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////---Boutons---///////////////////////////////////////////////////////
    // Créer le bouton OK
    MonImage* ima = init_image("icon.png", *dim(32, 32), *cord(32, 32));
    btn* btnOk = creer_button(btnNormalFixed("Nom", "ok", "Tooltip", fixed->fixed_container, cord(10, 600), dim(64, 32), ima));

    // Connecter le signal clicked à la fonction onOkClick
    g_signal_connect(btnOk->button, "clicked", G_CALLBACK(onOkClick), NULL);

    // Créer le bouton Quitter
    MonImage* ima1 = init_image("icon.png", *dim(50, 50), *cord(50, 50));
    btn* btnQuitter = creer_button(btnNormalFixed("Nom", "quitter", "Tooltip", fixed->fixed_container, cord(10, 700), dim(64, 32), ima1));

    // Connecter le signal clicked à la fonction onQuitClick
    g_signal_connect(btnQuitter->button, "clicked", G_CALLBACK(onQuitClick), NULL);
    //Boutons radio
    // Créer un cadre (zone avec bordure noire)
    GtkWidget *frame1 = gtk_frame_new(NULL);
    gtk_widget_set_size_request(frame1, 200, 50); // Définir la taille (largeur, hauteur)

    // Appliquer une bordure noire
    gtk_widget_override_background_color(frame1, GTK_STATE_FLAG_NORMAL, &(GdkRGBA){0, 0, 1, 1}); // Bordure noire

    // Rendre l'intérieur transparent
    gtk_widget_override_background_color(frame1, GTK_STATE_FLAG_NORMAL, &(GdkRGBA){0, 0, 0, 0}); // Transparent

    // Créer un conteneur fixe à l'intérieur du cadre
    GtkWidget *fixed_zone1 = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(frame1), fixed_zone1);

    // Ajouter le cadre à la fenêtre principale ou un autre conteneur
    gtk_fixed_put(GTK_FIXED(fixed->fixed_container), frame1, 10, 220); // Positionner le cadre

    //Création du pére du groupe
    btn* pere=creer_button(btnRadioFixed("Femme","Femme","Femme",fixed->fixed_container,cord(10,230),NULL, NULL));
    //Création des autres boutons appartenant au meme groupe que 'pere'
    creer_button(btnRadioFixed("Homme","Homme","Homme",fixed->fixed_container,cord(100,230),pere->button, NULL));

    //Boutons check
    // Créer un cadre (zone avec bordure noire)
    GtkWidget *frame = gtk_frame_new(NULL);
    gtk_widget_set_size_request(frame, 100, 50); // Définir la taille (largeur, hauteur)

    // Appliquer une bordure noire
    gtk_widget_override_background_color(frame, GTK_STATE_FLAG_NORMAL, &(GdkRGBA){0, 0, 1, 1}); // Bordure noire

    // Rendre l'intérieur transparent
    gtk_widget_override_background_color(frame, GTK_STATE_FLAG_NORMAL, &(GdkRGBA){0, 0, 0, 0}); // Transparent

    // Créer un conteneur fixe à l'intérieur du cadre
    GtkWidget *fixed_zone = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(frame), fixed_zone);

    // Ajouter le cadre à la fenêtre principale ou un autre conteneur
    gtk_fixed_put(GTK_FIXED(fixed->fixed_container), frame, 10, 140); // Positionner le cadre

    // Ajouter les boutons dans la zone fixe
    creer_button(btnCheckFixed("Foot", "Football", "Football", cord(10, 10), fixed_zone, FALSE, NULL));
    creer_button(btnCheckFixed("Basket", "BasketBall", "BasketBall", cord(100, 10), fixed_zone, FALSE, NULL));
    creer_button(btnCheckFixed("Basket", "mon", "BasketBall", cord(200, 10), fixed_zone, FALSE, NULL));
    creer_button(btnCheckFixed("Basket", "mon", "BasketBall", cord(300, 10), fixed_zone, FALSE, NULL));


    /////////////////////////////////////---label---////////////////////////////////////////////////////////////:

    // Définition des coordonnées et des dimensions
    coordonnees C2 = {0, 100};  // Exemple de coordonnées (x, y)
    dimension D2 = {200, 100};   // Exemple de dimensions (largeur, hauteur)

    // Initialisation du label avec un texte, un titre, des coordonnées et des dimensions
    Monlabel *label1 = init_label("le de texte dans le Label.", "MonLabelTitre", C2, D2);

    // Création du widget GTK Label à partir de l'objet Monlabel
    Monlabel *created_labell = creer_label(label1);
    fixed_add_widget(fixed,label1->elem,C2.x,C2.y);






    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    //Boutons toggle
    creer_button(btnToggleFixed("Nom","Toggle","Tooltip",fixed->fixed_container, NULL,dim(100,32),NULL));
    //Boutons switch
    creer_button(btnSwitchFixed("switch","switch","switch",fixed->fixed_container,cord(900,600),dim(64,8),NULL, TRUE));
    //Button spin
    creer_button(btnSpinFixed("spin","spin","spin",dim(128,32), cord(1050,600), fixed->fixed_container, spinObj(5,2)));


////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////---scale---//////////////////////////////////////////////////////
    coordonnees scale_coord = {500, 700};  // Position de l'échelle
    GtkWidget *scale_widget = create_horizontal_scale("Choose Value:", 0, 100, 50);
    fixed_add_widget(fixed, scale_widget, scale_coord.x, scale_coord.y);


///////////////////////////////////////
    gtk_widget_show_all(maFenetre->window);


    // Boucle principale GTK
    gtk_main();

    // Libération des ressources


    return 0;
}
