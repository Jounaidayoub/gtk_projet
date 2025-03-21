//
// Created by Dell on 1/30/2025.
//

#ifndef XML_TAHA_BOUTON_XML_H
#define XML_TAHA_BOUTON_XML_H
#include <gtk/gtk.h>
#include "button.h"
#include "xml_utility.h"

#define MAX 40


//fct de creation de window a partir xml
void window_xml(FILE *file)
{
    gchar *title_gchar,*subtitle_gchar,*bgColor_gchar,*icon_gchar;
    int avoirHeaderBool,closeBtnBool,reduceBtnBool,maxBtnBool;
    int headerHeight;int headerWidth;
    // HexColor *color=NULL;
    MonImage *bg_img =  NULL;
    //hexa_color *color=NULL;
    // char resizable,avoirHeader,border_size,bgColor[MAX];
    char resizable,avoirHeader,border_size,bgImage[MAX];

    // bgImage[0] = '\0';//initialisation
    // bgColor[0] = '\0';//initialisation
    int test, width_int,height_int;
    Mywindow* maFenetre =NULL;
    maFenetre=(Mywindow*)malloc(sizeof(Mywindow));
    // Vérification de l'allocation de mémoire
    if(!maFenetre) exit(-1);
    //si on trouve property balise retourne 22
    while((test=balise(file))==22)
    {
        char mot[MAX];
        int ind=0;
        Epeurerblanc(file);
        fseek(file, 6, SEEK_CUR);//Sauter le mot "name="
        fscanf(file, "%s", mot);//Lire la proprité(name="proprieté")
        printf("\nla 1ere propriété du window est: %s", mot);//le contenu(1ere balise property)
        //recuperation du title
        //Si la propriété est un titre
        if (!(strcmp("title\"", mot)))
        {
            char title[MAX];
            lire_gchar_str(file,title);//Lire le contenu de la balise property
            title_gchar = g_strdup(title);
            strcpy(maFenetre->title,title_gchar);
        }
        else if (!(strcmp("subtitle\"", mot)))
        {
            char subtitle[MAX];
            lire_gchar_str(file,subtitle);//Lire le contenu de la balise property
            subtitle_gchar = g_strdup(subtitle);
            strcpy(maFenetre->subtitle,subtitle_gchar);
        }
            //recuperation de width
        else if (!(strcmp("width\"", mot)))
        {
            char width[MAX];
            lire_gchar_str(file,width);
            width_int=atoi(width);
            maFenetre->dim.width=width_int;
            printf("width de window: %d",width_int);
        }
            //recuperation de height
        else if (!(strcmp("height\"", mot)))
        {
            char height[MAX];
            lire_gchar_str(file,height);
            height_int=atoi(height);
            maFenetre->dim.height=height_int;
            printf("\nheight de window: %d",height_int);
        }
            //recuperation de resizable
        else if (!(strcmp("resizable\"", mot)))
        {
            resizable=lire_gchar(file);
            //convertir en entier
            maFenetre->resisable=char_TO_int(resizable);
            //printf("\nresize -> %d",char_TO_int(resizable));
        }
            //recuperation de border
        else if (!(strcmp("border\"", mot)))
        {
            border_size= lire_gchar(file);
            //convertir en entier
            maFenetre->border_size=char_TO_int(border_size);
            printf("\nborder size -> %d",char_TO_int(border_size));
        }
            //recuperation de position
        else if (!(strcmp("position\"", mot)))
        {
            char position;
            position= lire_gchar(file);
            maFenetre->position=position;
        }
            //recuperation de x
        else if (!(strcmp("x\"", mot)))
        {
            char coordonnes_x[MAX];
            lire_gchar_str(file,coordonnes_x);
            //convertir en entier
            maFenetre->cord.x=atoi(coordonnes_x);
        }
            //recupearation de y
        else if (!(strcmp("y\"", mot)))
        {
            char coordonnes_y[MAX];
            lire_gchar_str(file,coordonnes_y);
            //convertir en entier
            maFenetre->cord.y=atoi(coordonnes_y);
        }
        //recuperation de bgColor
        else if (!(strcmp("bgColor\"", mot)))
        {
            char bgColor[MAX];
            lire_gchar_str(file,bgColor);
            bgColor_gchar = g_strdup(bgColor);
            //color=init_hexa_color(color,bgColor_gchar);
            //maFenetre->bgColor=*color;
        }
            //recuperation de l'icon
        else if (!(strcmp("icon\"", mot)))
        {
            char icon[MAX];
            lire_gchar_str(file,icon);
            icon_gchar = g_strdup(icon);
            strcpy(maFenetre->icon_name,icon_gchar);
        }
            //recuperation choix concerant header bar
        else if (!(strcmp("headerBar\"", mot)))
        {
            char headerBar=lire_gchar(file);
            avoirHeaderBool=char_TO_int(headerBar);      
        }
        else if (!(strcmp("closeBtn\"", mot)))
        {    
            char closeBtn=lire_gchar(file);
            closeBtnBool=char_TO_int(closeBtn);   
        }
        else if (!(strcmp("reduceBtn\"", mot)))
        {   
            char reduceBtn=lire_gchar(file);
            reduceBtnBool=char_TO_int(reduceBtn);        
        }
        else if (!(strcmp("maxBtn\"", mot)))
        { 
            char maxBtn=lire_gchar(file);
            maxBtnBool=char_TO_int(maxBtn);    
        }

        //int headerHeight,int headerWidth
        // else if (!(strcmp("bgImage\"", mot)))
        // {
        //     lire_gchar_str(file,bgImage);
        //     // icon_gchar = g_strdup(bgImage);
        //     // bgImage[0] = '\0';
        //     // MonImage *background_image =
        //     //         create_image_widget(icon_gchar,
        //     //                             maFenetre->cord.x, maFenetre->cord.y, maFenetre->dim);
        //     // maFenetre->bgImg=*background_image;
             
        // }
                //recuperation de bgColor
        // else if (!(strcmp("bgColor\"", mot)))
        // {
        //     char bgColor[MAX];
        //     lire_gchar_str(file,bgColor);
        //     bgColor_gchar = g_strdup(bgColor);

        // }
        else if (!(strcmp("bgImage\"", mot))){
            char bgImageString[MAX];
            lire_gchar_str(file,bgImageString);
            // strcpy(bgImage,bgImageString);
                strcpy(maFenetre->bgImg.path,bgImageString); // Remplacez par le chemin de votre image
                maFenetre->bgImg.dim.width = maFenetre->dim.width;
                maFenetre->bgImg.dim.height = maFenetre->dim.height;
                maFenetre->bgImg.cord.x = 0;
                maFenetre->bgImg.cord.y = 0;
                // maFenetre->bgImg=bg_img;
                creer_image(&(maFenetre->bgImg));

        }
    }
    //creation de la fenêtre
    create_window(maFenetre);
    // HexColor* color=hex_color_init("#7d7d7d");
    // gtk_widget_override_background_color(maFenetre->window, GTK_STATE_FLAG_NORMAL,
    //                                      color->color);
    if(strlen(bgImage)!=0) hasOverlay=1;
        // Ajouter le header bar
    if(avoirHeaderBool){
        GtkWidget* header_bar = ajouterHeader(maFenetre,50,90,
                  maFenetre->title,maFenetre->icon_name,50,50, maFenetre->subtitle);
        char hlayout[256] = ":";
        if(reduceBtnBool) {
            strcat(hlayout, "minimize,");
        }
        if(maxBtnBool) {
            strcat(hlayout, "maximize,");
        }
        if(closeBtnBool){
            strcat(hlayout, "close");
        }
        gtk_header_bar_set_decoration_layout (GTK_HEADER_BAR(header_bar), hlayout);
    }


// bg image
    // if(strlen(bgImage) != 0){
    //     gchar css_data[256];
    //     sprintf(css_data, "window {"
    //     "   background-image: url('%s');"
    //     "   background-size: cover;"
    //     "   background-repeat: no-repeat;"
    //     "   background-position: center;"
    //     "}", bgImage);

    // GtkCssProvider *css_provider = gtk_css_provider_new();
    // gtk_css_provider_load_from_data(css_provider, css_data, -1, NULL);

    // GtkStyleContext *context = gtk_widget_get_style_context(maFenetre->window);
    // gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    // }

    printf("===================================> %d",hasOverlay);
    /*if(char_TO_int(avoirHeader)==1)
        ajouterHeader(maFenetre,50,900,
                      maFenetre->title,maFenetre->icon_name,50,50);
    */
     g_signal_connect(maFenetre->window, "destroy", G_CALLBACK(gtk_widget_destroy), NULL);
    //si child balise retourne 23
    //si la balise est: <child>
    if((test==23))
    {
        parents[0]=maFenetre->window;
        creer_object(file,0);
    }
    gtk_widget_show_all(maFenetre->window);
}

