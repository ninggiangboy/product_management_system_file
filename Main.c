#include <corecrt.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

int checkProductID();
int checkCategoryID();
int amountProduct();
void connectByID();

int writeNewProduct();
int overwriteData();
int readCategory();
int readProduct();

void inputProduct();
void outputProduct();
void outputCategory();
void sortProduct();
void searchByName();
void searchByCategory();
void searchAdvanced();
void updateProduct();
void removeProduct();

struct Categories{
	int catID;
	char catName[20];
};

struct Products{
	char proID[20];
	char proName[50];
	float price;
	int quantity;
	struct Categories category;
};

void login(){
	char user[] = "user";
	char pass[] = "pass";
	char userEnter[20];
	char passEnter[20];
	int flag=0;
	do {
		system("cls");
		fflush(stdin);
		printf("\n======================== Login ========================\n\n");
		printf("\tEnter Username: ");
		fgets(userEnter, sizeof(userEnter), stdin);
		userEnter[strlen(userEnter) - 1 ] = 0;
		printf("\tEnter Password: ");
		fgets(passEnter, sizeof(passEnter), stdin);
		passEnter[strlen(passEnter) - 1 ] = 0;
		if (strcmp(user, userEnter) == 0 && strcmp(pass, passEnter) == 0){
			flag = 1;
		} else {
			printf("\n\tLogin is unsuccessful!\n");
			system("pause");
		} 
	} while (flag==0);
}

FILE *fp;

struct Products product[1000], temp;
struct Categories category[100];
char productsFile[] = "products.dat";
char categoriesFile[] = "categories.dat";

int main() {
	login();

	time_t t = time(NULL);
    struct tm tm;
    localtime_s(&tm, &t);

	char selection;
	do {
		memset(product, 0, sizeof(product));
		system("cls");
		printf("\n\t\tDate and time: %d-%02d-%02d %02d:%02d\n", tm.tm_mday, tm.tm_mon + 1,tm.tm_year + 1900, tm.tm_hour, tm.tm_min);
		printf("\n======================== Products Management ========================\n\n");
		printf("\t\t<A> Add Products\n");
		printf("\t\t<B> List of Products\n");
		printf("\t\t<C> List of Categories\n");
		printf("\t\t<D> Sort Products by price\n");
		printf("\t\t<E> Search Products by name\n");
		printf("\t\t<F> Search Products by category\n");
		printf("\t\t<G> Advanced Search\n");
		printf("\t\t<H> Update Products\n");
		printf("\t\t<I> Remove Products\n");
		printf("\t\t<K> Lock / Sign out\n");
		printf("\t\t<0> Exit!\n\n");

		printf("\tPlease select a function: ");
		selection = _getch();
		system("cls");

		switch(toupper(selection)){
			case 'A':
				inputProduct();
				break;
			case 'B':
				outputProduct();
				break;
			case 'C':
				outputCategory();
				break;
			case 'D':
				sortProduct();
				break;
			case 'E':
				searchByName();
				break;
			case 'F':
				searchByCategory();
				break;
			case 'G':
				searchAdvanced();
				break;
			case 'H':
				updateProduct();
				break;
			case 'I':
				removeProduct();
				break;
			case 'K':
				login();
				break;
			case '0':
				fflush(stdin);
				printf("\n\tAre you sure you want to close this programme [Y/N]: ");
				char exit = getchar();
				if (exit == 'Y' || exit == 'y') printf("\n================ END PROGRAMME ================\n");
				else selection = exit;
				break;	
		}
	} while (selection != '0');

	return 0;
}

int checkProductID(char ID[]){
	int n=readProduct();
	int flag = 0;
	for(int i=0;i<n;i++){
		if(_strcmpi(ID, product[i].proID) == 0){
			flag = 1;
			break;
		}
	} return flag;
}

int checkCategoryID(int ID){
	int n=readCategory();
	int flag = 0;
	for(int i=0;i<n;i++){
		if(ID == category[i].catID){
			flag = 1;
			break;		
		}
	} return flag;
}

int amountProduct(int ID){
	int n=readProduct();
	int number = 0;
	for(int i=0;i<n;i++){
		if(ID == product[i].category.catID){
			number++;
		}
	} return number;
}

