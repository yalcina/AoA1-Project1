#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <ctime>
#include <fstream>
#include <climits>   // ULLONG_MAX

using namespace std;


class  Product {
  public:
	char  timestamp[27];
	char  price[10];
	int order;
	
	double price_conv;
	unsigned long long int  time_modified;
	
	void convert_time_to_integer();
};



void  Product::convert_time_to_integer(){
	char *ptr = timestamp;
	char *x;
	char  string_time[14] = {'\0'};
	
	string_time[0] = ptr[9];
	string_time[1] = ptr[11];
	string_time[2] = ptr[12];
	string_time[3] = ptr[14];
	string_time[4] = ptr[15];
	string_time[5] = ptr[17];
	string_time[6] = ptr[18];
	string_time[7] = ptr[20];
	string_time[8] = ptr[21];
	string_time[9] = ptr[22];
	string_time[10] = ptr[23];
	string_time[11] = ptr[24];
	string_time[12] = ptr[25];

	time_modified = strtoull(string_time, &x, 10);
}



class  Storage {
	Product  *product_list;
	int number;
	
  public:
  	Storage(int num) : product_list(NULL) , number(num){}    // Constructor
  	~Storage();
  	
  	void fetch_from_CSV();
  	void shuffle();
  	void sort_by_INSERTION(char*);
  	void sort_by_MERGE(char*);
  	void write_to_CSV();
  	void print_storage();
  private:
  	void split_string(char*, char*, char*);  //  fetch_from_CSV() will call this
  	void insertion_sort_t();   //  Private insertion sort function called by main sort_by_INSERTION() function according to timestamp
  	void insertion_sort_p();   //  Private insertion sort function called by main sort_by_INSERTION() function according to last price
  	void merge_sort_t(int, int);   //  Private merge sort function called by main sort_by_MERGE() function according to timestamp
  	void merge_sort_p(int, int);   //  Private merge sort function called by main sort_by_MERGE() function according to last price
  	
  	void merging_t(int, int, int);
  	void merging_p(int, int, int);
};


void  Storage::insertion_sort_t(){
	Product *x = product_list;
	Product temp;
	int i, j;
	
	for( i = 1; i < number; i++ ){
		temp = x[i];
		
		j = i - 1;
		
		while( j >= 0  &&  (x[j].time_modified > temp.time_modified) ){
			x[j+1] = x[j];
			j = j - 1;
		}
		
		x[j+1] = temp;
	}
}



void  Storage::insertion_sort_p(){
	Product *x = product_list;
	Product temp;
	int i, j;
	
	for( i = 1; i < number; i++ ){
		temp = x[i];
		
		j = i -1;
		
		while( j >= 0  &&  (x[j].price_conv > temp.price_conv) ){
			x[j+1] = x[j];
			j = j - 1;
		}
		
		x[j+1] = temp;
	}
}





void  Storage::merge_sort_t(int p, int r){
	int q;
	Product *x = product_list;
	
	if(p < r){
		q = (p+r)/2;
		merge_sort_t( p, q );
		merge_sort_t( q+1, r );
		
		merging_t(p, q, r);
	}
}



void  Storage::merging_t(int p, int q, int r){
	Product *x = product_list;
	
	int n1 = q-p+1;
	int n2 = r-q;
	
	Product *Left = new Product [n1 + 1];
	Product *Right = new Product [n2 + 1];
	
	for(int i = 0; i < n1; i++){
		Left[i] = x[p+i];
	}
	Left[n1].time_modified = ULLONG_MAX;
	
	for(int j = 0; j < n2; j++){
		Right[j] = x[q+j+1];
	}
	Right[n2].time_modified = ULLONG_MAX;
	
	int i = 0;
	int j = 0;
	
	for( int k = p; k <= r; k++ ){
		if( Left[i].time_modified  <  Right[j].time_modified ){
			x[k] = Left[i];
			i++;
		}
		else {
			x[k] = Right[j];
			j++;
		}	
	}

	delete [] Left;
	delete [] Right;
}





void  Storage::merge_sort_p(int p, int r){
	int q;
	Product *x = product_list;
	
	if(p < r){
		q = (p+r)/2;
		merge_sort_p( p, q );
		merge_sort_p( q+1, r );
		
		merging_p(p, q, r);
	}
}


void  Storage::merging_p(int p, int q, int r){
	Product *x = product_list;
	
	int n1 = q-p+1;
	int n2 = r-q;
	
	Product *Left = new Product [n1 + 1];
	Product *Right = new Product [n2 + 1];
	
	for(int i = 0; i < n1; i++){
		Left[i] = x[p+i];
	}
	Left[n1].price_conv = 1000000000000000000.0;
	
	for(int j = 0; j < n2; j++){
		Right[j] = x[q+j+1];
	}
	Right[n2].price_conv = 1000000000000000000.0;
	
	int i = 0;
	int j = 0;
	
	for( int k = p; k <= r; k++ ){
		if( Left[i].price_conv  <  Right[j].price_conv ){
			x[k] = Left[i];
			i++;
		}
		else {
			x[k] = Right[j];
			j++;
		}	
	}

	delete [] Left;
	delete [] Right;
}




