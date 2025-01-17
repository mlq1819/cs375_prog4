#include "Algorithms.h"

using namespace std;

unsigned int max(unsigned int a, unsigned int b){
#if DEBUG
cout << "max(" << a << "," << b << ")";
#endif
	if(a>b)
		return a;
	return b;
}

unsigned int max(unsigned int a, unsigned int b, unsigned int c){
#if DEBUG
cout << "max(" << a << "," << b << "," << c << ")";
#endif
	unsigned int max = a;
	if(b>max)
		max=b;
	if(c>max)
		max=c;
	return max;
}

void swap(Item * A, unsigned int a, unsigned int b){
	Item a_i = Item(A[a]);
	A[a]=Item(A[b]);
	A[b]=a_i;
}

unsigned int partition(Item * A, unsigned int low, unsigned int high){
	Item pivot = A[high];
	unsigned int i = low;
	for(unsigned int j=low; j<high; j++){
		if(A[j]<pivot){
			if(i!=j)
				swap(A, i, j);
			i++;
		}
	}
	swap(A, i, high);
	return i;
}

void reverse(Item * A, unsigned int low, unsigned int high){
	unsigned int i=low, j=high;
	while(i<j){
		Item a = Item(A[i]);
		A[i]=A[j];
		A[j]=a;
		i++;
		j--;
	}
}

void quicksort(Item * A, unsigned int low, unsigned int high){
#if DEBUG
cout << "Entering quicksort: (p/w;p,w)" << endl;
for(unsigned int i=low; i<high; i++)
	A[i].print();
cout << endl;
#endif	
	quicksort_helper(A,low,high-1);
	reverse(A, low, high-1);
#if DEBUG
cout << "Exiting quicksort: (p/w;p,w)" << endl;
for(unsigned int i=low; i<high; i++)
	A[i].print();
cout << endl;
#endif	
}

void quicksort_helper(Item * A, unsigned int low, unsigned int high){
	if(low<high){
		unsigned int p = partition(A, low, high);
		if(p>0)
			quicksort_helper(A, low, p-1);
		quicksort_helper(A, p+1, high);
	}
}

bool isSorted(const Item * A, size_t size){
	for(unsigned int i=1; i<size; i++){
		if(A[i-1]<A[i])
			return false;
	}
	return true;
}

dEntry::dEntry(){
	this->filled=false;
	this->entry=0;
}

dTable::dTable(){
	this->size=0;
	this->capacity=0;
	this->table=NULL;
}

dTable::dTable(size_t size, unsigned int cap){
#if DEBUG
cout << "Starting table..." << endl;
#endif
	this->size=size+1;
	this->capacity=cap;
	this->table = (dEntry **) malloc((capacity+1)*sizeof(void *));
	for(unsigned int c=0; c<this->capacity+1; c++){
		this->table[c] = (dEntry *) malloc(this->size*sizeof(void *));
		for(unsigned int n=0; n<this->size; n++){
			this->table[c][n]=dEntry();
			if(c==this->capacity)
				this->table[c][n].filled=true;
		}
		this->table[c][0].filled=true;
	}
#if DEBUG
this->print();
cout << "Completed table!" << endl;
#endif
}

dTable::dTable(const dTable &o){
#if DEBUG
cout << "Copying table..." << endl;
#endif
	this->size=o.size;
	this->capacity=o.capacity;
	this->table=(dEntry **) malloc((this->capacity+1)*sizeof(void *));
	for(unsigned int c=0; c<this->capacity+1; c++){
		this->table[c] = (dEntry *) malloc(this->size*sizeof(void *));
		for(unsigned int n=0; n<this->size; n++){
			this->table[c][n]=dEntry();
			this->table[c][n].filled=o.table[c][n].filled;
			this->table[c][n].entry=o.table[c][n].entry;
		}
	}
#if DEBUG
cout << "Copied table!" << endl;
#endif
}

dTable::~dTable(){
#if DEBUG
cout << "In dTable's destructor!" << endl;
#endif
	if(this->table!=NULL){
		for(unsigned int c=0; c<this->capacity+1; c++)
			if(this->table[c]!=NULL)
				free(this->table[c]);
		free(this->table);
	}
#if DEBUG
cout << "Adiós!" << endl;
#endif
}