void connectByID(int n_p, int n_c){
	for(int i=0;i<n_p;i++){
		for(int j=0;j<n_c;j++){
			if(product[i].category.catID == category[j].catID)
				snprintf(product[i].category.catName, sizeof(product[i].category.catName), "%s", category[j].catName);
		}
	}
}

int readCategory(){
	errno_t status = fopen_s(&fp, categoriesFile, "r");
	if(status != 0){
		printf("\nError: Can not open file..\n");
		fclose(fp);
		return -1;
	}
	int i = 0;
	while (feof(fp) == 0){
		fscanf_s(fp, "%d|", &category[i].catID, sizeof(category[i].catID));
		fscanf_s(fp, "%s\n", category[i].catName, sizeof(category[i].catName));
		i++;
	}
	fclose(fp);
	return i;
}

int readProduct(){
	errno_t status = fopen_s(&fp, productsFile, "r");
	if(status != 0){
		printf("\nError: Can not open file.\n");
		fclose(fp);
		return -1;
	}
	int i = 0;
	while (feof(fp) == 0) {
		fscanf_s(fp, "%[^|]|", product[i].proID, sizeof(product[i].proID));
		fscanf_s(fp, "%[^|]|", product[i].proName, sizeof(product[i].proName));
		fscanf_s(fp, "%f|", &product[i].price, sizeof(product[i].price));
		fscanf_s(fp, "%d|", &product[i].quantity, sizeof(product[i].quantity));
		fscanf_s(fp, "%d\n",  &product[i].category.catID, sizeof(product[i].category.catID));
		i++;
	}
	fclose(fp);
	return i;
}

int writeNewProduct(int i){
	errno_t status = fopen_s(&fp, productsFile, "a");
	if(status != 0){
		printf("Error: Can not connect to file!\n");
	}		
	fprintf(fp, "%s|%s|%.2f|%d|%d\n", product[i].proID, product[i].proName, product[i].price, product[i].quantity, product[i].category.catID);
	fclose(fp);
	return status;
}

int overwriteData(char fileName[], int n){
	errno_t status = fopen_s(&fp, fileName, "w");
	if(status != 0){
		printf("Error: Can not connect to file!\n");
	}		
	for(int i=0;i<n; i++){
		fprintf(fp, "%s|%s|%.2f|%d|%d\n", product[i].proID, product[i].proName, product[i].price, product[i].quantity, product[i].category.catID);
	}
	fclose(fp);
	return status;
}

void inputProduct(){
	int n;
	printf("\n\tEnter number of new products: ");
	scanf_s("%d", &n);
	fflush(stdin);
	for (int i = 0; i < n; i++){
		printf("\n\tProduct: #%d\n", i+1);
		int flag;

		printf("\t\tProduct ID: ");
		
		do {
			char ID[50];
			snprintf(ID, sizeof(ID), "%s", fgets(product[i].proID, sizeof(product[i].proID), stdin));
			ID[strlen(ID)-1] = 0;
			fflush(stdin);
			flag = 0;
			if(checkProductID(ID) != 0){
				printf("\t\tProduct ID already exists.");
				printf("\n\t\tEnter again: ");
				flag = 1;
			} else {
				for (int i = 0; i < strlen(ID); i++){
					ID[i] = toupper(ID[i]);
				}
				snprintf(product[i].proID, sizeof(product[i].proID), "%s", ID);
			} 
			
		} while (flag == 1);
		

		printf("\t\tProduct Name: ");
		fgets(product[i].proName, sizeof(product[i].proName), stdin);
		product[i].proName[strlen(product[i].proName)-1] = 0;

		printf("\t\tPrice: $");
		do{
			scanf_s("%f", &product[i].price);
			flag = 0;
			if(product[i].price <= 0){
				printf("\n\t\tPrice must be greater than $0.");
				printf("\n\t\tEnter again: $");
				flag = 1;
			}
		} while(flag == 1);

		printf("\t\tQuantity [0-100]: ");
		do{
			scanf_s("%d", &product[i].quantity);
			flag = 0;
			if(product[i].quantity < 0 || product[i].quantity > 100){
				printf("\n\t\tQuantity must be in the range of [0-100].");
				printf("\n\t\tEnter again: ");
				flag = 1;
			}
		} while(flag == 1);	

		printf("\t\tCategory ID: ");
		do {
			scanf_s("%d", &product[i].category.catID);
			fflush(stdin);
			flag = 0;
			if(checkCategoryID(product[i].category.catID) == 0){
				printf("\n\t\tCategory ID is not exists.");
				printf("\n\t\tEnter again: ");
				flag = 1;
			}
		} while(flag == 1);
		if(writeNewProduct(i) == 0) printf("\n\t-> Update new product success.\n");
		else printf("\n\t ->Update new product failse.\n");
	}
	
	printf("\nPress any key to go to Main Menu!\n");
	printf("Or press 'Tab' to continue using the function.\n");
	char selection = _getch();
	if (selection == '\t') {
		system("cls");
		inputProduct();
	}
}

