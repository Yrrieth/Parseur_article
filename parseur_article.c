#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*void lire (FILE* fichier_entree, FILE* fichier_a){
	int c = 0,c2 = 0, c3 = 0;
	fprintf(fichier_a, "<p>");
	while((c = fgetc(fichier_entree)) != EOF)
		if(c == '.' || c == '!' || c == '?'){
			if((c2 = fgetc(fichier_entree)) != EOF){
				if(c2 == 32 || c2 == 10 || c2 == 13) {
					//fseek(fichier_a, -1, SEEK_CUR);
	    			fprintf(fichier_a, "%c </p>\n<p>",c2);
				}
				
				else
					fprintf(fichier_a, "%c",c);
				ungetc(c2,fichier_entree);
			}
		}
		else 
			fprintf(fichier_a, "%c", c);
}	*/

void lire (FILE* fichier_entree, FILE* fichier_a){
	int c = 0,c2 = 0, c3 = 0;
	int taille = 10;
	char buffer[taille];
	int pasDeLettreApres = 0;

	while((c = fgetc(fichier_entree)) != EOF) { // Boucle sur tous le texte
		// 46 == '.', 
		if (c == '.' || c == '!' || c == '?' || c == ' ') {
			fprintf(fichier_a, "%c", c);
			while ((c2 = fgetc(fichier_entree)) != EOF) { // Boucle pour déterminer "le"
				if (c2 == 'L' || c2 == 'l') {
					buffer[0] = c2;
					continue;
				} else if (c2 == 'e' && (buffer[0] == 'L' || buffer[0] == 'l')) {
					buffer[1] = c2;

					while ((c3 = fgetc(fichier_entree)) != EOF) { // Boucle sur la lettre qui suit "le"
						if (c3 == ' ') {
							fprintf(fichier_a, "<trouve>");
							for (int i = 0; i < 2; i++) {
								fprintf(fichier_a, "%c", buffer[i]);
							}
							fprintf(fichier_a, "</trouve>%c", c3);
							memset(buffer, 0, sizeof(buffer));
							pasDeLettreApres = 1;
							break;
						} else {
							for (int i = 0; i < 2; i++) {
								fprintf(fichier_a, "%c", buffer[i]);
							}
							fprintf(fichier_a, "%c", c3);
							memset(buffer, 0, sizeof(buffer));
							break;
						}
					} 
					break;
				} else {
					fprintf(fichier_a, "%c", c2);
					memset(buffer, 0, sizeof(buffer));
					break;
				}
			}
		} else {
			fprintf(fichier_a, "%c", c);
		}
	}
}

int main (int argc, char *argv[]){

	if(argc < 3){
		fprintf(stderr, "Il manque des arguments\n");
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
