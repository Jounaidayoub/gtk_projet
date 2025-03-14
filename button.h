//
// Created by ACER on 06/01/2025.
//

#ifndef TEST1_BUTTON_H
#define TEST1_BUTTON_H
#include "global.h"
#include "includes_button.h"

#include <stdbool.h>
#define MAX_LENGTH 100
#define MARKUP_LENGTH 50
#include "coordonnees.h"
//Les positions prédéfinis de l'icone d'un button
/*
typedef enum {
    LEFT,
    RIGHT,
    TOP,
    BOTTOM
} BtnImagePosition;
*/



//Définition d'une énumération 'BtnType' comportant les types de boutons possibles.
typedef enum
{
    NORMAL,//Bouton normal avec un label.
    RADIO,//Bouton de type radio (un choix à la foix).
    CHECKBOX,//Bouton à cocher (choix multiples).
    TOGGLE,//Bouton à bascule.
    SPIN,//Sélecteur numérique (augmenter/diminuer une valeur avec les fleches haut/bas
    // ou directement en saisissant la valeur souhaité.
    SWITCH,//Bouton à bascule plus moderne.
    NONE //Bouton simple sans label.
}BtnType;


/******************************Style************************************/
typedef struct {
    gchar* police;
    HexColor* color;
    gint taille;
    gint gras;
    char* bgcolor;
    int border;
    int border_radius;
} Style;


//La structure définissant le bouton, comportant les differents parametres
// permettant de le personnaliser.
//!Pour définir la background color on va utiliser du CSS car la fonction associé est abondonné
typedef struct btn{
    GtkWidget* button;//Le bouton à personnaliser.
    BtnType btype;//Le type du bouton.
    gchar nom[MAX_LENGTH];//L'identifiant du bouton.
    gchar label[MAX_LENGTH];//Le texte affiché sur le bouton.
    gboolean hasMnemonic; //Le bouton a t il un mnemonic (! Pour que la mnemonic fonctionne
    // il faut qu'il soit précedé par '_' dans le label).
    gchar tooltip[MAX_LENGTH];//L'info-bulle du bouton( format: markup).
    MonImage* img;//Une image de type MonImage.
    dimension* dim;//La dimension du bouton(hauteur, largeur).
    gboolean isFixed;//Boolean indiquant si le button est fixé (çad son conteneur doit être un widget: GtkFixed).
    coordonnees* pos;//La position du bouton (! marche seulement si le conteneur est un GtkFixed ).
    Margin* margin;//La marge d'un bouton.
    gboolean isSensitive;//Boolean indiquant si le button est cliquable.
    GtkWidget* container;//Le conteneur du bouton.
    GtkWidget* groupeMember;//Un bouton radio qui appartient au même groupe(util juste pour les boutons radio).
    GtkAlign align;//L'alignement du bouton.
    double opacity;//l'opacité du bouton.
    gboolean isChecked;//Si le bouton est coché.
    spinObj* sp;//L'objet contenant les parametres du button spin.
    //style
    Style* style; // style du bouton
    gchar* police; // police du bouton
    gchar* color; // couleur du bouton
    gint taille; // taille de la police du bouton
    gint gras; // épaisseur de la police du bouton
    gchar* bgcolor;
}btn;
/**
 * @brief Copie les éléments d'une structure Style dans une autre.
 *
 * @param dest Pointeur vers la structure Style de destination.
 * @param src Pointeur vers la structure Style source.
 * @return Pointeur vers la structure Style de destination.
 */
/**
 * @brief Copie les éléments d'une structure Style dans une autre.
 *
 * @param src Pointeur vers la structure Style source.
 * @return Pointeur vers la nouvelle structure Style de destination.
 */
// Style* copy_style(const Style* src) {
//     if (!src) return NULL;

//     Style* dest = (Style*)malloc(sizeof(Style));
//     if (!dest) {
//         fprintf(stderr, "Erreur d'allocation mémoire pour le style de destination\n");
//         return NULL;
//     }

//     dest->police = src->police ? g_strdup(src->police) : "Sans";
//     // dest->color = src->color ? g_memdup(src->color, sizeof(HexColor)) : NULL;
//     dest->taille = src->taille;
//     dest->gras = src->gras;
//     dest->bgcolor = src->bgcolor ? g_strdup(src->bgcolor) : "white";
//     dest->border = src->border;
//     dest->border_radius = src->border_radius;

//     return dest;
// }

