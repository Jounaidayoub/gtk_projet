//
// Created by Dell on 1/30/2025.
//

#ifndef XML_TAHA_XML_UTILITY_H
#define XML_TAHA_XML_UTILITY_H

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#define MAX 40
#define BL ' '
#define RC '\n'

//Déclaration des fonctions de creation des widgets
void window_xml(FILE *file);
void button_xml(FILE *file, int parent);
void checkbox_xml(FILE *file, int parent) ;
void BoiteDialog_xml(FILE *file,int parent);
void radioList_xml(FILE *file, int  parent) ;
void label_xml(FILE *file, int parent);
void entry_xml(FILE *file,int parent);
void TextView_xml(FILE *file, int parent);
void box_xml(FILE *file,int parent);
void ProgressBar_xml(FILE *file, int parent);

//tableau global qui contient les widgets parents
//0: window, 2:fixed, 4:menu_parent
GtkWidget *parents[5]={NULL,NULL,NULL,NULL,NULL};
//indice si global de bakground image de fenetre
int hasOverlay=0;
GtkWidget *overlay=NULL;
//liste radio
//Liste *group_radio_liste=NULL;
//checkbox radio
GList *buttonList = NULL;
/////////////*************************************************************
//epeurer blanc de fichier
void Epeurerblanc(FILE *file)
{
    char car;
    while((car=fgetc(file))==BL ||  car== RC);
    ungetc(car,file);
}
//---> fonction qui transforme un caractere en entier
int char_TO_int(char car)
{
    return ((int)(car-'0'));
}//fin de la fonction char_TO_int
///-------------> lire_gchar
void lire_gchar_str(FILE* file,char str[MAX])
{
    char car;
    int ind=0;
    Epeurerblanc(file);
    fseek(file, 1, SEEK_CUR);
    while (((car = fgetc(file)) != EOF) && (car != '<')) {
        str[ind++] = car; // Lire le mot
    }
    str[ind] = '\0';
    printf("mot lue de lire_gchar_str%s\n", str);
    fseek(file, 10, SEEK_CUR);
}
///pour lire la valeur d'lement property
char lire_gchar(FILE* file)
{
    Epeurerblanc(file);
    char result,car;
    fseek(file, 1, SEEK_CUR);
    while (((car = fgetc(file)) != EOF) && (car != '<')) {
        result = car; // Lire le mot
    }
    fseek(file, 10, SEEK_CUR);
    return ((char)result);
}
//pour lire un nombre de deplacement bien definis
/***********************************************s***********************************************************
 Nom            : lire_gchar_str_with_deplacement()
 Entrée         : file - Pointeur vers le fichier
                  str - Chaîne de caractères à remplir
                  deplacement - Déplacement dans le fichier après la lecture de la chaîne
 Sortie         : Aucune
 Description    : Lit une chaîne de caractères à partir du fichier et effectue un déplacement
                  spécifié après la lecture de la chaîne.
**********************************************************************************************************/
void lire_gchar_str_with_deplacement(FILE* file, char str[MAX], int deplacement)
{
    char car;
    int ind=0;
    // Ignorer les espaces blancs et avancer d'un caractère, pour aller vers le contenu du propriété(la valeur <property name="bla"> valeur</property>
    Epeurerblanc(file);
    fseek(file, 1, SEEK_CUR);
    //Enlever les blanc au début de la valeur
    Epeurerblanc(file);
    // Lire la chaîne de caractères jusqu'à atteindre un chevron ou la fin du fichier
    while (((car = fgetc(file)) != EOF) && (car != '<')) {
        str[ind++] = car; // Lire le caractère
    }
    str[ind] = '\0'; // Terminer la chaîne
    printf("%s\n", str); // Afficher la chaîne lue
    fseek(file, deplacement, SEEK_CUR);
}
//pour lire la valeur d'ume element item de comboBox
void lire_gchar_str_item(FILE* file,char str[MAX])
{
    char car;
    int ind=0;
    Epeurerblanc(file);
    fseek(file, 1, SEEK_CUR);
    while (((car = fgetc(file)) != EOF) && (car != '<')) {
        str[ind++] = car; // Lire le mot
    }
    str[ind] = '\0';
    printf("%s\n", str);
    fseek(file, 6, SEEK_CUR);
}


