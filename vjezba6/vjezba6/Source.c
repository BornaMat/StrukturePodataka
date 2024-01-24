#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

#define EXIT_SUCCESS 0
#define FILE_OPEN_ERROR -1
#define MALLOC_ERROR -2
#define SCANF_ERROR -3

struct _artikli;
typedef struct _artikli* artiklPozicija;
typedef struct _artikli {
	char naziv[20];
	int amount;
	int price;
	artiklPozicija next;
}Artikl;

struct _racuni;
typedef struct _racuni* racunPozicija;
typedef struct _racuni {
	char datum[20];
	racunPozicija next;
	artiklPozicija headArtikl;
}Racuni;

int addReceipt(racunPozicija, racunPozicija);
int readReceipt(char*, racunPozicija);
int addArticle(artiklPozicija, artiklPozicija);
int deleteReceipt(racunPozicija);
int deleteArticle(artiklPozicija head);
int printReceipt(racunPozicija);
printArticle(artiklPozicija);

int main() {
	Racuni headRacun = { .datum = {0}, .next = NULL, .headArtikl = NULL };
	char noviRacun[MAX_LINE] = { 0 };
	FILE* fp = NULL;
	char buffer[MAX_LINE] = { 0 };

	fp = fopen("racuni.txt", "r");
	if (fp == NULL) {
		printf("error fopen");
		return FILE_OPEN_ERROR;
	}
	int size = 0;

	while (fgets(noviRacun, MAX_LINE, fp) != NULL) {
		printf("%s\n", noviRacun);
		readReceipt(noviRacun, &headRacun);
	}
	printReceipt(&headRacun);
	deleteReceipt(&headRacun);
	fclose(fp);
	return EXIT_SUCCESS;
}
int addArticle(artiklPozicija head, artiklPozicija noviArtikl) {

	artiklPozicija current = head;

	while(current->next != NULL && strcmp(current->next->naziv, noviArtikl->naziv) <= 0)
		current = current->next;

	noviArtikl->next = current->next;
	current->next = noviArtikl;

	return EXIT_SUCCESS;
}

int addReceipt(racunPozicija head, racunPozicija noviRacun) {
	
	racunPozicija current = head;

	while (current->next != NULL && strcmp(current->next->datum, noviRacun->datum) <= 0)
		current = current->next;

	noviRacun->next = current->next;
	current->next = noviRacun;

	return EXIT_SUCCESS;
}
int readReceipt(char* fileName, racunPozicija racunHead) {
	char buffer[MAX_LINE] = { 0 };
	char datum[MAX_LINE] = { 0 };
	artiklPozicija noviArtikl = NULL;
	racunPozicija noviRacun = NULL;

	FILE* filePointer = NULL;
	filePointer = fopen(fileName, "r");
	if (filePointer == NULL) {
		printf("error fopen");
		return FILE_OPEN_ERROR;
	}

	noviRacun = (racunPozicija)malloc(sizeof(Racuni));
	if (noviRacun == NULL) {
		printf("malloc error\n");
		return MALLOC_ERROR;
	}

	fgets(datum,MAX_LINE,filePointer);
	strcpy(noviRacun->datum,datum);

	while (fgets(buffer,MAX_LINE,filePointer) != NULL) {

		noviArtikl = (artiklPozicija)malloc(sizeof(Artikl));
		if (noviArtikl == NULL) {
			printf("malloc error\n");
			return MALLOC_ERROR;
		}

		if (fscanf(filePointer,"%[^,], %d, %d", &noviArtikl->naziv, &noviArtikl->amount, &noviArtikl->price) != 3) { printf("pogresan file \n"); return SCANF_ERROR; }

		addArticle(&noviRacun->headArtikl,noviArtikl);
	}
	addReceipt(racunHead, noviRacun);
	fclose(filePointer);
	return EXIT_SUCCESS;

}
int deleteArticle(artiklPozicija head) {
	artiklPozicija temp = NULL;

	while (head->next != NULL) {
		temp = head->next;
		head->next = head->next->next;
		free(temp);
	}
	return EXIT_SUCCESS;
}

int deleteReceipt(racunPozicija head) {
	racunPozicija temp = NULL;

	while (head->next != NULL) {
		temp = head->next;
		head->next = head->next->next;
		deleteArticle(&temp->headArtikl);
		free(temp);
	}
	free(head);
	return 0;

}
int printArticle(artiklPozicija current) {
	while (current != NULL) {
		printf("\t artikl: %s \n kolicina: %d \n cijena: %d \n ", current->naziv, current->amount, current->price);
		current = current->next;
	}
	return EXIT_SUCCESS;
}

int printReceipt(racunPozicija current) {
	int count = 1;
	while (current != NULL) {
		printf("racun%d:\n", count);
		printArticle(current->headArtikl);
		count++;
		current = current->next;
	}
	return EXIT_SUCCESS;
}