void copy_style_to_btn(btn* b, const Style* src) {
    if (!b || !src) return;

    b->police = src->police ? g_strdup(src->police) : "Sans";
    b->color = src->color ? g_strdup(src->color->hex_code) : "black";
    b->taille = src->taille;
    b->gras = src->gras;
    b->style = (Style*)malloc(sizeof(Style));
    if (!b->style) {
        fprintf(stderr, "Erreur d'allocation mémoire pour le style du bouton\n");
        return;
    }
    b->style->police = b->police;
    b->style->color = src->color;
    b->style->bgcolor = src->bgcolor;
    b->style->taille = b->taille;
    b->style->gras = b->gras;
    b->style->bgcolor = src->bgcolor ? g_strdup(src->bgcolor) : "white";
    b->style->border = src->border;
    b->style->border_radius = src->border_radius;
}

/**************************************
 * NOM: allocateBtn.
 * ENTRÉS: void.
 * SORTIS: un pointeur vers un 'objet' de type 'btn'.
 * DESCRIPTION: Allouer l'espace mémoire pour un bouton de type 'btn' et le retourner.
 *              En cas d'échec d'allocation on sort du programme avec un message d'erreur.
***************************************/
btn* allocateBtn()
{
    btn* mybtn=(btn*)malloc(sizeof(btn));//Allocation
    // mybtn->style = (Style*)malloc(sizeof(Style));
    //Retourner un message d'erreur et sortire du programme si l'allocation est échouée
    if(!mybtn)
    {
        // if(!mybtn->style){
            perror("Erreur d'allocation de bouton dans allocateBtn\n");
            exit -1;
        // }
    }
    //Retourner le bouton
    return (btn*) mybtn;
}

/**************************************
 * NOM: initBtn.
 * ENTRÉS: ...
 * SORTIS: un pointeur vers un 'objet' de type 'btn' après son initialisation.
 * DESCRIPTION: Allouer l'espace mémoire pour un bouton de type 'btn' .
 *              Initialiser les champs du bouton par les valeurs passés en paramétre.
 *              Donner des valeurs par défauts à certains champs (dimension,position,marge).
 *              Retourner le bouton.
 *              !L'image passée doit etre initié par init_image
***************************************/
//Initialiser le bouton
btn* initBtn(BtnType btype,gchar nom[MAX_LENGTH],gchar label[MAX_LENGTH],
             gboolean hasMnemonic,const gchar tooltip[MARKUP_LENGTH],MonImage* img,
             dimension* dim,gboolean isFixed,coordonnees* pos,Margin* margin,gboolean isSensitive,
             GtkWidget* container,GtkWidget* groupeMember,GtkAlign align, double opacity,
             gboolean isChecked, spinObj* sp)
{
    //Allocation du bouton
    btn* mybtn=allocateBtn();

    //Initialisation
    //Initialiser le type
    mybtn->btype = btype;
    // Copier le nom
    if (nom)
        strncpy(mybtn->nom, nom, MAX_LENGTH);
    //Copier le label
    if (label)
        strncpy(mybtn->label, label, MAX_LENGTH);
    //Copier l'info-bulle
    if (tooltip)
        strncpy(mybtn->tooltip, tooltip, MARKUP_LENGTH);
    //Si le bouton a un mnemonic
    mybtn->hasMnemonic=hasMnemonic;
    // Copier la dimension
    if(dim)
        mybtn->dim = dim;
        //En cas d'abscence de la dimension prendre une par défaut .
    else
        mybtn->dim=DEFAULT_BTN_DIM;
    // Copier la position
    if(pos)
        mybtn->pos = pos;
        //En cas d'abscence de la position prendre une par défaut .
    else
        mybtn->pos=DEFAULT_BTN_POS;
    // copier la marge
    if(margin)
        mybtn->margin = margin;
        //En cas d'abscence de la marge prendre une par défaut .
    else
        mybtn->margin=DEFAULT_BTN_MARGIN;
    // Le bouton est fixé ou pas.
    mybtn->isFixed = isFixed;
    mybtn->isSensitive = isSensitive; // Définir sensitivité
    mybtn->container = container; // Assigner containeur
    mybtn->groupeMember = groupeMember; // Assigner le membre du groupe (pour les buttons radio)
    mybtn->align = align; // Définir l'alignment
    mybtn->opacity=opacity;// Définir l'opacité
    mybtn->isChecked=isChecked;// Définir si le bouton (checkbox) est coché ou pas.
    mybtn->sp=sp;//Copier l'objet contenant les parametres d'un bouton de type SPIN.
    //assigner à l'image de bouton
    mybtn->img = img;
    if(img){
        creer_image(mybtn->img);
    }

    //Retourner le bouton
    return (btn*) mybtn;
}

