#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
    @brief tahtayi ekrana yazdirir
    @param n tahtanin boyutu
    @param board tahta
*/
void printSolution(int **board, int n){
    int i,j;
    for(i=0;i<n;i++){
        printf("----");
    }
    printf("\n");
    for(i= 0; i<n; i++){
    	printf("|");
        for(j =0;j <n; j++){
            if(board[i][j] == 1){
                printf("Q  |");
            }
            else{
                printf("   |");
            }
        }
        printf("\n");
        for(j=0;j<n;j++){
	        printf("----");
	    }
        printf("\n");
    }
    printf("\n");
}

/*
    @brief tahtanin sartlari saglayip saglamadigini kontrol eder
    @param board tahta
    @param n tahtanin boyutu
    @return 1 eger tahta sartlari sagliyorsa, 0 degilse
*/
int isValidBoard(int **board, int n){
    int row, col, i;
    for(row=0; row<n; row++){
        for(col=0; col<n; col++){
            if(board[row][col]==1){
                // Aynı satir ve sutun kontrolu
                for(i=0; i<n; i++){
                    if((i != col && board[row][i] == 1) || (i != row && board[i][col] == 1)){
                        return 0;//aynı satirda veya sutunda vezir varsa 0 dondur
                    }
                    if(i>0){//capraz kontrol
                        if ((row + i < n && col + i < n && board[row + i][col + i] == 1) || // sag-alt
                            (row - i >= 0 && col + i < n && board[row - i][col + i] == 1) || // sag-ust
                            (row + i < n && col - i >= 0 && board[row + i][col - i] == 1) || // sol-alt
                            (row - i >= 0 && col - i >= 0 && board[row - i][col - i] == 1)){ // sol-ust
                            return 0;//caprazda vezir varsa 0 dondur
                        }
                    }
                }
            }
        }
    }
    return 1;//eger tahta sartlari sagliyorsa 1 dondur
}

/*
    @brief brute force ile tahtanin n li tum kombinasyonlarini bulup sartlari saglayanlari ekrana yazdirir
    @param board tahta
    @param n tahtanin boyutu
    @param queens vezir sayisi
    @param startRow baslangic satiri
    @param startCol baslangic sutunu
    @param count cozum sayisi
    @param row satir indexi
    @param col sutun indexi
*/
void bruteForceGenerateCombinations(int **board, int n, int queens, int startRow, int startCol, int *count, long long int *countIter){
    if(queens== 0){//vezir sayisi 0 olunca tahtayi kontrol et
        (*countIter)++;
        if(isValidBoard(board, n)){//eger tahta sartlari sagliyorsa tahtayi yazdir
            (*count)++;//cozum sayisini arttir
            printSolution(board, n);
        }
        return;
    }
    int row, col;
    //Baslangic satirindan itibaren tum satirlari dolas
    for(row=startRow; row<n; row++){
        if(row==startRow)
            col= startCol;// ilk satirda baslangic sutunundan basla
        else
            col=0;//diger satirlarda ilk sutundan basla
        while(col<n){
            board[row][col] = 1; //bulundugu yere bir vezir yerlestir
            //kalan vezirler icin fonksiyonu geri cagir
            bruteForceGenerateCombinations(board, n, queens - 1, row, col + 1, count, countIter);
            board[row][col] = 0;//bulundugu yeri temizle
            col++;
        }
    }
}
/*
    @brief optimized1 icin tahtanin ayni satirinda vezir bulunamadigi olasiliklardan sartlari saglayanlari ekrana yazdirir
    @param board tahta
    @param n tahtanin boyutu
    @param row satir indexi
    @param count cozum sayisi

*/
int isValidOpt1(int **board, int n){
    int row, col, i;
    for(row=0; row<n; row++){
        for(col=0; col<n; col++){
            if(board[row][col]==1){
                for(i=0; i<n; i++){
                    if((i != row && board[i][col] == 1)){
                        return 0;//ayni sutunda vezir varsa 0 dondur
                    }
                    if(i>0){//capraz kontrol
                        if ((row + i < n && col + i < n && board[row + i][col + i] == 1) || // sag-alt
                            (row - i >= 0 && col + i < n && board[row - i][col + i] == 1) || // sag-ust
                            (row + i < n && col - i >= 0 && board[row + i][col - i] == 1) || // sol-alt
                            (row - i >= 0 && col - i >= 0 && board[row - i][col - i] == 1)) { // sol-ust
                            return 0;//caprazda vezir varsa 0 dondur
                        }
                    }
                }
            }
        }
    }
    return 1;//eger tahta sartlari sagliyorsa 1 dondur
}

