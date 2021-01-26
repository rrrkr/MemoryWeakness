#include<stdio.h>
#include<stdlib.h>
#include<time.h>

/*
*   オブジェクト形式マクロ
*/
#define NUMBER 20
#define WIDTH NUMBER / 4//トランプ表示の幅は5
#define HEIGHT NUMBER / 5//トランプ表示の高さは4
/*
*   構造体   
*/
typedef struct{
    const char *upperCase;//A~Jのアルファベットの大文字を入れるための変数
    unsigned int set : 1;//カードがめくられている状態かを区別するためのオンオフの変数
} charBox;

/*
*   グローバル変数
*/
int char_array[NUMBER/2] = {0};//乱数計算にてアルファベットが重複しないようにカウントする配列
charBox array2[NUMBER] = {0};//COM対戦用の配列

/*
*   乱数を取得するための関数
*/
int getRandom(int min,int max)
{
    return min + (int)(rand() *(max - min + 1.0)/(1.0 + RAND_MAX));
}

/*
*   A~Jまでのアルファベットの大文字を設定する関数   
*/
void setChar(charBox array[])
{
    getRandom(0,9);//最初の乱数が固定されてしまうため
    int i;
    for(i=0;i<NUMBER;i++){
        do{
            int j = getRandom(0,9);
            if(char_array[j] < 2){//A~Jまでのアルファベットは各2つまで
                char_array[j]++;
                array[i].upperCase =  j + 'A';//アルファベットのAに0~9を足すことで文字を決めている
            }
        }while(array[i].upperCase == NULL);//文字が設定されるまで繰り返す
    }
}

/*
*   トランプを出力する関数
*/
void output(const charBox array[])
{
    int i,j;
    for(i=0;i<HEIGHT;i++){
        puts("+--+  +--+  +--+  +--+  +--+");
        for(j=0;j<WIDTH;j++){
            int d = i*5+j;//1~20の数値にするため
            if(array[d].set)//array[d].setが1であればアルファベットを表示
                printf("| %c|  ",array[d].upperCase);
            else    
                printf("|%2d|  ",d+1);//0であれば数字をそのまま表示
        }
        putchar('\n');
        puts("+--+  +--+  +--+  +--+  +--+");
    }
}

/*
*   カード番号を入力する関数
*/
void inputNum(int *num_1)
{
    do{
        printf("カード番号を選んでください（1 ~ 20）：");
        scanf("%i",num_1);
        (*num_1)--;
    }while(*num_1 < 0 || *num_1 > 19);//0~19以外の数値が入力されている間は繰り返す
}

/*
*   続けて同じカード番号を入力された時のための関数
*/
void inputSame(const int *num_1,int *num_2)
{
    do{
        printf("同じカード番号を選ばないでください（1 ~ 20）：");
        scanf("%i",num_2);
        (*num_2)--;
    }while((*num_2 < 0 || *num_2 > 19) || *num_1 == *num_2);
}

/*
*   あたり、はずれを判定する関数
*/
int decision(charBox array[],const int *num_1,const int *num_2)
{
    if(array[*num_1].upperCase == array[*num_2].upperCase){//トランプ１枚目とトランプ２枚目に設定されている文字が等しいならばあたり
        puts("あたりです！\n");
        array2[*num_1].set = 0;//COM対戦用の配列の状態をオフ
        array2[*num_2].set = 0;
        return 1;
    }else{
        puts("はずれです！\n");
        array[*num_1].set = 0;//プレイヤーの配列の状態をオン
        array[*num_2].set = 0;
        array2[*num_1].set = 1;//COM対戦用の配列の状態をオン
        array2[*num_2].set = 1;
        return 0;
    }
}

/*
*   プレイヤー側のターン一回分の関数、
*/
int roundPLAYER(charBox array[])
{   
    int pnum_1,pnum_2;
    int set = 2;
    int count = 0;
     do{
        output(array);
        switch(set){
            case 2:
                inputNum(&pnum_1);
                array[pnum_1].set = 1;
                continue;//処理を飛ばす
            case 1:
                inputNum(&pnum_2);
                if(pnum_1 == pnum_2)    
                    inputSame(&pnum_1,&pnum_2);
                    array[pnum_2].set = 1;
                    continue;
            default:
                break;
            }
        count += decision(array,&pnum_1,&pnum_2);
            
        }while(set--);  
        return count;
}