void outputProduct(){
	printf("\n\t\t\t\t\tList of Products\n\n");
	printf("---------------------------------------------------------------------------------------------------");
	printf("\n\t%-10s\t%-20s\t%-10s\t%-10s\t%-10s\n", "ID","Name","Price", "Quantity", "Category" );
	printf("---------------------------------------------------------------------------------------------------\n\n" );
	int n_p = readProduct();
	int n_c = readCategory();
	connectByID(n_p, n_c);
	if(n_p!=-1){
		for(int i=0; i<n_p; i++){
			printf("\t%-10s\t%-20s\t%-10.2f\t%-10d\t%-10s\n\n", product[i].proID, product[i].proName, product[i].price, product[i].quantity, product[i].category.catName);
		} printf("---------------------------------------------------------------------------------------------------\n");
	}
	printf("\nPress any key to go to Main Menu!\n");
	_getch();
}

void outputCategory(){
	printf("\n\t\t\tList of Categories\n\n");
	printf("--------------------------------------------------------------");
	printf("\n\t%-10s\t%-20s\t%-20s\n", "ID", "Category", "Products" );
	printf("--------------------------------------------------------------\n\n" );
	int n_c=readCategory();
	if (n_c!=-1){
		for(int i=0; i<n_c; i++){
		printf("\t%-10d\t%-20s\t%-20d\n\n", category[i].catID, category[i].catName, amountProduct(category[i].catID));
		} printf("--------------------------------------------------------------\n");
	}
	printf("\nPress any key to go to Main Menu!\n");
	_getch();
}

void sortProduct(){
	int n_p = readProduct();
	int n_c = readCategory();
	connectByID(n_p, n_c);

	for(int i=0;i<n_p;i++){
		for(int j=0;j<n_p-i-1;j++){
			if (product[j].price < product[j+1].price){
				temp = product[j];  
				product[j] = product[j+1]; 
				product[j+1] = temp;
			}
		}
	} printf("\n\t\tProducts sorted in descending order of price.\n\n");

	printf("---------------------------------------------------------------------------------------------------");
	printf("\n\t%-10s\t%-20s\t%-10s\t%-10s\t%-10s\n", "ID","Name","Price", "Quantity", "Category" );
	printf("---------------------------------------------------------------------------------------------------\n\n" );

	if(n_p!=-1){
		for(int i=0; i<n_p; i++){
			printf("\t%-10s\t%-20s\t%-10.2f\t%-10d\t%-10s\n\n", product[i].proID, product[i].proName, product[i].price, product[i].quantity, product[i].category.catName);
		} printf("---------------------------------------------------------------------------------------------------\n");
	}
	printf("\nPress any key to go to Main Menu!\n");
	_getch();
}

