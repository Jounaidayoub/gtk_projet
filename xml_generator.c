#include <gtk/gtk.h>
#include "global.h"
#include "xml_utility.h"
#include "bouton_xml.h"
#include "checkbox_xml.h"
#include "dialog_xml.h"
#include "radio_xml.h"
#include "taha_template.h"
#include "box.h"

void on_open_file_button_clicked(GtkWidget *button, gpointer user_data) {
    GtkWidget *file_dialog;
    GtkWindow *parent_window = GTK_WINDOW(user_data);

    // Créer la boîte de dialogue FileChooser
    file_dialog = gtk_file_chooser_dialog_new("Ouvrir un fichier",
                                              parent_window,
                                              GTK_FILE_CHOOSER_ACTION_OPEN,
                                              "_Annuler", GTK_RESPONSE_CANCEL,
                                              "_Ouvrir", GTK_RESPONSE_ACCEPT,
                                              NULL);

    // Afficher la boîte de dialogue et récupérer la réponse
    if (gtk_dialog_run(GTK_DIALOG(file_dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(file_dialog);

        // Récupérer le chemin du fichier sélectionné
        filename = gtk_file_chooser_get_filename(chooser);
        g_print("Fichier sélectionné : %s\n", filename);

        // Libérer la mémoire
        g_free(filename);
    }

    // Détruire la boîte de dialogue après utilisation
    gtk_widget_destroy(file_dialog);
}



int maind(int argc, char *argv[]) {
    // Initialisation de GTK
    gtk_init(&argc, &argv);
////////////////////////////////---création de window---///////////////////////////////////////////////
    // Dimensions et coordonnées de la fenêtre
    dimension dim = {1000, 1500}; // Largeur: 800px, Hauteur: 600px
    coordonnees cord = {200, 100}; // Position: x=200, y=100

    // Couleur de fond
    HexColor *bg_color = hex_color_init("#FFC0CB"); // Couleur rose

    // Image de fond
    MonImage bg_img = {
            .path = "background.png", // Remplacez par le chemin de votre image
            .dim = dim,
            .cord = cord
    };

    // Initialisation de la fenêtre
    Mywindow *maFenetre = init_window("Ma Première Fenêtre GTK", "icon.png",
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
fixed= init_fixed("principal",dim);
    creer_fixed(fixed);

    gtk_container_add(GTK_CONTAINER(maFenetre->window),fixed->fixed_container);

    fixed_add_widget(fixed,maFenetre->bgImg.Image,0,0);//Appliquer l'arriere plan




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////---comboBox---//////////////////////////////////////////////////////////////////

    // Définir les dimensions et les coordonnées de la ComboBox
    dimension dim1 = {50, 30};  // Largeur et hauteur
    coordonnees cord1 = {50, 50};  // Position (x, y)

    // Initialiser la structure gtkComboBox
    gtkComboBox *combo = init_comboBox(dim1, cord1, "monComboBox", fixed->fixed_container);

    // Créer la ComboBox avec la fonction create_ComboBox
    create_ComboBox(combo);

    // Ajouter des éléments à la ComboBox
    ajouterElementComboBox(combo, "1", "Option 1");
    ajouterElementComboBox(combo, "2", "Option 2");
    ajouterElementComboBox(combo, "3", "Option 3");

    // Définir l'élément actif (sélectionner l'option 1)
    elementActive(combo, 0);


//////////////////////////////////---progressBar--/////////////////////////////////////////
    {
        // Initialiser les dimensions
        dimension dim_pulse = {.width = 300, .height = 30};
        dimension dim_fraction = {.width = 300, .height = 30};

        // // Créer une barre de progression de type pulsation
        // progress_bar_type_pulse *pbar_pulse = Init_ProgressBar_Type_Pulse(
        //         &dim_pulse, TRUE, 'b', 100);

        // GtkWidget *pulse_widget = add_progressbar_Tpulse(pbar_pulse);
        // // Position dans le conteneur fixed
        // fixed_add_widget(fixed, pulse_widget, 500, 900);


        // // Créer une barre de progression fractionnée

        // progress_bar_type_fraction *pbar_fraction = Init_ProgressBar_Type_Fraction(
        //         &dim_fraction, TRUE, 'r', 10.0);
        // // Simuler un téléchargement avec `DownloadProgress`
        // DownloadProgress *progress = g_malloc(sizeof(DownloadProgress));
        // progress->progress_bar = GTK_PROGRESS_BAR(pbar_fraction->pbar);
        // progress->total_size = 1024 * 100;  // 100 KB
        // progress->current_size = 0;
        // progress->is_active = TRUE;

        // GtkWidget *fraction_widget = add_progressbar_Tfraction(pbar_fraction, progress);
        // fixed_add_widget(fixed, fraction_widget, 500, 920);
    }
////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////---boite_Dialogue---///////////////////////////////////////
    // Exemple de boîte de dialogue avec position prédéfinie
    dimension *dim3 = creer_dimension(300,300);

    GtkWidget *content_label = gtk_label_new("Ceci est une boîte de dialogue avec position prédéfinie.");
    boite_dialog *dialog1 = DIALOG(
            content_label,//Contenu(un widget)
            "Dialogue Prédefini",//titre
            dim3,//dimension
            NULL, // Pas d'icône pour cet exemple
            maFenetre->window,//Parent(conteneur)
            TRUE,//modal ou pas
            GTK_WIN_POS_CENTER_ON_PARENT//Position
    );
    creer_boite_dialogue(dialog1);
    g_signal_connect(dialog1->dialog, "response", G_CALLBACK(gtk_widget_destroy), NULL);
////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////---fileChooser---////////////////////////////////////////////////
    // Créer un bouton
    GtkWidget *button = gtk_button_new_with_label("Ouvrir un fichier");
    fixed_add_widget(fixed,button,700,400); // Positionner à (150, 100)

    // Connecter le signal "clicked" du bouton à la fonction qui ouvre le FileChooser
    g_signal_connect(button, "clicked", G_CALLBACK(on_open_file_button_clicked), fixed->fixed_container);
//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////:---image---////////////////////////////////////////////////////:
 MonImage *image;
 dimension *img_dim= creer_dimension(100,100);
 coordonnees *img_coord= create_coordonnees(1000,100);
 image= init_image("ex_img.jpg",*img_dim,*img_coord);
    creer_image(image);
    fixed_add_widget(fixed,image->Image,img_coord->x,img_coord->y);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////---Boutons---///////////////////////////////////////////////////////
    //Bouton normal avec image
    MonImage* ima = init_image("icon.png",*dim(32,32), *cord(32,32));
    creer_button(btnNormalFixed("Nom","Label","Tooltip",fixed->fixed_container, cord(0,600),dim(64,32),ima));

    //Boutons radio
    //Création du pére du groupe
    btn* pere=creer_button(btnRadioFixed("Femme","Femme","Femme",fixed->fixed_container,cord(200,600),NULL, NULL));
    //Création des autres boutons appartenant au meme groupe que 'pere'
    creer_button(btnRadioFixed("Homme","Homme","Homme",fixed->fixed_container,cord(300,600),pere->button, NULL));

    //Boutons check
    creer_button(btnCheckFixed("Foot", "Football", "Football", cord(450, 600), fixed->fixed_container, FALSE, NULL));
    creer_button(btnCheckFixed("Basket", "BasketBall", "BasketBall", cord(550, 600), fixed->fixed_container, FALSE, NULL));
    //Boutons toggle
    creer_button(btnToggleFixed("Nom","Toggle","Tooltip",fixed->fixed_container, NULL,dim(700,32),NULL));
    //Boutons switch
    creer_button(btnSwitchFixed("switch","switch","switch",fixed->fixed_container,cord(900,600),dim(64,8),NULL, TRUE));
    //Button spin
    creer_button(btnSpinFixed("spin","spin","spin",dim(128,32), cord(1050,600), fixed->fixed_container, spinObj(5,2)));


////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////
    gtk_widget_show_all(maFenetre->window);

    // Boucle principale GTK
    gtk_main();

    // Libération des ressources


    return 0;
}


int main(int argc, char *argv[]) {
    // Initialisation de GTK
    gtk_init(&argc, &argv);
    FILE *file = fopen("data.html", "r");
    if (!file) {
        printf("ERREUR d'ouverture du fichier !!");
        return -1;
    }
    creer_object(file, 0);
    gtk_main();
    return 0;
}


void open_dialog(GtkWidget* widget, gpointer data){
    fixedo* fix = (fixedo*) data;
    dialogue_template("Boite", "Voulez vous afficher une image",
                      "ex_img.jpg", "icon.png", *cord(1200, 600),
                      fix->fixed_container);
}

int mainf(int argc, char *argv[]){
    // Initialisation de GTK
    gtk_init(&argc, &argv);
////////////////////////////////---création de window---///////////////////////////////////////////////
    // Dimensions et coordonnées de la fenêtre
    dimension dim = {1000, 1500}; // Largeur: 800px, Hauteur: 600px
    coordonnees cord = {200, 100}; // Position: x=200, y=100

    // Couleur de fond
    HexColor *bg_color = hex_color_init("#FFC0CB"); // Couleur rose

    // Image de fond
    MonImage bg_img = {
            .path = "background.png", // Remplacez par le chemin de votre image
            .dim = dim,
            .cord = cord
    };

    // Initialisation de la fenêtre
    Mywindow *maFenetre = init_window("Test des fonctions de GTK+", "icon.png",
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

    ajouterHeader(maFenetre,50,900,"Test des Fonctions GTK+",
                  "ex_img.jpg",50,50);

///////////////////////////////////////---fixed---//////////////////////////////////////////////////////

    fixedo *fixed;
    fixed= init_fixed("principal",dim);
    creer_fixed(fixed);

    gtk_container_add(GTK_CONTAINER(maFenetre->window),fixed->fixed_container);

    fixed_add_widget(fixed,maFenetre->bgImg.Image,0,0);//Appliquer l'arriere plan

    ////////////////////////////////////---menu---///////////////////////////////////////////////////////
    menu(fixed);

   //////////////////////////////////liste des radios////////////////////
    {
        //Les labels des boutons
        char *labels[256] = {"Faible", "Moyen", "Bien", "Très bien", NULL};
        //Le style commun des boutons
        Style *st = init_style("Colspan", hex_color_init("#000"), 8, 1, NULL, -1, -1);
        //Creer le box
        StyledBox *bx = init_styled_box(GTK_ORIENTATION_HORIZONTAL, TRUE,
                                        16, NULL,
                                        "light-gray", "0", "3", cord(10, 600),dim(20, 20), fixed->fixed_container);
        create_styled_box(bx);
        //Creer la liste des boutons radios
        liste_radios(labels, st, bx);
    }
    //////////////////////////////////liste des checks////////////////////
    {
        //Les labels des boutons
        char *labelsc[256] = {"Convival", "Ineressant", "Facile à utiliser", "Optimise le travail", NULL};
        //Le style commun des boutons
        Style *stc = init_style("Colspan", hex_color_init("#000"), 12, 1, NULL, -1, -1);
        //Creer le box
        StyledBox *bxc = init_styled_box(GTK_ORIENTATION_HORIZONTAL, TRUE,
                                         16, NULL,
                                         "light-gray", "0", "3",cord(10, 300), dim(20, 20),fixed->fixed_container);
        create_styled_box(bxc);
        //Creer la liste des boutons radios
        liste_checks(labelsc, stc, bxc);

    }
    //Associer un label au liste radio
    //Monlabel* l1 = creer_label(init_label("Vos connaissance de GTK!", "ti", *cord(10,550), *dim(0,0)));
    //fixed_add_widget(fixed, l1->elem, 10, 560);

    //Associer un label au liste checks
    //Monlabel* l2 = creer_label(init_label("Vos commentaires sur GTK!", "ti", *cord(10,250), *dim(0,0)));
    //fixed_add_widget(fixed, l2->elem, 10, 260);

    ////////////////////////Dialogue prédefinie//////////////////////////////
    /*dialogue_template("HEllo", "im a message",
                      "ex_img.jpg", "icon.png", *cord(0, 100),
                      fixed->fixed_container);
                      */
    //////////////////////////////////  Button ///////////////////////////////////////////////////////
    btn* bb = btnNormalFixed("nom", "label", "tooltip", fixed->fixed_container,
                             cord(100, 100), dim(50, 20), NULL);
    creer_button(bb);
    g_signal_connect(bb->button, "clicked", G_CALLBACK(open_dialog), fixed->fixed_container);

    Style* bst = init_style(NULL, hex_color_init("#ffff"),
                            -1, -1, "ex_img.jpg", -1,
                            -1);
    appliquer_style_button(bst, bb);
    /////////////////////////////Text view/////////////////////////////

    StyledBox* bbox = init_styled_box(GTK_ORIENTATION_HORIZONTAL,
                                      TRUE, 15, "", "", "5", "1", cord(100, 600), dim(20, 20),fixed->fixed_container);
    create_styled_box(bbox);
    // Initialisation du TextView avec un texte, un titre, des coordonnées et des dimensions
    MonTextView *textview = init_textview(" exemple de texte dans le TextView.", "MonTextViewTitre",
                                          *cord(100, 500), *dim(300, 50), bbox->widget);

    // Création du widget GTK TextView à partir de l'objet MonTextView
    MonTextView *created_textview = creer_textview(textview);
    ///////////////////////////////////////Combobox///////////////////////////////////////////////
    // Initialiser la structure gtkComboBox
    gtkComboBox *combo = init_comboBox(*dim(90, 30), *cord(90, 50),
                                       "monComboBox", fixed->fixed_container);

    // Créer la ComboBox avec la fonction create_ComboBox
    create_ComboBox(combo);

    // Ajouter des éléments à la ComboBox
    ajouterElementComboBox(combo, "1", "Option 1");
    ajouterElementComboBox(combo, "2", "Option 2");
    ajouterElementComboBox(combo, "3", "Option 3");

    // Définir l'élément actif (sélectionner l'option 1)
    elementActive(combo, 0);

    //////////////////////////////Test////////////////////////////

    StyledBox* container = init_styled_box(GTK_ORIENTATION_HORIZONTAL, TRUE, 30, "",
                                      "", "5", "2", cord(150, 150),
                                           dim(20, 20),fixed->fixed_container);
    create_styled_box(container);

    //JOurs
    {
        StyledBox* jour = init_styled_box(GTK_ORIENTATION_VERTICAL, TRUE, 0, "",
                                          "", "0", "0", cord(150, 150),
                                          dim(20, 20),container->widget);
        create_styled_box(jour);

        creer_label(init_label("Jours", "Jours", NULL, NULL, jour->widget, "red", 1, "16", "Colspan"));

        gtkComboBox *cjour = init_comboBox(*dim(20, 20), *cord(0, 0), "", jour->widget);
        create_ComboBox(cjour);
        for (int i = 0; i < 31; i++) {
            char snum[2];
            itoa(i, snum, 10);
            ajouterElementComboBox(cjour, snum, snum);
        }
        elementActive(cjour, 1);
    }

    //mois
    {
        StyledBox* mois = init_styled_box(GTK_ORIENTATION_VERTICAL, TRUE, 15, "",
                                          "", "0", "0", cord(150, 150),
                                          dim(20, 20),container->widget);
        create_styled_box(mois);

        creer_label(init_label("Mois", "Mois", NULL, NULL, mois->widget, "black", 1, "12", "Colspan"));

        gtkComboBox *cjour = init_comboBox(*dim(20, 20), *cord(0, 0), "", mois->widget);
        create_ComboBox(cjour);
        for (int i = 0; i < 13; i++) {
            char snum[2];
            itoa(i, snum, 10);
            ajouterElementComboBox(cjour, snum, snum);
        }
        elementActive(cjour, 1);

    }

    ///////////////////////////---entry---///////////////////////////////////////////////////////////////////////

    {
        // Initialiser les dimensions
        dimension dim_basic = {.width = 200, .height = 30};
        dimension dim_password = {.width = 200, .height = 30};

        // Initialiser un entry_type_basic
        entry_type_basic *basic_entry = Init_Entry_Basic(

                &dim_basic,
                TRUE,  // editable
                TRUE,  // visible
                "Entrez votre texte ici...",
                50,    // longueur maximale
                "Texte par défaut",
                fixed->fixed_container,
                cord(600, 200)
        );

        // Ajouter l'entrée basic au conteneur
        GtkWidget *basic_entry_widget = creer_entry_basic(basic_entry);
        //fixed_add_widget(fixed,basic_entry_widget,600,200);

        // Initialiser un entry_type_password
        entry_type_password *password_entry = Init_Entry_Password(

                &dim_password,
                "Mot de passe...",
                '*',
                fixed->fixed_container,
                cord(600, 250)
        );

        // Ajouter l'entrée password au conteneur
        GtkWidget *password_entry_widget = creer_entry_pass(password_entry);
        //fixed_add_widget(fixed,password_entry_widget,600,250);

        // Ajouter des classes CSS (facultatif)
        entry_set_css_class(basic_entry_widget, "basic-entry");
        entry_set_css_class(password_entry_widget, "password-entry");

        //Submit button(work just for the last one)
        g_signal_connect(bb->button, "clicked", G_CALLBACK(simule_entry_activation), basic_entry_widget);
        g_signal_connect(bb->button, "clicked", G_CALLBACK(simule_entry_activation), password_entry_widget);

    }
    ///////////////////////////////////////////////////////////////////////////////////////////////
   gtk_widget_show_all(maFenetre->window);

    // Boucle principale GTK
    gtk_main();

    // Libération des ressources
    return 0;
}

int maine(int argc, char *argv[]) {
    // Initialisation de GTK
    gtk_init(&argc, &argv);
////////////////////////////////---création de window---///////////////////////////////////////////////
    // Dimensions et coordonnées de la fenêtre
    dimension dim = {1000, 1500}; // Largeur: 800px, Hauteur: 600px
    coordonnees cord = {200, 100}; // Position: x=200, y=100

    // Couleur de fond
    HexColor *bg_color = hex_color_init("#FFC0CB"); // Couleur rose

    // Image de fond
    MonImage bg_img = {
            .path = "background.png", // Remplacez par le chemin de votre image
            .dim = dim,
            .cord = cord
    };

    // Initialisation de la fenêtre
    Mywindow *maFenetre = init_window("Presentation Ordianteur", "icon.png",
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

    ajouterHeader(maFenetre, 50, 900, "Presentation Ordianteur",
                  "ex_img.jpg", 50, 50);

///////////////////////////////////////---fixed---//////////////////////////////////////////////////////

    fixedo *fixed;
    fixed = init_fixed("principal", dim);
    creer_fixed(fixed);

    gtk_container_add(GTK_CONTAINER(maFenetre->window), fixed->fixed_container);

    fixed_add_widget(fixed, maFenetre->bgImg.Image, 0, 0);//Appliquer l'arriere plan

    ///////////////////////////////////////Exam/////////////////////////////////////

    //MENU
    menu(fixed);

    //Liste radio
    //////////////////////////////////liste des radios////////////////////
    {
        dimension diml = {10, 15}; // Largeur: 800px, Hauteur: 600px
        coordonnees cordl = {0, 200}; // Position: x=200, y=100

        dimension dimr = {100, 150}; // Largeur: 800px, Hauteur: 600px
        coordonnees cordr = {0, 220}; // Position: x=200, y=100

        creer_label(init_label("La qualité de votre oridinateur", "Jours",
                               &cordl, &diml, fixed->fixed_container,
                               "black", 1, "16", "Colspan"));

        //Les labels des boutons
        char *labels[256] = {"Très bonne", "Moyenne", "Faible",  NULL};
        //Le style commun des boutons
        Style *st = init_style("Colspan", hex_color_init("#000"), 8, 1, NULL, -1, -1);
        //Creer le box
        StyledBox *bx = init_styled_box(GTK_ORIENTATION_HORIZONTAL, TRUE,
                                        16, NULL,
                                        "", "0", "3",
                                        &cordr, dim(20, 20),fixed->fixed_container);
        create_styled_box(bx);
        //Creer la liste des boutons radios
        liste_radios(labels, st, bx);
    }

    //////////////////////////////////liste des checks////////////////////
    {

        dimension diml = {10, 10}; // Largeur: 800px, Hauteur: 600px
        coordonnees cordl = {0, 400}; // Position: x=200, y=100

        dimension dimr = {1000, 1500}; // Largeur: 800px, Hauteur: 600px
        coordonnees cordr = {0, 420}; // Position: x=200, y=100

        creer_label(init_label("Son ergonomie", "erognomie",
                               &cordl, &diml, fixed->fixed_container,
                               "black", 1, "16", "Colspan"));

        //Les labels des boutons
        char *labelsc[256] = {"Convival", "Ineressant", "Facile à utiliser", "Optimise le travail", NULL};
        //Le style commun des boutons
        Style *stc = init_style("Colspan", hex_color_init("#000"), 8, 1, NULL, -1, -1);
        //Creer le box
        StyledBox *bxc = init_styled_box(GTK_ORIENTATION_HORIZONTAL, TRUE,
                                         16, NULL,
                                         "", "0", "3",
                                         &cordr, dim(20, 20),fixed->fixed_container);
        create_styled_box(bxc);
        //Creer la liste des boutons radios
        liste_checks(labelsc, stc, bxc);

    }

    ///////////////////////////////////////Combobox///////////////////////////////////////////////
    // Initialiser la structure gtkComboBox
    creer_label(init_label("Liste deroulante", "Jours",
                           cord(1200, 200), dim(0,0), fixed->fixed_container,
                           "black", 1, "16", "Colspan"));

    gtkComboBox *combo = init_comboBox(*dim(300, 30), *cord(1200, 230),
                                       "monComboBox", fixed->fixed_container);

    // Créer la ComboBox avec la fonction create_ComboBox
    create_ComboBox(combo);

    // Ajouter des éléments à la ComboBox
    ajouterElementComboBox(combo, "1", "HP");
    ajouterElementComboBox(combo, "2", "DELL");
    ajouterElementComboBox(combo, "3", "ASUS");
    ajouterElementComboBox(combo, "4", "ACER");

    // Définir l'élément actif (sélectionner l'option 1)
    elementActive(combo, 0);



    //////////////////////////////////liste des radios////////////////////
    {
        dimension diml = {10, 15}; // Largeur: 800px, Hauteur: 600px
        coordonnees cordl = {0, 500}; // Position: x=200, y=100

        dimension dimr = {100, 150}; // Largeur: 800px, Hauteur: 600px
        coordonnees cordr = {0, 530}; // Position: x=200, y=100

        creer_label(init_label("Utilisation de votre ordinateur", "Jourss",
                               &cordl, &diml, fixed->fixed_container,
                               "black", 1, "16", "Colspan"));

        //Les labels des boutons
        char *labels[256] = {"Très bonne", "Moyenne", "Faible",  NULL};
        //Le style commun des boutons
        Style *st = init_style("Colspan", hex_color_init("#000"), 8, 1, NULL, -1, -1);
        //Creer le box
        StyledBox *bx = init_styled_box(GTK_ORIENTATION_VERTICAL, TRUE,
                                        16, NULL,
                                        "", "0", "3",
                                        &cordr, dim(20, 20),fixed->fixed_container);
        create_styled_box(bx);
        //Creer la liste des boutons radios
        liste_radios(labels, st, bx);
    }

    //100 1500
    //////////////////////////////////  Button ///////////////////////////////////////////////////////

        btn *bb = btnNormalFixed("OK", "OK", "OK", fixed->fixed_container,
                                 cord(1300, 850), dim(150, 50), NULL);
        creer_button(bb);
        g_signal_connect(bb->button, "clicked", G_CALLBACK(open_dialog), fixed->fixed_container);

        Style *bst = init_style("Colspan", hex_color_init("#ffff"),
                                8, -1, "ex_img.jpg", -1,
                                -1);
        appliquer_style_button(bst, bb);



    //////////////////////////////////  Button ///////////////////////////////////////////////////////
    {
        btn *bb = btnNormalFixed("Quitter", "Quitter", "Quitter", fixed->fixed_container,
                                 cord(1000, 850), dim(150, 50), NULL);
        creer_button(bb);
        g_signal_connect(bb->button, "clicked", gtk_main_quit, fixed->fixed_container);

        Style *bst = init_style(NULL, hex_color_init("#1111"),
                                -1, -1, "background.png", -1,
                                -1);
        appliquer_style_button(bst, bb);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    gtk_widget_show_all(maFenetre->window);

    // Boucle principale GTK
    gtk_main();

    // Libération des ressources
    return 0;
}