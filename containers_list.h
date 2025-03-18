//
// Created by Dell on 3/1/2025.
//

#ifndef XML_TAHA_CONTAINERS_LIST_H
#define XML_TAHA_CONTAINERS_LIST_H

#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "coordonnees.h"
#include "dimension.h"
#include "box.h"
#include "widget_types.h"  // Add include for WidgetType enum

#define MAX_NAME_LENGTH 256  // Définir une taille maximale pour le nom
#define MAX_PROPS 20        // Maximum number of properties
#define MAX_PROP_NAME 64    // Maximum property name length
#define MAX_PROP_VALUE 256  // Maximum property value length

// Structure pour stocker une propriété avec un nom et une valeur
typedef struct {
    char name[MAX_PROP_NAME];
    char value[MAX_PROP_VALUE];
} WidgetProperty;

/**
 * @brief Structure représentant un arbre avec un nom, un widget, et des pointeurs vers les enfants et les frères.
 */
typedef struct arbre {
    char nom[MAX_NAME_LENGTH];   /**< Le nom de l'élément */
    GtkWidget *widget;           /**< Pointeur vers le widget associé */
    struct arbre *frere;         /**< Pointeur vers le frère de l'élément */
    struct arbre *fils;          /**< Pointeur vers le premier fils de l'élément */
    int is_container;            /**< Indicateur pour savoir si c'est un conteneur (1) ou non (0) */
    
    // Ajout pour les propriétés du widget
    WidgetProperty properties[MAX_PROPS]; /**< Tableau des propriétés du widget */
    int prop_count;                      /**< Nombre de propriétés stockées */
    
    // Style properties for special elements (label, button, etc)
    WidgetProperty style_props[MAX_PROPS]; /**< Tableau des propriétés de style */
    int style_prop_count;                /**< Nombre de propriétés de style */
    
    // Changed: Use enum for widget type instead of string
    WidgetType type;                      /**< Type de widget (enum) */
    
    // Added: Pointer to the widget-specific structure
    void *widget_data;                    /**< Pointeur vers la structure de données du widget */
} Arbre;

// Fonction pour ajouter une propriété à un noeud Arbre
void add_property_to_node(Arbre* node, const char* name, const char* value) {
    if (node->prop_count < MAX_PROPS) {
        strncpy(node->properties[node->prop_count].name, name, MAX_PROP_NAME-1);
        strncpy(node->properties[node->prop_count].value, value, MAX_PROP_VALUE-1);
        node->prop_count++;
    }
}

// Fonction pour ajouter une propriété de style à un noeud Arbre
void add_style_property_to_node(Arbre* node, const char* name, const char* value) {
    if (node->style_prop_count < MAX_PROPS) {
        strncpy(node->style_props[node->style_prop_count].name, name, MAX_PROP_NAME-1);
        strncpy(node->style_props[node->style_prop_count].value, value, MAX_PROP_VALUE-1);
        node->style_prop_count++;
    }
}

// Changed: Updated to set enum type instead of string
void set_widget_type(Arbre* node, WidgetType type) {
    node->type = type;
}

/**
 * @brief Fonction pour allouer de la mémoire pour un nouveau noeud de l'arbre.
 *
 * Cette fonction crée un nouveau noeud de type `Arbre` et initialise ses champs à des valeurs .
 *
 * @return Arbre* Le pointeur vers la structure nouvellement allouée.
 */
Arbre* allouer_arbre(char* nom, GtkWidget* widget, Arbre* frere, Arbre* fils, bool is_container) {
    // Allocation mémoire pour le noeud
    Arbre* nouveau_noeud = (Arbre*)malloc(sizeof(Arbre));

    // Vérification si l'allocation a échoué
    if (nouveau_noeud == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour l'arbre.\n");
        exit(EXIT_FAILURE);
    }

    // Initialisation des champs à des valeurs
    strcpy(nouveau_noeud->nom, nom);
    nouveau_noeud->widget = widget;
    nouveau_noeud->frere = frere;
    nouveau_noeud->fils = fils;
    nouveau_noeud->is_container = is_container;
    
    // Initialize property counts
    nouveau_noeud->prop_count = 0;
    nouveau_noeud->style_prop_count = 0;
    nouveau_noeud->type = WIDGET_UNKNOWN;  // Default to unknown type
    nouveau_noeud->widget_data = NULL;     // Default to no widget data

    return nouveau_noeud;
}