void   Storage::fetch_from_CSV(){
	product_list = new Product [number];
	
	
	ifstream CSV_File("log_inf.csv");
	string single_line;
	char *char_line;

	getline(CSV_File, single_line, '\n');   //  disposing of the tags
	
	for(int i = 0; i < number; i++){
		for(int j = 0; j < 27; j++){  product_list[i].timestamp[j] = 0; }
		for(int k = 0; k < 10; k++){  product_list[i].price[k] = 0; }
		
		
		getline(CSV_File, single_line, '\n');
		
		char_line = new char [single_line.size()+1];
		strcpy(char_line, &single_line[0]);
		
		split_string(product_list[i].timestamp,   product_list[i].price,   char_line);
		
		
		product_list[i].convert_time_to_integer();
		product_list[i].price_conv  =  atof(product_list[i].price);

		product_list[i].order = i+1;
	}
}


void  Storage::shuffle(){
	srand (time(NULL));
	int random = 0;
	
	Product temp;
	
	for(int i = 0; i < number; i++){
		random = rand() % number;
		
		temp = product_list[i];
		product_list[i] = product_list[random];
		product_list[random] = temp;
		
	}
}


void  Storage::sort_by_INSERTION(char *var){
	char c = *var;
	
	if( c == 't' ){
		cout  <<  "\n\nInsertion Sort will be run by Timestamp\n\n";
		insertion_sort_t();
	}
	
	else if( c == 'p' ){
		cout  <<  "\n\nInsertion Sort will be run by Price\n\n";
		insertion_sort_p();
	}
	
	else {
		cout << "ERROR:   Insertion sort could not be run..!!!";
	}
}




void  Storage::sort_by_MERGE(char *var){
	char c = *var;
	
	if( c == 't' ){
		cout  <<  "\n\nMerge Sort will be run by Timestamp\n\n";
		merge_sort_t(0, number-1);
	}
	
	else if( c == 'p' ){
		cout  <<  "\n\nMerge Sort will be run by Price\n\n";
		merge_sort_p(0, number-1);
	}
	
	else {
		cout << "ERROR:   Insertion sort could not be run..!!!";
	}
}

Storage::~Storage(){
	delete  [] product_list;
	number = 0;
}



void  Storage::split_string(char *tm, char *prc, char *line){
	char *front = line;
	char *tail = line;
	
	while( *front == ' ' ){
		front++;
		tail++;
	}
	
	while( *front != ',' ){
		front++;
	}
	
	while( tail != front ){
		*tm = *tail;
		
		tm++;
		tail++;
	}
	
	tm = '\0';
	
	while(*front == ','){
		front++;
	}
	
	while( *front != ','){
		front++;
	}
	
	while( *front == ','){
		front++;
	}
	
	tail = front;
	
	while( *front != ','){
		front++;
	}
	
	while( tail != front ){
		*prc = *tail;
		
		prc++;
		tail++;
	}
	prc = '\0';
}



void  Storage::print_storage(){
	cout << "\n\t\t\t\t\t\tTimestamp\t\t\tLast Price\n\n";
	
	for( int i = 0; i < number; i++ ){
		cout  << "  Actual order in CSV:  "  <<  product_list[i].order << "\t\t:" << product_list[i].timestamp << ":" << "\t\t" << ":" << product_list[i].price << ":\n\n";
	}
}


int main(int argc, char *argv[]){

	int size = 1000;  
	char request[] = "t";  
	char b = 'i';     
	clock_t t1, t2;

	Storage  Database(size);
	Database.fetch_from_CSV();
	Database.shuffle();

	cout << "\n\n\nDo you want to sort the DATABASE..?   If yes, press ENTER: " << endl;
	getchar();

	if( b == 'i' ){
		t1 = clock();
		Database.sort_by_INSERTION(request);
		t2 = clock();
		
		cout << "\n\nMicroseconds elapsed for " << size <<  " elements in INSERTION SORT:  " << t2 - t1 << "\n\n";
	}
	
	else if (b == 'm'){
		t1 = clock();
		Database.sort_by_MERGE(request);
		t2 = clock();
		
		cout << "\n\nMicroseconds elapsed for " << size <<  " elements in MERGE SORT:  " << t2 - t1 << "\n\n";
	}

	cout << "\n\n\nSorting is over, do you want to print the DATABASE..?   If yes, press ENTER: " << endl;
	getchar();
	
	Database.print_storage();

	getchar();
	return 0;
}