//Creer le button
//L'utilité de retourner le btn c'est que lorsqu'on crée des boutons radios on doit
//spécifier leurs peres donc on doit récuperer le pere après sa création

/**************************************
 * NOM: creer_button.
 * ENTRÉS: btn* mybtn; pointeur vers un bouton (type btn)
 * SORTIS: un pointeur vers l'objet passé en parametre
 *          (c'est util lorsqu'on veut creer des boutons radio, puiqu'on doit creer le pere puis
 *          faire passer ce pere comme parametre aux autres boutons appartenant au même groupe).
 * DESCRIPTION: Allouer l'espace mémoire pour un bouton de type 'btn' .
 *              Creer le bouton selon son type.
 *              Ajuster les propriétés du bouton(sa dimension, sa position, ajouter l'image...)
 *              Retourner le pointeur vers le bouton (type btn)
***************************************/
btn* creer_button(btn* mybtn)
{
    //Structure vide
    if(!mybtn){
        printf("La structure de bouton n'existe pas.\n");
        exit(-1);
    }
    //Creer le bouton selon le type
    switch (mybtn->btype) {
        case NORMAL:
            mybtn->button = gtk_button_new_with_label(mybtn->label);
            break;
        case RADIO:
            mybtn->button = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(mybtn->groupeMember), mybtn->label);
            break;
        case CHECKBOX:
            mybtn->button = gtk_check_button_new_with_label(mybtn->label);
            break;
        case TOGGLE:
            mybtn->button = gtk_toggle_button_new_with_label(mybtn->label);
            break;
        case SPIN:
            // Créer le GtkSpinButton avec l'ajustement(adjustement a des valeurs par défaut pour le page_increment, page_size et le climb_rate)
            mybtn->button = gtk_spin_button_new(gtk_adjustment_new(mybtn->sp->start,
                                                                   mybtn->sp->borneInf,
                                                                   mybtn->sp->borneSup,
                                                                   mybtn->sp->step,
                                                                   10.0, 0.0),
                                                1.0, mybtn->sp->digits); // Incrément de 1, pas de décimales

            break;
        case SWITCH:
            mybtn->button = gtk_switch_new();
            gtk_switch_set_active(GTK_SWITCH(mybtn->button), mybtn->isChecked);//L'état du switch
            break;
        case NONE:
            mybtn->button = gtk_button_new(); // Default to a normal button
            break;
    }

    //Changer l'identifiant du bouton
    gtk_widget_set_name(mybtn->button, mybtn->nom);
    //Ajouter l'option d'avoir un mnemonic (pour la mnemonic fonctionne il faut que le label soit précédé par '_')
    if(mybtn->hasMnemonic)
        gtk_button_set_use_underline(GTK_BUTTON(mybtn->button), TRUE);
    //Si le tooltip n'est pas vide, alors l'afficher
    if(mybtn->tooltip[0] !='\0')
    {
        //Associer une tooltip au bouton
        gtk_widget_set_has_tooltip(mybtn->button, TRUE);
        gtk_widget_set_tooltip_markup(mybtn->button, mybtn->tooltip);
    }
        //Si le tooltip est vide alors ne pas l'afficher
    else{
        gtk_widget_set_has_tooltip(mybtn->button, FALSE);
    }
    //}

    //Ajuster l'opacité du bouton
    gtk_widget_set_opacity(mybtn->button, mybtn->opacity);
    //Ajuster la dimension du button
    gtk_widget_set_size_request(mybtn->button, mybtn->dim->width, mybtn->dim->height);
    //Fixer le bouton dans ce conteneur à une position bien définie(le conteneur doit etre fixed)
    if (mybtn->isFixed && mybtn->container) {
        gtk_fixed_put(GTK_FIXED(mybtn->container), mybtn->button, mybtn->pos->x, mybtn->pos->y);
    }
    //Ajuster les margins du bouton

    gtk_widget_set_margin_start(mybtn->button, mybtn->margin->mleft);
    gtk_widget_set_margin_end(mybtn->button, mybtn->margin->mright);
    gtk_widget_set_margin_top(mybtn->button, mybtn->margin->mtop);
    gtk_widget_set_margin_bottom(mybtn->button, mybtn->margin->mbottom);
    printf("\nMargins: %d %d %d %d", mybtn->margin->mleft,mybtn->margin->mright,mybtn->margin->mtop,mybtn->margin->mbottom);
    //Activer ou désactiver la sensibilitée
    gtk_widget_set_sensitive(mybtn->button, mybtn->isSensitive);
    // Donner le focus au bouton
    gtk_widget_grab_focus(mybtn->button);
    //Ajouter le bouton au conteneur, s'il ne s'agit d'un conteneur fixe
    if((!mybtn->isFixed) && ((mybtn->container)!=NULL))
    {
        gtk_container_add(GTK_CONTAINER(mybtn->container), mybtn->button);
        //gtk_box_pack_start(GTK_BOX(mybtn->container), mybtn->button, TRUE, TRUE, 0);

    }
    //Aligner le bouton
    gtk_widget_set_halign(mybtn->button, mybtn->align);
    //Cocher le bouton
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(mybtn->button), mybtn->isChecked); // Select the radio button


    //Ajouter l'image
    //L'image (pour l'instant) est aligné par défaut à droite (GTK_POS_RIGHT)
    if(mybtn->img) {
        gtk_button_set_image(GTK_BUTTON(mybtn->button), GTK_WIDGET(mybtn->img->Image));
        gtk_button_set_always_show_image(GTK_BUTTON(mybtn->button), TRUE);
        //...............Test phase..................///
        //Ajuster la position de l'image
        gtk_button_set_image_position(GTK_BUTTON(mybtn->button), GTK_POS_RIGHT);
        //Ajouter un peu
        /*
        GtkWidget *fixed = gtk_fixed_new();
        // Position the image at (x, y) inside the GtkFixed
        gtk_fixed_put(GTK_FIXED(fixed), mybtn->img->Image, mybtn->img->Img_cord.x, mybtn->img->Img_cord.y); // Adjust x=20, y=10 as needed
        // Add GtkFixed to the button
        gtk_container_add(GTK_CONTAINER(mybtn->button), fixed);
         */
    }

    /*
    //Ajuster la couleur de l'arriere plan
    GdkRGBA color;
    gdk_rgba_parse(&color, "blue"); // You can use color names or hex codes
    gtk_widget_override_background_color(mybtn->button, GTK_STATE_FLAG_NORMAL, &color);
*/
    return(btn*) mybtn;
}