/**
 * @brief Insère un élément dans l'arbre en tant que fils du noeud dont le nom correspond à nomDuPere.
 *
 * La fonction parcourt récursivement l'arbre. Lorsqu'un noeud ayant pour nom nomDuPere
 * et marqué comme conteneur est trouvé, l'élément 'elem' est inséré en tant que fils.
 * Si le noeud possède déjà un premier fils, 'elem' est ajouté en fin de la liste des frères.
 *
 * @param racine La racine de l'arbre.
 * @param elem L'élément à insérer.
 * @param nomDuPere Le nom du noeud parent (const, non modifié).
 * @return Arbre* La racine de l'arbre mise à jour.
 */
Arbre* insererArbre(Arbre* racine, Arbre* elem, const char* nomDuPere){
    if(!racine){
        perror("errr");
        exit(-1);
    }
    Arbre* temp = racine;
    if((strcmp(nomDuPere, temp->nom) == 0) && temp->is_container){
        //Inserer comme frere du premier fils
        if(temp->fils){
            temp = temp->fils;
            //Parcourir les freres
            while(temp->frere){
                temp = temp->frere;
            }
            //Inserer dans freres
            temp->frere = elem;
        }
        else {
            //inserer comme premier fils
            temp->fils = elem;
        }
        return racine;
    }

    if(racine->fils)
        insererArbre(racine->fils, elem, nomDuPere);

    if(racine->frere)
        insererArbre(racine->frere, elem, nomDuPere);

    return racine;
}

/**
 * @brief Affiche l'arbre sous forme d'arborescence.
 *
 * La fonction affiche le nom de chaque noeud précédé d'une indentation correspondant à son niveau dans l'arbre.
 *
 * @param racine Le noeud courant de l'arbre.
 * @param niveau Le niveau d'indentation (0 pour la racine).
 */
void afficher_arbre(Arbre* racine, int niveau) {
    if (racine == NULL) {
        return;
    }

    // Affichage de l'indentation correspondant au niveau
    for (int i = 0; i < niveau; i++) {
        printf("    "); // 4 espaces par niveau
    }

    // Afficher le nom et, si c'est un conteneur, l'indiquer
    printf("%s%s\n", racine->nom, racine->is_container ? " [container]" : "");

    // Afficher récursivement le premier fils avec une indentation augmentée
    afficher_arbre(racine->fils, niveau + 1);

    // Afficher récursivement les frères au même niveau
    afficher_arbre(racine->frere, niveau);
}

#define MAX_CONTAINER_NAMES 100

/**
 * @brief Fonction récursive auxiliaire qui parcourt l'arbre et insère dans le tableau
 *        les noms des noeuds dont is_container est vrai.
 *
 * @param node Le noeud courant de l'arbre.
 * @param noms Le tableau préalloué pour stocker les noms (taille MAX_CONTAINER_NAMES).
 * @param indice Pointeur vers l'indice courant dans le tableau.
 */
void getNomsConteneursRec(Arbre *node, char *noms[MAX_CONTAINER_NAMES], int *indice) {
    if (node == NULL || *indice >= MAX_CONTAINER_NAMES - 1) {
        return;
    }

    if (node->is_container) {
        noms[*indice] = node->nom;  // On stocke directement le pointeur vers la chaîne
        (*indice)++;
    }

    // Parcours récursif sur le premier fils puis sur les frères
    getNomsConteneursRec(node->fils, noms, indice);
    getNomsConteneursRec(node->frere, noms, indice);
}

/**
 * @brief Remplit un tableau de chaînes de caractères avec les noms des conteneurs présents dans l'arbre.
 *
 * La fonction parcourt l'arbre et insère dans le tableau préalloué (de taille MAX_CONTAINER_NAMES)
 * les noms des noeuds dont is_container est vrai. Le tableau est terminé par un élément NULL.
 *
 * @param racine La racine de l'arbre à parcourir.
 * @param noms Le tableau de chaînes de caractères préalloué (taille MAX_CONTAINER_NAMES) qui sera rempli.
 */