/*
    @brief optimized1 ile tahtanin ayni satirinda vezir bulunamadigi olasiliklardan sartlari saglayanlari ekrana yazdirir
    @param board tahta
    @param n tahtanin boyutu
    @param row satir indexi
    @param count cozum sayisi
*/
void optimized1GenerateCombinations(int **board, int n, int row, int *count, long long int *countIter){
    //eger tum satirlara vezir yerlestirilmisse 
    if(row==n){
        (*countIter)++;
        // Tahtanin gecerli bir cozum olup olmadigini kontrol et
        if(isValidOpt1(board, n)){
            (*count)++;               
            printSolution(board, n);  
        }
        return; 
    }
    int col;
    //bulunulan satirdaki her sutun icin olasiliklari olustur
    for(col= 0;col<n;col++){
        board[row][col] = 1; //veziri sutuna yerlestir
        //bir sonraki satir icin fonksiyonu geri cagir
        optimized1GenerateCombinations(board, n, row + 1, count, countIter);
        board[row][col] = 0; //veziri kaldir
    }
}
/*
    @brief optimized2 icin tahtanin ayni satir ve ayni sutunda vezir bulunamadigi olasiliklardan sartlari saglayanlari ekrana yazdirir caprazlarin kontrolu yeterlidir
    @param board tahta
    @param n tahtanin boyutu
    @param row satir indexi
    @return 1 eger tahta sartlari sagliyorsa, 0 degilse
*/
int isValidOpt2(int **board, int n){
    int row, col, i;
    for(row = 0; row < n; row++){
        for(col = 0; col < n; col++){
            if(board[row][col] == 1){
                for(i = 1; i < n; i++){                   
                    //capraz kontrol
                        if ((row + i < n && col + i < n && board[row + i][col + i] == 1) || // sag-alt
                            (row - i >= 0 && col + i < n && board[row - i][col + i] == 1) || // sag-ust
                            (row + i < n && col - i >= 0 && board[row + i][col - i] == 1) || // sol-alt
                            (row - i >= 0 && col - i >= 0 && board[row - i][col - i] == 1)) { // sol-ust
                            return 0;//caprazda vezir varsa 0 dondur
                        }
                    
                }
            }
        }
    }
    return 1;//eger tahta sartlari sagliyorsa 1 dondur
}
/*
    @brief optimized2 ile tahtanin ayni satir ve ayni sutunda vezir bulunamadigi olasiliklardan sartlari saglayanlari ekrana yazdirir
    @param board tahta
    @param n tahtanin boyutu
    @param row satir indexi
    @param count cozum sayisi
    @param row satir indexi
    @param col sutun indexi
*/
void optimized2GenerateCombinations(int **board, int n, int row, int *count, long long int *countIter){
    if(row == n){// Eger tum satirlara vezir yerlestirildiyse
        (*countIter)++;
        if(isValidOpt2(board, n)){// Tahtanin gecerli bir cozum olup olmadigini kontrol et
            (*count)++;
            printSolution(board, n);
        }
        return;
    }
    int i,col;
    // Mevcut satirdaki her sutun icin olasiliklari dene
    for(col = 0; col < n; col++){
        i = 0;
        // Ayni sutunda vezir var mi?
        while(i<row && board[i][col]== 0){
            i++;// Eger o sutunda vezir yoksa bir sonraki satira bak
        }
        if(board[i][col]== 0){// Eger sutunda vezir yoksa bu sutuna bir vezir yerlestirebilirsin
            board[row][col]= 1;  // Veziri yerlestir
            optimized2GenerateCombinations(board, n, row + 1, count, countIter);  // Sonraki satir icin islemi devam ettir
            board[row][col] =0;  // Veziri kaldır
        }     
    }
}
/*
    @brief backtracking yonteminde tahtanin ayni satir ve ayni sutunda ve caprazda vezir bulunup bulunmadigini kontrol eder
    @param board tahta
    @param n tahtanin boyutu
    @param row satir indexi
    @param col sutun indexi
    @return 1 eger vezir yerlestirilebiliyorsa, 0 degilse
*/
int isSafeBackTracking(int **board, int n, int row, int col){
    int i,j;
    // Ayni sutunda baska bir vezir var mi?
    for(i=0; i<row; i++){
        if (board[i][col] == 1) 
        return 0;
    }
    // Sol ust capraz
    i=row - 1, j=col - 1;
    while(i>=0 && j>=0){
        if (board[i][j] == 1) 
            return 0;
        i--;
        j--;
    }
    // Sag ust capraz
    i=row - 1, j=col + 1; 
    while(i>= 0 && j < n){
        if (board[i][j] ==1) 
            return 0;
        i--;
        j++;
    }
    return 1; 
}
/*
    @brief backtracking yontemi ile tahtanin ayni satir ve ayni sutunda ve caprazda vezir bulunamadigi olasiliklardan sartlari saglayanlari ekrana yazdirir
    @param board tahta
    @param n tahtanin boyutu
    @param row satir indexi
    @param count cozum sayisi
    @param row satir indexi
    @param col sutun indexi

*/
void backtrackingGenerateCombinations(int **board, int n, int row, int *count, long long int *countIter){
    if(row==n){// Eger tum satirlara vezir yerlestirildiyse
        (*count)++;
        printSolution(board, n);
        return;
    }
    int col;
    for(col=0; col<n; col++){
        if(isSafeBackTracking(board, n, row, col)){ // Eger gecerliyse
            (*countIter)++;
            board[row][col] = 1;      // Veziri yerlestir
            backtrackingGenerateCombinations(board, n, row + 1, count, countIter); // Sonraki satira gec
            board[row][col] = 0;      // Geri al 
        }
    }
    return;
}
int main(){
    int n;
    int dd=1;
    while(dd){
        printf("Choose the size of the board: ");
        scanf("%d", &n);
        if(n<4){
        	printf("No valid Solution\n");
		}
		else{
			int i,j;
	        //tahtanin olusturulmasi
	        int **board = (int **)malloc(n * sizeof(int *));
	        for(i= 0;i< n;i++){
	            board[i] = (int *)malloc(n * sizeof(int));
	        }
	        for(i=0; i<n; i++){
	            for (j = 0; j < n; j++){
	                board[i][j] = 0;
	            }
	        }
	        time_t start_time, end_time;
	        printf("Choose the mode: \n");
	        printf("1. Brute Force\n2. Optimized 1\n3. Optimized 2\n4. Backtracking\n5. All\n6. Exit\n");
	        int mode,count;
            long long int countIter;
	        scanf("%d", &mode);
	        if(mode==1){//Brute Force
	            printf("Brute Force\n");
	            count = 0;
                countIter = 0;
	            start_time = clock();
	            bruteForceGenerateCombinations(board, n, n, 0, 0, &count, &countIter);
	            end_time = clock();
	            printf("Duration: %f seconds\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
	            printf("Total number of solutions: %d\n", count);
                printf("Total number of iterations: %lld\n", countIter);	
	        }
	        else if(mode==2){//Optimized 1
	            printf("Optimized 1\n");
	            count = 0;
                countIter = 0;
	            start_time = clock();
	            optimized1GenerateCombinations(board, n, 0, &count, &countIter);
	            end_time = clock();
	            printf("Duration: %f seconds\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
	            printf("Total number of solutions: %d\n", count);
                printf("Total number of iterations: %lld\n", countIter);
	        }
	        else if(mode==3){//Optimized 2
	            printf("Optimized 2\n");
	            count = 0;
                countIter = 0;
	            start_time = clock();
	            optimized2GenerateCombinations(board, n, 0, &count, &countIter);
	            end_time = clock();
	            printf("Duration: %f seconds\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
	            printf("Total number of solutions: %d\n", count);
                printf("Total number of iterations: %lld\n", countIter);
	        } 
	        else if(mode==4){//Backtracking
	            printf("Backtracking\n");
	            count = 0;
                countIter = 0;
	            start_time = clock();
	            backtrackingGenerateCombinations(board, n, 0, &count, &countIter);
	            end_time = clock();
	            printf("Duration: %f seconds\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
	            printf("Total number of solutions: %d\n", count);
                printf("Total number of iterations: %lld\n", countIter);
	        } 
	        else if(mode==5){//hepsinin arka arkaya calistirilmasi
	            printf("All\n");
	            printf("Brute Force\n");
	            count = 0;
                countIter = 0;
	            start_time = clock();
	            bruteForceGenerateCombinations(board, n, n, 0, 0, &count, &countIter);
	            end_time = clock();
	            int countBrute,countOpt1,countOpt2,countBack;
                long long int countIterBrute,countIterOpt1,countIterOpt2,countIterBack;
	            double durationbrute = (double)(end_time - start_time) / CLOCKS_PER_SEC;
	            countBrute=count; 
                countIterBrute=countIter;
	            printf("Optimized 1\n");          
	            count=0;
                countIter=0;
	            start_time = clock();
	            optimized1GenerateCombinations(board, n, 0, &count, &countIter);
	            end_time = clock();
	            double durationopt1 = (double)(end_time - start_time) / CLOCKS_PER_SEC;
	            countOpt1=count;
                countIterOpt1=countIter;
	            printf("Optimized 2\n");
	            count=0;
                countIter=0;
	            start_time = clock();
	            optimized2GenerateCombinations(board, n, 0, &count, &countIter);
	            end_time = clock();
	            double durationopt2 = (double)(end_time - start_time) / CLOCKS_PER_SEC;
	            countOpt2=count;
                countIterOpt2=countIter;
	            printf("Backtracking\n");
	            count=0;
                countIter=0;
	            start_time = clock();
	            backtrackingGenerateCombinations(board, n, 0, &count, &countIter);
	            end_time = clock();
	            double durationback = (double)(end_time - start_time) / CLOCKS_PER_SEC;
	            countBack=count;
                countIterBack=countIter;
	            //cozum sayilari
	            printf("Solution counts\n");
	            printf("Brute Force: Total number of solutions: %d\n", countBrute);
	            printf("Optimized 1: Total number of solutions: %d\n", countOpt1);
	            printf("Optimized 2: Total number of solutions: %d\n", countOpt2);
	            printf("Backtracking: Total number of solutions: %d\n", count);
                //iterasyon sayilari
                printf("Iteration counts\n");
                printf("Brute Force: Total number of iterations: %lld\n", countIterBrute);
                printf("Optimized 1: Total number of iterations: %lld\n", countIterOpt1);
                printf("Optimized 2: Total number of iterations: %lld\n", countIterOpt2);
                printf("Backtracking: Total number of iterations: %lld\n", countIterBack);
	            //islem sureleri
	            printf("Durations\n");
	            printf("Duration of Brute Force: %f seconds\n", durationbrute);
	            printf("Duration of Optimized 1: %f seconds\n", durationopt1);
	            printf("Duration of Optimized 2: %f seconds\n", durationopt2);
	            printf("Duration of Backtracking: %f seconds\n", durationback);
	            printf("\nTOTAL DURATION: %f seconds\n\n", durationbrute+durationopt1+durationopt2+durationback);	
	        } 
	        else if (mode ==6){
	            printf("Exit\n");
	            dd = 0;
	        } 
	        else{
	            printf("Invalid mode\n");
	        }
	        //tahtanin serbest birakilmasi
	        for(i=0; i<n; i++){
	            free(board[i]);
	        }
	        free(board);	
		}       
    }
    return 0;
}