void searchByCategory(){
	fflush(stdin);
	printf("\n\tEnter category of Product [Keyword] for search: ");
	char searchCategory[20];
	fgets(searchCategory, sizeof(searchCategory), stdin);
	searchCategory[strlen(searchCategory)-1] = 0;
	for(int i = 0; i < strlen(searchCategory); i++){
  		searchCategory[i] = tolower(searchCategory[i]);
	}
	int n_p = readProduct();
	int n_c = readCategory();
	connectByID(n_p, n_c);
	int markedID[1000];
	int flag=0;
	for(int i=0;i<n_p;i++){
		char catName[20];
		snprintf(catName, sizeof(catName), "%s", product[i].category.catName);
		catName[0] = tolower(catName[0]);
		if (strstr(catName, searchCategory) != NULL){
			markedID[product[i].category.catID] = 1;
			flag++;
			if(flag==1){
				printf("\n---------------------------------------------------------------------------------------------------");
				printf("\n\t%-10s\t%-20s\t%-10s\t%-10s\t%-10s\n", "ID","Name","Price", "Quantity", "Category" );
				printf("---------------------------------------------------------------------------------------------------\n\n" );
			}
			printf("\t%-10s\t%-20s\t%-10.2f\t%-10d\t%-10s\n\n", product[i].proID, product[i].proName, product[i].price, product[i].quantity, product[i].category.catName);
		}
	} 
	if(flag==0) printf("\n\t-> Not found the Product.\n");
	else {
		printf("---------------------------------------------------------------------------------------------------\n");
		printf("\n\tShowing results for: ");
		flag = 0;
		for (int i = 0; i < n_c; i++) {
			if (markedID[category[i].catID] == 1) {
				flag++;
				if(flag == 1) printf("%s", category[i].catName);
				else printf(", %s", category[i].catName);
			} 
		} printf(".");
	} 

	printf("\n\nPress any key to go to Main Menu!\n");
	printf("Or press 'Tab' to continue using the function.\n");
	char selection = _getch();
	if (selection == '\t') {
		system("cls");
		searchByCategory();
	}
}

void searchByName(){
	fflush(stdin);
	printf("\n\tEnter Product [Keyword] for search: ");
	char searchName[50];
	fgets(searchName, sizeof(searchName), stdin);
	searchName[strlen(searchName)-1] = 0;
	for(int i = 0; i < strlen(searchName); i++){
  		searchName[i] = tolower(searchName[i]);
	}
	int n_p = readProduct();
	int n_c = readCategory();
	connectByID(n_p, n_c);
	int flag=0;
	for(int i=0;i<n_p;i++){
		char proName[50];
		snprintf(proName, sizeof(proName), "%s", product[i].proName);
		for(int i = 0; i < strlen(proName); i++){
  		proName[i] = tolower(proName[i]);
		}
		if (strstr(proName, searchName) != NULL){
			flag++;
			if(flag==1){
				printf("\n---------------------------------------------------------------------------------------------------");
				printf("\n\t%-10s\t%-20s\t%-10s\t%-10s\t%-10s\n", "ID","Name","Price", "Quantity", "Category" );
				printf("---------------------------------------------------------------------------------------------------\n\n" );
			}
			printf("\t%-10s\t%-20s\t%-10.2f\t%-10d\t%-10s\n\n", product[i].proID, product[i].proName, product[i].price, product[i].quantity, product[i].category.catName);
		}
	}
	if(flag==0) printf("\n\t-> Not found the Product.\n");
	else printf("---------------------------------------------------------------------------------------------------\n");

	printf("\n\nPress any key to go to Main Menu!\n");
	printf("Or press 'Tab' to continue using the function.\n");
	char selection = _getch();
	if (selection == '\t') {
		system("cls");
		searchByName();
	}
}

