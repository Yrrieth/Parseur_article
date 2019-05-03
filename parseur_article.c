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

/*int compare_prepo_pron (char buffer[]) {
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
}*/


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

int lit_fichier_dico (FILE* fichier_dico, char buffer[]) {
	int c;
	int taille = 256;
	char mot_fichier[taille];
	while (fgets(mot_fichier, taille, fichier_dico) != NULL) {
		mot_fichier[strlen(mot_fichier) - 1] = '\0';
		if (strcmp(mot_fichier, buffer) == 0) {
			return 1;
		}
	}
	return 0;
}

void cree_fichier_dico (FILE* fichier_dico, char buffer[]) {
	int c;
	int taille = 1024;
	char mot_fichier[taille];
	while (fgets(mot_fichier, taille, fichier_dico) != NULL) {
		mot_fichier[strlen(mot_fichier) - 1] = '\0';
		if (strcmp(mot_fichier, buffer) == 0) {
			return;
		}
	}
	fprintf(fichier_dico, "%s\n", buffer);
}

void lire (FILE* fichier_entree, FILE* fichier_a, FILE* fichier_article, FILE* fichier_pronom, int prem_lecture){
	// index
	int c = 0;
	int taille = 256;
	int indexCourant = 0; // index courant du buffer
	int ponctuationPrecedente;

	// string
	char buffer[taille];       // Stocke le mot actuel
	char motPrecedent[taille]; // Stocke le mot précédant le buffer
	char apostrophe[2];        // Stocke le caractère ' 
                               //(ici, fgetc retourne un entier qu'il faudra convertir en char grâce à sprintf() pour concaténer avec strcat(), voir plus bas)

	// boolean
	int mot_identique;
	int pron_suj;
	int prepo_articl;
	int bool_motSuiv;  // Initialisé lorsqu'on trouve un mot identique,
	                   // Puis, lors de la boucle suivante, stocke le buffer dans un fichier
	int bool_motPrecedent;

	if (prem_lecture == 0) {
		fseek(fichier_entree, 0, SEEK_SET);
	}

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
					bool_motPrecedent = 0;

					fichier_pronom = fopen("fichier_pronom.txt", "a+");
				} else if (prepo_articl == 1) {
					fprintf(fichier_a, "<article>");
					bool_motSuiv = 2;
					bool_motPrecedent = 0;

					fichier_article = fopen("fichier_article.txt", "a+");
				} else {
					if (prem_lecture == 1) {
						fprintf(fichier_a, "<trouve>");
					} else if (prem_lecture == 0) {
						bool_motPrecedent = 1;
					}
				}

				if (c == '\'') { // Dans le cas où c vaut l' ou L'
					fprintf(fichier_a, "%s%c", buffer, c);

					if (pron_suj == 1) {
						fprintf(fichier_a, "</pronom>");
					} else if (prepo_articl == 1) {
						fprintf(fichier_a, "</article>");
					} else {
						if (prem_lecture == 1) {
							fprintf(fichier_a, "</trouve>");	
						}
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
						if (prem_lecture == 1) {
							fprintf(fichier_a, "</trouve>%c", c);
						}
					}
				}
			} else { // Sinon, ce n'est pas un mot identique
				// On stocke le mot qui suit le mot recherché dans un fichier
				if (bool_motSuiv == 1) { // Si c'était un pronom
					//fprintf(fichier_pronom, "%s\n", buffer);
					cree_fichier_dico(fichier_pronom, buffer);
					bool_motSuiv = 0;

					fclose(fichier_pronom);
				} else if (bool_motSuiv == 2) { // Si c'était un article
					//fprintf(fichier_article, "%s\n", buffer);
					cree_fichier_dico(fichier_article, buffer);
					bool_motSuiv = 0;

					fclose(fichier_article);
				}
				if (prem_lecture == 0) {
					if (compare_mot(motPrecedent) == 1 && bool_motPrecedent == 1) {
						
						fichier_pronom = fopen("fichier_pronom.txt", "a+");
						fichier_article = fopen("fichier_article.txt", "a+");
						//perror("fopen:");

						if (lit_fichier_dico(fichier_pronom, buffer) == 1) {
							fprintf(fichier_a, "<pronom>%s</pronom>%c", motPrecedent, ponctuationPrecedente);
							fprintf(fichier_a, "%s%c", buffer, c);
							/*fprintf(fichier_a, "<pronom>");
							fprintf(fichier_a, "%s", motPrecedent);
							fprintf(fichier_a, "</pronom>");
							fprintf(fichier_a, "%c", ponctuationPrecedente);
							fprintf(fichier_a, "%s%c", buffer, c);*/
							
						} else if (lit_fichier_dico(fichier_article, buffer) == 1) {
							fprintf(fichier_a, "<article>%s</article>%c", motPrecedent, ponctuationPrecedente);
							fprintf(fichier_a, "%s%c", buffer, c);

						} else {
							fprintf(fichier_a, "<trouve>%s</trouve>%c", motPrecedent, ponctuationPrecedente);
							fprintf(fichier_a, "%s%c", buffer, c);
						}
						fclose(fichier_pronom);
						fclose(fichier_article);
						bool_motPrecedent = 0;
					} else {
						fprintf(fichier_a, "%s%c", buffer, c);
					}
				}
			}

			ponctuationPrecedente = c;

			memset(motPrecedent, 0, sizeof(motPrecedent)); // Pour effacer le contenu de motPrecedent
			
			/** strcpy(motPrecedent, buffer);
			 * Copie le buffer actuel
			 * pour que dans la boucle suivante,
			 * la copie devient le mot précédent
			 */
			strcpy(motPrecedent, buffer);

			memset(buffer, 0, sizeof(buffer)); // Pour effacer le contenu du buffer
		} else { // Sinon, ce n'est pas une ponctuation
			buffer[indexCourant] = c;
			indexCourant++;
		}
	}
}