/*
Entrés: FILE *file: fichier à lire
       char mot[MAX]: la propriété
       int parent: le conteneur du bouton(0: window, 2: fixed...)
Description: récupére le contenu des propriétés du bouton et crée une structure btn initialisé
            Initialise le bouton
            Creer le bouton
            Stylise le bouton
*/
void init_btn_normal_xml(FILE *file, int parent){

    //Variables pour initier le bouton
    char color1[MAX], color2[MAX],boxEmplacement;
    gchar *expand_gchar,*fill_gchar;
    gint type_bg = -1; // 0:pour simple background ; 1:pour simple background modern;
    int test, hasMnemonic;
    char x[10], y[10], nom[MAX];;
    char width[MAX], height[MAX];
    gchar *bgColor_gchar, *icon_gchar;
    char title[MAX], icon[MAX], bgcolor[MAX], taille[MAX], border[MAX],
    color[MAX], police[MAX], gras[MAX],border_radius[MAX];
    icon[0] = '\0';
    char car;
    //Lire tout les properties
    while ((test = balise(file)) == 22) {//Tant que la balise à lire est un <property>
        char mot[MAX];
        int ind = 0;
        Epeurerblanc(file);//Supprimer les white spaces
        fseek(file, 6, SEEK_CUR);//Sauter le: name="
        fscanf(file, "%s", mot);//Lire le type de la propriété(text,icon,x,y...)
        // Récupération du titre du bouton
        if (!(strcmp("text\"", mot))) {
            lire_gchar_str_with_deplacement(file, title, 10);
        }
            // Récupération de l'icône du bouton
        else if (!(strcmp("icon\"", mot))) {
            lire_gchar_str_with_deplacement(file, icon, 10);
        }else if (!(strcmp("nom\"", mot))) {
            lire_gchar_str_with_deplacement(file, nom, 10);
        }  else if (!(strcmp("mnemonic\"", mot))) {
            hasMnemonic = char_TO_int(lire_gchar(file));
            if(hasMnemonic){
                //Ajouter le underscore ('_') au title
                char tt[MAX]; tt[0] = '_'; tt[1] = '\0';
                strcat(tt, title);
                strcpy(title, tt);
            }
        }
            // Récupération de la position x du bouton
        else if (!(strcmp("x\"", mot))) {
            lire_gchar_str_with_deplacement(file, x, 10);
        }
            // Récupération de la position y du bouton
        else if (!(strcmp("y\"", mot))) {
            lire_gchar_str_with_deplacement(file, y, 10);
        }
            // Récupération de la largeur du bouton
        else if (!(strcmp("width\"", mot))) {
            lire_gchar_str_with_deplacement(file, width, 10);
        }
            // Récupération de la hauteur du bouton
        else if (!(strcmp("height\"", mot))) {
            lire_gchar_str_with_deplacement(file, height, 10);
        }
        if (!(strcmp("color\"", mot))) {
            lire_gchar_str_with_deplacement(file, color, 10);
        }
        else if (!(strcmp("police\"", mot))) {
            lire_gchar_str_with_deplacement(file, police, 10);
        }
        else if (!(strcmp("taille\"", mot))) {
            lire_gchar_str_with_deplacement(file, taille, 10);
        }
        else if (!(strcmp("gras\"", mot))) {
            lire_gchar_str_with_deplacement(file, gras, 10);

        }
        else if (!(strcmp("bgcolor\"", mot))) {
            type_bg = 0;
            lire_gchar_str_with_deplacement(file, bgcolor, 10);
        }
        else if (!(strcmp("border\"", mot))) {
            lire_gchar_str_with_deplacement(file, border, 10);
        }
        else if (!(strcmp("border_radius\"", mot))) {
            lire_gchar_str_with_deplacement(file, border_radius, 10);
        }
        /*
        else if (!(strcmp("modern_bgcolor\"", mot))) {
            type_bg = 1;
            Epeurerblanc(file);
            fseek(file, 1, SEEK_CUR);
            while (((car = fgetc(file)) != EOF) && (car != '<') && (car != ',')) {
                color1[ind++] = car; // Lire le mot
            }
            color1[ind] = '\0';
            ind = 0;
            while (((car = fgetc(file)) != EOF) && (car != '<')) {
                color2[ind++] = car; // Lire le mot
            }
            color2[ind] = '\0';
            fseek(file, 13, SEEK_CUR);
        }
         */

        //Si on veut ajuster l'emplacement du bouton dans le box
        /*
        else if (!(strcmp("boxEmplacement\"", mot))) {
            Epeurerblanc(file);
            boxEmplacement=lire_gchar(file);
        }
         */
        /*
        else if (!(strcmp("expand\"", mot))) {
            char expand[MAX];
            Epeurerblanc(file);
            fseek(file, 1, SEEK_CUR);
            while (((car = fgetc(file)) != EOF) && (car != '<')) {
                expand[ind++] = car; // Lire le mot
            }
            expand[ind] = '\0';
            printf("%s\n", expand);
            fseek(file, 10, SEEK_CUR);
            expand_gchar = g_strdup(expand);
        }
         */
        /*
        else if (!(strcmp("fill\"", mot))) {
            char fill[MAX];
            Epeurerblanc(file);
            fseek(file, 1, SEEK_CUR);
            while (((car = fgetc(file)) != EOF) && (car != '<')) {
                fill[ind++] = car; // Lire le mot
            }
            fill[ind] = '\0';
            printf("%s\n", fill);
            fseek(file, 10, SEEK_CUR);
            fill_gchar= g_strdup(fill);
        }
         */
    }//fin de while()
    //Creer le style
    HexColor* h = hex_color_init(color);
    Style* style = init_style(police, h, atoi(taille), atoi(gras), bgcolor, atoi(border), atoi(border_radius));

    //Creer l'image du bouton
    MonImage* ima = NULL;//Initier l'image avec NULL
    //Creer l'image si le path est saisie
    if(strlen(icon) > 0){
        ima = init_image(icon,*dim(24,24), *cord(0,0));
    }

    btn* mybtn = NULL;
    //Si le parent est fixed
    if(parent == 2){
        mybtn = btnNormalFixed(nom, title, title,
                       parents[parent], cord(atoi(x), atoi(y)),
                       dim(atoi(width), atoi(height)),
                       ima
        );
    }
    //Si le parent n'est pas fixed
    else{
        mybtn = btnNormal(nom, title, title,
                       parents[parent], margin(atoi(x), atoi(y), atoi(x), atoi(y)),
                       dim(atoi(width), atoi(height)),
                       ima
        );
    }
    //Creer le bouton
    creer_button(mybtn);
    //Appliquer le style au bouton
    appliquer_style_button(style, mybtn);
}