void searchAdvanced(){
	int n_p = readProduct();
	int n_c = readCategory();
	connectByID(n_p, n_c);
	int markedID[1000];
	int flag;
	int n_s;

	printf("\n\tSearch filter [Enter '-1' to skip a property]:");
	
	printf("\n\n\t\tKeyword [Product Name or Category]: ");
	fflush(stdin);
	char search[50];
	fgets(search, sizeof(search), stdin);
	search[strlen(search)-1] = 0;
	if(strcmp(search, "-1")==0){
		n_s = n_p;
		for (int i = 0; i < n_p; i++) {
			markedID[i] = i;
		}
	} else {
		for(int i = 0; i < strlen(search); i++){
	  		search[i] = tolower(search[i]);
		}
		flag = 0;
		for(int i=0;i<n_p;i++){
			char proName[50];
			char proCat[20];
			snprintf(proName, sizeof(proName), "%s", product[i].proName);
			for(int i = 0; i < strlen(proName); i++){
	  		proName[i] = tolower(proName[i]);
			}
			snprintf(proCat, sizeof(proCat), "%s", product[i].category.catName);
			proCat[0] = tolower(proCat[0]);
			if (strstr(proName, search) != NULL || strstr(proCat, search) != NULL){
				flag++;
				markedID[flag-1] = i;
			}
		}
		n_s = flag;
	}
	
	printf("\n\t\tPrice range: ");
	printf("\n\t\t     [min]: $");
	float priceMin;
	do{
		fflush(stdin);
		scanf_s("%f", &priceMin);
		flag = 0;
		if(priceMin == -1){
			priceMin = 0;
		} else if (priceMin <= 0){
			printf("\t\tPrice min must be greater than $0.");
			printf("\n\t\tEnter again: ");
			flag = 1;
		}
	} while(flag == 1);

	printf("\t\t     [max]: $");
	float priceMax;
	do{
		fflush(stdin);
		scanf_s("%f", &priceMax);
		flag = 0;
		if(priceMax == -1){
			priceMax = 9999999;
		} else if (priceMax <= priceMin){
			if(priceMin > 0){
				printf("\t\tPrice max must be greater than price min.");
			} else if (priceMin == 0){
				printf("\n\t\tPrice max must be greater than $0.");
			}
			printf("\n\t\tEnter again: $");
			flag = 1;
		} 
	} while(flag == 1);	

	printf("\n\t\tQuantity range: ");
	printf("\n\t\t     [min]: ");
	int quantityMin;
	do{
		fflush(stdin);
		scanf_s("%d", &quantityMin);
		flag = 0;
		if(quantityMin == -1){
			quantityMin = 0;
		} else if (quantityMin < 0 || quantityMin > 100){
			printf("\t\tQuantity min must be in the range of [0-100].");
			printf("\n\t\tEnter again: ");
			flag = 1;
		}
	} while(flag == 1);

	printf("\t\t     [max]: ");
	float quantityMax;
	do{
		fflush(stdin);
		scanf_s("%f", &quantityMax);
		flag = 0;
		if(quantityMax == -1){
			quantityMax = 100;
		} else if (quantityMax < quantityMin){
			if(quantityMin > 0){
				printf("\t\tQuantity max must be greater than quantity min.");
			} else if (quantityMin == 0 && quantityMax > 100){
				printf("\n\t\tQuantity max must be in the range of [0-100].");
			}
			printf("\n\t\tEnter again: $");
			flag = 1;
		}
	} while(flag == 1);	

	flag = 0;
	for (int i = 0; i < n_s; i++) {
		if(product[markedID[i]].price >= priceMin && product[markedID[i]].price <= priceMax && product[markedID[i]].quantity >= quantityMin && product[markedID[i]].quantity <= quantityMax){
			flag++;
			if (flag == 1) {
				printf("\n---------------------------------------------------------------------------------------------------");
				printf("\n\t%-10s\t%-20s\t%-10s\t%-10s\t%-10s\n", "ID","Name","Price", "Quantity", "Category" );
				printf("---------------------------------------------------------------------------------------------------\n\n" );
			}
			printf("\t%-10s\t%-20s\t%-10.2f\t%-10d\t%-10s\n\n", product[markedID[i]].proID, product[markedID[i]].proName, product[markedID[i]].price, product[markedID[i]].quantity, product[markedID[i]].category.catName);
		}
	}
	if(flag==0) printf("\n\t-> Not found the Product.\n");
	else printf("---------------------------------------------------------------------------------------------------\n");

	printf("\n\nPress any key to go to Main Menu!\n");
	printf("Or press 'Tab' to continue using the function.\n");
	char selection = _getch();
	if (selection == '\t') {
		system("cls");
		searchAdvanced();
	}
}