void dTable::print() const {
	for(unsigned int c=0; c<this->capacity+1; c++){
		if(c==this->capacity){
			for(unsigned int n=0; n<this->size;n++){
				cout << "==========\t ";
				if(n==0)
					cout << "++\t ";
			}
			cout << endl;
		}
		for(unsigned int n=0; n<this->size; n++){
			if(this->table[c][n].filled)
				cout << "<" << this->table[c][n].entry << ">[" << c << "][" << n << "]\t";
			else
				cout << "<N>[" << c << "][" << n << "]\t ";
			if(n==0)
				cout << "||\t ";
		}
		cout << endl;
	}
	cout << "\n" << endl;
}

Algorithm::Algorithm(Item * items, size_t size, unsigned int capacity){
#if DEBUG
cout << "Creating Algorithm Object..." << endl;
#endif
	this->items = new Item[size];
	for(unsigned int i=0; i<size; i++)
		this->items[i]=Item(items[i]);
	this->size=size;
	this->capacity=capacity;
	this->table = new dTable(this->size, this->capacity);
#if DEBUG
cout << "Created Algorithm Object!" << endl;
this->print();
cout << endl;
#endif
}

Algorithm::Algorithm(const Algorithm &o){
#if DEBUG
cout << "Copying Algorithm Object..." << endl;
#endif
	this->size=o.size;
	this->items = new Item[this->size];
	for(unsigned int i=0; i<this->size; i++)
		this->items[i]=Item(o.items[i]);
	this->capacity=o.capacity;
	this->table=new dTable(*(o.table));
#if DEBUG
cout << "Coppied Algorithm Object!" << endl;
#endif
}

Algorithm::Algorithm(){
#if DEBUG
cout << "Creating empty (invalid) Algorithm Object..." << endl;
#endif
	this->items=NULL;
	this->size=0;
	this->capacity=0;
	this->table=new dTable();
#if DEBUG
cout << "Created empty (invalid) Algorithm Object!" << endl;
#endif
}

Algorithm::~Algorithm(){
	delete this->table;
	this->table=NULL;
	delete[] this->items;
	this->items=NULL;
}

unsigned int Algorithm::getMaxPossible() const {
	return getMaxPossible(0);
}

unsigned int Algorithm::getMaxPossible(unsigned int start) const {
	unsigned int total_profit=0;
	unsigned int total_weight=0;
	unsigned int i = start;
	while(i<this->size && total_weight+this->items[i].getWeight()<=this->capacity){
		total_profit+=this->items[i].getProfit();
		total_weight+=this->items[i].getWeight();
	}
	if(i<this->size && total_weight!=this->capacity){
		unsigned int difference = this->capacity-total_weight;
		total_profit+=(difference*this->items[i].getProfit())/this->items[i].getWeight();
		total_weight+=difference;
	}
	return total_profit;
}

unsigned int Algorithm::greedy1(){
#if DEBUG
cout << "In Greedy1()" << endl;
#endif
	if(!isSorted(this->items, this->size))
		quicksort(this->items, 0, this->size);
	unsigned int total_profit=0;
	unsigned int total_weight=0;
	for(unsigned int i=0; i<this->size; i++){
		if(total_weight+this->items[i].getWeight()<=this->capacity){	
#if DEBUG
cout << "Added item with p= " << this->items[i].getProfit() << ", w=" << this->items[i].getWeight() << endl;
#endif
			total_weight+=this->items[i].getWeight();
			total_profit+=this->items[i].getProfit();
		}
	}
	return total_profit;
}

unsigned int Algorithm::greedy2(){
#if DEBUG
cout << "In Greedy2()" << endl;
#endif
	vector<Item> solution = vector<Item>();
	if(!isSorted(this->items, this->size))
		quicksort(this->items, 0, this->size);
	unsigned int pmax, gmax;
	gmax=greedy1();
	pmax=greedy2_helper(0);
	if(pmax>gmax)
		return pmax;
	return gmax;
}

unsigned int Algorithm::greedy2_helper(unsigned int i) const {
	if(i>=this->size)
		return 0;
	unsigned int h = greedy2_helper(i+1);
	unsigned int j = this->items[i].getProfit();
	if(this->items[i].getWeight()>this->capacity)
		j=0;
	if(j>h)
		return j;
	return h;
}

unsigned int Algorithm::backtrack(){
#if DEBUG
cout << "In backtrack()" << endl;
#endif
	if(!isSorted(this->items, this->size))
		quicksort(this->items, 0, this->size);
	unsigned int first_bound = getMaxPossible(0);
	return backtrack_helper(0, 0, first_bound, 0, 0);
}

