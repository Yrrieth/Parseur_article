#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare (char buffer[]) {
	int i, j;
	char *mot[] = {"le", "Le", "la", "La", "les", "Les", "l", "L"};
	int identique = 0;
	int nombreMaxMot = sizeof(mot)/sizeof(mot[0]);
	for (i = 0; i < nombreMaxMot; i++) {
		if (strcmp(buffer, mot[i]) == 0) {
			identique = 1;
			return identique;
		}
	}
	return identique;
}

void lire (FILE* fichier_entree, FILE* fichier_a){
	int c = 0,c2 = 0, c3 = 0;
	int taille = 256;
	int indexCourant = 0;
	char buffer[taille];
	int identique;

	while((c = fgetc(fichier_entree)) != EOF) { // Boucle sur tous le texte
		if (c == '.' || c == '!' || c == '?' || c == ' ' || (c == '\'')) {
			indexCourant = 0;
			identique = compare(buffer);

			if (identique == 1) {
				fprintf(fichier_a, "<trouve>");

				if (c == '\'') { // Dans le cas où c vaut l' ou L'
					fprintf(fichier_a, "%s%c", buffer, c);
					fprintf(fichier_a, "</trouve>");
				} else {
					fprintf(fichier_a, "%s", buffer);
					fprintf(fichier_a, "</trouve>%c", c);
				}
				memset(buffer, 0, sizeof(buffer)); // Pour effacer le contenu du buffer
			} else {
				fprintf(fichier_a, "%s%c", buffer, c);
				memset(buffer, 0, sizeof(buffer)); // Pour effacer le contenu du buffer
			}
		} else {
			buffer[indexCourant] = c;
			indexCourant++;
		}
	}
}

int main (int argc, char *argv[]){
	if(argc < 3){
		fprintf(stderr, "Il manque des arguments :\n./a.out fichier_a_parser.txt resultat.xml\n");
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
    lire(fichier_entree,fichier_sortie);
	fclose(fichier_entree);
	fclose(fichier_sortie);
	return 0;
}
