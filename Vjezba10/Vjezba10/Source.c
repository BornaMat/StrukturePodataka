#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXIT_SUCCESS 0
#define FILE_OPEN_ERROR -1;
#define SCANF_ERROR -2
#define MALLOC_ERROR -3
#define MAX_SIZE 1024

struct _countryList;
typedef struct _countryList* CountryListPosition;
typedef struct _countryList {
	char name[20];
	CountryListPosition next;
	CityListPosition link;
} countryList;

struct _cityList;
typedef struct _cityList* CityListPosition;
typedef struct _cityList {
	int population;
	char name[20];
	CityListPosition next;
} cityList;

struct _countryTree;
typedef struct _countryTree* CountryTreePosition;
typedef struct _countryTree {
	char name[20];
	CountryTreePosition left;
	CountryTreePosition right;
	CityListPosition link;
} countryTree;

struct _cityTree;
typedef struct _cityTree* CityTreePosition;
typedef struct _cityTree {
	int population;
	char name[20];
	CityTreePosition left;
	CityTreePosition right;
} cityTree;

int addCountryToList(CountryListPosition current, CountryListPosition newCountry);
CountryTreePosition addCountryToTree(CountryTreePosition current, CountryTreePosition newCountry);
int addCityToList(CityListPosition current, CityListPosition newCity);
CityTreePosition AddCityToTree(CityTreePosition current, CityTreePosition newCity);

int main()
{
	countryList countryListHead = { .name = {0}, .next = NULL, .link = NULL };
	CountryTreePosition CountryTree = NULL;
	cityList cityListhead = { .name = {0}, .population = 0,.next = NULL };
	CityTreePosition CityTree = NULL;

	char buffer[MAX_SIZE] = { 0 };
	char country[MAX_SIZE] = { 0 }, countryFilePath[24] = { 0 }, city[20] = { 0 };
	int population = 0;
	FILE* countryFilePointer = NULL;
	FILE* filePointer = NULL;

	filePointer = fopen("drzave.txt", "r");
	if (filePointer == NULL) { printf("file error\n"); return FILE_OPEN_ERROR; }

	while (!feof(filePointer)) {
		fgets(buffer, MAX_SIZE, filePointer);
		if (sscanf(buffer, "%[^,], %s", country, countryFilePath) != 2) { printf("sscanf error.\n"); return SCANF_ERROR; }

		CountryListPosition newCountryList = NULL;
		newCountryList = (CountryListPosition)malloc(sizeof(countryList));
		if (newCountryList == NULL) { printf("malloc error\n"); return MALLOC_ERROR; }

		strcpy(newCountryList->name, country);

		CountryTreePosition newCountryTree = NULL;
		newCountryTree = (CountryTreePosition)malloc(sizeof(countryTree));
		if (newCountryTree == NULL) { printf("malloc error\n"); return MALLOC_ERROR; }

		strcpy(newCountryTree->name, country);

		addCountryToList(countryListHead.next, newCountryList);
		CountryTree = addCountryToTree(CountryTree, newCountryTree);

		countryFilePointer = fopen(countryFilePath, "r");
		while (!feof(countryFilePointer)) {
			fgets(buffer, MAX_SIZE, countryFilePointer);
			if (sscanf(buffer, "%[^,], %d", city, &population) != 2) { printf("sscanf error.\n"); return SCANF_ERROR; }

			CityListPosition newCityList = NULL;
			newCityList = (CityListPosition)malloc(sizeof(cityList));
			if (newCityList == NULL) { printf("malloc error\n"); return MALLOC_ERROR; }

			strcpy(newCityList->name, city);
			newCityList->population = population;

			CityTreePosition newCityTree = NULL;
			newCityTree = (CityTreePosition)malloc(sizeof(cityTree));
			if (newCityTree == NULL) { printf("malloc error\n"); return MALLOC_ERROR; }

			strcpy(newCityTree->name, city);
			newCityTree->population = population;

			addCityToList(newCountryList, newCityTree);
			newCityTree = addCityToTree(newCountry, newCityTree);
		}
		fclose(countryFilePointer);
	}

	fclose(filePointer);
	return EXIT_SUCCESS;
}
int addCountryToList(CountryListPosition current, CountryListPosition newCountry) {
	while (current->next != NULL && strcmp(newCountry->name, current->next->name) > 0) current = current->next;

	newCountry->next = current->next;
	current->next = newCountry;

	return EXIT_SUCCESS;
}

CountryTreePosition addCountryToTree(CountryTreePosition current, CountryTreePosition newCountry) {
	if (current == NULL) {
		current = newCountry;
		current->left = NULL;
		current->right = NULL;
	}

	if (strcmp(current->name, newCountry->name) > 0) {
		current->left = addCountryToTree(current->left, newCountry);
	}
	else if (strcmp(current->name, newCountry->name) < 0) {
		current->right = addCountryToTree(current->right, newCountry);
	}
	return current;
}
int addCityToList(CityListPosition current, CityListPosition newCity) {
	while (current->next != NULL && current->next->population > newCity->population) { current = current->next; }
	if (current->next->population == newCity->population) {
		while (current->next != NULL && strcmp(current->next->name, newCity->name) > 0) { current = current->next; }
	}

	newCity->next = current->next;
	current->next = newCity;

	return EXIT_SUCCESS;
}
CityTreePosition AddCityToTree(CityTreePosition current, CityTreePosition newCity) {
	if (current == NULL) {
		current = newCity;
		current->left = NULL;
		current->right = NULL;
	}
	if (current->population == newCity->population) {
		if (strcmp(current->name, newCity->name) > 0) {
			current->left = addCityToTree(current->left, newCity);
		}
		else if (strcmp(current->name, newCity->name) < 0) {
			current->right = addCityToTree(current->right, newCity);
		}
	}
	else {
		if (current->population > newCity->population) {
			current->left = addCityToTree(current->left, newCity);
		}
		else if (current->population < newCity->population) {
			current->right = addCityToTree(current->right, newCity);
		}
	}
	return current;
}