///retourne un indice sur la balise qu'on est sur laquelle
///baliseeeeeeee
int balise(FILE *file)
{
    char motlue[MAX],car;
    int ind;
    ind = 0;
    //Epurer le blanc
    Epeurerblanc(file);
    //Lire le type de la balise
    while ((car = fgetc(file)) != EOF && (car != BL)  && (car != '>'))
    {
        motlue[ind++] = car; // Lire le mot
    }
    motlue[ind] ='\0';
    //Retourner la valeur correspondante à la balise
    if(!(strcmp("<window",motlue)))  return 0;
    else if(!(strcmp("<fixed",motlue)))  return 1;
    else if(!(strcmp("<button",motlue)))  return 2;
    else if (!(strcmp("<checkbox",motlue)))  return 3;
    else if (!(strcmp("<radio",motlue)))  return 4;
    else if (!(strcmp("<label",motlue)))  return 5;
    else if (!(strcmp("<TextView",motlue)))  return 6;
    else if(!(strcmp("<Dialog",motlue)))  return 12;
    else if(!(strcmp("<menubarre",motlue)))  return 13;
    else if(!(strcmp("<menu",motlue)))  return 14;
    else if(!(strcmp("<submenu",motlue))) return 15;
    else if(!(strcmp("<menuitem",motlue)))  return 16;
    else if(!(strcmp("<box",motlue)))  return 17;
    else if(!(strcmp("<item",motlue)))  return 18;
    else if(!(strcmp("<items",motlue)))  return 19;
    else if(!(strcmp("<comboBox",motlue)))  return 20;
    else if(!(strcmp("<entry",motlue))) return 21;
    else if(!(strcmp("<property",motlue))) return 22;
    else if(!(strcmp("<child",motlue))) return 23;
    else if(!(strcmp("<textstyle",motlue))) return 24;
    else if(!(strcmp("<buttonstyle",motlue))) return 25;
    else if(!(strcmp("<children",motlue))) return 26;
    else if(!(strcmp("<labelstyle",motlue))) return 27;
    else if(!(strcmp("<checkboxstyle",motlue))) return 28;
    else if(!(strcmp("<radiostyle",motlue))) return 29;
    else if(!(strcmp("<calendrier",motlue))) return 30;
    else if(!(strcmp("<radioList",motlue))) return 31;
    else if(!(strcmp("<filechooser",motlue))) return 32;
    else if(!(strcmp("<image",motlue))) return 33;
    else if(!(strcmp("<spin",motlue))) return 34;
    else if (!(strcmp("<signal",motlue))) return 35;
    else if (!(strcmp("<checkboxList",motlue))) return 36;
    else if (!(strcmp("</radioList",motlue))){return -31;}
    else if (!(strcmp("</checkboxList",motlue))){return -36;}
    else if(!(strcmp("<ProgressBar",motlue))) return 37;

    else if (!(strcmp("</checkbox",motlue))){return -3;}
    else if (!(strcmp("</radio",motlue))){return -4;}
    //La balise <child></child> est toujours la fille d'une balise de positionnement,
    //(box, fixed...) donc apres le </child> ça vient la fermeture de la balise de
    //positionnement, dont on a pas besoin de traiter sa fermeture donc on va la sauter
    else if(!(strcmp("</child",motlue)))
    {
        //Sauter la balise fermante de positionement
        fseek(file,9,SEEK_CUR);
        balise(file);
    }
    else if(!(strcmp("</menu",motlue))) {
        //fseek(file,8,SEEK_CUR);
        //balise(file);
        return -14;
    }
    else if(!(strcmp("</menuitem",motlue))) {
        //fseek(file,8,SEEK_CUR);
        balise(file);
    }
    else if(!(strcmp("</submenu",motlue))) {
        // balise(file);
        return -15;
    }
        return -1;
}//fin de la fonction balise


//void creer_object(FILE *file,int parent);


//fct de creation d'un objet en fonction de la balise
//Entrés: FILE *file: fichier xml, int parent: l'indice du conteneur(voir tableau parents[])
//Description: appele la fonction responsable à la creation de l'objet de la balise
//             lue en fonction de code de retour de la fonction balise().
//              en cas d'une balise <fixed>, une balise <children> doit la succeder,
//              cette derniere doit contenir les objets qu'on veut creer
void creer_object(FILE *file,int parent)
{
    //L'identifiant du type de balise (label ou window ou...)
    int id = balise(file);
    //Creation de widget en fonction de la balise
    switch(id)
    {
        case 0: window_xml(file) ;break;
        //Si la balise est le fixed
        case 1:{
            int test;
            //Creation d'un conteneur fixed
            GtkWidget *fixed = gtk_fixed_new();
            //Si la fenetre a un background alors placer le fixed comme une surcouche
            //puisque le window ne peut pas avoir le background et le fixed comme fils à la fois
            if(hasOverlay==1)
                gtk_overlay_add_overlay(GTK_OVERLAY(overlay), fixed);
            //Si la fenetre n'a pas de background alors le fils du window sera le fixed
            else gtk_container_add(GTK_CONTAINER(parents[parent]),fixed);
            //Mise à jour du parent pour qu'il soit le fixed, ce qui va nous permettre d'ajouter
            //les élement dans le fixed
            parents[2]=fixed;
            //Si la balise qui vient apres le <fixed> est <children>
            if((test=balise(file))==26)
            {
                //Creer le widget qui vient après le <children> dont son parent
                // est le fixed (parents[2])
                creer_object(file,2);
            }
            break;
        }
        case 2: button_xml(file,parent);break;
        case 3:checkbox_xml(file, parent);break;
        case 12: BoiteDialog_xml(file,parent);break;
        case 5:label_xml(file,parent);break;
        case 21: entry_xml(file,parent);break;
        case 6:TextView_xml(file, parent);break;
        case 17: box_xml(file,parent);break;
        case 31:radioList_xml(file,parent);break;
        case 37: ProgressBar_xml(file, parent); break;


        /*
        case 13: menu_barre_xml(file,parent);break;
        case 20: comboBox_xml(file,parent);break;
        case 30:calender_xml(file,parent);break;
         */
        /*
        case 32:filechooser_xml(file,parent);break;
        case 33:image_xml(file,parent);break;
        case 34:spin_xml(file,parent);break;
        */
        default:printf("pas de children");break;
    }
}


#endif //XML_TAHA_XML_UTILITY_H