/**********************************************************************************************************
 Nom            : init_style()
 Entrée         : police - Police à assigner au style
                  color - Couleur à assigner au style
                  taille - Taille de police à assigner au style
                  gras - Indicateur de style gras à assigner au style
 Sortie         : Pointeur vers la structure Style mise à jour
 Description    : Initialise une structure Style avec les valeurs spécifiées et retourne un pointeur
                  vers la structure mise à jour.
                  Par défaut les chaines de caracteres sont NULL, sauf si les chaines correspondants sont non vides
                  Si un parametre de type entier n'est pas désiré, remplisser le avec '-1'
 **********************************************************************************************************/
Style *init_style(gchar police[50], HexColor* color, gint taille,
                  gint gras, char* bgcolor, int border, int border_radius) {
    // Allouer de la mémoire pour le style s'il n'existe pas
        Style* stl = (Style*)malloc(sizeof(Style));
        if (!stl) {
            fprintf(stderr, "Erreur d'allocation mémoire\n");
            return NULL;
        }

    //Initier les attributs de style
        stl->police = stl->bgcolor = NULL;
    //Si la police est non vide alors copier la
    if(police)
        if(police[0] != '\0' )
            stl->police = police;
    //Si la bgcolor est non vide alors copier la
    if(bgcolor)
        if(bgcolor[0] != '\0')
            stl->bgcolor = bgcolor;
    //Copier la couleur
    stl->color = color;
    //Copier la taille
    stl->taille = taille;
    //Copier le gras
    stl->gras = gras;
    //Copier le border
    stl->border = border;
    //Copier le border_radius
    stl->border_radius = border_radius;
    return (Style *)stl;
}

