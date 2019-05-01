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
				   "me", "te", "se"};
	int nombreMaxMot = sizeof(mot)/sizeof(mot[0]);
	for (i = 0; i < nombreMaxMot; i++) {
		if (strcmp(buffer, mot[i]) == 0) {
			identique = 1;
			return identique;
		}
	}
	return identique;
}

/*
void lire (FILE* fichier_entree, FILE* fichier_a){
	int c = 0;
	int taille = 256;
	int indexCourant = 0; // index courant du buffer
	char buffer[taille];
	int identique;
	char motPrecedent[taille];
	char apostrophe[3];
	int pron_suj;
	char balise_ouv[taille];
	char balise_ferm[taille];
	
	while((c = fgetc(fichier_entree)) != EOF) { // Boucle sur chaque caractère du texte
		if (c == '.' || c == '!' || c == '?' || c == ' ' || (c == '\'')) { // Si c'est une ponctuation == Fin de mot
			indexCourant = 0;
			identique = compare_mot

			(buffer);

			//printf("%s\n", motPrecedent);

			if (identique == 1) {
				//pron_suj = compare_pron_suj(motPrecedent);
				//if ()
				strcpy(balise_ouv, "<trouve>");
				strcpy(balise_ferm, "</trouve>");

				fprintf(fichier_a, "%s", balise_ouv);

				if (c == '\'') { // Dans le cas où c vaut l' ou L'
					fprintf(fichier_a, "%s%c", buffer, c);
					fprintf(fichier_a, "%s", balise_ferm);
					sprintf(apostrophe, "%d", c);  // int devient un char*
					strcat(buffer, apostrophe);    // Concatène (l ou L) et '
				} else {
					fprintf(fichier_a, "%s", buffer);
					fprintf(fichier_a, "%s%c", balise_ferm, c);
				}
			} else {
				fprintf(fichier_a, "%s%c", buffer, c);
			}
			memset(motPrecedent, 0, sizeof(motPrecedent)); // Pour effacer le contenu de motPrecedent
			
			// strcpy(motPrecedent, buffer);
			// Copie le buffer actuel
			// pour que dans la boucle suivante,
			// la copie devient le mot précédent
			//
			strcpy(motPrecedent, buffer);

			memset(buffer, 0, sizeof(buffer)); // Pour effacer le contenu du buffer
		} else { 
			buffer[indexCourant] = c;
			indexCourant++;
		}
	}
}
*/

void lire (FILE* fichier_entree, FILE* fichier_a){
	int c = 0;
	int taille = 256;
	int indexCourant = 0; // index courant du buffer
	char buffer[taille];
	char motPrecedent[taille];
	char apostrophe[3];

	// boolean
	int identique;
	int pron_suj;
	
	while((c = fgetc(fichier_entree)) != EOF) { // Boucle sur chaque caractère du texte
		if (c == ',' || c == '.' || c == '!' || c == '?' || c == ' ' || c == '\'' ||
			c == '\t' || c == '\n') { // Si c'est une ponctuation == Fin de mot
			indexCourant = 0;
			identique = compare_mot(buffer);

			if (identique == 1) {
				pron_suj = compare_pron_suj(motPrecedent);
				if (pron_suj == 1) {
					fprintf(fichier_a, "<pronom>");
				} else {
					fprintf(fichier_a, "<trouve>");
				}

				if (c == '\'') { // Dans le cas où c vaut l' ou L'
					fprintf(fichier_a, "%s%c", buffer, c);

					if (pron_suj == 1) {
						fprintf(fichier_a, "</pronom>");
					} else {
						fprintf(fichier_a, "</trouve>");
					}

					sprintf(apostrophe, "%d", c);  // int devient un char*
					strcat(buffer, apostrophe);    // Concatène (l ou L) et '
				} else {
					fprintf(fichier_a, "%s", buffer);

					if (pron_suj == 1) {
						fprintf(fichier_a, "</pronom>%c", c);
					} else {
						fprintf(fichier_a, "</trouve>%c", c);
					}
				}
			} else {
				fprintf(fichier_a, "%s%c", buffer, c);	
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

	fichier_entree = fopen(argv[1], "r+");
	fichier_sortie = fopen(argv[2], "w+");
	if(fichier_entree == NULL || fichier_sortie == NULL){
		fprintf(stderr, "bug à la lecture/création du fichier\n");
		exit(1);
	}

	fprintf(fichier_sortie, "<?xml version=\"1.0\" encoding=\"utf-8\">\n");
	lire(fichier_entree, fichier_sortie);
	fclose(fichier_entree);
	fclose(fichier_sortie);
	return 0;
}