/*
*   コンピュータの一枚目を決定する関数
*/
int inputCOM_1(const charBox array[])
{
    int i,j;
    for(i=0;i<NUMBER;i++){
        for(j=1;j<NUMBER;j++){
            if(i != j && (array2[i].set & array2[j].set) == 1 && array2[i].upperCase == array2[j].upperCase)
                return i;
        }
    }
    while(1){
        int r = getRandom(0,19);
        if(array[r].set == 0 && array2[r].set == 0)
            return r;
    }
}

/*
*   コンピュータの2枚目を決定する関数
*/
int inputCOM_2(const charBox array[],const int cnum_1)
{
    int i;
    for(i=0;i<NUMBER;i++){
        if(cnum_1 != i && (array2[cnum_1].set & array2[i].set) == 1 && array2[cnum_1].upperCase == array2[i].upperCase)
            return i;
    }
    while(1){
        int r = getRandom(0,19);
        if(array[r].set == 0 && array2[r].set == 0)
            return r;
    }
}

/*
*   COMのターン一回分の関数
*/
int roundCOM(charBox array[])
{   
    int cnum_1,cnum_2;
    int set = 2;
    int count = 0;
     do{
        output(array);
        switch(set){
            case 2:
                puts("1枚目");
                cnum_1 = inputCOM_1(array);
                array[cnum_1].set = 1;
                continue;//処理を飛ばす
            case 1:
                puts("2枚目");
                cnum_2 = inputCOM_2(array,cnum_1);
                array[cnum_2].set = 1;
                continue;
            }
        count += decision(array,&cnum_1,&cnum_2);
            
        }while(set--);  
        return count;
}

/*
*   プレイヤー側がもう一度めくれるかどうかを判定し、カウントする関数
*/
void turn(charBox array[],const int player,int *point,int *count)
{
    int tmp = 2;
    while(tmp){
        if(*count == 0) 
            break;        
        if(tmp != 2)  
            printf("もう一度カードをめくってください。");
        printf("PLAYER%02iの番です。\n",player);
        tmp = roundPLAYER(array);
        if(tmp == 1) (*point)++;
        *count -= tmp;   
    }
}

/*
*   コンピュータ側がもう一度めくれるかどうかを判定し、カウントする関数
*/
void turnCOM(charBox array[],int *point,int *count)
{
    int tmp = 2;
    while(tmp){
        if(*count == 0) break;//カウント        
        if(tmp != 2)  printf("もう一度カードをめくってください。");
        puts("COMの番です。");
        tmp = roundCOM(array);//あたりであれば１、はずれであれば0
        if(tmp == 1) (*point)++;//得点を増やす
        *count -= tmp;//トランプの残り回数をtmpだけ減らす   
    }
}

/*
*   mainの関数
*/
int main()
{
    charBox array[NUMBER] = {0};//構造体配列

    int count = NUMBER / 2;//トランプ残り（１セット2枚の１０セット分）
    int points[2] = {0};//得点
    int i;

    srand((unsigned int)time(NULL));//現在時刻から乱数を生成
    setChar(array);

    for(i=0;i<NUMBER;i++)
        array2[i] = array[i];//コンピュータ対戦用の配列にコピー

    int mode;//モード選択用の変数
    printf("モード選択（1人用...1　/　2人対戦...2　/ COM対戦...3）：");
    if(scanf("%i",&mode) == 1 && mode <= 3 && mode > 0){//モード選択の入力が1つで入力範囲内であればtrue
        while(count){//トランプ残り回数が0になるまで繰り返す
            switch(mode){
                case 1:
                    count -= roundPLAYER(array);
                    break;
                case 2:
                    turn(array,1,&points[0],&count);//turn(構造体配列名、プレイヤー、得点、残り回数)
                    turn(array,2,&points[1],&count);
                    break;
                case 3:
                    turn(array,1,&points[0],&count);
                    turnCOM(array,&points[1],&count);//turnCOM(構造体配列名、得点、残り回数)
                    break;
            }
        }
    }else{
        puts("モード選択が正しく入力されませんでした。");
        return 0;
    }

    switch(mode){
        case 2:
            printf("PLAYER01は%i点、PLAYER02は%i点です。\n",points[0],points[1]);
            printf(points[0]==points[1]?"同点です。\n":(points[0]>points[1] ? "PLAYER01の勝利です。\n":"PLAYER02の勝利です。\n"));
            break;
        case 3:
            printf("PLAYERは%i点、COMは%i点です。\n",points[0],points[1]);
            printf(points[0]==points[1]?"同点です。\n":(points[0]>points[1] ? "PLAYERの勝利です。\n":"COMの勝利です。\n"));

    }
    return 0;
}