//Prend un bouton et un style et applique les propriétés qui sont "deprecated" en
//utilisant le css (border, border-radius, background-color)
void regler_css(btn *b, Style* stl) {
    // Create a new CSS provider
    GtkCssProvider *provider = gtk_css_provider_new();

    // Construct the dynamic CSS string
    //BAckground
    printf("#hello { background: %s;}", stl->bgcolor);
    char* bgcolor = stl->bgcolor ? stl->bgcolor : "";
    gchar *css ;
    //Si le 'bgcolor' est une image(contient une extension) alors appliquer l'image comme background
    if (strchr(bgcolor, '.')) {
        css = g_strdup_printf("#%s { background: url('%s') no-repeat center center ; \
                                        background-size: cover;\
                                        border: %dpx solid black; \
                                        border-radius: %dpx;}",
                              b->nom, bgcolor, stl->border, stl->border_radius);
    }
        //Si le 'bgcolor' est une background color
    else{
        css = g_strdup_printf("#%s { background: %s ; \
                                        border: %dpx solid black; \
                                        border-radius: %dpx;}",
                              b->nom, bgcolor, stl->border, stl->border_radius);
    }

    // Load the CSS into the provider
    gtk_css_provider_load_from_data(provider, css, -1, NULL);

    // Get the style context of the button
    GtkStyleContext *context = gtk_widget_get_style_context(b->button);
    gtk_style_context_add_provider(context,
                                   GTK_STYLE_PROVIDER(provider),
                                   GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Free allocated memory
    g_free(css);
    g_object_unref(provider);
}


/**********************************************************************************************************
 Nom            : appliquer_style_button()
 Entrée         : monStyle - Pointeur vers la structure Style contenant les spécifications du style
                  Mb - Widget (bouton) auquel appliquer le style
 Sortie         : Aucune
 Description    : Applique un style spécifié à un bouton en fonction des paramètres spécifiés dans la
                  structure Style. Le style peut inclure une police, une taille de police, du texte en gras,
                  une couleur de texte, etc(si ses attributs sont non vides(!= -1 pour les entiers et != NULL pour les strings ).
**********************************************************************************************************/
void appliquer_style_button(Style *monStyle, btn *b) {
    //font
    {
        // Création d'une nouvelle description de police Pango
        PangoFontDescription *font_desc = pango_font_description_new();

        // Appliquer la famille de police spécifiée dans le style
        if(monStyle -> police){
            pango_font_description_set_family(font_desc, monStyle->police);
            // Appliquer la police au widget du bouton
            gtk_widget_override_font(b->button, font_desc);
        }
        // Appliquer la taille de police spécifiée dans le style
        if(monStyle -> taille > -1){
            pango_font_description_set_size(font_desc, monStyle->taille * PANGO_SCALE);
            // Appliquer la police au widget du bouton
            gtk_widget_override_font(b->button, font_desc);
        }
        //Mettre le text en gras
        if (monStyle->gras == 1){
            pango_font_description_set_weight(font_desc, PANGO_WEIGHT_BOLD);
            pango_font_description_set_size(font_desc, monStyle->taille * PANGO_SCALE);
            // Appliquer le gras au widget du bouton
            gtk_widget_override_font(b->button, font_desc);
        }

        // Libérer la mémoire allouée pour la description de police
        pango_font_description_free(font_desc);
    }
    // Récupérer le label du bouton
    GtkWidget *lab = gtk_bin_get_child(GTK_BIN(b->button));

    // Modifier la couleur du texte
    if(monStyle->color)
        gtk_widget_override_color(lab, GTK_STATE_FLAG_NORMAL, monStyle->color->color);

    //Modifier le background et le border radius(tout ce qui est css)
    regler_css(b, monStyle);
}


/////////box des boutons
#include "fixed.h"
#include "box.h"

/**
 * @brief Crée une liste de boutons radio à partir d'une liste de labels.
 *
 * Cette fonction génère une série de boutons radio en utilisant les labels fournis.
 * Le premier label est utilisé pour créer un bouton "père", et les labels suivants
 * sont utilisés pour créer des boutons "enfants" qui sont associés au bouton père.
 * Chaque bouton est stylisé selon le style fourni et ajouté à un conteneur StyledBox.
 *
 * @param labels Tableau de chaînes de caractères contenant les labels des boutons radio.
 *               Le premier élément est utilisé pour le bouton père, les autres pour les enfants.
 *               Le tableau doit se terminer par un pointeur NULL.
 * @param st Pointeur vers une structure Style contenant les styles à appliquer aux boutons.
 *           Si NULL, aucun style n'est appliqué.
 * @param bx Pointeur vers une structure StyledBox dans laquelle les boutons seront ajoutés.
 *
 * @note La fonction suppose que le widget du StyledBox (bx->widget) est déjà créé et valide.
 * @note Les marges des boutons sont fixées à 0 pixels sur tous les côtés.
 *
 * @example
 * char* labels[] = {"Option 1", "Option 2", "Option 3", NULL};
 * Style *style = { ... }; // Définir le style
 * StyledBox *box = { ... }; // Définir le conteneur
 * liste_radios(labels, style, box);
 */

btn** liste_radios(char* labels[256], Style* st, StyledBox* bx) {


    int count = 0;
    while (labels[count]) count++; // Count the number of labels

    // Allocate memory for the array of buttons (NULL-terminated)
    btn** buttons = malloc((count + 1) * sizeof(btn*));
    if (!buttons) return NULL; // Check allocation failure


    //Creer le pere
    btn* pere = btnRadio(labels[0], labels[0], labels[0], bx->widget, margin(0, 0, 0, 0),
                        NULL, NULL);
    creer_button(pere);//Creer le pere

    //Ajouter le pere au box
    //gtk_box_pack_start(GTK_BOX(box), pere->button, TRUE, TRUE, 0);

    //appliquer le style au bouton pere
    if(st){
        // pere->style = *st;
        copy_style_to_btn(pere, st);
        appliquer_style_button(st, pere);
    }
    buttons[0] = pere; // Store the parent button
    //Creer les autres boutons
        // Create the child radio buttons
        for (int j = 1; j < count; j++) {
            btn* b = btnRadio(labels[j], labels[j], labels[j], bx->widget, margin(0, 0, 0, 0), pere->button, NULL);
            creer_button(b);
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(b->button), FALSE);
            if (st){ 
                copy_style_to_btn(b, st);
                appliquer_style_button(st, b);
            }
            buttons[j] = b; // Store the child button
        }
    /*
    int j = 0;//Commencer par le 2eme label
    char **i = labels;
    while(i[++j]){
        //Creer le bouton
        btn* b = btnRadio(i[j], i[j], i[j], bx->widget, margin(0, 0, 0, 0),
                             pere->button, NULL);
        creer_button(b);//Creer le pere
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(b->button), FALSE); // Select the radio button

        //appliquer le style au bouton
        if(st)
            appliquer_style_button(st, b);
        //Ajouter le bouton au box
        //gtk_box_pack_start(GTK_BOX(box), b->button, TRUE, TRUE, 0);

    }
    */
   buttons[count] = NULL; // NULL-terminate the array
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pere->button), TRUE); // Select the radio button
        return buttons;
}