void getNomsConteneurs(Arbre *racine, char *noms[MAX_CONTAINER_NAMES]) {
    int indice = 0;
    getNomsConteneursRec(racine, noms, &indice);
    noms[indice] = NULL;  // Terminer le tableau par NULL
}


/**
 * @brief Retourne le widget associé au noeud dont le nom correspond au nom donné.
 *
 * La fonction parcourt récursivement l'arbre à partir de la racine et compare le nom de chaque
 * noeud au nom recherché. Si une correspondance est trouvée, le widget associé est retourné.
 *
 * @param racine La racine de l'arbre à parcourir.
 * @param nom Le nom à rechercher dans l'arbre.
 * @return GtkWidget* Le widget associé au nom donné, ou NULL s'il n'est pas trouvé.
 */
GtkWidget* getWidgetByNom(Arbre *racine, const char *nom) {
    if (racine == NULL) {
        return NULL;
    }

    if (strcmp(racine->nom, nom) == 0) {
        return racine->widget;
    }

    GtkWidget *resultat = getWidgetByNom(racine->fils, nom);
    if (resultat != NULL) {
        return resultat;
    }

    return getWidgetByNom(racine->frere, nom);
}


/**
 * @brief Fonction auxiliaire qui ajoute les noeuds de l'arbre dans un GtkTreeStore.
 *
 * Cette fonction parcourt l'arbre représenté par le pointeur 'node' et ajoute chaque
 * noeud dans le GtkTreeStore 'store'. Les noeuds enfants (pointés par 'fils') sont
 * ajoutés en tant qu'enfants du noeud parent, tandis que les noeuds frères (pointés par 'frere')
 * sont ajoutés au même niveau que le noeud courant.
 *
 * @param node Le noeud courant de l'arbre (structure Arbre).
 * @param store Le GtkTreeStore dans lequel insérer les données.
 * @param parent Le GtkTreeIter du parent dans le store (NULL si le noeud est de niveau supérieur).
 */
static void ajouter_noeuds_dans_tree_store(Arbre *node, GtkTreeStore *store, GtkTreeIter *parent) {
    while (node != NULL) {
        GtkTreeIter iter;
        // Ajouter le noeud courant dans le store, sous le parent donné (ou NULL pour la racine)
        gtk_tree_store_append(store, &iter, parent);
        gtk_tree_store_set(store, &iter, 0, node->nom, -1);

        // Ajouter récursivement les enfants du noeud courant
        if (node->fils != NULL) {
            ajouter_noeuds_dans_tree_store(node->fils, store, &iter);
        }
        // Passer au noeud frère
        node = node->frere;
    }
}

/**
 * @brief Crée une arborescence visuelle (GtkTreeView) à partir d'une structure Arbre.
 *
 * La fonction crée un GtkTreeStore, y insère les noeuds de l'arbre 'racine' et
 * crée un GtkTreeView qui utilise ce modèle pour afficher visuellement l'arborescence.
 *
 * @param racine La racine de la structure Arbre.
 * @return GtkWidget* Le widget GtkTreeView affichant l'arborescence.
 */
