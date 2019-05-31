#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* convertis_prem_lettre_en_maj (char mot[]) {
	int taille = 256;
	char* motEnMaj = malloc(taille * sizeof(char));
	strcpy(motEnMaj,mot);
	motEnMaj[0] = mot[0] - 32;  // 32, car il y a une différence de 32 symboles dans le tableau ASCII
	                            // pour obtenir la même lettre en majuscule
	return motEnMaj;
}

int compare_mot (char buffer[]) {
	int i, j;
	int taille = 256;
	char* motEnMaj = malloc(taille * sizeof(char));
	int identique = 0;
	char *mot[] = {"le", "la", "les", "l"};
	int nombreMaxMot = sizeof(mot)/sizeof(mot[0]);

	for (i = 0; i < nombreMaxMot; i++) {
		motEnMaj = convertis_prem_lettre_en_maj(mot[i]);
		if (strcmp(buffer, mot[i]) == 0 || strcmp(buffer, motEnMaj) == 0) {
			identique = 1;
			free(motEnMaj);
			return identique;
		}
		memset(motEnMaj, 0, sizeof(motEnMaj));
	}
	free(motEnMaj);
	return identique;
}

int compare_pron_suj (char buffer[]) {
	int i, j;
	int taille = 256;
	char* motEnMaj = malloc(taille * sizeof(char));
	int identique = 0;
	char *mot[] = {"je", "tu", "il", "elle", "on", "nous", "vous", "ils", "elles",
				   "me", "te", "se", "j", "ne", "qui", "chacun"};
	int nombreMaxMot = sizeof(mot)/sizeof(mot[0]);

	for (i = 0; i < nombreMaxMot; i++) {
		motEnMaj = convertis_prem_lettre_en_maj(mot[i]);
		if (strcmp(buffer, mot[i]) == 0 || strcmp(buffer, motEnMaj) == 0) {
			identique = 1;
			free(motEnMaj);
			return identique;
		}
		memset(motEnMaj, 0, sizeof(motEnMaj));
	}
	free(motEnMaj);
	return identique;
}

int compare_prepo_articl (char buffer[]) {
	int i, j;
	int taille = 256;
	char* motEnMaj = malloc(taille * sizeof(char));
	int identique = 0;
	char *mot[] = {"sur", "sous", "entre", "devant", "derrière", "dans", "chez",
				   "avant", "après", "vers", "depuis", "pendant", "pour", "entre", "contre",
				   "vers", "tout", "toute", "tous", "toutes", "avec", "par", "parmi",
				   "dessous", "dessus", "que", "duquel", "desquels", "desquelles", 
				   "auquel", "auxquels", "auxquelles", "lequel", "laquelle", "lesquels", "lesquelles",
				   // Gentillé, ex: "Monsieur le maire"
				   "monsieur", "m", "madame", "mme", "mademoiselle", "mlle"
				   };
	int nombreMaxMot = sizeof(mot)/sizeof(mot[0]);

	for (i = 0; i < nombreMaxMot; i++) {
		motEnMaj = convertis_prem_lettre_en_maj(mot[i]);
		if (strcmp(buffer, mot[i]) == 0 || strcmp(buffer, motEnMaj) == 0) {
			identique = 1;
			free(motEnMaj);
			return identique;
		}
		memset(motEnMaj, 0, sizeof(motEnMaj));
	}
	free(motEnMaj);
	return identique;
}

int compare_lexique_nominal (char buffer[]) {
	int i, j;
	int taille = 256;
	char* motEnMaj = malloc(taille * sizeof(char));
	int identique = 0;
	char *mot[] = {"un", "une", "du", "des", "quel", "quelle", "quels", "au", "aux"
				  };
	int nombreMaxMot = sizeof(mot)/sizeof(mot[0]);

	for (i = 0; i < nombreMaxMot; i++) {
		motEnMaj = convertis_prem_lettre_en_maj(mot[i]);
		if (strcmp(buffer, mot[i]) == 0 || strcmp(buffer, motEnMaj) == 0) {
			identique = 1;
			free(motEnMaj);
			return identique;
		}
		memset(motEnMaj, 0, sizeof(motEnMaj));
	}
	free(motEnMaj);
	return identique;
}