void cree_fichier_xml (FILE* fichier_entree, FILE* fichier_sortie, FILE* fichier_article, FILE* fichier_pronom, int prem_lecture) {
	fprintf(fichier_sortie, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
	fprintf(fichier_sortie, "<?xml-stylesheet href=\"res.xsl\" type=\"text/xsl\"?>\n");
	fprintf(fichier_sortie, "<texte>\n");

	lire(fichier_entree, fichier_sortie, fichier_article, fichier_pronom, prem_lecture);

	fprintf(fichier_sortie, "\n</texte>");
	fprintf(fichier_sortie, "\n<!-- %d -->\n", prem_lecture);
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
	fichier_article = fopen("fichier_article.txt", "a+");
	fichier_pronom = fopen("fichier_pronom.txt", "a+");

	if(fichier_entree == NULL || fichier_sortie == NULL || fichier_article == NULL || fichier_pronom == NULL){
		fprintf(stderr, "bug à la lecture/création du fichier\n");
		exit(1);
	}

	int prem_lecture = 1;

	cree_fichier_xml(fichier_entree, fichier_sortie, fichier_article, fichier_pronom, prem_lecture);
	//fseek(fichier_sortie, 0, SEEK_SET);
	fclose(fichier_sortie);

	int rm = remove(argv[2]);
	if (rm == 0) {
		printf("Supprimé avec succès\n");
	} else {
		fprintf(stderr, "bug à la suppression du fichier\n");
		exit(1);
	}

	fichier_sortie = fopen(argv[2], "w+");
	prem_lecture = 0;
	cree_fichier_xml(fichier_entree, fichier_sortie, fichier_article, fichier_pronom, prem_lecture);

	fclose(fichier_entree);
	fclose(fichier_sortie);
	//fclose(fichier_pronom);
	//fclose(fichier_article);
	return 0;
}
