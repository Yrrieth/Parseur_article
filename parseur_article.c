#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare_mot (char buffer[]) {
	int i, j;
	int identique = 0;
	char *mot[] = {"le", "Le", "la", "La", "les", "Les", "l", "L"};
	int nombreMaxMot = sizeof(mot)/sizeof(mot[0]);

	for (i = 0; i < nombreMaxMot; i++) {
		if (strcmp(buffer, mot[i]) == 0) {
			identique = 1;
			return identique;
		}
	}
	return identique;
}

int compare_pron_suj (char buffer[]) {
	int i, j;
	int identique = 0;
	char *mot[] = {"je", "tu", "il", "elle", "on", "nous", "vous", "ils", "elles",
				   "Je", "Tu", "Il", "Elle", "On", "Nous", "Vous", "Ils", "Elles",
				   "me", "te", "se", "J", "j"};
	int nombreMaxMot = sizeof(mot)/sizeof(mot[0]);
	for (i = 0; i < nombreMaxMot; i++) {
		if (strcmp(buffer, mot[i]) == 0) {
			identique = 1;
			return identique;
		}
	}
	return identique;
}

int compare_prepo_pron (char buffer[]) {
	int i, j;
	int identique = 0;
	char *mot[] = {"de", "à"};
	int nombreMaxMot = sizeof(mot)/sizeof(mot[0]);
	for (i = 0; i < nombreMaxMot; i++) {
		if (strcmp(buffer, mot[i]) == 0) {
			identique = 1;
			return identique;
		}
	}
	return identique;
}


int compare_prepo_articl (char buffer[]) {
	int i, j;
	int identique = 0;
	char *mot[] = {"sur", "sous", "entre", "devant", "derrière", "dans", "chez",
				   "avant", "après", "vers", "depuis", "pendant", "pour",
				   "vers", "tout", "toute", "tous", "toutes"};
	int nombreMaxMot = sizeof(mot)/sizeof(mot[0]);
	for (i = 0; i < nombreMaxMot; i++) {
		if (strcmp(buffer, mot[i]) == 0) {
			identique = 1;
			return identique;
		}
	}
	return identique;
}

void lire (FILE* fichier_entree, FILE* fichier_a, FILE* fichier_article, FILE* fichier_pronom){
	// index
	int c = 0;
	int taille = 256;
	int indexCourant = 0; // index courant du buffer

	// string
	char buffer[taille];       // Stocke le mot actuel
	char motPrecedent[taille]; // Stocke le mot précédant le buffer
	char apostrophe[2];        // Stocke le caractère ' 
                               //(ici, fgetc retourne un entier qu'il faudra convertir en char grâce à sprintf() pour concaténer avec strcat(), voir plus bas)
	char motSuivant[taille];   // Stocke le mot suivant le buffer

	// boolean
	int mot_identique;
	int pron_suj;
	int prepo_articl;
	int bool_motSuiv;
	
	while((c = fgetc(fichier_entree)) != EOF) { // Boucle sur chaque caractère du texte
		if (c == ',' || c == '.' || c == '!' || c == '?' || c == ' ' || c == '\'' ||
			c == ';' || c == '-' || c == '\t' || c == '\n') { // Si c'est une ponctuation == Fin de mot
			indexCourant = 0;
			mot_identique = compare_mot(buffer);
			//printf("%s\n", buffer);

			if (mot_identique == 1) {
				pron_suj = compare_pron_suj(motPrecedent);
				prepo_articl = compare_prepo_articl(motPrecedent);

				if (pron_suj == 1) {
					fprintf(fichier_a, "<pronom>");
					bool_motSuiv = 1;
				} else if (prepo_articl == 1) {
					fprintf(fichier_a, "<article>");
					bool_motSuiv = 2;
				} else {
					fprintf(fichier_a, "<trouve>");
				}

				if (c == '\'') { // Dans le cas où c vaut l' ou L'
					fprintf(fichier_a, "%s%c", buffer, c);

					if (pron_suj == 1) {
						fprintf(fichier_a, "</pronom>");
					} else if (prepo_articl == 1) {
						fprintf(fichier_a, "</article>");
					} else {
						fprintf(fichier_a, "</trouve>");
					}

					sprintf(apostrophe, "%d", c);  // int devient un char*
					strcat(buffer, apostrophe);    // Concatène (l ou L) et '
				} else {
					fprintf(fichier_a, "%s", buffer);

					if (pron_suj == 1) {
						fprintf(fichier_a, "</pronom>%c", c);
					} else if (prepo_articl == 1) {
						fprintf(fichier_a, "</article>%c", c);
					} else {
						fprintf(fichier_a, "</trouve>%c", c);
					}
				}
			} else {
				fprintf(fichier_a, "%s%c", buffer, c);

				if (bool_motSuiv == 1) {
					fprintf(fichier_pronom, "%s\n", buffer);
					bool_motSuiv = 0;
				} else if (bool_motSuiv == 2) {
					fprintf(fichier_article, "%s\n", buffer);
					bool_motSuiv = 0;
				}
			}
			memset(motPrecedent, 0, sizeof(motPrecedent)); // Pour effacer le contenu de motPrecedent
			
			/** strcpy(motPrecedent, buffer);
			 * Copie le buffer actuel
			 * pour que dans la boucle suivante,
			 * la copie devient le mot précédent
			 */
			strcpy(motPrecedent, buffer);

			memset(buffer, 0, sizeof(buffer)); // Pour effacer le contenu du buffer
		} else {
			buffer[indexCourant] = c;
			indexCourant++;
		}
	}
}

int main (int argc, char *argv[]){
	if(argc < 3){
		fprintf(stderr, "Il manque des arguments :\n./a.out texte_a_analyser.txt resultat.xml\n");
		exit(1);
	}
	FILE* fichier_entree = NULL;
	FILE* fichier_sortie = NULL;
	FILE* fichier_article = NULL;
	FILE* fichier_pronom = NULL;

	fichier_entree = fopen(argv[1], "r+");
	fichier_sortie = fopen(argv[2], "w+");
	fichier_article = fopen("fichier_article.txt", "w+");
	fichier_pronom = fopen("fichier_pronom.txt", "w+");

	if(fichier_entree == NULL || fichier_sortie == NULL || fichier_article == NULL || fichier_pronom == NULL){
		fprintf(stderr, "bug à la lecture/création du fichier\n");
		exit(1);
	}

	fprintf(fichier_sortie, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
	fprintf(fichier_sortie, "<?xml-stylesheet href=\"res.xsl\" type=\"text/xsl\"?>\n");
	fprintf(fichier_sortie, "<texte>\n");

	lire(fichier_entree, fichier_sortie, fichier_article, fichier_pronom);

	fprintf(fichier_sortie, "\n</texte>");
	fclose(fichier_entree);
	fclose(fichier_sortie);
	return 0;
}