GtkWidget* creer_arborescence_visuelle(Arbre *racine) {
    // Créer un GtkTreeStore avec une colonne de type chaîne de caractères
    GtkTreeStore *store = gtk_tree_store_new(1, G_TYPE_STRING);

    // Ajouter les noeuds de l'arbre dans le store
    ajouter_noeuds_dans_tree_store(racine, store, NULL);

    // Créer le GtkTreeView avec le modèle 'store'
    GtkWidget *tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    g_object_unref(store); // Le tree_view détient désormais une référence sur le modèle

    // Créer un renderer et une colonne pour afficher le nom des noeuds
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes("Arborescence", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

    return tree_view;
}


/**
 * @brief Recherche dans l'arbre le nom du conteneur associé au widget donné.
 *
 * La fonction parcourt récursivement l'arbre à partir de la racine et compare le champ
 * 'widget' de chaque noeud avec le widget fourni. Si une correspondance est trouvée, le
 * nom du conteneur (le champ 'nom') est retourné.
 *
 * @param racine La racine de l'arbre dans lequel effectuer la recherche.
 * @param widget Le widget du conteneur recherché.
 * @return char* Le nom du conteneur correspondant, ou NULL s'il n'est pas trouvé.
 */
char* getNomConteneurByWidget(Arbre *racine, GtkWidget *widget) {
    if (racine == NULL)
        return NULL;

    if (racine->widget == widget)
        return racine->nom;

    char *resultat = getNomConteneurByWidget(racine->fils, widget);
    if (resultat != NULL)
        return resultat;

    return getNomConteneurByWidget(racine->frere, widget);
}


#include <string.h>

/**
 * @brief Retourne l'indice d'une chaîne dans un tableau de chaînes.
 *
 * La fonction parcourt le tableau de chaînes, qui doit être terminé par un pointeur NULL,
 * et compare chaque élément avec la chaîne passée en paramètre. Si la chaîne est trouvée,
 * la fonction retourne son indice. Sinon, elle retourne -1.
 *
 * @param tableau Le tableau de chaînes de caractères (terminé par NULL).
 * @param chaine La chaîne à rechercher dans le tableau.
 * @return int L'indice de la chaîne dans le tableau, ou -1 si elle n'est pas trouvée.
 */
int getIndice(char *tableau[], char *chaine) {
    int i = 0;
    while (tableau[i] != NULL) {
        if (strcmp(tableau[i], chaine) == 0) {
            return i;
        }
        i++;
    }
    return -1;
}


/**
 * @brief Supprime un noeud de l'arbre ainsi que tous ses descendants.
 *
 * Cette fonction libère récursivement la mémoire allouée pour le noeud passé en paramètre,
 * ainsi que pour tous ses fils et frères. Elle doit être utilisée pour supprimer un noeud
 * détaché de l'arbre ou pour supprimer l'intégralité d'un sous-arbre.
 *
 * @param noeud Pointeur vers le noeud à supprimer.
 */
void supprimer_noeud(Arbre *noeud) {
    if (noeud == NULL) {
        return;
    }
    
    // Supprimer récursivement le sous-arbre des fils
    supprimer_noeud(noeud->fils);
    
    // Supprimer récursivement le sous-arbre des frères
    supprimer_noeud(noeud->frere);
    
    // Libérer le noeud courant
    free(noeud);
}


/**
 * @brief Supprime le premier noeud de l'arbre dont le nom correspond au paramètre, ainsi que tous ses descendants.
 *
 * La fonction parcourt récursivement l'arbre. Lorsqu'un noeud dont le nom correspond à 'nom' est trouvé,
 * il est supprimé (ainsi que tous ses fils et frères associés). Si ce noeud est la racine d'un sous-arbre,
 * la fonction retourne le pointeur vers le premier frère du noeud supprimé, ce qui met à jour la branche concernée.
 *
 * @param racine La racine de l'arbre ou du sous-arbre à traiter.
 * @param nom Le nom du noeud à supprimer.
 * @return Arbre* Le nouvel arbre (ou sous-arbre) après suppression du noeud.
 */
Arbre* supprimerNoeudParNom(Arbre* racine, const char* nom) {
    if (racine == NULL) {
        return NULL;
    }

    // Si le noeud courant correspond au nom recherché, le supprimer
    if (strcmp(racine->nom, nom) == 0) {
        Arbre* nouveau = racine->frere;  // Sauvegarder la suite de la liste au même niveau
        supprimer_noeud(racine);         // Libérer la mémoire du noeud et de ses descendants
        return nouveau;
    }

    // Sinon, traiter récursivement les fils et les frères
    racine->fils = supprimerNoeudParNom(racine->fils, nom);
    racine->frere = supprimerNoeudParNom(racine->frere, nom);
    
    return racine;
}

#endif //XML_TAHA_CONTAINERS_LIST_H