unsigned int Algorithm::backtrack_helper(unsigned int cur_profit, unsigned int cur_weight, unsigned int cur_bound, unsigned int cur_best, unsigned int cur_idx){
	if(cur_idx>=this->size){
#if DEBUG
cout << "Returning from idx " << cur_idx << endl;
#endif
		return 0;
	} else {
		bool promising = true;
		promising = promising && cur_weight<=this->capacity;
		promising = promising && cur_bound>cur_best;
		if(!promising){
#if DEBUG
cout << "idx " << cur_idx << " non-promising: weight=" << cur_weight << "; bound=" << cur_bound << "; best=" << cur_best << endl;
#endif
			return 0;
		}
#if DEBUG
cout << "idx " << cur_idx << " promising: weight=" << cur_weight << "; bound=" << cur_bound << "; best=" << cur_best << endl;
#endif
		unsigned int take = backtrack_helper(cur_profit+this->items[cur_idx].getProfit(), cur_weight+this->items[cur_idx].getWeight(), cur_bound, max(cur_profit, cur_best), cur_idx+1);
		unsigned int leave = backtrack_helper(cur_profit, cur_weight, cur_profit+getMaxPossible(cur_idx+1), max(take, cur_profit, cur_best), cur_idx+1);
#if DEBUG
if(max(take, cur_profit, leave)==cur_profit)
	cout << "Returning from idx " << cur_idx << ": profit=" << cur_profit << endl;
#endif
		return max(take, cur_profit, leave);
	}
}

unsigned int Algorithm::dynamic(){
#if DEBUG
cout << "In dynamic()" << endl;
#endif
	if(!isSorted(this->items, this->size))
		quicksort(this->items, 0, this->size);
	unsigned int temp = dynamic_helper(this->table->size-1, 0);
#if DEBUG
this->print();
this->table->print();
#endif
	return temp;
}

unsigned int Algorithm::dynamic_helper(unsigned int n, unsigned int c){
#if DEBUG
cout << "dynamic_helper: at table[" << c << "][" << n << "]..." << endl;
#endif
	unsigned int w,p;
	w=this->items[n-1].getWeight();
	p=this->items[n-1].getProfit();
	if(w+c<=this->capacity){
#if DEBUG
cout << "<=\tw+c (" << w << "+" << c << ") <= " << this->capacity << endl;
#endif
		unsigned int v1 = dGet(n-1, c);
		unsigned int v2 = dGet(n, c+1);
		unsigned int v3 = dGet(n-1, c+w)+p;
		this->table->table[c][n].entry=max(v1, v2, v3);
		this->table->table[c][n].filled=true;
#if DEBUG
cout << "table[" << c << "][" << n << "] = " << this->table->table[c][n].entry << endl;
#endif
	} else {
#if DEBUG
cout << ">\tw+c (" << w << "+" << c << ") > " << this->capacity << endl;
#endif
		unsigned int v1 = dGet(n-1, c);
		unsigned int v2 = dGet(n, c+1);
		this->table->table[c][n].entry=max(v1, v2);
		this->table->table[c][n].filled=true;
#if DEBUG
cout << "table[" << c << "][" << n << "] = " << this->table->table[c][n].entry << endl;
#endif
	}
	return this->table->table[c][n].entry;
}

unsigned int Algorithm::dGet(unsigned int n, unsigned int c){
#if DEBUG
cout << "dGet called for [" << c << "][" << n << "]:\t ";
#endif
	if(c>this->table->capacity+1){
#if DEBUG
cout << "c (" << c << ") exceeds maximum capacity" << endl;
#endif
		return 0;
	}
	if(!this->table->table[c][n].filled){
#if DEBUG
cout << "entry not filled; filling table entry..." << endl;
#endif
		unsigned int toReturn = dynamic_helper(n,c);
#if DEBUG
cout << "... (dGet [" << c << "][" << n << "]) got entry value " << toReturn << endl;
#endif
		return toReturn;
	}
#if DEBUG
cout << "table entry=" << this->table->table[c][n].entry << "\n" << endl;
#endif
	return this->table->table[c][n].entry;
}

void Algorithm::print() const {
	cout << "\n<p,w>    \t ||\t ";
	for(unsigned int i=0; i<this->size; i++)
		this->items[i].print();
	cout << endl;
}