void updateProduct(){
	fflush(stdin);
	printf("\n\tEnter Product ID for update: ");
	char updateID[20];
	fgets(updateID, sizeof(updateID), stdin);
	updateID[strlen(updateID)-1] = 0;
	int n_p = readProduct();
	int n_c = readCategory();
	connectByID(n_p, n_c);
	int flag=0;
	int i;
	for(i=0;i<n_p;i++){
		if(_strcmpi(updateID,product[i].proID)==0){
			flag = 1;
			printf("\n---------------------------------------------------------------------------------------------------");
			printf("\n\t%-10s\t%-20s\t%-10s\t%-10s\t%-10s\n", "ID","Name","Price", "Quantity", "Category" );
			printf("---------------------------------------------------------------------------------------------------\n\n" );
			printf("\t%-10s\t%-20s\t%-10.2f\t%-10d\t%-10s\n\n", product[i].proID, product[i].proName, product[i].price, product[i].quantity, product[i].category.catName);
			printf("---------------------------------------------------------------------------------------------------\n");
			break;
		}
	} 
	if(flag==0) printf("\n\t-> Not found the Product.\n");
	else {
		fflush(stdin);
		printf("\n\tEdit the Product\n");
		printf("\t\tProduct Name: ");
		fgets(product[i].proName, sizeof(product[i].proName), stdin);
		product[i].proName[strlen(product[i].proName)-1] = 0;

		printf("\t\tPrice: $");
		do{
			scanf_s("%f", &product[i].price);
			flag = 0;
			if(product[i].price <= 0){
				printf("\n\t\tPrice must be greater than $0.");
				printf("\n\t\tEnter again: $");
				flag = 1;
			}
		} while(flag == 1);

		printf("\t\tQuantity [0-100]: ");
		do{
			scanf_s("%d", &product[i].quantity);
			flag = 0;
			if(product[i].quantity < 0 || product[i].quantity > 100){
				printf("\n\t\tQuantity must be in the range of [0-100].");
				printf("\n\t\tEnter again: ");
				flag = 1;
			}
		} while(flag == 1);	

		printf("\t\tCategory ID: ");
		do {
			scanf_s("%d", &product[i].category.catID);
			fflush(stdin);
			flag = 0;
			if(checkCategoryID(product[i].category.catID) == 0){
				printf("\n\t\tCategory ID is not exists.");
				printf("\n\t\tEnter again: ");
				flag = 1;
			}
		} while(flag == 1);
		if(overwriteData(productsFile, n_p) == 0) printf("\n\t-> Update product success.\n");
	}
	printf("\nPress any key to go to Main Menu!\n");
	printf("Or press 'Tab' to continue using the function.\n");
	char selection = _getch();
	if (selection == '\t') {
		system("cls");
		updateProduct();
	}
}

void removeProduct(){
	fflush(stdin);
	printf("\n\tEnter Product ID for update: ");
	char updateID[20];
	fgets(updateID, sizeof(updateID), stdin);
	updateID[strlen(updateID)-1] = 0;
	int n_p = readProduct();
	int n_c = readCategory();
	connectByID(n_p, n_c);
	int flag=0;
	int i;
	for(i=0;i<n_p;i++){
		if(_strcmpi(updateID,product[i].proID)==0){
			flag = 1;
			printf("\n---------------------------------------------------------------------------------------------------");
			printf("\n\t%-10s\t%-20s\t%-10s\t%-10s\t%-10s\n", "ID","Name","Price", "Quantity", "Category" );
			printf("---------------------------------------------------------------------------------------------------\n\n" );
			printf("\t%-10s\t%-20s\t%-10.2f\t%-10d\t%-10s\n\n", product[i].proID, product[i].proName, product[i].price, product[i].quantity, product[i].category.catName);
			printf("---------------------------------------------------------------------------------------------------\n");
			break;
		}
	}
	if(flag==0) printf("\n\t-> Not found the Product.\n");
	else {
		printf("\n\tAre you sure you want to delete this product [Y/N]: ");
		char selection = getchar();
		if (selection == 'Y' || selection == 'y') {
			for(int j=i;i<n_p;i++){
				product[i]=product[i+1];
			} n_p--;
			if(overwriteData(productsFile, n_p) == 0) printf("\n\t-> Remove product success.\n");
		} else printf("\n\t-> Remove product failse.\n");
	}
	printf("\nPress any key to go to Main Menu!\n");
	printf("Or press 'Tab' to continue using the function.\n");
	char selection = _getch();
	if (selection == '\t') {
		system("cls");
		removeProduct();
	}
}