/**
 * @brief Crée une liste de boutons à cocher (checkboxes) à partir d'une liste de labels.
 *
 * Cette fonction génère une série de boutons à cocher en utilisant les labels fournis.
 * Chaque bouton est créé avec les propriétés spécifiées, stylisé selon le style fourni,
 * et ajouté à un conteneur StyledBox.
 *
 * @param labels Tableau de chaînes de caractères contenant les labels des boutons à cocher.
 *               Le tableau doit se terminer par un pointeur NULL.
 * @param st Pointeur vers une structure Style contenant les styles à appliquer aux boutons.
 *           Si NULL, aucun style n'est appliqué.
 * @param bx Pointeur vers une structure StyledBox dans laquelle les boutons seront ajoutés.
 *
 * @note La fonction suppose que le widget du StyledBox (bx->widget) est déjà créé et valide.
 * @note Les marges des boutons sont fixées à 0 pixel sur tous les côtés.
 *
 * @example
 * char* labels[] = {"Option 1", "Option 2", "Option 3", NULL};
 * Style* style = { ... }; // Définir le style
 * StyledBox* box = { ... }; // Définir le conteneur
 * liste_checks(labels, style, box);
 */
void liste_checks(char* labels[256], Style* st, StyledBox* bx) {

    //Creer les autres boutons
    int j = -1;//Commencer par le 1er label
    while(labels[++j]){
        //Creer le bouton
        btn* b = btnCheck(labels[j], labels[j], labels[j], margin(0, 0, 0, 0),
                          bx->widget, FALSE, NULL);
        creer_button(b);//Creer le pere
        //appliquer le style au bouton
        if(st)
            appliquer_style_button(st, b);
        //Ajouter le bouton au box
        //gtk_box_pack_start(GTK_BOX(box), b->button, TRUE, TRUE, 0);

    }
}




#endif //TEST1_BUTTON_H