/*
char *pronom_possesif[] = {"mien", "mienne", "tien", "tienne", "sien", "sienne",
						   "miens", "miennes", "tiens", "tiennes", "siens", "siennes",
						   "nôtre", "nôtres", "vôtre", "vôtres", "leur", "leurs"
						  };
*/

int compare_mot_fichier_dico (FILE* fichier_dico, char buffer[]) {
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

void ecrit_dans_fichier_dico (FILE* fichier_dico, char buffer[]) {
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
	int ponctuationPrecedente = 0;

	// string
	char buffer[taille];       // Stocke le mot actuel
	char motPrecedent[taille]; // Stocke le mot précédant le buffer actuel

	// boolean
	int mot_identique;
	int pron_suj;
	int prepo_articl;
	int bool_motSuiv;  // Initialisé lorsqu'on trouve un mot identique,
	                   // Puis, lors de la boucle suivante, stocke le buffer dans un fichier
	int bool_motPrecedent; // Lors de la seconde relecture, reçoit 1 quand on est dans une balise
	                       // avec un cas indeterminé

	if (prem_lecture == 0) {
		fseek(fichier_entree, 0, SEEK_SET);
		fseek(fichier_pronom, 0, SEEK_SET);
		fseek(fichier_article, 0, SEEK_SET);
	}

	memset(buffer, 0, sizeof(buffer)); // Au début, le buffer n'est pas initialisé, il faut donc l'initialiser
	                                   // Ici, toutes les cases du buffer est initialisé à 0;
	memset(motPrecedent, 0, sizeof(motPrecedent));

	while((c = fgetc(fichier_entree)) != EOF) { // Boucle sur chaque caractère du texte
		if (c == ',' || c == '.' || c == '!' || c == '?' || c == ' ' || c == '\'' ||
			c == ';' || c == '_' || c == '\t' || c == '\n') { // Si c'est une ponctuation == Fin de mot
			indexCourant = 0;
			mot_identique = compare_mot(buffer);

			if (mot_identique == 1) {
				pron_suj = compare_pron_suj(motPrecedent);
				prepo_articl = compare_prepo_articl(motPrecedent);

				if (pron_suj == 1) {
					fprintf(fichier_a, "<pronom>&lt;pp");
					bool_motSuiv = 1;
					bool_motPrecedent = 0;
				} else if (prepo_articl == 1) {
					fprintf(fichier_a, "<article>&lt;art");
					bool_motSuiv = 2;
					bool_motPrecedent = 0;
				} else {
					if (prem_lecture == 1) {
						fprintf(fichier_a, "<trouve>&lt;idt");
					} else if (prem_lecture == 0) {
						bool_motPrecedent = 1;
					}
				}

				if (c == '\'') { // Dans le cas où c vaut l' ou L'
					if (pron_suj == 1) {
						fprintf(fichier_a, "&gt;");
						fprintf(fichier_a, "%s%c", buffer, c);
						fprintf(fichier_a, "&lt;pp&gt;</pronom>");
					} else if (prepo_articl == 1) {
						fprintf(fichier_a, "&gt;");
						fprintf(fichier_a, "%s%c", buffer, c);
						fprintf(fichier_a, "&lt;art&gt;</article>");
					} else {
						if (prem_lecture == 1) {
							fprintf(fichier_a, "&gt;");
							fprintf(fichier_a, "%s%c", buffer, c);
							fprintf(fichier_a, "&lt;idt&gt;</trouve>");	
						}
					}
				} else {
					if (pron_suj == 1) {
						fprintf(fichier_a, "&gt;");
						fprintf(fichier_a, "%s", buffer);
						fprintf(fichier_a, "&lt;pp&gt;</pronom>%c", c);
					} else if (prepo_articl == 1) {
						fprintf(fichier_a, "&gt;");
						fprintf(fichier_a, "%s", buffer);
						fprintf(fichier_a, "&lt;art&gt;</article>%c", c);
					} else {
						if (prem_lecture == 1) {
							fprintf(fichier_a, "&gt;");
							fprintf(fichier_a, "%s", buffer);
							fprintf(fichier_a, "&lt;idt&gt;</trouve>%c", c);
						}
					}
				}
			} else if (compare_lexique_nominal(buffer) == 1 && prem_lecture == 1) { // Sinon, si un c'est un article
				bool_motSuiv = 2;
				fprintf(fichier_a, "%s%c", buffer, c);
			} else { // Sinon, ce n'est pas un mot identique

				if (prem_lecture == 1) {  // Première lecture
					// On stocke le mot qui suit le mot recherché dans un fichier/dico
					if (bool_motSuiv == 1) { // Si c'était un pronom
						fseek(fichier_pronom, 0, SEEK_SET);
						ecrit_dans_fichier_dico(fichier_pronom, buffer);
						bool_motSuiv = 0;

					} else if (bool_motSuiv == 2) { // Si c'était un article
						fseek(fichier_article, 0, SEEK_SET);
						ecrit_dans_fichier_dico(fichier_article, buffer);
						bool_motSuiv = 0;
					}

					fprintf(fichier_a, "%s%c", buffer, c);
				} else if (prem_lecture == 0) {  // Seconde lecture
					// Lorsque le mot précédent est l'un des mots que l'on cherche (motPrecedent)
					// et que ce mot a été balisé comme un cas indeterminé (bool_motPrecedent)
					//printf("%s %s %d\n", motPrecedent, buffer, bool_motPrecedent);
					if (compare_mot(motPrecedent) == 1 && bool_motPrecedent == 1) { 
						
						fseek(fichier_pronom, 0, SEEK_SET);
						fseek(fichier_article, 0, SEEK_SET);

						if (compare_mot_fichier_dico(fichier_pronom, buffer) == 1) {
							if (ponctuationPrecedente == '\'') {
								fprintf(fichier_a, "<pronom>&lt;pp&gt;%s%c&lt;/pp&gt;</pronom>", motPrecedent, ponctuationPrecedente);	
							} else {
								fprintf(fichier_a, "<pronom>&lt;pp&gt;%s&lt;/pp&gt;</pronom>%c", motPrecedent, ponctuationPrecedente);
							}
							
							fprintf(fichier_a, "%s%c", buffer, c);
							
						} else if (compare_mot_fichier_dico(fichier_article, buffer) == 1) {
							if (ponctuationPrecedente == '\'') {
								fprintf(fichier_a, "<article>&lt;art&gt;%s%c&lt;/art&gt;</article>", motPrecedent, ponctuationPrecedente);
							} else {
								fprintf(fichier_a, "<article>&lt;art&gt;%s&lt;/art&gt;</article>%c", motPrecedent, ponctuationPrecedente);
							}
							fprintf(fichier_a, "%s%c", buffer, c);

						} else {
							if (ponctuationPrecedente == '\'') {
								fprintf(fichier_a, "<trouve>&lt;idt&gt;%s%c&lt;/idt&gt;</trouve>", motPrecedent, ponctuationPrecedente);	
							} else {
								fprintf(fichier_a, "<trouve>&lt;idt&gt;%s&lt;/idt&gt;</trouve>%c", motPrecedent, ponctuationPrecedente);	
							}
							fprintf(fichier_a, "%s%c", buffer, c);
						}
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
	printf("Première lecture, création du dictionnaire en cours...\n");

	int prem_lecture = 1;

	cree_fichier_xml(fichier_entree, fichier_sortie, fichier_article, fichier_pronom, prem_lecture);
	fclose(fichier_sortie);

	int rm = remove(argv[2]);
	if (rm == 0) {
		printf("Fichier supprimé avec succès.\n");
	} else {
		fprintf(stderr, "bug à la suppression du fichier\n");
		exit(1);
	}

	fichier_sortie = fopen(argv[2], "w+");
	if (fichier_sortie == NULL) {
		fprintf(stderr, "bug à la lecture/création du fichier\n");
		exit(1);
	} 
	printf("Seconde lecture, balisage en cours...\n");
	prem_lecture = 0;
	cree_fichier_xml(fichier_entree, fichier_sortie, fichier_article, fichier_pronom, prem_lecture);

	printf("Lecture terminée.\n");
	fclose(fichier_entree);
	fclose(fichier_sortie);
	fclose(fichier_pronom);
	fclose(fichier_article);
	return 0;
}