//fct de creation de button a partir xml
void button_xml(FILE *file, int parent) {
    btn* bb = NULL;
    // Si la balise est un <property>
   // bb = init_btn_normal_xml(file, parent);
    init_btn_normal_xml(file, parent);
    //Creer le bouton
    //creer_button(bb);
    //styler_btn(bb);
    //Si la balise est un <btn-normale-style>
        /*
        else if (test == 25) {
            if (!(strcmp("color\"", mot))) {
                char color[MAX];
                lire_gchar_str_with_deplacement(file, color, 13);
                // Affecter la couleur lue
                strcpy(buttonStyle->color.hexa_color, color);
            }
            else if (!(strcmp("police\"", mot))) {
                char police[MAX];
                lire_gchar_str_with_deplacement(file, police, 13);
                strcpy(buttonStyle->police, police);
            }
            else if (!(strcmp("taille\"", mot))) {
                lire_gchar_str_with_deplacement(file, taille, 13);
                // Convertir en entier
                buttonStyle->taille = atoi(taille);
            }
            else if (!(strcmp("gras\"", mot))) {
                char gras;
                Epeurerblanc(file);
                fseek(file, 1, SEEK_CUR);
                while (((car = fgetc(file)) != EOF) && (car != '<')) {
                    gras = car; // Lire le mot
                }
                printf("%c\n", gras);
                fseek(file, 13, SEEK_CUR);
                // Convertir en entier
                buttonStyle->gras = char_TO_int(gras);
            }
            else if (!(strcmp("bgcolor\"", mot))) {
                type_bg = 0;
                char bgcolor[MAX];
                lire_gchar_str_with_deplacement(file, bgcolor, 13);
                strcpy(maButton->back_ground.hexa_color, bgcolor);
            }
            else if (!(strcmp("modern_bgcolor\"", mot))) {
                type_bg = 1;
                Epeurerblanc(file);
                fseek(file, 1, SEEK_CUR);
                while (((car = fgetc(file)) != EOF) && (car != '<') && (car != ',')) {
                    color1[ind++] = car; // Lire le mot
                }
                color1[ind] = '\0';
                ind = 0;
                while (((car = fgetc(file)) != EOF) && (car != '<')) {
                    color2[ind++] = car; // Lire le mot
                }
                color2[ind] = '\0';
                fseek(file, 13, SEEK_CUR);
            }
        }
        */
    //}
    // Si test n'égal pas property (22), ignorer la balise fermante de button

    // Appliquer le style au bouton
    // Création du bouton
    /*
    maButton = creer_button_normale(maButton);
    gtk_widget_set_name(maButton->Button, "mon-bouton");
    if (type_bg == 0) {
        if (!load_css("style.css", maButton->Button,
                      maButton->back_ground.hexa_color))
        {
            g_print("error de mis en forme de l'arriere plan");
        }
    }
    else if (type_bg == 1) {
        if (!load_css_modern("style.css", maButton->Button,
                             color1, color2))
        {
            g_print("error de mis en forme de l'arriere plan");
        }
    }
    appliquer_style_button(buttonStyle, maButton->Button);
    */

    // Appel récursif pour traiter les autres balises
    creer_object(file, parent);
}



#endif //XML_TAHA_BOUTON_XML_H
